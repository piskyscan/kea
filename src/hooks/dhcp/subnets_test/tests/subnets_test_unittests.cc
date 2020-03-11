// Copyright (C) 2019-2020 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

/// @file This file contains tests which verify flexible option.

#include <config.h>
#include <dhcp/pkt4.h>
#include <hooks/callout_manager.h>
#include <hooks/hooks.h>

#include <gtest/gtest.h>
#include <sstream>
#include "../subnets_test_log.h"

using namespace std;
using namespace isc;
using namespace isc::dhcp;
using namespace isc::hooks;
using namespace isc::subnets_test;
using namespace isc::util;

namespace {

/// @brief Test fixture for exercising bootp library callout.
/// It fetches the CalloutManager and prepares stub packets that can be used in
/// tests.
class SubnetsTest : public ::testing::Test {
public:

    /// @brief Constructor.
    SubnetsTest() {
    }

    /// @brief Destructor.
    virtual ~SubnetsTest() {
    }
};

}  // namespace
