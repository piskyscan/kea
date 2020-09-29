// Copyright (C) 2017-2020 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>
#include <dhcpsrv/cfg_shared_networks.h>

using namespace isc::asiolink;

namespace isc {
namespace dhcp {

void
CfgSharedNetworks4::addServerId(const SharedNetwork4Ptr& network) {
    const IOAddress& server_id = network->getServerId();
    if (server_id.isV4Zero()) {
        return;
    }
    auto& name_set = server_ids_[server_id];
    static_cast<void>(name_set.insert(network->getName()));
}

void
CfgSharedNetworks4::delServerId(const SharedNetwork4Ptr& network) {
    const IOAddress& server_id = network->getServerId();
    if (server_id.isV4Zero()) {
        return;
    }
    auto& name_set = server_ids_[server_id];
    static_cast<void>(name_set.erase(network->getName()));
    if (name_set.empty()) {
        static_cast<void>(server_ids_.erase(server_id));
    }
}

bool
CfgSharedNetworks4::hasNetworkWithServerId(const IOAddress& server_id) const {
    const auto& name_set = server_ids_.find(server_id);
    if (name_set != server_ids_.cend()) {
        // Should not happen.
        if (name_set->second.empty()) {
            return (false);
        }
        return (true);
    }
    return (false);
}

} // end of namespace isc::dhcp
} // end of namespace isc
