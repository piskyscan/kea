// Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

/// @file load_unload.cc Defines the load and unload hooks library functions.

#include <config.h>

#include <hooks/hooks.h>
#include <subnets_test_log.h>

using namespace isc::hooks;
using namespace isc::subnets_test;


// Functions accessed by the hooks framework use C linkage to avoid the name
// mangling that accompanies use of the C++ compiler as well as to avoid
// issues related to namespaces.
extern "C" {

/// @brief Called by the Hooks library manager when the library is loaded.
///
/// @return Returns 0 upon success, non-zero upon failure.
int load(LibraryHandle&) {
    LOG_INFO(subnets_test_logger, SUBNETS_TEST_LOAD);
    return (0);
}

/// @brief Called by the Hooks library manager when the library is unloaded.
///
/// @return Always returns 0.
int unload() {
    LOG_INFO(subnets_test_logger, SUBNETS_TEST_UNLOAD);
    return (0);
}

}
