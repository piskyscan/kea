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

#include <stdlib.h> // for exit codes
#include <fstream>
#include <sstream>

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
		datadir_(DHCP_DATA_DIR) {
	// DHCP_DATA_DIR must be set set with -DDHCP_DATA_DIR="..." in Makefile.am
	// Note: the definition of DHCP_DATA_DIR needs to include quotation marks
	// See AM_CPPFLAGS definition in Makefile.am
	parseConfigFile();
}

CfgMgr::~CfgMgr() {
}

CfgMgr& CfgMgr::getInstance() {
	static CfgMgr instance;
	return instance;
}

string CfgMgr::getDataDir() {
	return (datadir_);
}

void CfgMgr::parseConfigFile() {
	CfgParams cfg_params;
	string config_file = datadir_ + "/" + string(SERVER_CONFIG_FILE);
	LOG(CFG) << "Reading config from file " << config_file << endl;

	ifstream cfg_file(config_file.c_str());
	cfg_file >> cfg_params;
	cfg_file.close();
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
