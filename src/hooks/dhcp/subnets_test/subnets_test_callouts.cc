// Copyright (C) 2017-2020 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// Functions accessed by the hooks framework use C linkage to avoid the name
// mangling that accompanies use of the C++ compiler as well as to avoid
// issues related to namespaces.

#include <config.h>

#include <dhcp/pkt4.h>
#include <dhcp/pkt6.h>
#include <dhcpsrv/subnet.h>
#include <exceptions/exceptions.h>
#include <hooks/hooks.h>
#include <subnets_test_log.h>

using namespace isc;
using namespace isc::dhcp;
using namespace isc::hooks;
using namespace isc::subnets_test;


extern "C" {
/// @brief  This callout is called at the "subnet4_select" hook.
///
/// This function randomly select one of available subnets
///
/// @param handle CalloutHandle which provides access to context.
///
/// @return 0 upon success, non-zero otherwise.
int subnet4_select(CalloutHandle& handle) {
    CalloutHandle::CalloutNextStep status = handle.getStatus();
    if (status == CalloutHandle::NEXT_STEP_DROP ||
        status == CalloutHandle::NEXT_STEP_SKIP) {
        return (0);
    }

    try {
        // Get subnet collection. If it's empty just bail nothing to do.
        const isc::dhcp::Subnet4Collection *subnets = NULL;
        handle.getArgument("subnet4collection", subnets);
        if (subnets->empty()) {
            return (0);
        }

        Subnet4Ptr subnet = (*subnets)[rand() % subnets->size()];
        handle.setArgument("subnet6", subnet);
    } catch (const std::exception& ex) {
        LOG_ERROR(subnets_test_logger, SUBNETS_TEST_SUBNET4_SELECT_ERROR)
            .arg(ex.what());
        return (1);
    }

    return (0);
}

/// @brief  This callout is called at the "subnet6_select" hook.
///
/// This function randomly select one of available subnets
///
/// @param handle CalloutHandle which provides access to context.
///
/// @return 0 upon success, non-zero otherwise.
int subnet6_select(CalloutHandle& handle) {
    CalloutHandle::CalloutNextStep status = handle.getStatus();
    if (status == CalloutHandle::NEXT_STEP_DROP ||
        status == CalloutHandle::NEXT_STEP_SKIP) {
        return (0);
    }

    try {
        // Get subnet collection. If it's empty just bail nothing to do.
        const isc::dhcp::Subnet6Collection *subnets = NULL;
        handle.getArgument("subnet6collection", subnets);
        if (subnets->empty()) {
            return (0);
        }

        Subnet6Ptr subnet = (*subnets)[rand() % subnets->size()];
        handle.setArgument("subnet6", subnet);
    } catch (const std::exception& ex) {
        LOG_ERROR(subnets_test_logger, SUBNETS_TEST_SUBNET6_SELECT_ERROR)
            .arg(ex.what());
        return (1);
    }

    return (0);
}

/// @brief  This callout is called at the "pkt4_send" hook.
///
/// This function sets the pkt remote address to relay address
///
/// @param handle CalloutHandle which provides access to context.
///
/// @return 0 upon success, non-zero otherwise.
int pkt4_send(CalloutHandle& handle) {
    CalloutHandle::CalloutNextStep status = handle.getStatus();
    if (status == CalloutHandle::NEXT_STEP_DROP) {
        return (0);
    }

    // Get the parameters.
    Pkt4Ptr query;
    Pkt4Ptr response;
    handle.getArgument("query4", query);
    handle.getArgument("response4", response);

    if (status == CalloutHandle::NEXT_STEP_SKIP) {
        isc_throw(InvalidOperation, "packet pack already handled");
    }

    response->setRemoteAddr(query->getRemoteAddr());

    return (0);
}

/// @brief  This callout is called at the "pkt6_send" hook.
///
/// This function sets the pkt remote address to relay address
///
/// @param handle CalloutHandle which provides access to context.
///
/// @return 0 upon success, non-zero otherwise.
int pkt6_send(CalloutHandle& handle) {
    CalloutHandle::CalloutNextStep status = handle.getStatus();
    if (status == CalloutHandle::NEXT_STEP_DROP) {
        return (0);
    }

    // Get the parameters.
    Pkt6Ptr query;
    Pkt6Ptr response;
    handle.getArgument("query6", query);
    handle.getArgument("response6", response);

    if (status == CalloutHandle::NEXT_STEP_SKIP) {
        isc_throw(InvalidOperation, "packet pack already handled");
    }

    response->setRemoteAddr(query->getRemoteAddr());

    return (0);
}

/// @brief This function is called to retrieve the multi-threading compatibility.
///
/// @return 1 which means compatible with multi-threading.
int multi_threading_compatible() {
    return (1);
}

} // end extern "C"
