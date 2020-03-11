SUBDIRS = .

AM_CPPFLAGS = -I$(top_builddir)/src/lib -I$(top_srcdir)/src/lib
AM_CPPFLAGS += -I$(top_builddir)/src/hooks/dhcp/subnets_test -I$(top_srcdir)/src/hooks/dhcp/subnets_test
AM_CPPFLAGS += $(BOOST_INCLUDES)
AM_CPPFLAGS += -DBOOTP_LIB_SO=\"$(abs_top_builddir)/src/hooks/dhcp/subnets_test/.libs/libdhcp_subnets_test.so\"
AM_CPPFLAGS += -DINSTALL_PROG=\"$(abs_top_srcdir)/install-sh\"

AM_CXXFLAGS = $(KEA_CXXFLAGS)

if USE_STATIC_LINK
AM_LDFLAGS = -static
endif

# Unit test data files need to get installed.
EXTRA_DIST =

CLEANFILES = *.gcno *.gcda

# TESTS_ENVIRONMENT = $(LIBTOOL) --mode=execute $(VALGRIND_COMMAND)
LOG_COMPILER = $(LIBTOOL)
AM_LOG_FLAGS = --mode=execute

TESTS =
if HAVE_GTEST
TESTS += subnets_test_unittests

subnets_test_unittests_SOURCES = run_unittests.cc
subnets_test_unittests_SOURCES += subnets_test_unittests.cc

subnets_test_unittests_CPPFLAGS = $(AM_CPPFLAGS) $(GTEST_INCLUDES) $(LOG4CPLUS_INCLUDES)

subnets_test_unittests_LDFLAGS  = $(AM_LDFLAGS) $(CRYPTO_LDFLAGS) $(GTEST_LDFLAGS)

subnets_test_unittests_CXXFLAGS = $(AM_CXXFLAGS)

subnets_test_unittests_LDADD  = $(top_builddir)/src/hooks/dhcp/subnets_test/libsubnets_test.la
subnets_test_unittests_LDADD += $(top_builddir)/src/lib/stats/libkea-stats.la
subnets_test_unittests_LDADD += $(top_builddir)/src/lib/dhcp/libkea-dhcp++.la
subnets_test_unittests_LDADD += $(top_builddir)/src/lib/hooks/libkea-hooks.la
subnets_test_unittests_LDADD += $(top_builddir)/src/lib/database/libkea-database.la
subnets_test_unittests_LDADD += $(top_builddir)/src/lib/cc/libkea-cc.la
subnets_test_unittests_LDADD += $(top_builddir)/src/lib/asiolink/libkea-asiolink.la
subnets_test_unittests_LDADD += $(top_builddir)/src/lib/dns/libkea-dns++.la
subnets_test_unittests_LDADD += $(top_builddir)/src/lib/cryptolink/libkea-cryptolink.la
subnets_test_unittests_LDADD += $(top_builddir)/src/lib/log/libkea-log.la
subnets_test_unittests_LDADD += $(top_builddir)/src/lib/util/libkea-util.la
subnets_test_unittests_LDADD += $(top_builddir)/src/lib/exceptions/libkea-exceptions.la
subnets_test_unittests_LDADD += $(LOG4CPLUS_LIBS)
subnets_test_unittests_LDADD += $(CRYPTO_LIBS)
subnets_test_unittests_LDADD += $(BOOST_LIBS)
subnets_test_unittests_LDADD += $(GTEST_LDADD)
endif
noinst_PROGRAMS = $(TESTS)
