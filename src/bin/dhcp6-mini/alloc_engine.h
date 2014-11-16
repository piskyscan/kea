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

#ifndef ALLOC_ENGINE_H
#define ALLOC_ENGINE_H

#include <asiolink/io_address.h>
#include <lease_mgr.h>
#include <subnet.h>
#include <dhcp/duid.h>
#include <dhcp/hwaddr.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <map>

namespace isc {
namespace dhcpMini {

/// @brief DHCPv6 allocation engine
///
/// This class represents DHCP allocation engine. It is responsible
/// for picking subnets, choosing and allocating a lease, extending,
/// renewing, releasing and possibly expiring leases.
class AllocEngine : public boost::noncopyable {
protected:

    /// @brief base class for all address/prefix allocation algorithms
    ///
    /// This is an abstract class that should not be used directly, but rather
    /// specialized implementations should be used instead.
    class Allocator {
    public:

        /// @brief picks one address out of available pools in a given subnet
        ///
        /// This method returns one address from the available pools in the
        /// specified subnet. It should not check if the address is used or
        /// reserved - AllocEngine will check that and will call pickAddress
        /// again if necessary. The number of times this method is called will
        /// increase as the number of available leases will decrease.
        ///
        /// This method can also be used to pick a prefix. We should not rename
        /// it to pickLease(), because at this early stage there is no concept
        /// of a lease yet. Here it is a matter of selecting one address or
        /// prefix from the defined pool, without going into details who it is
        /// for or who uses it. I thought that pickAddress() is less confusing
        /// than pickResource(), because nobody would immediately know what the
        /// resource means in this context.
        ///
        /// @param subnet next address will be returned from pool of that subnet
        /// @param duid Client's DUID
        /// @param hint client's hint
        ///
        /// @return the next address
		virtual isc::asiolink::IOAddress pickAddress(const Subnet6Ptr& subnet,
				const isc::dhcp::DuidPtr& duid,
				const isc::asiolink::IOAddress& hint) = 0;

        /// @brief Default constructor.
        ///
        /// Specifies which type of leases this allocator will assign
        /// @param pool_type specifies pool type (addresses, temp. addr or prefixes)
        Allocator(Lease::Type pool_type)
            :pool_type_(pool_type) {
        }

        /// @brief virtual destructor
        virtual ~Allocator() {
        }
    protected:

        /// @brief defines pool type allocation
        Lease::Type pool_type_;
    };

    /// defines a pointer to allocator
    typedef boost::shared_ptr<Allocator> AllocatorPtr;

    /// @brief Address/prefix allocator that iterates over all addresses
    ///
    /// This class implements iterative algorithm that returns all addresses in
    /// a pool iteratively, one after another. Once the last address is reached,
    /// it starts allocating from the beginning of the first pool (i.e. it loops
    /// over).
    class IterativeAllocator : public Allocator {
    public:

        /// @brief default constructor
        ///
        /// Does not do anything
        /// @param type - specifies allocation type
        IterativeAllocator(Lease::Type type);

        /// @brief returns the next address from pools in a subnet
        ///
        /// @param subnet next address will be returned from pool of that subnet
        /// @param duid Client's DUID (ignored)
        /// @param hint client's hint (ignored)
        /// @return the next address
		virtual isc::asiolink::IOAddress pickAddress(const Subnet6Ptr& subnet,
				const isc::dhcp::DuidPtr& duid,
				const isc::asiolink::IOAddress& hint);
    protected:

        /// @brief Returns an address increased by one
        ///
        /// This method works for both IPv4 and IPv6 addresses. For example,
        /// increase 192.0.2.255 will become 192.0.3.0.
        ///
        /// @param addr address to be increased
        /// @return address increased by one
		static isc::asiolink::IOAddress increaseAddress(
				const isc::asiolink::IOAddress& addr);
    };

public:
    /// @brief Default constructor.
    ///
    /// Instantiates necessary services, required to run DHCPv6 server.
    /// In particular, creates IfaceMgr that will be responsible for
    /// network interaction. Will instantiate lease manager, and load
    /// old or create new DUID.
    ///
    /// @param engine_type selects allocation algorithm
    /// @param attempts number of attempts for each lease allocation before
    ///        we give up (0 means unlimited)
    AllocEngine(unsigned int attempts);

    /// @brief Allocates an IPv6 lease
    ///
    /// This method uses currently selected allocator to pick an address from
    /// specified subnet, creates a lease for that address and then inserts
    /// it into LeaseMgr (if this allocation is not fake).
    ///
    /// @param subnet subnet the allocation should come from
    /// @param duid Client's DUID
    /// @param iaid iaid field from the IA_NA container that client sent
    /// @param hint a hint that the client provided
    /// @param type lease type (IA, TA or PD)
    /// @param fake_allocation is this real i.e. REQUEST (false) or just picking
    ///        an address for SOLICIT that is not really allocated (true)
    /// @param [out] old_leases Collection to which this function will append
    ///        old leases. Leases are stored in the same order as in the
    ///        collection of new leases, being returned. For newly allocated
    ///        leases (not renewed) the NULL pointers are stored in this
    ///        collection as old leases.
    ///
    /// @return Allocated IPv6 leases (may be empty if allocation failed)
	Lease6Collection allocateLeases6(const Subnet6Ptr& subnet,
			const isc::dhcp::DuidPtr& duid, const uint32_t iaid,
			const isc::asiolink::IOAddress& hint, Lease::Type type,
			bool fake_allocation, Lease6Collection& old_leases);

    /// @brief returns allocator for a given pool type
    /// @param type type of pool (V4, IA, TA or PD)
    /// @return pointer to allocator handing a given resource types
    AllocatorPtr getAllocator(Lease::Type type);

    /// @brief Destructor. Used during DHCPv6 service shutdown.
    virtual ~AllocEngine();

private:
    /// @brief creates a lease and inserts it in LeaseMgr if necessary
    ///
    /// Creates a lease based on specified parameters and tries to insert it
    /// into the database. That may fail in some cases, i.e. when there is another
    /// allocation process and we lost a race to a specific lease.
    ///
    /// @param subnet subnet the lease is allocated from
    /// @param duid client's DUID
    /// @param iaid IAID from the IA_NA container the client sent to us
    /// @param addr an address that was selected and is confirmed to be
    ///        available
    /// @param type lease type (IA, TA or PD)
    /// @param callout_handle a callout handle (used in hooks). A lease callouts
    ///        will be executed if this parameter is passed (and there are callouts
    ///        registered)
    /// @param fake_allocation is this real i.e. REQUEST (false) or just picking
    ///        an address for SOLICIT that is not really allocated (true)
    /// @return allocated lease (or NULL in the unlikely case of the lease just
    ///         became unavailable)
	Lease6Ptr createLease6(const Subnet6Ptr& subnet,
			const isc::dhcp::DuidPtr& duid, const uint32_t iaid,
			const isc::asiolink::IOAddress& addr, const Lease::Type type,
			bool fake_allocation = false);

    /// @brief Reuses expired IPv6 lease
    ///
    /// Updates existing expired lease with new information. Lease database
    /// is updated if this is real (i.e. REQUEST, fake_allocation = false), not
    /// dummy allocation request (i.e. SOLICIT, fake_allocation = true).
    ///
    /// @param expired old, expired lease
    /// @param subnet subnet the lease is allocated from
    /// @param duid client's DUID
    /// @param iaid IAID from the IA_NA container the client sent to us
    /// @param fake_allocation is this real i.e. REQUEST (false) or just picking
    ///        an address for SOLICIT that is not really allocated (true)
    /// @return refreshed lease
	Lease6Ptr reuseExpiredLease(Lease6Ptr& expired, const Subnet6Ptr& subnet,
			const isc::dhcp::DuidPtr& duid, const uint32_t iaid,
			bool fake_allocation = false);

    /// @brief a pointer to currently used allocator
    ///
    /// For IPv4, there will be no allocator
    /// For IPv6, there will be 3 allocators: TYPE_NA, TYPE_TA, TYPE_PD
    std::map<Lease::Type, AllocatorPtr> allocators_;

    /// @brief number of attempts before we give up lease allocation (0=unlimited)
    unsigned int attempts_;
};

}; // namespace isc::dhcp
}; // namespace isc

#endif // ALLOC_ENGINE_H
