// Copyright (C) 2019 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <exceptions/exceptions.h>

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <gtest/gtest.h>
#include <scripts_cfg.h>

using namespace std;
using namespace hooks::scripts;

namespace {

#if 0
/// @brief Convenience method for reliably building test file path names.
///
/// Function prefixes the given file name with a path to unit tests directory
/// so we can reliably find test data files.
///
/// @param name base file name of the test file
std::string testFilePath(const std::string& name) {
    return (std::string(TEST_DATA_BUILDDIR) + "/" + name);
}
#endif


} // end of anonymous namespace
