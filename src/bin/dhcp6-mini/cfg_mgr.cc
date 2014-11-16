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

#include <cfg_mgr.h>
#include <dhcp6-mini_log.h>

#include <boost/lexical_cast.hpp>

#include <stdlib.h> // for exit codes
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

namespace {

static const char* SERVER_CONFIG_FILE = "b10-dhcp6-mini-config.ini";

bool isMultivalued(const string& value) {
	size_t multivalue_delim = value.find_first_of("[");
	return multivalue_delim == string::npos ? false : true;
}

}; // anonymous namespace

namespace isc {
namespace dhcpMini {

CfgMgr::CfgMgr() :
		datadir_(DHCP_DATA_DIR), verbose_(false) {
	// DHCP_DATA_DIR must be set set with -DDHCP_DATA_DIR="..." in Makefile.am
	// Note: the definition of DHCP_DATA_DIR needs to include quotation marks
	// See AM_CPPFLAGS definition in Makefile.am
}

CfgMgr::~CfgMgr() {
}

CfgMgr& CfgMgr::getInstance() {
	static CfgMgr instance;
	return instance;
}

Subnet6Ptr CfgMgr::getSubnet6() {
	if (subnet_) {
		// Currently only one subnet can be configured, let's just use it
		LOG(DBG)<< "Retrieved subnet " << subnet_->toText() << endl;
		return (subnet_);
	}

	LOG(DBG)<< "No suitable subnet is defined" << endl;
	return (Subnet6Ptr());
}

string CfgMgr::getDataDir() {
	return (datadir_);
}

void CfgMgr::enableVerboseOutput() {
	verbose_ = true;
}

bool CfgMgr::isVerboseOutputEnabled() {
	return verbose_;
}

CfgMgr::GetParam::GetParam(const CfgMgr& c, const string paramName)
{
	param_value_ = (c.cfg_params_).find(paramName)->second;
}

CfgMgr::GetParam::operator Triplet<uint32_t> ()
{
	Triplet<uint32_t> t = boost::lexical_cast<uint32_t>(param_value_);
	return t;
}

CfgMgr::GetParam::operator string ()
{
	return param_value_;
}

void CfgMgr::parseConfigFile() {
	string config_file = datadir_ + "/" + string(SERVER_CONFIG_FILE);
	LOG(CFG) << "Reading config from file " << config_file << endl;

	ifstream cfg_file(config_file.c_str());
	cfg_file >> cfg_params_;
	cfg_file.close();

	createConfig();
}

void CfgMgr::createConfig() {
	createPool();
	createSubnet();
}

void CfgMgr::createPool() {
	vector<string> str_pools;
	string pool_param = GetParam(*this, "pool");

	size_t delim_position = pool_param.find_first_of("[");
	if (delim_position == string::npos) {
		//Single pool definition
		str_pools.push_back(pool_param);
	} else {
		//Array of pools
		pool_param.erase(remove(pool_param.begin(), pool_param.end(), '['),
				pool_param.end());
		pool_param.erase(remove(pool_param.begin(), pool_param.end(), ']'),
				pool_param.end());

		string delimiter = ",";
		do {
			delim_position = pool_param.find(delimiter);
		    string str_pool = pool_param.substr(0, delim_position);

			str_pool.erase(remove(str_pool.begin(), str_pool.end(), '"'),
					str_pool.end());

		    str_pools.push_back(str_pool);

		    pool_param.erase(0, delim_position + delimiter.length());
		} while (delim_position != string::npos);
	}

	for(size_t i = 0; i < str_pools.size(); i++ ) {
		string str_pool = str_pools[i];

		//Remove whitespaces
		str_pool.erase(remove(str_pool.begin(), str_pool.end(), ' '),
				str_pool.end());
		str_pool.erase(remove(str_pool.begin(), str_pool.end(), '\t'),
				str_pool.end());

		//Handle prefix/len notation
		delim_position = str_pool.find_first_of("/");

		if (delim_position != string::npos) {
			isc::asiolink::IOAddress addr("::");
			uint8_t len = 0;

			addr = isc::asiolink::IOAddress(str_pool.substr(0, delim_position));

			// start with the first character after /
			string prefix_len = str_pool.substr(delim_position + 1);

			// It is lexical cast to int and then downcast to uint8_t.
			// Direct cast to uint8_t (which is really an unsigned char)
			// will result in interpreting the first digit as output
			// value and throwing exception if length is written on two
			// digits (because there are extra characters left over).
			len = boost::lexical_cast<int>(prefix_len);

			PoolPtr pool(PoolPtr(new Pool6(Lease::TYPE_NA, addr, len)));
			pools_.push_back(pool);
		}

		//Handle min-max notation
		delim_position = str_pool.find_first_of("-");

		if (delim_position != string::npos) {
			isc::asiolink::IOAddress min(str_pool.substr(0,delim_position));
			isc::asiolink::IOAddress max(str_pool.substr(delim_position + 1));

			PoolPtr pool(PoolPtr(new Pool6(Lease::TYPE_NA, min, max)));
			pools_.push_back(pool);
		}
	}
}

void CfgMgr::initSubnet(isc::asiolink::IOAddress addr, uint8_t len) {
	// Get all 'time' parameters
	Triplet<uint32_t> t1 = GetParam(*this, "renew-timer");
	Triplet<uint32_t> t2 = GetParam(*this, "rebind-timer");
	Triplet<uint32_t> pref = GetParam(*this, "preferred-lifetime");
	Triplet<uint32_t> valid = GetParam(*this, "valid-lifetime");

	LOG(DBG)<< "New subnet has been added to configuration: "
			<< addr << "/" << static_cast<int>(len)
			<< " with params t1=" << t1
			<< ", t2=" << t2
			<< ", pref=" << pref
			<< ", valid=" << valid << endl;

	// Create a new subnet
	Subnet6* subnet6 = new Subnet6(addr, len, t1, t2, pref, valid);
	subnet_.reset(subnet6);
}

void CfgMgr::createSubnet() {
	string subnet_param = GetParam(*this, "subnet");
	//Remove whitespaces and quotation marks
	subnet_param.erase(remove(subnet_param.begin(), subnet_param.end(), ' '),
			subnet_param.end());
	subnet_param.erase(remove(subnet_param.begin(), subnet_param.end(), '\t'),
			subnet_param.end());
	subnet_param.erase(remove(subnet_param.begin(), subnet_param.end(), '"'),
			subnet_param.end());

	size_t delim_position = subnet_param.find_first_of("/");
	if (delim_position == string::npos) {
		LOG(ERR)<< "Invalid subnet syntax (prefix/len expected): "
				<< subnet_param << endl;
		return;
	}

	// Try to create the address object. It also validates that
	// the address syntax is ok.
	isc::asiolink::IOAddress addr(subnet_param.substr(0, delim_position));
	uint8_t len = boost::lexical_cast<unsigned int>(
			subnet_param.substr(delim_position + 1));

	// Call the subclass's method to instantiate the subnet
	initSubnet(addr, len);

	// Add pools to it
	for(size_t i = 0; i < pools_.size(); i++ ) {
		subnet_->addPool(pools_[i]);
	}
}

istream& operator>>(istream& is, CfgParams& params) {
	string line, key, value;

	while (getline(is, line)) {
		// Skip empty lines
		if (line.empty()) {
			continue;
		}

		// Skip commented lines
		if (line[0] == '#') {
			continue;
		}

		// Split line by a first whitespace
		size_t delim_position = line.find_first_of(" \t");
		if (delim_position == string::npos) {
			LOG(ERR)<< "Config file is corrupted. No value assigned in line: "
					<< line << endl;
			exit(EXIT_FAILURE);
		}

		// Extract key
		key = line.substr(0, delim_position);
		trimWhitespaces(key);

		// Extract value
		value = line.substr(delim_position + 1, line.length());
		if (!isMultivalued(value)) {
			trimWhitespaces(value);
		}

		// Put the extracted {key, value} pair in the map
		params[key] = value;
	}

	// Print params from config file
	for (CfgParams::iterator it = params.begin(); it != params.end(); ++it) {
		LOG(CFG)<< it->first << " => " << it->second << endl;
	}

	return is;
}

void trimWhitespaces(string& str) {
	stringstream trimmer;

	trimmer << str;
	str.clear();
	trimmer >> str;
}

}; // end of isc::dhcpMini namespace
}; // end of isc namespace
