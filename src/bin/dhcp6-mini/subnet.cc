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

#include <addr_utilities.h>
#include <dhcp6-mini_log.h>
#include <io_address.h>
#include <subnet.h>
#include <dhcp/option_space.h>

#include <stdlib.h> // for exit codes
#include <sstream>

namespace isc {
namespace dhcpMini {

// This is an initial value of subnet-id. See comments in subnet.h for details.
SubnetID Subnet::static_id_ = 1;

Subnet::Subnet(const isc::dhcpMini::IOAddress& prefix, uint8_t len,
               const Triplet<uint32_t>& t1,
               const Triplet<uint32_t>& t2,
               const Triplet<uint32_t>& valid_lifetime)
    :id_(generateNextID()), prefix_(prefix), prefix_len_(len),
     t1_(t1), t2_(t2), valid_(valid_lifetime),
     last_allocated_ia_(lastAddrInPrefix(prefix, len)),
     last_allocated_ta_(lastAddrInPrefix(prefix, len)),
     last_allocated_pd_(lastAddrInPrefix(prefix, len))
      {
    if (prefix.isV6() && len > 128) {
        LOG(ERR) << "Invalid prefix length specified for subnet: " << len << endl;
    }
}

bool Subnet::inRange(const isc::dhcpMini::IOAddress& addr) const {
	isc::dhcpMini::IOAddress first = firstAddrInPrefix(prefix_, prefix_len_);
	isc::dhcpMini::IOAddress last = lastAddrInPrefix(prefix_, prefix_len_);

	return ((first <= addr) && (addr <= last));
}

void Subnet::addOption(const isc::dhcp::OptionPtr& option, bool persistent,
		const std::string& option_space) {
	// Check that the option space name is valid.
	if (!isc::dhcp::OptionSpace::validateName(option_space)) {
		LOG(ERR)<< "Invalid option space name: '" << option_space << "'" << endl;
	}
	validateOption(option);

	// Actually add new option descriptor.
	option_spaces_.addItem(OptionDescriptor(option, persistent), option_space);
}

void Subnet::delOptions() {
	option_spaces_.clearItems();
}

Subnet::OptionContainerPtr Subnet::getOptionDescriptors(
		const std::string& option_space) const {
	return (option_spaces_.getItems(option_space));
}

Subnet::OptionDescriptor Subnet::getOptionDescriptor(
		const std::string& option_space, const uint16_t option_code) {
	OptionContainerPtr options = getOptionDescriptors(option_space);
	if (!options || options->empty()) {
		return (OptionDescriptor(false));
	}
	const OptionContainerTypeIndex& idx = options->get<1>();
	const OptionContainerTypeRange& range = idx.equal_range(option_code);
	if (std::distance(range.first, range.second) == 0) {
		return (OptionDescriptor(false));
	}

	return (*range.first);
}

isc::dhcpMini::IOAddress Subnet::getLastAllocated(Lease::Type type) const {
    // check if the type is valid (and throw if it isn't)
    checkType(type);

    switch (type) {
    case Lease::TYPE_NA:
        return last_allocated_ia_;
    case Lease::TYPE_TA:
        return last_allocated_ta_;
    default:
        LOG(ERR) << "Pool type " << type << " not supported" << endl;
        exit(EXIT_FAILURE);
    }
}

void Subnet::setLastAllocated(Lease::Type type,
		const isc::dhcpMini::IOAddress& addr) {

    // check if the type is valid (and throw if it isn't)
    checkType(type);

    switch (type) {
    case Lease::TYPE_NA:
        last_allocated_ia_ = addr;
        return;
    default:
    	LOG(ERR) << "Pool type " << type << " not supported" << endl;
    	exit(EXIT_FAILURE);
    }
}

std::string Subnet::toText() const {
	std::stringstream tmp;
	tmp << prefix_ << "/" << static_cast<unsigned int>(prefix_len_);
	return (tmp.str());
}

const PoolCollection& Subnet::getPools(Lease::Type type) const {
    // check if the type is valid (and throw if it isn't)
    checkType(type);

    switch (type) {
    case Lease::TYPE_NA:
        return (pools_);
    default:
        LOG(ERR) << "Unsupported pool type: " << static_cast<int>(type) << endl;
        exit(EXIT_FAILURE);
    }
}

PoolCollection& Subnet::getPoolsWritable(Lease::Type type) {
    // check if the type is valid (and throw if it isn't)
    checkType(type);

    switch (type) {
    case Lease::TYPE_NA:
        return (pools_);
    default:
		LOG(ERR) << "Invalid pool type specified: "
		<< static_cast<int>(type) << endl;
		exit(EXIT_FAILURE);
    }
}

const PoolPtr Subnet::getPool(Lease::Type type,
		const isc::dhcpMini::IOAddress& hint, bool anypool /* true */) const {
	// check if the type is valid (and throw if it isn't)
	checkType(type);

	const PoolCollection& pools = getPools(type);

	PoolPtr candidate;
	for (PoolCollection::const_iterator pool = pools.begin();
			pool != pools.end(); ++pool) {

		// if we won't find anything better, then let's just use the first pool
		if (anypool && !candidate) {
			candidate = *pool;
		}

		// if the client provided a pool and there's a pool that hint is valid
		// in, then let's use that pool
		if ((*pool)->inRange(hint)) {
			return (*pool);
		}
	}
	return (candidate);
}

void Subnet::addPool(const PoolPtr& pool) {
	IOAddress first_addr = pool->getFirstAddress();
	IOAddress last_addr = pool->getLastAddress();

	if (!inRange(first_addr) || !inRange(last_addr)) {
		LOG(ERR) << "Pool (" << first_addr << "-" << last_addr
		<< " does not belong in this (" << prefix_ << "/"
		<< static_cast<int>(prefix_len_) << ") subnet" << endl;
	}

	// check if the type is valid (and throw if it isn't)
	checkType(pool->getType());

	// Add the pool to the appropriate pools collection
	getPoolsWritable(pool->getType()).push_back(pool);
}

void Subnet::delPools(Lease::Type type) {
	getPoolsWritable(type).clear();
}

void Subnet::setIface(const std::string& iface_name) {
	iface_ = iface_name;
}

std::string Subnet::getIface() const {
	return (iface_);
}

bool Subnet::inPool(Lease::Type type,
		const isc::dhcpMini::IOAddress& addr) const {

	// Let's start with checking if it even belongs to that subnet.
	if (!inRange(addr)) {
		return (false);
	}

	const PoolCollection& pools = getPools(type);

	for (PoolCollection::const_iterator pool = pools.begin();
			pool != pools.end(); ++pool) {
		if ((*pool)->inRange(addr)) {
			return (true);
		}
	}

	// There's no pool that address belongs to
	return (false);
}

Subnet6::Subnet6(const isc::dhcpMini::IOAddress& prefix, uint8_t length,
		const Triplet<uint32_t>& t1, const Triplet<uint32_t>& t2,
		const Triplet<uint32_t>& preferred_lifetime,
		const Triplet<uint32_t>& valid_lifetime) :
		Subnet(prefix, length, t1, t2, valid_lifetime), preferred_(
				preferred_lifetime) {
	if (!prefix.isV6()) {
		LOG (ERR)<< "Non IPv6 prefix " << prefix
		<< " specified in subnet6" << endl;
	}
}

void Subnet6::checkType(Lease::Type type) const {
	if ((type != Lease::TYPE_NA) && (type != Lease::TYPE_TA)
			&& (type != Lease::TYPE_PD)) {
		LOG(ERR) << "Invalid Pool type: " << Lease::typeToText(type)
		<< "(" << static_cast<int>(type)
		<< "), must be TYPE_NA, TYPE_TA or TYPE_PD for Subnet6" << endl;
	}
}

void Subnet6::validateOption(const isc::dhcp::OptionPtr& option) const {
	if (!option) {
		LOG(ERR) << "Option configured for subnet must not be NULL" << endl;
	} else if (option->getUniverse() != isc::dhcp::Option::V6) {
		LOG(ERR) << "Expected V6 option to be added to the subnet" << endl;
	}
}

}; // end of isc::dhcpMini namespace
}; // end of isc namespace
