SUBDIRS = .

#EXTRA_DIST = testdata/get_config.json

AM_CPPFLAGS = -I$(top_srcdir)/src/lib -I$(top_builddir)/src/lib
AM_CPPFLAGS += -I$(top_srcdir)/src -I$(top_builddir)/src
AM_CPPFLAGS += -I$(top_srcdir)/src/bin -I$(top_builddir)/src/bin
AM_CPPFLAGS += -DTEST_DATA_BUILDDIR=\"$(abs_top_builddir)/src/bin/ngrv/tests\"
AM_CPPFLAGS += -DCFG_EXAMPLES=\"$(abs_top_srcdir)/doc/examples/ngrv\"
AM_CPPFLAGS += -DTEST_DATA_BUILDDIR=\"$(abs_top_builddir)/src/bin/ngrv/tests\"
AM_CPPFLAGS += -DSYNTAX_FILE=\"$(srcdir)/../ngrv_parser.yy\"
AM_CPPFLAGS += $(BOOST_INCLUDES) $(NGHTTP2_INCLUDES)

CLEANFILES = *.json *.log

AM_CXXFLAGS = $(KEA_CXXFLAGS)

if USE_STATIC_LINK
AM_LDFLAGS = -static
endif

TESTS_ENVIRONMENT = \
        $(LIBTOOL) --mode=execute $(VALGRIND_COMMAND)

TESTS =
if HAVE_GTEST

TESTS += ngrv_unittests

ngrv_unittests_SOURCES  = 
ngrv_unittests_SOURCES += run_unittests.cc

ngrv_unittests_CPPFLAGS = $(AM_CPPFLAGS) $(GTEST_INCLUDES)
ngrv_unittests_LDFLAGS = $(AM_LDFLAGS) $(CRYPTO_LDFLAGS)

ngrv_unittests_LDADD = $(top_builddir)/src/bin/ngrv/libngrv.la
ngrv_unittests_LDADD += $(top_builddir)/src/lib/process/testutils/libprocesstest.la
ngrv_unittests_LDADD += $(top_builddir)/src/lib/process/libkea-process.la
ngrv_unittests_LDADD += $(top_builddir)/src/lib/cfgrpt/libcfgrpt.la
ngrv_unittests_LDADD += $(top_builddir)/src/lib/config/libkea-cfgclient.la
ngrv_unittests_LDADD += $(top_builddir)/src/lib/dhcp/libkea-dhcp++.la
ngrv_unittests_LDADD += $(top_builddir)/src/lib/hooks/libkea-hooks.la
ngrv_unittests_LDADD += $(top_builddir)/src/lib/testutils/libkea-testutils.la
ngrv_unittests_LDADD += $(top_builddir)/src/lib/cc/libkea-cc.la
ngrv_unittests_LDADD += $(top_builddir)/src/lib/asiolink/libkea-asiolink.la
ngrv_unittests_LDADD += $(top_builddir)/src/lib/dns/libkea-dns++.la
ngrv_unittests_LDADD += $(top_builddir)/src/lib/cryptolink/libkea-cryptolink.la
ngrv_unittests_LDADD += $(top_builddir)/src/lib/log/libkea-log.la
ngrv_unittests_LDADD += $(top_builddir)/src/lib/util/unittests/libutil_unittests.la
ngrv_unittests_LDADD += $(top_builddir)/src/lib/util/libkea-util.la
ngrv_unittests_LDADD += $(top_builddir)/src/lib/exceptions/libkea-exceptions.la
ngrv_unittests_LDADD += $(LOG4CPLUS_LIBS) $(CRYPTO_LIBS)
ngrv_unittests_LDADD += $(BOOST_LIBS) $(GTEST_LDADD) $(NGHTTP2_LIBS)
endif

#noinst_EXTRA_DIST = configs-list.txt

noinst_PROGRAMS = $(TESTS)
