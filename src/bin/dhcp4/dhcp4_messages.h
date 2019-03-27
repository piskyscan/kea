// File created from ../../../src/bin/dhcp4/dhcp4_messages.mes on Mon Mar 25 2019 20:13

#ifndef DHCP4_MESSAGES_H
#define DHCP4_MESSAGES_H

#include <log/message_types.h>

namespace isc {
namespace dhcp {

extern const isc::log::MessageID DHCP4_ACTIVATE_INTERFACE;
extern const isc::log::MessageID DHCP4_ALREADY_RUNNING;
extern const isc::log::MessageID DHCP4_BUFFER_RECEIVED;
extern const isc::log::MessageID DHCP4_BUFFER_RECEIVE_FAIL;
extern const isc::log::MessageID DHCP4_BUFFER_UNPACK;
extern const isc::log::MessageID DHCP4_BUFFER_WAIT_SIGNAL;
extern const isc::log::MessageID DHCP4_CB_FETCH_UPDATES_FAIL;
extern const isc::log::MessageID DHCP4_CB_FETCH_UPDATES_RETRIES_EXHAUSTED;
extern const isc::log::MessageID DHCP4_CLASS_ASSIGNED;
extern const isc::log::MessageID DHCP4_CLASS_UNCONFIGURED;
extern const isc::log::MessageID DHCP4_CLASS_UNDEFINED;
extern const isc::log::MessageID DHCP4_CLASS_UNTESTABLE;
extern const isc::log::MessageID DHCP4_CLIENTID_IGNORED_FOR_LEASES;
extern const isc::log::MessageID DHCP4_CLIENT_FQDN_DATA;
extern const isc::log::MessageID DHCP4_CLIENT_FQDN_PROCESS;
extern const isc::log::MessageID DHCP4_CLIENT_HOSTNAME_DATA;
extern const isc::log::MessageID DHCP4_CLIENT_HOSTNAME_PROCESS;
extern const isc::log::MessageID DHCP4_CLIENT_NAME_PROC_FAIL;
extern const isc::log::MessageID DHCP4_COMMAND_RECEIVED;
extern const isc::log::MessageID DHCP4_CONFIG_COMPLETE;
extern const isc::log::MessageID DHCP4_CONFIG_LOAD_FAIL;
extern const isc::log::MessageID DHCP4_CONFIG_NEW_SUBNET;
extern const isc::log::MessageID DHCP4_CONFIG_OPTION_DUPLICATE;
extern const isc::log::MessageID DHCP4_CONFIG_PACKET_QUEUE;
extern const isc::log::MessageID DHCP4_CONFIG_RECEIVED;
extern const isc::log::MessageID DHCP4_CONFIG_START;
extern const isc::log::MessageID DHCP4_CONFIG_UPDATE;
extern const isc::log::MessageID DHCP4_DB_RECONNECT_ATTEMPT_FAILED;
extern const isc::log::MessageID DHCP4_DB_RECONNECT_ATTEMPT_SCHEDULE;
extern const isc::log::MessageID DHCP4_DB_RECONNECT_DISABLED;
extern const isc::log::MessageID DHCP4_DB_RECONNECT_NO_DB_CTL;
extern const isc::log::MessageID DHCP4_DB_RECONNECT_RETRIES_EXHAUSTED;
extern const isc::log::MessageID DHCP4_DDNS_REQUEST_SEND_FAILED;
extern const isc::log::MessageID DHCP4_DEACTIVATE_INTERFACE;
extern const isc::log::MessageID DHCP4_DECLINE_LEASE;
extern const isc::log::MessageID DHCP4_DECLINE_LEASE_MISMATCH;
extern const isc::log::MessageID DHCP4_DECLINE_LEASE_NOT_FOUND;
extern const isc::log::MessageID DHCP4_DHCP4O6_BAD_PACKET;
extern const isc::log::MessageID DHCP4_DHCP4O6_PACKET_RECEIVED;
extern const isc::log::MessageID DHCP4_DHCP4O6_PACKET_SEND;
extern const isc::log::MessageID DHCP4_DHCP4O6_PACKET_SEND_FAIL;
extern const isc::log::MessageID DHCP4_DHCP4O6_RECEIVE_FAIL;
extern const isc::log::MessageID DHCP4_DHCP4O6_RECEIVING;
extern const isc::log::MessageID DHCP4_DHCP4O6_RESPONSE_DATA;
extern const isc::log::MessageID DHCP4_DYNAMIC_RECONFIGURATION;
extern const isc::log::MessageID DHCP4_DYNAMIC_RECONFIGURATION_FAIL;
extern const isc::log::MessageID DHCP4_EMPTY_HOSTNAME;
extern const isc::log::MessageID DHCP4_FLEX_ID;
extern const isc::log::MessageID DHCP4_GENERATE_FQDN;
extern const isc::log::MessageID DHCP4_HANDLE_SIGNAL_EXCEPTION;
extern const isc::log::MessageID DHCP4_HOOKS_LIBS_RELOAD_FAIL;
extern const isc::log::MessageID DHCP4_HOOK_BUFFER_RCVD_DROP;
extern const isc::log::MessageID DHCP4_HOOK_BUFFER_RCVD_SKIP;
extern const isc::log::MessageID DHCP4_HOOK_BUFFER_SEND_SKIP;
extern const isc::log::MessageID DHCP4_HOOK_DECLINE_SKIP;
extern const isc::log::MessageID DHCP4_HOOK_LEASE4_RELEASE_SKIP;
extern const isc::log::MessageID DHCP4_HOOK_LEASES4_COMMITTED_DROP;
extern const isc::log::MessageID DHCP4_HOOK_LEASES4_COMMITTED_PARK;
extern const isc::log::MessageID DHCP4_HOOK_PACKET_RCVD_SKIP;
extern const isc::log::MessageID DHCP4_HOOK_PACKET_SEND_SKIP;
extern const isc::log::MessageID DHCP4_HOOK_SUBNET4_SELECT_DROP;
extern const isc::log::MessageID DHCP4_HOOK_SUBNET4_SELECT_SKIP;
extern const isc::log::MessageID DHCP4_INFORM_DIRECT_REPLY;
extern const isc::log::MessageID DHCP4_INIT_FAIL;
extern const isc::log::MessageID DHCP4_INIT_REBOOT;
extern const isc::log::MessageID DHCP4_LEASE_ADVERT;
extern const isc::log::MessageID DHCP4_LEASE_ALLOC;
extern const isc::log::MessageID DHCP4_NCR_CREATE;
extern const isc::log::MessageID DHCP4_NCR_CREATION_FAILED;
extern const isc::log::MessageID DHCP4_NOT_RUNNING;
extern const isc::log::MessageID DHCP4_NO_LEASE_INIT_REBOOT;
extern const isc::log::MessageID DHCP4_NO_SOCKETS_OPEN;
extern const isc::log::MessageID DHCP4_OPEN_CONFIG_DB;
extern const isc::log::MessageID DHCP4_OPEN_SOCKET;
extern const isc::log::MessageID DHCP4_OPEN_SOCKET_FAIL;
extern const isc::log::MessageID DHCP4_PACKET_DROP_0001;
extern const isc::log::MessageID DHCP4_PACKET_DROP_0002;
extern const isc::log::MessageID DHCP4_PACKET_DROP_0003;
extern const isc::log::MessageID DHCP4_PACKET_DROP_0004;
extern const isc::log::MessageID DHCP4_PACKET_DROP_0005;
extern const isc::log::MessageID DHCP4_PACKET_DROP_0006;
extern const isc::log::MessageID DHCP4_PACKET_DROP_0007;
extern const isc::log::MessageID DHCP4_PACKET_DROP_0008;
extern const isc::log::MessageID DHCP4_PACKET_DROP_0009;
extern const isc::log::MessageID DHCP4_PACKET_NAK_0001;
extern const isc::log::MessageID DHCP4_PACKET_NAK_0002;
extern const isc::log::MessageID DHCP4_PACKET_NAK_0003;
extern const isc::log::MessageID DHCP4_PACKET_NAK_0004;
extern const isc::log::MessageID DHCP4_PACKET_OPTIONS_SKIPPED;
extern const isc::log::MessageID DHCP4_PACKET_PACK;
extern const isc::log::MessageID DHCP4_PACKET_PACK_FAIL;
extern const isc::log::MessageID DHCP4_PACKET_PROCESS_EXCEPTION;
extern const isc::log::MessageID DHCP4_PACKET_PROCESS_STD_EXCEPTION;
extern const isc::log::MessageID DHCP4_PACKET_RECEIVED;
extern const isc::log::MessageID DHCP4_PACKET_SEND;
extern const isc::log::MessageID DHCP4_PACKET_SEND_FAIL;
extern const isc::log::MessageID DHCP4_PARSER_COMMIT_EXCEPTION;
extern const isc::log::MessageID DHCP4_PARSER_COMMIT_FAIL;
extern const isc::log::MessageID DHCP4_PARSER_EXCEPTION;
extern const isc::log::MessageID DHCP4_PARSER_FAIL;
extern const isc::log::MessageID DHCP4_POST_ALLOCATION_NAME_UPDATE_FAIL;
extern const isc::log::MessageID DHCP4_QUERY_DATA;
extern const isc::log::MessageID DHCP4_RELEASE;
extern const isc::log::MessageID DHCP4_RELEASE_EXCEPTION;
extern const isc::log::MessageID DHCP4_RELEASE_FAIL;
extern const isc::log::MessageID DHCP4_RELEASE_FAIL_NO_LEASE;
extern const isc::log::MessageID DHCP4_RELEASE_FAIL_WRONG_CLIENT;
extern const isc::log::MessageID DHCP4_RESERVED_HOSTNAME_ASSIGNED;
extern const isc::log::MessageID DHCP4_RESPONSE_DATA;
extern const isc::log::MessageID DHCP4_RESPONSE_FQDN_DATA;
extern const isc::log::MessageID DHCP4_RESPONSE_HOSTNAME_DATA;
extern const isc::log::MessageID DHCP4_RESPONSE_HOSTNAME_GENERATE;
extern const isc::log::MessageID DHCP4_SERVER_FAILED;
extern const isc::log::MessageID DHCP4_SHUTDOWN;
extern const isc::log::MessageID DHCP4_SHUTDOWN_REQUEST;
extern const isc::log::MessageID DHCP4_SRV_CONSTRUCT_ERROR;
extern const isc::log::MessageID DHCP4_SRV_D2STOP_ERROR;
extern const isc::log::MessageID DHCP4_SRV_DHCP4O6_ERROR;
extern const isc::log::MessageID DHCP4_STARTED;
extern const isc::log::MessageID DHCP4_STARTING;
extern const isc::log::MessageID DHCP4_START_INFO;
extern const isc::log::MessageID DHCP4_SUBNET_DATA;
extern const isc::log::MessageID DHCP4_SUBNET_DYNAMICALLY_CHANGED;
extern const isc::log::MessageID DHCP4_SUBNET_SELECTED;
extern const isc::log::MessageID DHCP4_SUBNET_SELECTION_FAILED;
extern const isc::log::MessageID DHCP6_DHCP4O6_PACKET_RECEIVED;

} // namespace dhcp
} // namespace isc

#endif // DHCP4_MESSAGES_H
