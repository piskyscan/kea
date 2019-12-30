// Copyright (C) 2019 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

/// @file This file contains tests which exercise the load and unload
/// functions in the script hook library. In order to test
/// the load function, one must be able to pass it hook library
/// parameters. The the only way to populate these parameters is by
/// actually loading the library via HooksManager::loadLibraries().

#include <config.h>

#include <cc/data.h>
#include <dhcpsrv/cfgmgr.h>
#include <hooks/hooks_manager.h>
#include <process/daemon.h>

#include <gtest/gtest.h>
#include <errno.h>

using namespace std;
using namespace isc;
using namespace isc::hooks;
using namespace isc::data;
using namespace isc::dhcp;
using namespace isc::process;

namespace {

/// @brief Test fixture for testing loading and unloading the Scripts library
class LibLoadTest : public ::testing::Test {
public:
    /// @brief Constructor
    LibLoadTest() {
        reset();
    }

    /// @brief Destructor
    /// Removes files that may be left over from previous tests
    virtual ~LibLoadTest() {
        reset();
    }

    /// @brief Removes files that may be left over from previous tests
    virtual void reset() {
        HooksManager::unloadLibraries();
    }

    /// @brief Adds library/parameters to list of libraries to be loaded
    void addLib(const std::string& lib, ConstElementPtr params) {
        libraries_.push_back(make_pair(lib, params));
    }

    /// @brief Load all specified libraries.
    ///
    /// The libraries are stored in libraries_
    bool loadLibs() {
        return (HooksManager::loadLibraries(libraries_));
    }

    /// @brief Unloads all libraries.
    void unloadLibs() {
        EXPECT_NO_THROW(HooksManager::unloadLibraries());
    }

    /// @brief Return valid configuration with two scripts in JSON format.
    ConstElementPtr createGoodConfig() const;

    /// @brief Return invalid configuration (no scripts at all) in JSON format.
    ConstElementPtr createBadConfig() const;

    HookLibsCollection libraries_;
};

ConstElementPtr
LibLoadTest::createGoodConfig() const {
    std::string config_text ="[ \"script1.sh\", \"script2.py\" ]";
    return (Element::fromJSON(config_text));
}

ConstElementPtr
LibLoadTest::createBadConfig() const {
    std::string config_text ="[ ]"; // empty list

    return (Element::fromJSON(config_text));
}


// Simple test that checks the library can be loaded in a DHCPv4 server.
TEST_F(LibLoadTest, validLoadDhcp4) {
    // Prepare parameters,
    ElementPtr params = Element::createMap();
    params->set("scripts", createGoodConfig());

    // Set family and proc name.
    CfgMgr::instance().setFamily(AF_INET);
    Daemon::setProcName("kea-dhcp4");

    addLib(LIBDHCP_SCRIPTS_SO, params);
    EXPECT_TRUE(loadLibs());
}

// Simple test that checks the library can be loaded in a DHCPv6 server.
TEST_F(LibLoadTest, validLoadDhcp6) {
    // Prepare parameters,
    ElementPtr params = Element::createMap();
    params->set("scripts", createGoodConfig());

    // Set family and proc name.
    CfgMgr::instance().setFamily(AF_INET6);
    Daemon::setProcName("kea-dhcp6");

    addLib(LIBDHCP_SCRIPTS_SO, params);
    EXPECT_TRUE(loadLibs());
}

// Simple test that checks the library will refuse to load if
// no scripts are specified.
TEST_F(LibLoadTest, invalidLoadDhcp4) {
    // Prepare parameters,
    ElementPtr params = Element::createMap();
    params->set("scripts", createBadConfig());

    // Set family and proc name.
    CfgMgr::instance().setFamily(AF_INET);
    Daemon::setProcName("kea-dhcp4");

    addLib(LIBDHCP_SCRIPTS_SO, params);

    // At least one script should be specified.
    EXPECT_FALSE(loadLibs());
}

// Simple test that checks the library can be loaded in a DHCPv6 server
// only if it is set for IPv6.
TEST_F(LibLoadTest, invalidLoadDhcp6) {
    // Prepare parameters,
    ElementPtr params = Element::createMap();
    params->set("scripts", createBadConfig());

    // Set family and proc name.
    CfgMgr::instance().setFamily(AF_INET6);
    Daemon::setProcName("kea-dhcp6");

    addLib(LIBDHCP_SCRIPTS_SO, params);

    // At least one script should be specified.
    EXPECT_FALSE(loadLibs());
}

// Simple test that checks the library can be loaded and unloaded several times
// in a DHCPv4 server.
TEST_F(LibLoadTest, severalLoads4) {

    // Prepare parameters,
    ElementPtr params = Element::createMap();
    params->set("scripts", createGoodConfig());

    // Set family and proc name.
    CfgMgr::instance().setFamily(AF_INET);
    Daemon::setProcName("kea-dhcp4");

    addLib(LIBDHCP_SCRIPTS_SO, params);

    EXPECT_TRUE(loadLibs());
    unloadLibs();

    EXPECT_TRUE(loadLibs());
    unloadLibs();
}

// Simple test that checks the library can be loaded and unloaded several times
// in a DHCPv6 server.
TEST_F(LibLoadTest, severalLoads6) {

    // Prepare parameters,
    ElementPtr params = Element::createMap();
    params->set("scripts", createGoodConfig());

    // Set family and proc name.
    CfgMgr::instance().setFamily(AF_INET6);
    Daemon::setProcName("kea-dhcp6");

    addLib(LIBDHCP_SCRIPTS_SO, params);

    EXPECT_TRUE(loadLibs());
    unloadLibs();

    EXPECT_TRUE(loadLibs());
    unloadLibs();
}

} // end of anonymous namespace
