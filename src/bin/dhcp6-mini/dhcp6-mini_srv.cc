// Copyright (C) 2014 Internet Systems Consortium, Inc. ("ISC")
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
// OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#include <dhcp6-mini_srv.h>
#include <dhcp6-mini_log.h>
#include <dhcp/iface_mgr.h>
#include <dhcp/libdhcp++.h>
#include <dhcp/option_custom.h>
#include <dhcp/option_int_array.h>
#include <dhcp/option6_iaaddr.h>
#include <cfg_mgr.h>
#include <io_utilities.h>
#include <lease.h>
#include <lease_mgr.h>
#include <range_utilities.h>

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include <cstring>
#include <iostream>
#include <iterator> // std::distance
#include <sstream>
#include <utility> // std::pair
#include <vector>

using namespace std;

namespace {

/// @brief defines if certain option may, must or must not appear
typedef enum {
    FORBIDDEN, MANDATORY, OPTIONAL
} RequirementLevel;

/// @brief Minimum length of a MAC address to be used in DUID generation.
static const size_t MIN_MAC_LEN = 6;

}; // anonymous namespace

namespace isc {
namespace dhcpMini {

//static const char* SERVER_DUID_FILE = "b10-dhcp6-mini-serverid";

Dhcpv6Srv::Dhcpv6Srv(uint16_t port) :
        alloc_engine_(), serverid_(), shutdown_(true), port_(port) {
    if (isc::dhcp::IfaceMgr::instance().countIfaces() == 0) {
        LOG(ERR)<< "There are no network interfaces available "
                "for DHCPv6 server" << endl;
        return;
    }
    // Create error handler. This handler will be called every time
    // the socket opening operation fails. We use this handler to
    // log a warning.
    isc::dhcp::IfaceMgrErrorMsgCallback error_handler =
            boost::bind(&Dhcpv6Srv::ifaceMgrSocket6ErrorHandler, _1);
    isc::dhcp::IfaceMgr::instance().openSockets6(port_, error_handler);

    generateServerID();
    LOG(INF) << "Generated DUID is " << duidToString(getServerID()) << endl;

    // Instantiate allocation engine
    alloc_engine_.reset(new AllocEngine(100));

    // All done, so can proceed
    shutdown_ = false;
}

Dhcpv6Srv::~Dhcpv6Srv() {
    isc::dhcp::IfaceMgr::instance().closeSockets();
}

void Dhcpv6Srv::shutdown() {
    LOG(DBG)<< "Shutting down the server..." << endl;
    shutdown_ = true;
}

isc::dhcp::Pkt6Ptr Dhcpv6Srv::receivePacket(int timeout) {
    return (isc::dhcp::IfaceMgr::instance().receive6(timeout));
}

void Dhcpv6Srv::sendPacket(const isc::dhcp::Pkt6Ptr& packet) {
    isc::dhcp::IfaceMgr::instance().send(packet);
}

bool Dhcpv6Srv::testServerID(const isc::dhcp::Pkt6Ptr& pkt) {
    isc::dhcp::OptionPtr server_id = pkt->getOption(D6O_SERVERID);
    if (server_id) {
        // Let us test received ServerID if it is same as ServerID
        // which is beeing used by server
        if (getServerID()->getData() != server_id->getData()) {
            LOG(DBG)<< "Dropping packet " << pkt->getName() << " (transid="
                    << pkt->getTransid() << ", interface=" << pkt->getIface()
                    << ") having mismatched server identifier" << endl;
            return (false);
        }
    }
    // retun True if: no serverid received or ServerIDs matching
    return (true);
}

bool Dhcpv6Srv::run() {
    while (!shutdown_) {
        /// @todo Calculate actual timeout to the next event (e.g. lease
        /// expiration) once we have lease database. The idea here is that
        /// it is possible to do everything in a single process/thread.
        /// For now, we are just calling select for 1000 seconds. There
        /// were some issues reported on some systems when calling select()
        /// with too large values. Unfortunately, I don't recall the details.
        //cppcheck-suppress variableScope This is temporary anyway
        const int timeout = 1000;

        // client's message and server's response
        isc::dhcp::Pkt6Ptr query;
        isc::dhcp::Pkt6Ptr rsp;

        query = receivePacket(timeout);

        // Timeout may be reached or signal received, which breaks select()
        // with no packet received
        if (!query) {
            continue;
        }

        // In order to parse the DHCP options, the server needs to use some
        // configuration information such as: existing option spaces, option
        // definitions etc. This is the kind of information which is not
        // available in the libdhcp, so we need to supply our own implementation
        // of the option parsing function here, which would rely on the
        // configuration data.
        query->setCallback(
                boost::bind(&Dhcpv6Srv::unpackOptions, this, _1, _2, _3));

        if (!query->unpack()) {
            LOG(DBG) << "Failed to parse incoming packet" << endl;
            continue;
        }

        // Check if received query carries server identifier matching
        // server identifier being used by the server.
        if (!testServerID(query)) {
            continue;
        }

        LOG(DBG)<< query->getName() << " packet received" << endl;
        LOG(DBG)<< "Received packet length "
                << static_cast<int>(query->getType())
                << ", data length " << query->getBuffer().getLength()
                << ", data is " << query->toText() << endl;

        switch (query->getType()) {
        case DHCPV6_SOLICIT:
            LOG(DBG)<< "Processing solicit" << endl;
            rsp = processSolicit(query);
            break;

        case DHCPV6_REQUEST:
            LOG(DBG)<< "Processing request" << endl;
            rsp = processRequest(query);
            break;

/*        case DHCPV6_RENEW:
            rsp = processRenew(query);
            break;*/

/*        case DHCPV6_REBIND:
            rsp = processRebind(query);
            break;*/

/*        case DHCPV6_CONFIRM:
            rsp = processConfirm(query);
            break;*/

        case DHCPV6_RELEASE:
            rsp = processRelease(query);
            break;

/*        case DHCPV6_DECLINE:
            rsp = processDecline(query);
            break;*/

/*        case DHCPV6_INFORMATION_REQUEST:
            rsp = processInfRequest(query);
            break;*/

        default:
            // We received a packet type that we do not recognize.
            LOG(DBG)<< "Received unknown message (type "
                    << static_cast<int>(query->getType())
                    << ") on interface " << query->getIface() << endl;
        }

        if (rsp) {
            rsp->setRemoteAddr(query->getRemoteAddr());
            rsp->setLocalAddr(query->getLocalAddr());

            if (rsp->relay_info_.empty()) {
                // Direct traffic, send back to the client directly
                rsp->setRemotePort(DHCP6_CLIENT_PORT);
            } else {
                // Relayed traffic, send back to the relay agent
                rsp->setRemotePort(DHCP6_SERVER_PORT);
            }

            rsp->setLocalPort(DHCP6_SERVER_PORT);
            rsp->setIndex(query->getIndex());
            rsp->setIface(query->getIface());

            LOG(DBG)<< "Responding with packet type "
                    << static_cast<int>(rsp->getType())
                    << " data is " << rsp->toText();

            rsp->pack();

            // Now all fields and options are constructed into output wire buffer.
            sendPacket(rsp);
        }
    }

    return (true);
}

void Dhcpv6Srv::generateServerID() {
    const isc::dhcp::IfaceMgr::IfaceCollection& ifaces =
            isc::dhcp::IfaceMgr::instance().getIfaces();

    // Let's find suitable interface.
    for (isc::dhcp::IfaceMgr::IfaceCollection::const_iterator iface =
            ifaces.begin(); iface != ifaces.end(); ++iface) {
        // MAC address should be at least 6 bytes. Although there is no such
        // requirement in any RFC, all decent physical interfaces (Ethernet,
        // WiFi, Infiniband, etc.) have 6 bytes long MAC address. We want to
        // base our DUID on real hardware address, rather than virtual
        // interface that pretends that underlying IP address is its MAC.
        if (iface->getMacLen() < MIN_MAC_LEN) {
            continue;
        }

        // Let's don't use loopback.
        if (iface->flag_loopback_) {
            continue;
        }

        // Let's skip downed interfaces. It is better to use working ones.
        if (!iface->flag_up_) {
            continue;
        }

        // Some interfaces (like lo on Linux) report 6-bytes long
        // MAC address 00:00:00:00:00:00. Let's not use such weird interfaces
        // to generate DUID.
        if (isRangeZero(iface->getMac(),
                iface->getMac() + iface->getMacLen())) {
            continue;
        }

        // Ok, we have useful MAC. Let's generate DUID-LLT based on
        // it. See RFC3315, Section 9.2 for details.

        // DUID uses seconds since midnight of 01-01-2000, time() returns
        // seconds since 01-01-1970. DUID_TIME_EPOCH substitution corrects that.
        time_t seconds = time(NULL);
        seconds -= DUID_TIME_EPOCH;

        isc::dhcp::OptionBuffer srvid(8 + iface->getMacLen());
        // We know that the buffer is more than 8 bytes long at this point.
        writeUint16(isc::dhcp::DUID::DUID_LLT, &srvid[0], 2);
        writeUint16(HWTYPE_ETHERNET, &srvid[2], 2);
        writeUint32(static_cast<uint32_t>(seconds), &srvid[4], 4);
        memcpy(&srvid[8], iface->getMac(), iface->getMacLen());

        serverid_ = isc::dhcp::OptionPtr(
                new isc::dhcp::Option(isc::dhcp::Option::V6, D6O_SERVERID,
                        srvid.begin(), srvid.end()));

        return;
    }

    // If we reached here, there are no suitable interfaces found.
    // Either interface detection is not supported on this platform or
    // this is really weird box. Let's use DUID-EN instead.
    // See Section 9.3 of RFC3315 for details.

    isc::dhcp::OptionBuffer srvid(12);
    writeUint16(isc::dhcp::DUID::DUID_EN, &srvid[0], srvid.size());
    writeUint32(ENTERPRISE_ID_ISC, &srvid[2], srvid.size() - 2);

    // Length of the identifier is company specific. I hereby declare
    // ISC "standard" of 6 bytes long pseudo-random numbers.
    srandom (time(NULL));fillRandom
    (&srvid[6], &srvid[12]);

    serverid_ = isc::dhcp::OptionPtr(
            new isc::dhcp::Option(isc::dhcp::Option::V6, D6O_SERVERID,
                    srvid.begin(), srvid.end()));
}

void Dhcpv6Srv::copyDefaultOptions(const isc::dhcp::Pkt6Ptr& question,
        isc::dhcp::Pkt6Ptr& answer) {
    // Add client-id.
    isc::dhcp::OptionPtr clientid = question->getOption(D6O_CLIENTID);
    if (clientid) {
        answer->addOption(clientid);
    }
}

void Dhcpv6Srv::appendDefaultOptions(const isc::dhcp::Pkt6Ptr&,
        isc::dhcp::Pkt6Ptr& answer) {
    // add server-id
    answer->addOption(getServerID());
}

void Dhcpv6Srv::appendRequestedOptions(const isc::dhcp::Pkt6Ptr& question,
        isc::dhcp::Pkt6Ptr& answer) {
    // Get the configured subnet suitable for the incoming packet.
    Subnet6Ptr subnet = selectSubnet(question);
    // Leave if there is no subnet matching the incoming packet.
    // There is no need to log the error message here because
    // it will be logged in the assignLease() when it fails to
    // pick the suitable subnet. We don't want to duplicate
    // error messages in such case.
    if (!subnet) {
        return;
    }

    // Client requests some options using ORO option. Try to
    // get this option from client's message.
    boost::shared_ptr < isc::dhcp::OptionIntArray<uint16_t> > option_oro =
            boost::dynamic_pointer_cast < isc::dhcp::OptionIntArray<uint16_t>
    > (question->getOption(D6O_ORO));
    // Option ORO not found. Don't do anything then.
    if (!option_oro) {
        return;
    }
    // Get the list of options that client requested.
    const std::vector<uint16_t>& requested_opts = option_oro->getValues();
    BOOST_FOREACH(uint16_t opt, requested_opts) {
        Subnet::OptionDescriptor desc = subnet->getOptionDescriptor("dhcp6", opt);
        if (desc.option) {
            answer->addOption(desc.option);
        }
    }
}

isc::dhcp::OptionPtr Dhcpv6Srv::createStatusCode(uint16_t code,
        const std::string& text) {
    // Get the definition of the option holding status code.
    isc::dhcp::OptionDefinitionPtr status_code_def =
            isc::dhcp::LibDHCP::getOptionDef(isc::dhcp::Option::V6,
                    D6O_STATUS_CODE);
    // This definition is assumed to be initialized in LibDHCP.
    assert(status_code_def);

    // As there is no dedicated class to represent Status Code
    // the OptionCustom class is used here instead.
    isc::dhcp::OptionCustomPtr option_status = isc::dhcp::OptionCustomPtr(
            new isc::dhcp::OptionCustom(*status_code_def,
                    isc::dhcp::Option::V6));
    assert(option_status);

    // Set status code to 'code' (0 - means data field #0).
    option_status->writeInteger(code, 0);
    // Set a message (1 - means data field #1).
    option_status->writeString(text, 1);
    return (option_status);
}

Subnet6Ptr Dhcpv6Srv::selectSubnet(const isc::dhcp::Pkt6Ptr& question) {
    Subnet6Ptr subnet;

    if (question->relay_info_.empty()) {
        // This is a direct (non-relayed) message
        // Try to find a subnet
        subnet = isc::dhcpMini::CfgMgr::getInstance().getSubnet6();

        if (!subnet) {
            // If no subnet was found, just report it
            // (shall be handled elsewhere)
            LOG(WRN)<< "Failed to select a subnet for incoming packet, src="
                    << question->getRemoteAddr().toText() << "type="
                    << question->getName() << endl;
        }
    } else {
        // This is a relayed message
        LOG(DBG)<< "Relayed messages are not supported" << endl;
    }

    return (subnet);
}

void Dhcpv6Srv::assignLeases(const isc::dhcp::Pkt6Ptr& question,
        isc::dhcp::Pkt6Ptr& answer) {

    // We need to allocate addresses for all IA_NA options in the client's
    // question (i.e. SOLICIT or REQUEST) message.

    // We need to select a subnet the client is connected in.
    Subnet6Ptr subnet = selectSubnet(question);
    if (!subnet) {
        // This particular client is out of luck today. We do not have
        // information about the subnet he is connected to. This likely means
        // misconfiguration of the server (or some relays). We will continue to
        // process this message, but our response will be almost useless: no
        // addresses or prefixes, no subnet specific configuration etc.
    } else {
        LOG(DBG)<< "The " << subnet->toText()
                << "subnet was selected for client assignment" << endl;
    }

    // Let's find client's DUID. Client is supposed to include its client-id
    // option almost all the time (the only exception is an anonymous inf-request,
    // but that is mostly a theoretical case). Our allocation engine needs DUID
    // and will refuse to allocate anything to anonymous clients.
    isc::dhcp::DuidPtr duid;
    isc::dhcp::OptionPtr opt_duid = question->getOption(D6O_CLIENTID);
    if (opt_duid) {
        duid = isc::dhcp::DuidPtr(new isc::dhcp::DUID(opt_duid->getData()));
    } else {
        // Let's drop the message. This client is not sane.
        LOG(ERR)<< "Mandatory client-id option is missing, message from "
                << question->getRemoteAddr().toText()
                << "dropped" << endl;
        return;
    }

    // Now that we have all information about the client, let's iterate over all
    // received options and handle IA_NA options one by one and store our
    // responses in answer message (ADVERTISE or REPLY).
    for (isc::dhcp::OptionCollection::iterator opt = question->options_.begin();
            opt != question->options_.end(); ++opt) {
        switch (opt->second->getType()) {
        case D6O_IA_NA: {
            isc::dhcp::OptionPtr answer_opt = assignIA_NA(
                    subnet,
                    duid,
                    question,
                    boost::dynamic_pointer_cast <isc::dhcp::Option6IA> (opt->second));
            if (answer_opt) {
                answer->addOption(answer_opt);
            }
            break;
        }
        default:
            break;
        }
    }
}

isc::dhcp::OptionPtr Dhcpv6Srv::assignIA_NA(const Subnet6Ptr& subnet,
        const isc::dhcp::DuidPtr& duid, const isc::dhcp::Pkt6Ptr& query,
        boost::shared_ptr<isc::dhcp::Option6IA> ia) {
    // If there is no subnet selected for handling this IA_NA, the only thing to do left is
    // to say that we are sorry, but the user won't get an address. As a convenience, we
    // use a different status text to indicate that (compare to the same status code,
    // but different wording below)
    if (!subnet) {
        // Create empty IA_NA option with IAID matching the request.
        // Note that we don't use OptionDefinition class to create this option.
        // This is because we prefer using a constructor of Option6IA that
        // initializes IAID. Otherwise we would have to use setIAID() after
        // creation of the option which has some performance implications.
        boost::shared_ptr<isc::dhcp::Option6IA> ia_rsp(
                new isc::dhcp::Option6IA(D6O_IA_NA, ia->getIAID()));

        // Insert status code NoAddrsAvail.
        ia_rsp->addOption(createStatusCode(STATUS_NoAddrsAvail,
                "Sorry, no subnet available."));
        return (ia_rsp);
    }

    // Check if the client sent us a hint in his IA_NA. Clients may send an
    // address in their IA_NA options as a suggestion (e.g. the last address
    // they used before).
    boost::shared_ptr<isc::dhcp::Option6IAAddr> hint_opt =
            boost::dynamic_pointer_cast<isc::dhcp::Option6IAAddr>(ia->getOption(D6O_IAADDR));
    isc::asiolink::IOAddress hint("::");
    if (hint_opt) {
        hint = (isc::asiolink::IOAddress)hint_opt->getAddress();
    }

    LOG(DBG)<< "Server is processing IA_NA option (duid="
            << (duid ? duid->toText() : "(no-duid)")
            << ", iaid=" << ia->getIAID()
            << ", hint=" << (hint_opt ? hint.toText() : "(no hint)")
            << ")" << endl;

    // "Fake" allocation is processing of SOLICIT message. We pretend to do an
    // allocation, but we do not put the lease in the database. That is ok,
    // because we do not guarantee that the user will get that exact lease. If
    // the user selects this server to do actual allocation (i.e. sends REQUEST)
    // it should include this hint. That will help us during the actual lease
    // allocation.
    bool fake_allocation = false;
    if (query->getType() == DHCPV6_SOLICIT) {
        fake_allocation = true;
    }

    // Use allocation engine to pick a lease for this client. Allocation engine
    // will try to honour the hint, but it is just a hint - some other address
    // may be used instead. If fake_allocation is set to false, the lease will
    // be inserted into the LeaseMgr as well.
    Lease6Collection old_leases;
    Lease6Collection leases = alloc_engine_->allocateLeases6(subnet, duid,
            ia->getIAID(),
            hint, Lease::TYPE_NA,
            fake_allocation,
            old_leases);

    Lease6Ptr lease;
    if (!leases.empty()) {
        lease = *leases.begin();
    }

    // Create IA_NA that we will put in the response.
    // Do not use OptionDefinition to create option's instance so
    // as we can initialize IAID using a constructor.
            boost::shared_ptr<isc::dhcp::Option6IA> ia_rsp(
                    new isc::dhcp::Option6IA(D6O_IA_NA, ia->getIAID()));

            if (lease) {
                // We have a lease! Let's wrap its content into IA_NA option
                // with IAADDR suboption.
                if (fake_allocation) {
                    LOG(DBG)<< "Address lease " << lease->addr_.toText()
                            << " advertised (client duid="
                            << (duid ? duid->toText() : "(no-duid)")
                            << ", iaid=" << ia->getIAID()
                            << ")" << endl;
                } else {
                    LOG(DBG)<< "Address lease " << lease->addr_.toText()
                            << " has been allocated (client duid="
                            << (duid ? duid->toText() : "(no-duid)")
                            << ", iaid=" << ia->getIAID()
                            << ")" << endl;
                }

                ia_rsp->setT1(subnet->getT1());
                ia_rsp->setT2(subnet->getT2());

                boost::shared_ptr < isc::dhcp::Option6IAAddr> addr(
                        new isc::dhcp::Option6IAAddr(
                                D6O_IAADDR,
                                lease->addr_,
                                lease->preferred_lft_,
                                lease->valid_lft_));
                ia_rsp->addOption(addr);

                // It would be possible to insert status code=0(success) as well,
                // but this is considered waste of bandwidth as absence of status
                // code is considered a success.

                Lease6Ptr old_lease;
                if (!old_leases.empty()) {
                    old_lease = *old_leases.begin();
                }
            } else {
                // Allocation engine did not allocate a lease. The engine logged
                // cause of that failure. The only thing left is to insert
                // status code to pass the sad news to the client.
                if (fake_allocation) {
                    LOG(DBG)<< "Failed to advertise an address lease for client duid="
                            << (duid ? duid->toText() : "(no-duid)")
                            << ", iaid=" << ia->getIAID() << endl;
                } else {
                    LOG(DBG)<< "Failed to grant an address lease for client duid="
                            << (duid ? duid->toText() : "(no-duid)")
                            << ", iaid=" << ia->getIAID() << endl;
                }

                ia_rsp->addOption(createStatusCode(STATUS_NoAddrsAvail,
                        "Sorry, no address could be allocated."));
            }
            return (ia_rsp);
}

void Dhcpv6Srv::releaseLeases(const isc::dhcp::Pkt6Ptr& release,
        isc::dhcp::Pkt6Ptr& reply) {

    // We need to release addresses for all IA_NA options in the client's
    // RELEASE message.

    // Let's find client's DUID. Client is supposed to include its client-id
    // option almost all the time (the only exception is an anonymous inf-request,
    // but that is mostly a theoretical case). Our allocation engine needs DUID
    // and will refuse to allocate anything to anonymous clients.
    isc::dhcp::OptionPtr opt_duid = release->getOption(D6O_CLIENTID);
    if (!opt_duid) {
        // This should not happen. We have checked this before.
        // see sanityCheck() called from processRelease()
        LOG(WRN)<< "Client (address=" << release->getRemoteAddr().toText()
                << ") sent RELEASE message without mandatory client-id" << endl;

        reply->addOption(createStatusCode(STATUS_UnspecFail,
                "You did not include mandatory client-id"));
        return;
    }
    isc::dhcp::DuidPtr duid(new isc::dhcp::DUID(opt_duid->getData()));

    // Let's set the status to be success by default. We can override it with
    // error status if needed. The important thing to understand here is that
    // the global status code may be set to success only if all IA options were
    // handled properly. Therefore the releaseIA_NA option may turn the status
    // code to some error, but can't turn it back to success.
    int general_status = STATUS_Success;
    for (isc::dhcp::OptionCollection::iterator opt = release->options_.begin();
            opt != release->options_.end(); ++opt) {
        switch (opt->second->getType()) {
        case D6O_IA_NA: {
            isc::dhcp::OptionPtr answer_opt = releaseIA_NA(
                    duid,
                    general_status,
                    boost::dynamic_pointer_cast < isc::dhcp::Option6IA> (opt->second));
            if (answer_opt) {
                reply->addOption(answer_opt);
            }
            break;
        }
        default:
            // remaining options are ignored in this context
            ;
        }
    }

    // To be pedantic, we should also include status code in the top-level
    // scope, not just in each IA_NA. See RFC3315, section 18.2.6.
    // This behavior will likely go away in RFC3315bis.
    reply->addOption(createStatusCode(general_status,
            "Summary status for all processed IA_NAs"));
}

isc::dhcp::OptionPtr Dhcpv6Srv::releaseIA_NA(const isc::dhcp::DuidPtr& duid,
        int& general_status, boost::shared_ptr<isc::dhcp::Option6IA> ia) {
    // Release can be done in one of two ways:
    // Approach 1: extract address from client's IA_NA and see if it belongs
    // to this particular client.
    // Approach 2: find a subnet for this client, get a lease for
    // this subnet/duid/iaid and check if its content matches to what the
    // client is asking us to release.
    //
    // This method implements approach 1.

    // That's our response
    boost::shared_ptr<isc::dhcp::Option6IA> ia_rsp(
            new isc::dhcp::Option6IA(D6O_IA_NA, ia->getIAID()));

    boost::shared_ptr<isc::dhcp::Option6IAAddr> release_addr =
            boost::dynamic_pointer_cast < isc::dhcp::Option6IAAddr
            > (ia->getOption(D6O_IAADDR));
    if (!release_addr) {
        ia_rsp->addOption(createStatusCode(STATUS_NoBinding,
                "You did not include an address in your RELEASE"));
        general_status = STATUS_NoBinding;
        return (ia_rsp);
    }

    Lease6Ptr lease = LeaseMgr::getInstance().getLease6(Lease::TYPE_NA,
            release_addr->getAddress());

    if (!lease) {
        // client releasing a lease that we don't know about.

        // Insert status code NoAddrsAvail.
        ia_rsp->addOption(createStatusCode(STATUS_NoBinding,
                "Sorry, no known leases for this duid/iaid, can't release."));
        general_status = STATUS_NoBinding;

        LOG(INF)<< "Received RELEASE from unknown client (IA_NA, duid="
                << duid->toText() << ", iaid=" << ia->getIAID() << ")" << endl;

        return (ia_rsp);
    }

    if (!lease->duid_) {
        // Something is gravely wrong here. We do have a lease, but it does not
        // have mandatory DUID information attached. Someone was messing with our
        // database.
        LOG(ERR)<< "Address lease for address "
                << release_addr->getAddress().toText()
                << " does not have a DUID" << endl;

        general_status = STATUS_UnspecFail;
        ia_rsp->addOption(createStatusCode(STATUS_UnspecFail,
                "Database consistency check failed when trying to RELEASE"));

        return (ia_rsp);
    }

    if (*duid != *(lease->duid_)) {

        // Sorry, it's not your address. You can't release it.
        LOG(INF)<< "Client (duid=" << duid->toText()
                << ") tried to release address "
                << release_addr->getAddress().toText()
                << ", but it belongs to another client (duid="
                << lease->duid_->toText()
                << ")" << endl;

        general_status = STATUS_NoBinding;
        ia_rsp->addOption(createStatusCode(STATUS_NoBinding,
                "This address does not belong to you, you can't release it"));
        return (ia_rsp);
    }

    if (ia->getIAID() != lease->iaid_) {
        // This address belongs to this client, but to a different IA
        LOG(WRN)<< "Client (duid=" << duid->toText()
                << ") tried to release prefix "
                << release_addr->getAddress().toText()
                << ", but it used wrong IAID (expected " << lease->iaid_
                << ", but got " << ia->getIAID()
                << ")" << endl;

        ia_rsp->addOption(createStatusCode(STATUS_NoBinding,
                "This is your address, but you used wrong IAID"));
        general_status = STATUS_NoBinding;

        return (ia_rsp);
    }

    // It is not necessary to check if the address matches as we used
    // getLease6(addr) method that is supposed to return a proper lease.

    // Ok, we've passed all checks. Let's release this address.
    bool success = LeaseMgr::getInstance().deleteLease(lease->addr_);

    // Here the success should be true if we removed lease successfully
    // and false if skip flag was set or the removal failed for whatever reason
    if (!success) {
        ia_rsp->addOption(createStatusCode(STATUS_UnspecFail,
                "Server failed to release a lease"));

        LOG(ERR)<< "Failed to remove address lease for address "
                << lease->addr_.toText()
                << " for duid=" << duid->toText()
                << ", iaid=" << lease->iaid_ << endl;
        general_status = STATUS_UnspecFail;

        return (ia_rsp);
    } else {
        LOG(DBG)<< "Address " << lease->addr_.toText()
                << " belonging to client duid=" << duid->toText()
                << ", iaid=" << lease->iaid_
                << " was released properly" << endl;

        ia_rsp->addOption(createStatusCode(STATUS_Success,
                "Lease released. Thank you, please come again."));

        return (ia_rsp);
    }
}

isc::dhcp::Pkt6Ptr Dhcpv6Srv::processSolicit(
        const isc::dhcp::Pkt6Ptr& solicit) {

//    sanityCheck(solicit, MANDATORY, FORBIDDEN);

    LOG(DBG)<< "Entering the processSolicit method" << endl;
    isc::dhcp::Pkt6Ptr advertise(new isc::dhcp::Pkt6(
            DHCPV6_ADVERTISE, solicit->getTransid()));

    copyDefaultOptions(solicit, advertise);
    appendDefaultOptions(solicit, advertise);
    appendRequestedOptions(solicit, advertise);

    assignLeases(solicit, advertise);

    return (advertise);
}

isc::dhcp::Pkt6Ptr Dhcpv6Srv::processRequest(
        const isc::dhcp::Pkt6Ptr& request) {

//    sanityCheck(request, MANDATORY, MANDATORY);

    LOG(DBG)<< "Entering the processRequest method" << endl;
    isc::dhcp::Pkt6Ptr reply(new isc::dhcp::Pkt6(
            DHCPV6_REPLY, request->getTransid()));

    copyDefaultOptions(request, reply);
    appendDefaultOptions(request, reply);
    appendRequestedOptions(request, reply);

    assignLeases(request, reply);

    return (reply);
}

isc::dhcp::Pkt6Ptr Dhcpv6Srv::processRelease(
        const isc::dhcp::Pkt6Ptr& release) {

//    sanityCheck(release, MANDATORY, MANDATORY);

    isc::dhcp::Pkt6Ptr reply(
            new isc::dhcp::Pkt6(DHCPV6_REPLY, release->getTransid()));

    copyDefaultOptions(release, reply);
    appendDefaultOptions(release, reply);

    releaseLeases(release, reply);

    return (reply);
}

std::string Dhcpv6Srv::duidToString(const isc::dhcp::OptionPtr& opt) {
    stringstream tmp;

    isc::dhcp::OptionBuffer data = opt->getData();

    bool colon = false;
    for (isc::dhcp::OptionBufferConstIter it = data.begin(); it != data.end();
            ++it) {
        if (colon) {
            tmp << ":";
        }
        tmp << hex << setw(2) << setfill('0') << static_cast<uint16_t>(*it);
        if (!colon) {
            colon = true;
        }
    }

    return tmp.str();
}

size_t Dhcpv6Srv::unpackOptions(const isc::dhcp::OptionBuffer& buf,
        const std::string& option_space, isc::dhcp::OptionCollection& options) {
    size_t offset = 0;
    size_t length = buf.size();

    isc::dhcp::OptionDefContainer option_defs;
    if (option_space == "dhcp6") {
        // Get the list of stdandard option definitions.
        option_defs = isc::dhcp::LibDHCP::getOptionDefs(isc::dhcp::Option::V6);
    } else if (!option_space.empty()) {
        LOG(DBG)<< "Unsupported option space " << option_space << endl;
        return (offset);
    }

    // Get the search index #1. It allows to search for option definitions
    // using option code.
    const isc::dhcp::OptionDefContainerTypeIndex& idx = option_defs.get<1>();

    // The buffer being read comprises a set of options, each starting with
    // a two-byte type code and a two-byte length field.
    while (offset + 4 <= length) {
        // At this point, from the while condition, we know that there
        // are at least 4 bytes available following offset in the
        // buffer.
        uint16_t opt_type = readUint16(&buf[offset], 2);
        offset += 2;

        uint16_t opt_len = readUint16(&buf[offset], 2);
        offset += 2;

        if (offset + opt_len > length) {
            return (offset);
        }

        if (opt_type == D6O_RELAY_MSG) {
            LOG(DBG)<< "Rely message options are not supported" << endl;
            // do not create that relay-msg option
            offset += opt_len;
            continue;
        }

        // Get all definitions with the particular option code. Note that option
        // code is non-unique within this container however at this point we
        // expect to get one option definition with the particular code. If more
        // are returned we report an error.
        const isc::dhcp::OptionDefContainerTypeRange& range = idx.equal_range(opt_type);
        // Get the number of returned option definitions for the option code.
        size_t num_defs = std::distance(range.first, range.second);

        isc::dhcp::OptionPtr opt;
        if (num_defs > 1) {
            // Multiple options of the same code are not supported right now!
            LOG(ERR)<< "Internal error: multiple option definitions"
                    " for option type " << opt_type
                    << " returned. Currently it is not"
                    " supported to initialize multiple option definitions"
                    " for the same option code. This will be supported once"
                    " support for option spaces is implemented" << endl;
        } else if (num_defs == 0) {
            // @todo Don't crash if definition does not exist because only a few
            // option definitions are initialized right now. In the future
            // we will initialize definitions for all options and we will
            // remove this elseif. For now, return generic option.
            opt = isc::dhcp::OptionPtr(
                    new isc::dhcp::Option(isc::dhcp::Option::V6, opt_type,
                            buf.begin() + offset,
                            buf.begin() + offset + opt_len));
            opt->setEncapsulatedSpace("dhcp6");
        } else {
            // The option definition has been found. Use it to create
            // the option instance from the provided buffer chunk.
            const isc::dhcp::OptionDefinitionPtr& def = *(range.first);
            assert(def);
            opt = def->optionFactory(isc::dhcp::Option::V6, opt_type,
                    buf.begin() + offset,
                    buf.begin() + offset + opt_len,
                    boost::bind(&Dhcpv6Srv::unpackOptions, this, _1, _2, _3));
        }
        // add option to options
        options.insert(std::make_pair(opt_type, opt));
        offset += opt_len;
    }

    return (offset);
}

void Dhcpv6Srv::ifaceMgrSocket6ErrorHandler(const std::string& errmsg) {
    // Log the reason for socket opening failure and return.
    LOG(WRN)<< errmsg << endl;
}

}   // namespace dhcpMini
}   // namespace isc
