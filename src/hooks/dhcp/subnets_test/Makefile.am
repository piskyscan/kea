SUBDIRS = . tests

AM_CPPFLAGS  = -I$(top_builddir)/src/lib -I$(top_srcdir)/src/lib
AM_CPPFLAGS += $(BOOST_INCLUDES)
AM_CXXFLAGS  = $(KEA_CXXFLAGS)

EXTRA_DIST = libdhcp_subnets_test.dox
# Ensure that the message file is included in the distribution
EXTRA_DIST += subnets_test_messages.mes

CLEANFILES = *.gcno *.gcda

# convenience archive

noinst_LTLIBRARIES = libsubnets_test.la

libsubnets_test_la_SOURCES  =
libsubnets_test_la_SOURCES += load_unload.cc
libsubnets_test_la_SOURCES += subnets_test_callouts.cc
libsubnets_test_la_SOURCES += subnets_test_log.cc subnets_test_log.h
libsubnets_test_la_SOURCES += subnets_test_messages.cc subnets_test_messages.h
libsubnets_test_la_SOURCES += version.cc

libsubnets_test_la_CXXFLAGS = $(AM_CXXFLAGS)
libsubnets_test_la_CPPFLAGS = $(AM_CPPFLAGS)

noinst_LTLIBRARIES += libdhcp_subnets_test.la

libdhcp_subnets_test_la_SOURCES  =
libdhcp_subnets_test_la_LDFLAGS  = $(AM_LDFLAGS)
libdhcp_subnets_test_la_LDFLAGS  += -avoid-version -export-dynamic -module
# -rpath /nowhere is a hack to trigger libtool to not create a
# convenience archive, resulting in shared modules
libdhcp_subnets_test_la_LDFLAGS  += -rpath /nowhere
libdhcp_subnets_test_la_LIBADD  = libsubnets_test.la
libdhcp_subnets_test_la_LIBADD  += $(top_builddir)/src/lib/hooks/libkea-hooks.la
libdhcp_subnets_test_la_LIBADD  += $(top_builddir)/src/lib/log/libkea-log.la
libdhcp_subnets_test_la_LIBADD  += $(top_builddir)/src/lib/util/libkea-util.la
libdhcp_subnets_test_la_LIBADD  += $(top_builddir)/src/lib/exceptions/libkea-exceptions.la
libdhcp_subnets_test_la_LIBADD  += $(LOG4CPLUS_LIBS)

# If we want to get rid of all generated messages files, we need to use
# make maintainer-clean. The proper way to introduce custom commands for
# that operation is to define maintainer-clean-local target. However,
# make maintainer-clean also removes Makefile, so running configure script
# is required.  To make it easy to rebuild messages without going through
# reconfigure, a new target messages-clean has been added.
maintainer-clean-local:
	rm -f subnets_test_messages.h subnets_test_messages.cc

# To regenerate messages files, one can do:
#
# make messages-clean
# make messages
#
# This is needed only when a .mes file is modified.
messages-clean: maintainer-clean-local

if GENERATE_MESSAGES

# Define rule to build logging source files from message file
messages: subnets_test_messages.h subnets_test_messages.cc
	@echo Message files regenerated

subnets_test_messages.h subnets_test_messages.cc: subnets_test_messages.mes
	$(top_builddir)/src/lib/log/compiler/kea-msg-compiler $(top_srcdir)/src/hooks/dhcp/subnets_test/subnets_test_messages.mes

else

messages subnets_test_messages.h subnets_test_messages.cc:
	@echo Messages generation disabled. Configure with --enable-generate-messages to enable it.

endif

