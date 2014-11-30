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

#include <cfg_mgr.h>
#include <dhcp6-mini_log.h>
#include <dhcp6-mini_srv.h>

#include <stdlib.h> // for exit codes
#include <unistd.h> // for getopt
#include <iostream>

using namespace std;

namespace {
const char* const DHCP6_MINI_NAME = "b10-dhcp6-mini";

void usage() {
    cerr << "Usage: " << DHCP6_MINI_NAME << " [-h] [-v] [-p number]" << endl;
    cerr << "  -h: print this message" << endl;
    cerr << "  -v: verbose output" << endl;
    cerr << "  -p number: specify non-standard port number 1-65535 "
            << "(useful for testing only)" << endl;
    exit(EXIT_FAILURE);
}
} // end of anonymous namespace

int main(int argc, char* argv[]) {
    int ch;
    int port_number = DHCP6_SERVER_PORT; // The default. Any other values are
    // useful for testing only.
    bool verbose_mode = false; // Should server be verbose?

    while ((ch = getopt(argc, argv, "hvp:")) != -1) {
        switch (ch) {
        case 'v':
            verbose_mode = true;
            break;

        case 'p':
            port_number = strtol(optarg, NULL, 10);
            if (port_number <= 0 || port_number > 65535) {
                cerr << "Failed to parse port number: [" << optarg
                        << "], 1-65535 allowed." << endl;
                usage();
            }
            break;

        case 'h':
        default:
            usage();
        }
    }

    // Check for extraneous parameters.
    if (argc > optind) {
        usage();
    }

    // Initialize configuration manager
    isc::dhcpMini::CfgMgr::getInstance();

    // Check if verbose output shall be enabled
    if (verbose_mode) {
        LOG(INF) << "Verbose output enabled" << endl;
        isc::dhcpMini::CfgMgr::getInstance().enableVerboseOutput();
    }

    // Parse configuration file and create subnet and pools effectively
    isc::dhcpMini::CfgMgr::getInstance().parseConfigFile();

    LOG(INF) << "DHCPv6-mini server is starting..." << endl;
    LOG(INF) << "Port number: " << DHCP6_SERVER_PORT << endl;

    isc::dhcpMini::Dhcpv6Srv server(port_number);
    server.run();

    return (EXIT_SUCCESS);
}
