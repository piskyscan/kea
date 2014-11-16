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

#include <lease_mgr.h>
#include <dhcp6-mini_log.h>

namespace isc {
namespace dhcpMini {

LeaseMgr::LeaseMgr() {
}

LeaseMgr::~LeaseMgr() {
}

LeaseMgr& LeaseMgr::getInstance() {
	static LeaseMgr instance;
	return instance;
}

bool LeaseMgr::addLease(const Lease6Ptr& lease) {
    LOG(DBG)<< "Adding IPv6 lease with address "
    		<< lease->addr_.toText() << endl;

    if (getLease6(lease->type_, lease->addr_)) {
        // there is a lease with specified address already
        return (false);
    }
    storage6_.insert(lease);
    return (true);
}

Lease6Ptr LeaseMgr::getLease6(Lease::Type /* not used yet */,
		const isc::asiolink::IOAddress& addr) const {
    LOG(DBG)<< "Obtaining IPv6 lease for address " << addr.toText() << endl;

    Lease6Storage::iterator l = storage6_.find(addr);
    if (l == storage6_.end()) {
        return (Lease6Ptr());
    } else {
        return (Lease6Ptr(new Lease6(**l)));
    }
}

Lease6Collection LeaseMgr::getLeases6(Lease::Type /* not used yet */,
		const isc::dhcp::DUID& duid, uint32_t iaid, SubnetID subnet_id) const {
    LOG(DBG)<< "Obtaining IPv6 leases for IAID " << iaid
    		<< ", Subnet ID " << subnet_id
    		<< " and DUID " << duid.toText() << endl;

    // We are going to use index #1 of the multi index container.
    // We define SearchIndex locally in this function because
    // currently only this function uses this index.
    typedef Lease6Storage::nth_index<1>::type SearchIndex;
    // Get the index.
    const SearchIndex& idx = storage6_.get<1>();
    // Try to get the lease using the DUID, IAID and Subnet ID.
    SearchIndex::const_iterator lease =
        idx.find(boost::make_tuple(duid.getDuid(), iaid, subnet_id));
    // Lease was not found. Return empty pointer.
    if (lease == idx.end()) {
        return (Lease6Collection());
    }

    // Lease was found, return it to the caller.
    /// @todo: allow multiple leases for a single duid+iaid+subnet_id tuple
    Lease6Collection collection;
    collection.push_back(Lease6Ptr(new Lease6(**lease)));
    return (collection);
}

void LeaseMgr::updateLease6(const Lease6Ptr& lease) {
    LOG(DBG)<< "Updating IPv6 lease for address "
    		<< lease->addr_.toText() << endl;

    Lease6Storage::iterator lease_it = storage6_.find(lease->addr_);
    if (lease_it == storage6_.end()) {
        LOG(ERR)<< "failed to update the lease with address "
                << lease->addr_.toText() << " - no such lease" << endl;
        return;
    }
    **lease_it = *lease;
}

bool LeaseMgr::deleteLease(const isc::asiolink::IOAddress& addr) {
    LOG(DBG)<< "Deleting lease for address " << addr.toText() << endl;
    if (addr.isV4()) {
        // v4 lease
    	return (false);
    } else {
        // v6 lease
        Lease6Storage::iterator l = storage6_.find(addr);
        if (l == storage6_.end()) {
            // No such lease
            return (false);
        } else {
            storage6_.erase(l);
            return (true);
        }
    }
}

}; // end of isc::dhcpMini namespace
}; // end of isc namespace
