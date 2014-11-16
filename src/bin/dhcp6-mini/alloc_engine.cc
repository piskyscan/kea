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

#include <alloc_engine.h>
#include <dhcp6-mini_log.h>
#include <lease_mgr.h>

#include <cstring>
#include <ctime>
#include <vector>

namespace isc {
namespace dhcpMini {

AllocEngine::IterativeAllocator::IterativeAllocator(Lease::Type lease_type)
    :Allocator(lease_type) {
}

isc::asiolink::IOAddress AllocEngine::IterativeAllocator::increaseAddress(
		const isc::asiolink::IOAddress& addr) {
	// Get a buffer holding an address.
	const std::vector<uint8_t>& vec = addr.toBytes();
	// Get the address length.
	const int len = vec.size();

	uint8_t packed[isc::asiolink::V6ADDRESS_LEN];

	// Copy the address. It can be either V4 or V6.
	std::memcpy(packed, &vec[0], len);

	// Start increasing the least significant byte
	for (int i = len - 1; i >= 0; --i) {
		++packed[i];
		// if we haven't overflowed (0xff -> 0x0), than we are done
		if (packed[i] != 0) {
			break;
		}
	}

	return (isc::asiolink::IOAddress::fromBytes(addr.getFamily(), packed));
}

isc::asiolink::IOAddress AllocEngine::IterativeAllocator::pickAddress(
		const Subnet6Ptr& subnet, const isc::dhcp::DuidPtr&,
		const isc::asiolink::IOAddress&) {
    // Let's get the last allocated address. It is usually set correctly,
    // but there are times when it won't be (like after removing a pool or
    // perhaps restarting the server).
	isc::asiolink::IOAddress last = subnet->getLastAllocated(pool_type_);

    const PoolCollection& pools = subnet->getPools(pool_type_);

    if (pools.empty()) {
        LOG(ERR)<< "No pools defined in selected subnet" << endl;
    }

    // first we need to find a pool the last address belongs to.
    PoolCollection::const_iterator it;
    for (it = pools.begin(); it != pools.end(); ++it) {
        if ((*it)->inRange(last)) {
            break;
        }
    }

    // last one was bogus for one of several reasons:
    // - we just booted up and that's the first address we're allocating
    // - a subnet was removed or other reconfiguration just completed
    // - perhaps allocation algorithm was changed
    if (it == pools.end()) {
        // ok to access first element directly. We checked that pools is non-empty
    	isc::asiolink::IOAddress next = pools[0]->getFirstAddress();
        subnet->setLastAllocated(pool_type_, next);
        return (next);
    }

    // Ok, we have a pool that the last address belonged to, let's use it.

    isc::asiolink::IOAddress next("::");
    next = increaseAddress(last); // basically addr++

    if ((*it)->inRange(next)) {
        // the next one is in the pool as well, so we haven't hit pool boundary yet
        subnet->setLastAllocated(pool_type_, next);
        return (next);
    }

    // We hit pool boundary, let's try to jump to the next pool and try again
    ++it;
    if (it == pools.end()) {
        // Really out of luck today. That was the last pool. Let's rewind
        // to the beginning.
        next = pools[0]->getFirstAddress();
        subnet->setLastAllocated(pool_type_, next);
        return (next);
    }

    // there is a next pool, let's try first address from it
    next = (*it)->getFirstAddress();
    subnet->setLastAllocated(pool_type_, next);
    return (next);
}

AllocEngine::AllocEngine(unsigned int attempts)
    :attempts_(attempts) {

    // Set the basic (normal address) lease type
    Lease::Type basic_type = Lease::TYPE_NA;

    // Initalize normal address allocators
	allocators_[basic_type] = AllocEngine::AllocatorPtr(
			new IterativeAllocator(basic_type));
}

Lease6Collection AllocEngine::allocateLeases6(const Subnet6Ptr& subnet,
		const isc::dhcp::DuidPtr& duid, const uint32_t iaid,
		const isc::asiolink::IOAddress& hint, Lease::Type type,
		bool fake_allocation, Lease6Collection& old_leases) {

	AllocEngine::AllocatorPtr allocator = getAllocator(type);

	if (!allocator) {
		LOG(ERR)<< "No allocator specified for "
				<< Lease6::typeToText(type) << endl;
	}

	if (!subnet) {
		LOG(ERR)<< "Subnet is required for allocation" << endl;
	}

	if (!duid) {
		LOG(ERR)<< "DUID is mandatory for allocation" << endl;
	}

	// Check if there's existing lease for that subnet/duid/iaid
	// combination.
	Lease6Collection existing = LeaseMgr::getInstance().getLeases6(type, *duid,
			iaid, subnet->getID());

	// There is at least one lease for this client. We will return these
	// leases for the client.
	if (!existing.empty()) {
		// Return old leases so the server can see what has changed.
		old_leases = existing;
		return(existing);
	}

	// check if the hint is in pool and is available
	// This is equivalent of subnet->inPool(hint), but returns the pool
	Pool6Ptr pool = boost::dynamic_pointer_cast < Pool6
			> (subnet->getPool(type, hint, false));

	if (pool) {
		Lease6Ptr lease = LeaseMgr::getInstance().getLease6(type, hint);
		if (!lease) {
			// The hint is valid and not currently used, let's create a
			// lease for it
			lease = createLease6(subnet, duid, iaid, hint, type,
					fake_allocation);

			// It can happen that the lease allocation failed (we could
			// have lost the race condition. That means that the hint is
			// lo longer usable and we need to continue the regular
			// allocation path.
			if (lease) {
				// We are allocating a new lease (not renewing). So, the
				// old lease should be NULL.
				old_leases.push_back(Lease6Ptr());

				Lease6Collection collection;
				collection.push_back(lease);
				return (collection);
			}
		} else {
			if (lease->expired()) {
				// Copy an existing, expired lease so as it can be returned
				// to the caller.
				Lease6Ptr old_lease(new Lease6(*lease));
				old_leases.push_back(old_lease);

				/// We found a lease and it is expired, so we can reuse it
				lease = reuseExpiredLease(lease, subnet, duid, iaid,
						fake_allocation);

				/// @todo: We support only one lease per ia for now
				Lease6Collection collection;
				collection.push_back(lease);
				return (collection);
			}

		}
	}

	// Hint is in the pool but is not available. Search the pool until first of
	// the following occurs:
	// - we find a free address
	// - we find an address for which the lease has expired
	// - we exhaust number of tries
	//
	// @todo: Current code does not handle pool exhaustion well. It will be
	// improved. Current problems:
	// 1. with attempts set to too large value (e.g. 1000) and a small pool (e.g.
	// 10 addresses), we will iterate over it 100 times before giving up
	// 2. attempts 0 mean unlimited (this is really UINT_MAX, not infinite)
	// 3. the whole concept of infinite attempts is just asking for infinite loop
	// We may consider some form or reference counting (this pool has X addresses
	// left), but this has one major problem. We exactly control allocation
	// moment, but we currently do not control expiration time at all

	unsigned int i = attempts_;
	do {
		isc::asiolink::IOAddress candidate = allocator->pickAddress(subnet,
				duid, hint);

		Lease6Ptr existing = LeaseMgr::getInstance().getLease6(type, candidate);
		if (!existing) {
			// there's no existing lease for selected candidate, so it is
			// free. Let's allocate it.

			Lease6Ptr lease = createLease6(subnet, duid, iaid, candidate, type,
					fake_allocation);
			if (lease) {
				// We are allocating a new lease (not renewing). So, the
				// old lease should be NULL.
				old_leases.push_back(Lease6Ptr());

				Lease6Collection collection;
				collection.push_back(lease);
				return (collection);
			}

			// Although the address was free just microseconds ago, it may have
			// been taken just now. If the lease insertion fails, we continue
			// allocation attempts.
		} else {
			if (existing->expired()) {
				// Copy an existing, expired lease so as it can be returned
				// to the caller.
				Lease6Ptr old_lease(new Lease6(*existing));
				old_leases.push_back(old_lease);

				existing = reuseExpiredLease(existing, subnet, duid, iaid,
						fake_allocation);
				Lease6Collection collection;
				collection.push_back(existing);
				return (collection);
			}
		}

		// Continue trying allocation until we run out of attempts
		// (or attempts are set to 0, which means infinite)
		--i;
	} while ((i > 0) || !attempts_);

	// Unable to allocate an address, return an empty lease.
	LOG(WRN)<< "Failed to allocate an IPv6 address after "
			<< attempts_ << " attempt(s)" << endl;

    return (Lease6Collection());
}

Lease6Ptr AllocEngine::reuseExpiredLease(Lease6Ptr& expired,
		const Subnet6Ptr& subnet, const isc::dhcp::DuidPtr& duid,
		const uint32_t iaid, bool fake_allocation /*= false */) {

    if (!expired->expired()) {
        LOG(WRN)<< "Attempt to recycle lease that is still valid" << endl;
        return (expired);
    }

    // address, lease type stay the same
    expired->iaid_ = iaid;
    expired->duid_ = duid;
    expired->preferred_lft_ = subnet->getPreferred();
    expired->valid_lft_ = subnet->getValid();
    expired->t1_ = subnet->getT1();
    expired->t2_ = subnet->getT2();
    expired->cltt_ = time(NULL);
    expired->subnet_id_ = subnet->getID();
    expired->fixed_ = false;

    LOG(DBG)<< "Reusing expired IPv6 lease for address "
    		<< expired->addr_.toText() << endl;

    if (!fake_allocation) {
        // for REQUEST we do update the lease
    	LeaseMgr::getInstance().updateLease6(expired);
    }

    // We do nothing for SOLICIT. We'll just update database when
    // the client gets back to us with REQUEST message.

    // it's not really expired at this stage anymore - let's return it as
    // an updated lease
    return (expired);
}

Lease6Ptr AllocEngine::createLease6(const Subnet6Ptr& subnet,
		const isc::dhcp::DuidPtr& duid, const uint32_t iaid,
		const isc::asiolink::IOAddress& addr, const Lease::Type type,
		bool fake_allocation /*= false */) {

	Lease6Ptr lease(
			new Lease6(type, addr, duid, iaid, subnet->getPreferred(),
					subnet->getValid(), subnet->getT1(), subnet->getT2(),
					subnet->getID()));

    if (!fake_allocation) {
        // That is a real (REQUEST) allocation
        bool status = LeaseMgr::getInstance().addLease(lease);

        if (status) {
            return (lease);
        } else {
            // One of many failures with LeaseMgr. One notable case for that
            // is that we are working in multi-process mode and we lost a race
            // (some other process got that address first)
            return (Lease6Ptr());
        }
    } else {
        // That is only fake (SOLICIT without rapid-commit) allocation

        // It is for advertise only. We should not insert the lease into LeaseMgr,
        // but rather check that we could have inserted it.
		Lease6Ptr existing = LeaseMgr::getInstance().getLease6(Lease::TYPE_NA,
				addr);
        if (!existing) {
            return (lease);
        } else {
            return (Lease6Ptr());
        }
    }
}

AllocEngine::AllocatorPtr AllocEngine::getAllocator(Lease::Type type) {
	std::map<Lease::Type, AllocEngine::AllocatorPtr>::const_iterator alloc =
			allocators_.find(type);

    if (alloc == allocators_.end()) {
        LOG(ERR)<< "No allocator initialized for pool type "
        		<< Lease::typeToText(type) << endl;
    }
    return (alloc->second);
}

AllocEngine::~AllocEngine() {
    // no need to delete allocator. smart_ptr will do the trick for us
}

}; // end of isc::dhcpMini namespace
}; // end of isc namespace
