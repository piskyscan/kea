// Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TEST_MUTEX_UTILS_H
#define TEST_MUTEX_UTILS_H

#include <mutex>

namespace isc {
namespace test {

/// @brief Checks if specified mutex is already locked.
///
/// @param mtx Mutex.
bool isLocked(std::mutex& mtx) {
    std::unique_lock<std::mutex> test(mtx, std::defer_lock);
    return (test.try_lock());
}

}; // end of isc::test namespace
}; // end of isc namespace

#endif // TEST_MUTEX_UTILS_H
