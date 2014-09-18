#SUBDIRS = . tests

dhcp_data_dir = @localstatedir@/@PACKAGE@

AM_CPPFLAGS = -I$(top_srcdir)/src/lib -I$(top_builddir)/src/lib -DDHCP_DATA_DIR="\"$(dhcp_data_dir)\""
#AM_CPPFLAGS += -I$(top_srcdir)/src/bin -I$(top_builddir)/src/bin
#AM_CPPFLAGS += -I$(top_srcdir)/src/lib/cc -I$(top_builddir)/src/lib/cc
AM_CPPFLAGS += $(BOOST_INCLUDES)

AM_CXXFLAGS = $(B10_CXXFLAGS)
if USE_CLANGPP
# Disable unused parameter warning caused by some Boost headers when compiling with clang
AM_CXXFLAGS += -Wno-unused-parameter
endif

if USE_STATIC_LINK
AM_LDFLAGS = -static
endif

pkglibexecdir = $(libexecdir)/@PACKAGE@

#CLEANFILES = spec_config.h dhcp6_messages.h dhcp6_messages.cc s-messages

## There is no manual for mini-kea
#man_MANS = b10-dhcp6.8
#DISTCLEANFILES = $(man_MANS)
#EXTRA_DIST = $(man_MANS) b10-dhcp6.xml dhcp6.spec
#
#if GENERATE_DOCS
#
#b10-dhcp6.8: b10-dhcp6.xml
#	@XSLTPROC@ --novalid --xinclude --nonet -o $@ \
#        http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl \
#        $(srcdir)/b10-dhcp6.xml
#
#else
#
#$(man_MANS):
#	@echo Man generation disabled.  Creating dummy $@.  Configure with --enable-generate-docs to enable it.
#	@echo Man generation disabled.  Remove this file, configure with --enable-generate-docs, and rebuild BIND 10 > $@
#
#endif

#spec_config.h: spec_config.h.pre
#	$(SED) -e "s|@@LOCALSTATEDIR@@|$(localstatedir)|" spec_config.h.pre >$@

#dhcp6_messages.h dhcp6_messages.cc: s-messages

#s-messages: dhcp6_messages.mes
#	$(top_builddir)/src/lib/log/compiler/message $(top_srcdir)/src/bin/dhcp6/dhcp6_messages.mes
#	touch $@

#BUILT_SOURCES = spec_config.h dhcp6_messages.h dhcp6_messages.cc

pkglibexec_PROGRAMS = b10-dhcp6-mini

b10_dhcp6_mini_SOURCES  = main.cc
b10_dhcp6_mini_SOURCES += addr_utilities.cc addr_utilities.h
b10_dhcp6_mini_SOURCES += cfg_mgr.cc cfg_mgr.h
b10_dhcp6_mini_SOURCES += dhcp6-mini_log.cc dhcp6-mini_log.h
b10_dhcp6_mini_SOURCES += dhcp6-mini_srv.cc dhcp6-mini_srv.h
b10_dhcp6_mini_SOURCES += io_address.cc io_address.h
b10_dhcp6_mini_SOURCES += key_from_key.h
b10_dhcp6_mini_SOURCES += lease.cc lease.h
b10_dhcp6_mini_SOURCES += option_space_container.h
b10_dhcp6_mini_SOURCES += pool.cc pool.h
b10_dhcp6_mini_SOURCES += subnet.cc subnet.h
b10_dhcp6_mini_SOURCES += triplet.h

#nodist_b10_dhcp6_mini_SOURCES = dhcp6_messages.h dhcp6_messages.cc
#EXTRA_DIST += dhcp6_messages.mes

b10_dhcp6_mini_LDADD  = $(top_builddir)/src/lib/dhcp/libb10-dhcp++.la
#b10_dhcp6_mini_LDADD  = $(top_builddir)/src/lib/asiolink/libb10-asiolink.la
#b10_dhcp6_mini_LDADD += $(top_builddir)/src/lib/cc/libb10-cc.la
#b10_dhcp6_mini_LDADD += $(top_builddir)/src/lib/config/libb10-cfgclient.la
#b10_dhcp6_mini_LDADD += $(top_builddir)/src/lib/dhcp/libb10-dhcp++.la
#b10_dhcp6_mini_LDADD += $(top_builddir)/src/lib/dhcp_ddns/libb10-dhcp_ddns.la
#b10_dhcp6_mini_LDADD += $(top_builddir)/src/lib/dhcpsrv/libb10-dhcpsrv.la
#b10_dhcp6_mini_LDADD += $(top_builddir)/src/lib/exceptions/libb10-exceptions.la
#b10_dhcp6_mini_LDADD += $(top_builddir)/src/lib/log/libb10-log.la
#b10_dhcp6_mini_LDADD += $(top_builddir)/src/lib/util/libb10-util.la
#b10_dhcp6_mini_LDADD += $(top_builddir)/src/lib/hooks/libb10-hooks.la

b10_dhcp6_mini_dir = $(pkgdatadir)
#b10_dhcp6_DATA = dhcp6.spec
