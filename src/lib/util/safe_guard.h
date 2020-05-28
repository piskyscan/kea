// Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef SAFE_GUARD_H
#define SAFE_GUARD_H

/// @file safe_guard.h
///
/// In some cases we do not want either a recursive mutex essentially
/// because it hides bugs nor a deadlock. This safe guard provides the
/// same than standard lock guard but throws instead of locks forever.

#include <exceptions/exceptions.h>
#include <mutex>

namespace isc {
namespace util {

/// @brief Safe Guard.
class SafeGuard : public boost::noncopyable {
public:

    /// @brief Constructor.
    ///
    /// Tries to acquire the mutex and throws when it is already locked.
    ///
    /// @param mtx The mutex
    SafeGuard(std::mutex& mtx) : mtx_(mtx), owns_(false) {
        if (mtx_.try_lock()) {
            owns_ = true;
        } else {
            isc_throw(InvalidOperation,
                      "Entering the guard with an already locked mutex");
        }
    }

    /// @brief Destructor.
    ///
    /// Release owned mutex.
    ~SafeGuard() {
        if (owns_) {
            owns_ = false;
            mtx_.unlock();
        }
    }

private:

    /// @brief The mutex.
    std::mutex& mtx_;

    /// @brief The owns flag.
    bool owns_;
};

} // namespace util
} // namespace isc

#endif // SAFE_GUARD_H
