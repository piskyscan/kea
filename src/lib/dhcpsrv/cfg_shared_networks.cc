// Copyright (C) 2017-2019 Internet Systems Consortium, Inc. ("ISC")
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
CfgSharedNetworks4::serverIdAdd(const SharedNetwork4Ptr& network) {
    const IOAddress& server_id = network->getServerId();
    if (server_id.isV4Zero()) {
        return;
    }
    auto& name_set = server_ids_[server_id];
    static_cast<void>(name_set.insert(network->getName()));
}

void
CfgSharedNetworks4::serverIdDel(const SharedNetwork4Ptr& network) {
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

void
CfgSharedNetworks4::add(const SharedNetwork4Ptr& network) {
    ParentType::add(network);
    serverIdAdd(network);
}

void
CfgSharedNetworks4::del(const std::string& name) {
    auto& index = networks_.template get<SharedNetworkNameIndexTag>();
    auto shared_network = index.find(name);
    if (shared_network != index.end()) {
        // Delete all subnets from the network
        (*shared_network)->delAll();

        // Then delete the network from the networks list.
        serverIdDel(*shared_network);
        index.erase(shared_network);
    } else {
        isc_throw(BadValue, "unable to delete non-existing network '"
                  << name << "' from shared networks configuration");
    }
}

uint64_t
CfgSharedNetworks4::del(const uint64_t id) {
    auto& index = networks_.template get<SharedNetworkIdIndexTag>();
    auto sn_range = index.equal_range(id);

    // For each shared network found, dereference the subnets belonging to it.
    for (auto it = sn_range.first; it != sn_range.second; ++it) {
        (*it)->delAll();
        serverIdDel(*it);
    }

    // Remove the shared networks.
    return (static_cast<uint64_t>(index.erase(id)));
}

void
CfgSharedNetworks4::merge(CfgOptionDefPtr cfg_def, CfgSharedNetworks4& other) {
    auto& index = networks_.template get<SharedNetworkNameIndexTag>();

    // Iterate over the subnets to be merged. They will replace the existing
    // subnets with the same id. All new subnets will be inserted into this
    // configuration.
    auto other_networks = other.getAll();
    for (auto other_network = other_networks->begin();
         other_network != other_networks->end(); ++other_network) {

        // In theory we should drop subnet assignments from "other". The
        // idea being  those that come from the CB should not have subnets_
        // populated.  We will quietly throw them away, just in case.
        (*other_network)->delAll();

        // Check if the other network exists in this config.
        auto existing_network = index.find((*other_network)->getName());
        if (existing_network != index.end()) {

            // Somehow the same instance is in both, skip it.
            if (*existing_network == *other_network) {
                continue;
            }

            // Network exists, which means we're updating it.
            // First we need to move its subnets to the new
            // version of the network.
            const auto subnets = (*existing_network)->getAllSubnets();

            auto copy_subnets(*subnets);
            for (auto subnet = copy_subnets.cbegin(); subnet != copy_subnets.cend(); ++subnet) {
                (*existing_network)->del((*subnet)->getID());
                (*other_network)->add(*subnet);
            }

            // Now we discard the existing copy of the network.
            serverIdDel(*existing_network);
            index.erase(existing_network);
        }

        // Create the network's options based on the given definitions.
        (*other_network)->getCfgOption()->createOptions(cfg_def);

        // Add the new/updated nework.
        static_cast<void>(networks_.push_back(*other_network));
        serverIdAdd(*other_network);
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

void
CfgSharedNetworks6::merge(CfgOptionDefPtr cfg_def, CfgSharedNetworks6& other) {
    auto& index = networks_.template get<SharedNetworkNameIndexTag>();

    // Iterate over the subnets to be merged. They will replace the existing
    // subnets with the same id. All new subnets will be inserted into this
    // configuration.
    auto other_networks = other.getAll();
    for (auto other_network = other_networks->begin();
         other_network != other_networks->end(); ++other_network) {

        // In theory we should drop subnet assignments from "other". The
        // idea being  those that come from the CB should not have subnets_
        // populated.  We will quietly throw them away, just in case.
        (*other_network)->delAll();

        // Check if the other network exists in this config.
        auto existing_network = index.find((*other_network)->getName());
        if (existing_network != index.end()) {

            // Somehow the same instance is in both, skip it.
            if (*existing_network == *other_network) {
                continue;
            }

            // Network exists, which means we're updating it.
            // First we need to move its subnets to the new
            // version of the network.
            const auto subnets = (*existing_network)->getAllSubnets();

            auto copy_subnets(*subnets);
            for (auto subnet = copy_subnets.cbegin(); subnet != copy_subnets.cend(); ++subnet) {
                (*existing_network)->del((*subnet)->getID());
                (*other_network)->add(*subnet);
            }

            // Now we discard the existing copy of the network.
            index.erase(existing_network);
        }

        // Create the network's options based on the given definitions.
        (*other_network)->getCfgOption()->createOptions(cfg_def);

        // Add the new/updated nework.
        static_cast<void>(networks_.push_back(*other_network));
    }
}

} // end of namespace isc::dhcp
} // end of namespace isc
