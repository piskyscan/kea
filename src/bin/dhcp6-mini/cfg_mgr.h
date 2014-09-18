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

#ifndef DHCP6_MINI_CONFIG_MANAGER_H
#define DHCP6_MINI_CONFIG_MANAGER_H

#include <subnet.h>

#include <iostream>
#include <map>
#include <string>

using namespace std;

namespace isc {
namespace dhcpMini {

class CfgMgr {
public:
	static CfgMgr& getInstance();
	string getDataDir();

private:
	/// @brief A directory where data files (e.g. server-id) are stored
	string datadir_;

	/// @brief A container for IPv6 subnets
	SubnetCollection subnets_;

	CfgMgr();
	~CfgMgr();
	CfgMgr(CfgMgr const&);
	void operator=(CfgMgr const&);

	void parseConfigFile();
};

typedef map<string, string> CfgParams;

istream& operator>>(istream& is, CfgParams& params);
void trimWhitespaces(string& s);

}; // end of isc::dhcpMini namespace
}; // end of isc namespace

#endif // DHCP6_MINI_CONFIG_MANAGER_H
