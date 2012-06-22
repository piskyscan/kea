// Copyright (C) 2011-2012  Internet Systems Consortium, Inc. ("ISC")
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

#include <config.h>
#include <iostream>
#include <log/dummylog.h>
#include <log/logger_support.h>
#include <dhcp6/ctrl_dhcp6_srv.h>

using namespace std;
using namespace isc::dhcp;

/// This file contains entry point (main() function) for standard DHCPv6 server
/// component for BIND10 framework. It parses command-line arguments and
/// instantiates ControlledDhcpv6Srv class that is responsible for establishing
/// connection with msgq (receiving commands and configuration) and also
/// creating Dhcpv6 server object as well.
///
/// For detailed explanation or relations between main(), ControlledDhcpv6Srv,
/// Dhcpv6Srv and other classes, see \ref dhcpv6Session.

namespace {

const char* const DHCP6_NAME = "b10-dhcp6";

void
usage() {
    cerr << "Usage: b10-dhcp6 [-v]"
         << endl;
    cerr << "\t-v: verbose output" << endl;
    cerr << "\t-p number: specify non-standard port number 1-65535 "
         << "(useful for testing only)" << endl;
    exit(EXIT_FAILURE);
}
} // end of anonymous namespace

int
main(int argc, char* argv[]) {
    int ch;
    int port_number = DHCP6_SERVER_PORT; // The default. Any other values are
                                         // useful for testing only.
    bool verbose_mode = false; // Should server be verbose?

    while ((ch = getopt(argc, argv, "vp:")) != -1) {
        switch (ch) {
        case 'v':
            verbose_mode = true;
            isc::log::denabled = true;
            break;
        case 'p':
            port_number = strtol(optarg, NULL, 10);
            if (port_number == 0) {
                cerr << "Failed to parse port number: [" << optarg
                     << "], 1-65535 allowed." << endl;
                usage();
            }
            break;
        case ':':
        default:
            usage();
        }
    }

    // Initialize logging.  If verbose, we'll use maximum verbosity.
    isc::log::initLogger(DHCP6_NAME,
                         (verbose_mode ? isc::log::DEBUG : isc::log::INFO),
                         isc::log::MAX_DEBUG_LEVEL, NULL);

    cout << "b10-dhcp6: My pid=" << getpid() << ", binding to port "
         << port_number << ", verbose " << (verbose_mode?"yes":"no") << endl;

    if (argc - optind > 0) {
        usage();
    }

    int ret = EXIT_SUCCESS;

    try {

        cout << "b10-dhcp6: Initiating DHCPv6 server operation." << endl;

        ControlledDhcpv6Srv* server = new ControlledDhcpv6Srv(port_number);
        server->run();
        delete server;
        server = NULL;

        cout << "[b10-dhcp6] Initiating DHCPv6 operation." << endl;

        /// @todo: pass verbose to the actual server once logging is implemented
        Dhcpv6Srv* srv = new Dhcpv6Srv(port_number);

        srv->run();

    } catch (const std::exception& ex) {
        cerr << "[b10-dhcp6] Server failed: " << ex.what() << endl;
        ret = EXIT_FAILURE;
    }

    return (ret);
}
