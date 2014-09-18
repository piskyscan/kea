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

#include <dhcp6-mini_log.h>
#include <lease.h>

#include <sstream>

using namespace std;

namespace isc {
namespace dhcpMini {

Lease::Lease(const isc::dhcpMini::IOAddress& addr, uint32_t t1, uint32_t t2,
		uint32_t valid_lft, SubnetID subnet_id, time_t cltt) :
		addr_(addr), t1_(t1), t2_(t2), valid_lft_(valid_lft), cltt_(cltt),
		subnet_id_(subnet_id), fixed_(false) {
}

std::string Lease::typeToText(Lease::Type type) {
   switch (type) {
   case Lease::TYPE_NA:
       return string("IA_NA");
   case Lease::TYPE_TA:
       return string("IA_TA");
   case Lease::TYPE_PD:
       return string("IA_PD");
   default:
       stringstream tmp;
       tmp << "unknown (" << type << ")";
       return (tmp.str());
   }
}

bool Lease::expired() const {
	// Let's use int64 to avoid problems with negative/large uint32 values
	int64_t expire_time = cltt_ + valid_lft_;
	return (expire_time < time(NULL));
}

Lease6::Lease6(Type type, const isc::dhcpMini::IOAddress& addr,
		isc::dhcp::DuidPtr duid, uint32_t iaid, uint32_t preferred,
		uint32_t valid, uint32_t t1, uint32_t t2, SubnetID subnet_id) :
		Lease(addr, t1, t2, valid, subnet_id, 0/*cltt*/), type_(type),
		iaid_(iaid), duid_(duid), preferred_lft_(preferred) {
	if (!duid) {
		LOG(ERR)<< "DUID must be specified for a lease" << endl;
	}

	cltt_ = time(NULL);
}

const std::vector<uint8_t>& Lease6::getDuidVector() const {
	if (!duid_) {
		static std::vector<uint8_t> empty_vec;
		return (empty_vec);
	}

	return (duid_->getDuid());
}

std::string Lease6::toText() const {
    ostringstream stream;

    stream << "Type:          " << typeToText(type_) << "("
           << static_cast<int>(type_) << ")" << endl;
    stream << "Address:       " << addr_ << endl
           << "IAID:          " << iaid_ << endl
           << "Pref life:     " << preferred_lft_ << endl
           << "Valid life:    " << valid_lft_ << endl
           << "Cltt:          " << cltt_ << endl
           << "Subnet ID:     " << subnet_id_ << endl;

    return (stream.str());
}

bool Lease6::matches(const Lease6& other) const {
	return (addr_ == other.addr_
			&& type_ == other.type_
			&& iaid_ == other.iaid_
			&& *duid_ == *other.duid_);
}

bool Lease6::operator==(const Lease6& other) const {
	return (matches(other)
			&& preferred_lft_ == other.preferred_lft_
			&& valid_lft_ == other.valid_lft_
			&& t1_ == other.t1_
			&& t2_ == other.t2_
			&& cltt_ == other.cltt_
			&& subnet_id_ == other.subnet_id_
			&& fixed_ == other.fixed_);
}

}; // end of isc::dhcpMini namespace
}; // end of isc namespace
