// Copyright (C) 2014  Internet Systems Consortium, Inc. ("ISC")
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

#ifndef DHCP6_MINI_LOG_H
#define DHCP6_MINI_LOG_H

#include <sstream>

using namespace std;

#define LOG(level) isc::dhcpMini::Dhcp6MiniLogger().log(level)

/// @brief Log levels used to log information
typedef enum {
    ERR, WRN, INF, CFG, DBG
} LogLevel;

namespace isc {
namespace dhcpMini {

/// @brief Logger
///
/// This is an API for the logging functionality.
class Dhcp6MiniLogger {
public:
    /// @brief Constructor
    Dhcp6MiniLogger();

    /// @brief Destructor
    ~Dhcp6MiniLogger();

    /// @brief Constructs a log entry based on provided information
    ///
    /// @param ll is the information log level.
    /// @return returns a modified string stream with information to be logged.
    ostringstream& log(const LogLevel ll);

protected:
    /// @brief String stream with information to be logged
    ostringstream log_;

private:
    /// @brief Adds current time to the log entry
    void logTime();

    /// @brief Log level of a certain log entry
    LogLevel log_level_;
};

} // namespace dhcpMini
} // namespace isc

#endif // DHCP6_MINI_LOG_H
