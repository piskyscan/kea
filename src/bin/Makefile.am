# The following build order must be maintained.
SUBDIRS = dhcp4 dhcp6 d2 agent admin lfc keactrl

if PERFDHCP
SUBDIRS += perfdhcp
endif

if KEA_SHELL
SUBDIRS += shell
endif

if KEA_RBAC
SUBDIRS += secure_shell reverse_proxy
endif

if HAVE_SYSREPO
SUBDIRS += netconf
endif

check-recursive: all-recursive
