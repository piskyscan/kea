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
#include <pool.h>

#include <sstream>

namespace isc {
namespace dhcpMini {

Pool::Pool(Lease::Type type, const isc::dhcpMini::IOAddress& first,
		const isc::dhcpMini::IOAddress& last) :
		id_(getNextID()), first_(first), last_(last), type_(type) {
}

bool Pool::inRange(const isc::dhcpMini::IOAddress& addr) const {
	return (first_.smallerEqual(addr) && addr.smallerEqual(last_));
}

std::string Pool::toText() const {
	std::stringstream tmp;
	tmp << "type=" << Lease::typeToText(type_) << ", " << first_ << "-"
			<< last_;
	return (tmp.str());
}

Pool6::Pool6(Lease::Type type, const isc::dhcpMini::IOAddress& first,
		const isc::dhcpMini::IOAddress& last) :
		Pool(type, first, last) {

	// check if specified address boundaries are sane
	if (!first.isV6() || !last.isV6()) {
		LOG(ERR) << "Invalid Pool6 address boundaries: not IPv6" << endl;
	}

	if ((type != Lease::TYPE_NA) && (type != Lease::TYPE_TA)
			&& (type != Lease::TYPE_PD)) {
		LOG(ERR) << "Invalid Pool6 type: " << static_cast<int>(type)
				<< ", must be TYPE_IA, TYPE_TA or TYPE_PD" << endl;
	}

	if (last < first) {
		LOG(ERR) << "Upper boundary is smaller than lower boundary." << endl;
		// This check is a bit strict. If we decide that it is too strict,
		// we need to comment it and uncomment lines below.
		// On one hand, letting the user specify 2001::f - 2001::1 is nice, but
		// on the other hand, 2001::1 may be a typo and the user really meant
		// 2001::1:0 (or 1 followed by some hex digit), so a at least a warning
		// would be useful.

		// first_  = last;
		// last_ = first;
	}

	// TYPE_TA and TYPE_PD are not supported currently
	if (type != Lease::TYPE_NA) {
		LOG(ERR) << "Invalid Pool6 type specified:"
				<< static_cast<int>(type) << endl;
	}
}

std::string Pool6::toText() const {
	std::stringstream tmp;
	tmp << "type=" << Lease::typeToText(type_) << ", " << first_
			<< "-" << last_;
	return (tmp.str());
}

}; // end of isc::dhcpMini namespace
}; // end of isc namespace
