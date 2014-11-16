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

#include <asiolink/io_address.h>
#include <pool.h>
#include <subnet.h>
#include <triplet.h>

#include <iostream>
#include <map>
#include <string>

using namespace std;

namespace isc {
namespace dhcpMini {

/// @brief Definition of configuration parameters kept in the form
/// of key, value pairs
typedef map<string, string> CfgParams;

/// @brief Configuration Manager
///
/// This is an API for the configuration manager.
class CfgMgr {
public:
	/// @brief get instance of the configuration manager
	///
	/// @return a reference to the instance of the configuration manager
	static CfgMgr& getInstance();

    /// @brief get IPv6 subnet
    ///
    /// @return a subnet object (or NULL if no suitable match was fount)
    Subnet6Ptr getSubnet6();

	/// @brief returns path do the data directory
	///
	/// This method returns a path to writeable directory that DHCP servers
	/// can store data in.
	/// @return data directory
	string getDataDir();

	/// @brief enables verbose output
	void enableVerboseOutput();

	/// @brief verifies if verbose output is enabled
	///
	/// @return true if verbose output is enabled and false otherwise
	bool isVerboseOutputEnabled();

	/// @brief Reads the configuration file line by line and constructs
	/// key, value pairs based on them
	void parseConfigFile();

private:
	/// @brief Constructor
	CfgMgr();
	CfgMgr(CfgMgr const&);

	/// @brief Destructor
	~CfgMgr();

	/// @brief Overloaded assignment operator
	///
	/// This operator is overloaded in order not to allow assignment
	/// to the configuration manager object as we want only one instance
	/// of such in the whole scope.
	void operator=(CfgMgr const&);

	/// @brief Configuration parameters parser
	///
	/// This is an API for configuration parameters parser. The class
	/// is used for obtaining a single parameter value from a config
	/// file and converting it to a proper value type.
	class GetParam
	{
	public:
		/// @brief Constructor
		///
		/// @param c is the reference to the configuration manager instance.
		/// @param paramName is the parameter name.
		GetParam(const CfgMgr& c, const string paramName);

		/// @brief Overloaded operator for parameter values
		/// of the Triplet<uint32_t> type
	    operator Triplet<uint32_t>();

	    /// @brief Overloaded operator for parameter values
	    /// of the string type
	    operator string();

	private:
	    /// @brief A parameter value being processed
	    string param_value_;
	};

	/// @brief Creates configuration based on data provided
	/// in the configuration file
	void createConfig();

    /// @brief Creates a Pool6 object given a IPv6 prefix and the prefix length.
    ///
    /// @param addr is the IPv6 prefix of the pool.
    /// @param len is the prefix length.
    /// @param ptype is the type of IPv6 pool (Pool::PoolType). Note this is
    /// passed in as an int32_t and cast to PoolType to accommodate a
    /// polymorphic interface.
    /// @return returns a PoolPtr to the new Pool4 object.
	PoolPtr poolMaker(isc::asiolink::IOAddress &addr, uint32_t len,
			int32_t ptype = 0) {
		return (PoolPtr(new Pool6(static_cast<Lease::Type>(ptype), addr, len)));
	}

    /// @brief Creates a Pool6 object given starting and ending IPv6 addresses.
    ///
    /// @param min is the first IPv6 address in the pool.
    /// @param max is the last IPv6 address in the pool.
    /// @param ptype is the type of IPv6 pool (Pool::PoolType). Note this is
    /// passed in as an int32_t and cast to PoolType to accommodate a
    /// polymorphic interface.
    /// @return returns a PoolPtr to the new Pool4 object.
	PoolPtr poolMaker(isc::asiolink::IOAddress &min,
			isc::asiolink::IOAddress &max, int32_t ptype = 0) {
		return (PoolPtr(new Pool6(static_cast<Lease::Type>(ptype), min, max)));
	}

	/// @brief Initiates and stores parsed pools' values
	void createPool();

	/// @brief Instantiates the IPv6 Subnet based on a given IPv6 address
	/// and prefix length.
	///
	/// @param addr is IPv6 prefix of the subnet.
	/// @param len is the prefix length
	void initSubnet(isc::asiolink::IOAddress addr, uint8_t len);

	/// @brief Initiates and stores parsed subnet value
	void createSubnet();

	/// @brief A directory where data files (e.g. server-id) are stored
	string datadir_;

	/// @brief Determines wheter verbosed output shall be enabled
	bool verbose_;

	/// Pointer to the created subnet object.
	Subnet6Ptr subnet_;

	/// @brief A container for IPv6 pools
	PoolCollection pools_;

	/// @brief A container for configuration parameters
	CfgParams cfg_params_;
};

/// @brief Redirects input stream such as a file to a CfgParams instance
istream& operator>>(istream& is, CfgParams& params);

/// @brief Removes leading and trailing whitespaces from string
void trimWhitespaces(string& s);

}; // end of isc::dhcpMini namespace
}; // end of isc namespace

#endif // DHCP6_MINI_CONFIG_MANAGER_H
