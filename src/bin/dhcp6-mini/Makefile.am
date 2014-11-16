SUBDIRS = .

dhcp_data_dir = @localstatedir@/@PACKAGE@

AM_CPPFLAGS = -I$(top_srcdir)/src/lib -I$(top_builddir)/src/lib -DDHCP_DATA_DIR="\"$(dhcp_data_dir)\""
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

pkglibexec_PROGRAMS = b10-dhcp6-mini
b10_dhcp6_mini_SOURCES  = main.cc
b10_dhcp6_mini_SOURCES += addr_utilities.cc addr_utilities.h
b10_dhcp6_mini_SOURCES += alloc_engine.cc alloc_engine.h
b10_dhcp6_mini_SOURCES += cfg_mgr.cc cfg_mgr.h
b10_dhcp6_mini_SOURCES += dhcp6-mini_log.cc dhcp6-mini_log.h
b10_dhcp6_mini_SOURCES += dhcp6-mini_srv.cc dhcp6-mini_srv.h
b10_dhcp6_mini_SOURCES += io_utilities.h
b10_dhcp6_mini_SOURCES += key_from_key.h
b10_dhcp6_mini_SOURCES += lease.cc lease.h
b10_dhcp6_mini_SOURCES += lease_mgr.cc lease_mgr.h
b10_dhcp6_mini_SOURCES += option_space_container.h
b10_dhcp6_mini_SOURCES += pool.cc pool.h
b10_dhcp6_mini_SOURCES += range_utilities.h
b10_dhcp6_mini_SOURCES += subnet.cc subnet.h
b10_dhcp6_mini_SOURCES += triplet.h

b10_dhcp6_mini_LDADD  = $(top_builddir)/src/lib/dhcp/libb10-dhcp++.la
b10_dhcp6_mini_LDADD  += $(top_builddir)/src/lib/asiolink/libb10-asiolink.la
b10_dhcp6_mini_LDADD += $(top_builddir)/src/lib/exceptions/libb10-exceptions.la

b10_dhcp6_mini_dir = $(pkgdatadir)
