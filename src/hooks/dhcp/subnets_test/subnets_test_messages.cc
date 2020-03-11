// File created from ../../../../src/hooks/dhcp/subnets_test/subnets_test_messages.mes on Wed Mar 11 2020 20:24

#include <cstddef>
#include <log/message_types.h>
#include <log/message_initializer.h>

extern const isc::log::MessageID SUBNETS_TEST_LOAD = "SUBNETS_TEST_LOAD";
extern const isc::log::MessageID SUBNETS_TEST_SUBNET4_SELECT = "SUBNETS_TEST_SUBNET4_SELECT";
extern const isc::log::MessageID SUBNETS_TEST_SUBNET4_SELECT_ERROR = "SUBNETS_TEST_SUBNET4_SELECT_ERROR";
extern const isc::log::MessageID SUBNETS_TEST_SUBNET6_SELECT = "SUBNETS_TEST_SUBNET6_SELECT";
extern const isc::log::MessageID SUBNETS_TEST_SUBNET6_SELECT_ERROR = "SUBNETS_TEST_SUBNET6_SELECT_ERROR";
extern const isc::log::MessageID SUBNETS_TEST_UNLOAD = "SUBNETS_TEST_UNLOAD";

namespace {

const char* values[] = {
    "SUBNETS_TEST_LOAD", "Subnets test hooks library has been loaded",
    "SUBNETS_TEST_SUBNET4_SELECT", "Subnets test hooks library subnet4 selected subnet id %1",
    "SUBNETS_TEST_SUBNET4_SELECT_ERROR", "Subnets test hooks library subnet4 select error %1",
    "SUBNETS_TEST_SUBNET6_SELECT", "Subnets test hooks library subnet6 selected subnet id %1",
    "SUBNETS_TEST_SUBNET6_SELECT_ERROR", "Subnets test hooks library subnet6 select error %1",
    "SUBNETS_TEST_UNLOAD", "Subnets test hooks library has been unloaded",
    NULL
};

const isc::log::MessageInitializer initializer(values);

} // Anonymous namespace

