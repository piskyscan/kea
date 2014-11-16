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

#ifndef DHCPV6_MINI_SRV_H
#define DHCPV6_MINI_SRV_H

#include <alloc_engine.h>
#include <subnet.h>
#include <dhcp/dhcp6.h>
#include <dhcp/duid.h>
#include <dhcp/option.h>
#include <dhcp/option6_ia.h>
#include <dhcp/pkt6.h>

#include <boost/shared_ptr.hpp>

#include <string>

namespace isc {
namespace dhcpMini {

/// @brief DHCPv6 server service.
///
/// This class represents DHCPv6 server. It contains all
/// top-level methods and routines necessary for server operation.
/// In particular, it instantiates IfaceMgr, loads or generates DUID
/// that is going to be used as server-identifier, receives incoming
/// packets, processes them, manages leases assignment and generates
/// appropriate responses.
///
/// @note Only one instance of this class is instantiated as it encompasses
///       the whole operation of the server.  Nothing, however, enforces the
///       singleton status of the object.
class Dhcpv6Srv {
public:
	/// @brief Constructor
	///
	/// @param port UDP port to be opened for DHCP traffic
	Dhcpv6Srv(uint16_t port = DHCP6_SERVER_PORT);

	/// @brief Destructor.
	~Dhcpv6Srv();

	/// @brief Returns server-indentifier option.
	///
	/// @return server-id option
	isc::dhcp::OptionPtr getServerID() {
		return serverid_;
	}

	/// @brief Main server processing loop.
	///
	/// Main server processing loop. Receives incoming packets, verifies
	/// their correctness, generates appropriate answer (if needed) and
	/// transmits responses.
	///
	/// @return true, if being shut down gracefully, fail if experienced
	///         critical error.
	bool run();

	/// @brief Instructs the server to shut down.
	void shutdown();

private:
	/// @brief Compare received server id with our server id
	///
	/// Checks if the server id carried in a query from a client matches
	/// server identifier being used by the server.
	///
	/// @param pkt DHCPv6 packet carrying server identifier to be checked.
	/// @return true if server id carried in the query matches server id
	/// used by the server; false otherwise.
	bool testServerID(const isc::dhcp::Pkt6Ptr& pkt);

	/// @brief Processes incoming SOLICIT and returns response.
	///
	/// Processes received SOLICIT message and verifies that its sender
	/// should be served. In particular IA, TA and PD options are populated
	/// with to-be assigned addresses, temporary addresses and delegated
	/// prefixes, respectively. In the usual 4 message exchange, server is
	/// expected to respond with ADVERTISE message. However, if client
	/// requests rapid-commit and server supports it, REPLY will be sent
	/// instead of ADVERTISE and requested leases will be assigned
	/// immediately.
	///
	/// @param solicit SOLICIT message received from client
	///
	/// @return ADVERTISE, REPLY message or NULL
	isc::dhcp::Pkt6Ptr processSolicit(const isc::dhcp::Pkt6Ptr& solicit);

	/// @brief Processes incoming REQUEST and returns REPLY response.
	///
	/// Processes incoming REQUEST message and verifies that its sender
	/// should be served. In particular IA, TA and PD options are populated
	/// with assigned addresses, temporary addresses and delegated
	/// prefixes, respectively. Uses LeaseMgr to allocate or update existing
	/// leases.
	///
	/// @param request a message received from client
	///
	/// @return REPLY message or NULL
	isc::dhcp::Pkt6Ptr processRequest(const isc::dhcp::Pkt6Ptr& request);

    /// @brief Stub function that will handle incoming RELEASE messages.
    ///
    /// @param release message received from client
	isc::dhcp::Pkt6Ptr processRelease(const isc::dhcp::Pkt6Ptr& release);

    /// @brief Creates status-code option.
	///
	/// @param code status code value (see RFC3315)
	/// @param text textual explanation (will be sent in status code option)
	/// @return status-code option
	isc::dhcp::OptionPtr createStatusCode(uint16_t code, const std::string& text);

	/// @brief Selects a subnet for a given client's packet.
	///
	/// @param question client's message
	/// @return selected subnet (or NULL if no suitable subnet was found)
	isc::dhcpMini::Subnet6Ptr selectSubnet(const isc::dhcp::Pkt6Ptr& question);

	/// @brief Processes IA_NA option (and assigns addresses if necessary).
	///
	/// Generates response to IA_NA. This typically includes selecting (and
	/// allocating a lease in case of REQUEST) an address lease and creating
	/// IAADDR option. In case of allocation failure, it may contain
	/// status code option with non-zero status, denoting cause of the
	/// allocation failure.
	///
	/// @param subnet subnet the client is connected to
	/// @param duid client's duid
	/// @param query client's message (typically SOLICIT or REQUEST)
	/// @param answer server's response to the client's message.
	/// @param ia pointer to client's IA_NA option (client's request)
	///
	/// @return IA_NA option (server's response)
	isc::dhcp::OptionPtr assignIA_NA(const isc::dhcpMini::Subnet6Ptr& subnet,
			const isc::dhcp::DuidPtr& duid,
			const isc::dhcp::Pkt6Ptr& query,
			isc::dhcp::Option6IAPtr ia);

	/// @brief Releases specific IA_NA option
	///
	/// Generates response to IA_NA in Release message. This covers finding and
	/// removal of a lease that corresponds to the received address. If no such
	/// lease is found, an IA_NA response is generated with an appropriate
	/// status code.
	///
	/// As RFC 3315 requires that a single status code be sent for the whole message,
	/// this method may update the passed general_status: it is set to SUCCESS when
	/// message processing begins, but may be updated to some error code if the
	/// release process fails.
	///
	/// @param duid client's duid
	/// @param query client's message
	/// @param general_status a global status (it may be updated in case of errors)
	/// @param ia IA_NA option that is being released
	/// @return IA_NA option (server's response)
	isc::dhcp::OptionPtr releaseIA_NA(const isc::dhcp::DuidPtr& duid,
			int& general_status, boost::shared_ptr<isc::dhcp::Option6IA> ia);

    /// @brief Copies required options from client message to server answer.
	///
	/// Copies options that must appear in any server response (ADVERTISE, REPLY)
	/// to client's messages (SOLICIT, REQUEST, RENEW, REBIND, DECLINE, RELEASE).
	/// One notable example is client-id. Other options may be copied as required.
	///
	/// @param question client's message (options will be copied from here)
	/// @param answer server's message (options will be copied here)
	void copyDefaultOptions(const isc::dhcp::Pkt6Ptr& question,
			isc::dhcp::Pkt6Ptr& answer);

	/// @brief Appends default options to server's answer.
	///
	/// Adds required options to server's answer. In particular, server-id
	/// is added. Possibly other mandatory options will be added, depending
	/// on type (or content) of client message.
	///
	/// @param question client's message
	/// @param answer server's message (options will be added here)
	void appendDefaultOptions(const isc::dhcp::Pkt6Ptr& question,
			isc::dhcp::Pkt6Ptr& answer);

	/// @brief Appends requested options to server's answer.
	///
	/// Appends options requested by client to the server's answer.
	///
	/// @param question client's message
	/// @param answer server's message (options will be added here)
	void appendRequestedOptions(const isc::dhcp::Pkt6Ptr& question,
			isc::dhcp::Pkt6Ptr& answer);

	/// @brief Assigns leases.
	///
	/// It supports addresses (IA_NA) only. It does NOT support temporary
	/// addresses (IA_TA) nor prefixes (IA_PD).
	///
	/// @param question client's message (with requested IA_NA)
	/// @param answer server's message (IA_NA options will be added here).
	void assignLeases(const isc::dhcp::Pkt6Ptr& question,
			isc::dhcp::Pkt6Ptr& answer);

    /// @brief Attempts to release received addresses
    ///
    /// It iterates through received IA_NA options and attempts to release
    /// received addresses. If no such leases are found, or the lease fails
    /// proper checks (e.g. belongs to someone else), a proper status
    /// code is added to reply message. Released addresses are not added
    /// to REPLY packet, just its IA_NA containers.
    /// @param release client's message asking to release
    /// @param reply server's response
	void releaseLeases(const isc::dhcp::Pkt6Ptr& release,
			isc::dhcp::Pkt6Ptr& reply);

	/// @brief Sets server-identifier.
	///
	/// This method attempts to generate server-identifier DUID. It generates a
	/// new DUID using interface link-layer addresses (EUI-64) + timestamp (DUID
	/// type duid-llt, see RFC3315, section 9.2).
	void generateServerID();

	/// @brief converts DUID to text
	/// Converts content of DUID option to a text representation, e.g.
	/// 01:ff:02:03:06:80:90:ab:cd:ef
	///
	/// @param opt option that contains DUID
	/// @return string representation
	static std::string duidToString(const isc::dhcp::OptionPtr& opt);

	/// @brief dummy wrapper around IfaceMgr::receive6
	///
	/// This method is useful for testing purposes, where its replacement
	/// simulates reception of a packet. For that purpose it is protected.
	isc::dhcp::Pkt6Ptr receivePacket(int timeout);

	/// @brief dummy wrapper around IfaceMgr::send()
	///
	/// This method is useful for testing purposes, where its replacement
	/// simulates transmission of a packet. For that purpose it is protected.
	void sendPacket(const isc::dhcp::Pkt6Ptr& pkt);

	/// @brief Implements a callback function to parse options in the message.
	///
	/// @param buf a A buffer holding options in on-wire format.
	/// @param option_space A name of the option space which holds definitions
	/// of to be used to parse options in the packets.
	/// @param [out] options A reference to the collection where parsed options
	/// will be stored.
	/// @return An offset to the first byte after last parsed option.
	size_t unpackOptions(const isc::dhcp::OptionBuffer& buf,
			const std::string& option_space,
			isc::dhcp::OptionCollection& options);

	/// @brief Implements the error handler for socket open failure.
	///
	/// This callback function is installed on the @c isc::dhcp::IfaceMgr
	/// when IPv6 sockets are being open. When socket fails to open for
	/// any reason, this function is called. It simply logs the error message.
	///
	/// @param errmsg An error message containing a cause of the failure.
	static inline void ifaceMgrSocket6ErrorHandler(const std::string& errmsg);

	/// @brief Allocation Engine.
	/// Pointer to the allocation engine that we are currently using
	/// It must be a pointer, because we will support changing engines
	/// during normal operation (e.g. to use different allocators)
	boost::shared_ptr<AllocEngine> alloc_engine_;

	/// Server DUID (to be sent in server-identifier option)
	isc::dhcp::OptionPtr serverid_;

	/// Indicates if shutdown is in progress. Setting it to true will
	/// initiate server shutdown procedure.
	volatile bool shutdown_;

	/// UDP port number on which server listens.
	uint16_t port_;
};

}; // end of isc::dhcpMini namespace
}; // end of isc namespace

#endif // DHCP6_MINI_SRV_H
