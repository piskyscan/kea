// Copyright (C) 2012-2014 Internet Systems Consortium, Inc. ("ISC")
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

#ifndef LEASE_MGR_H
#define LEASE_MGR_H

#include <dhcp6-mini_log.h>
#include <asiolink/io_address.h>
#include <lease.h>
#include <subnet.h>
#include <dhcp/duid.h>
#include <dhcp/option.h>
#include <dhcp/hwaddr.h>

#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>

namespace isc {
namespace dhcpMini {

/// @brief Lease Manager
///
/// This is an API for lease database.
class LeaseMgr {
public:
    static LeaseMgr& getInstance();

    /// @brief Adds an IPv6 lease.
    ///
    /// @param lease lease to be added
    ///
    /// @result true if the lease was added, false if not (because a lease
    ///         with the same address was already there).
    bool addLease(const Lease6Ptr& lease);

    /// @brief Returns existing IPv6 lease for a given IPv6 address.
    ///
    /// For a given address, we assume that there will be only one lease.
    /// The assumption here is that there will not be site or link-local
    /// addresses used, so there is no way of having address duplication.
    ///
    /// @param type specifies lease type: (NA, TA or PD)
    /// @param addr address of the searched lease
    ///
    /// @return smart pointer to the lease (or NULL if a lease is not found)
    Lease6Ptr getLease6(Lease::Type type,
            const isc::asiolink::IOAddress& addr) const;

    /// @brief Returns existing IPv6 lease for a given DUID+IA combination
    ///
    /// There may be more than one address, temp. address or prefix
    /// for specified duid/iaid/subnet-id tuple.
    ///
    /// @param type specifies lease type: (NA, TA or PD)
    /// @param duid client DUID
    /// @param iaid IA identifier
    /// @param subnet_id subnet id of the subnet the lease belongs to
    ///
    /// @return Lease collection (may be empty if no lease is found)
    Lease6Collection getLeases6(Lease::Type type, const isc::dhcp::DUID& duid,
            uint32_t iaid, SubnetID subnet_id) const;

    /// @brief Updates IPv6 lease.
    ///
    /// @param lease6 The lease to be updated.
    void updateLease6(const Lease6Ptr& lease6);

    /// @brief Deletes IPv6 lease.
    ///
    /// @param addr Address of the lease to be deleted.
    ///
    /// @return true if deletion was successful, false if no such lease exists
    bool deleteLease(const isc::asiolink::IOAddress& addr);

private:
    /// @brief Constructor
    LeaseMgr();

    /// @brief Destructor
    ~LeaseMgr();

    LeaseMgr(LeaseMgr const&);
    void operator=(LeaseMgr const&);

protected:
    // This is a multi-index container, which holds elements that can
    // be accessed using different search indexes.
    typedef boost::multi_index_container<
                // It holds pointers to Lease6 objects.
                Lease6Ptr,
                boost::multi_index::indexed_by<
                    // Specification of the first index starts here.
                    // This index sorts leases by IPv6 addresses represented as
                    // IOAddress objects.
                    boost::multi_index::ordered_unique<
                        boost::multi_index::member<
                            Lease,
                            isc::asiolink::IOAddress,
                            &Lease::addr_>
                    >,

                    // Specification of the second index starts here.
                    boost::multi_index::ordered_unique<
                        // This is a composite index that will be used to search for
                        // the lease using three attributes: DUID, IAID, Subnet Id.
                        boost::multi_index::composite_key<
                            Lease6,
                            // The DUID can be retrieved from the Lease6
                            // object using a getDuidVector const function.
                            boost::multi_index::const_mem_fun<
                                Lease6,
                                const std::vector<uint8_t>&,
                                &Lease6::getDuidVector
                            >,
                            // The two other ingredients of this index
                            // are IAID and subnet id.
                            boost::multi_index::member<
                                Lease6,
                                uint32_t,
                                &Lease6::iaid_
                            >,
                            boost::multi_index::member<
                                Lease,
                                SubnetID,
                                &Lease::subnet_id_
                            >
                        >
                    >
            >
    > Lease6Storage; // Specify the type name of this container.

    /// @brief stores IPv6 leases
    Lease6Storage storage6_;
};

}; // end of isc::dhcpMini namespace
}; // end of isc namespace

#endif // LEASE_MGR_H
