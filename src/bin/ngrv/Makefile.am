SUBDIRS = . #tests

AM_CPPFLAGS = -I$(top_srcdir)/src/lib -I$(top_builddir)/src/lib
AM_CPPFLAGS += -I$(top_srcdir)/src/bin -I$(top_builddir)/src/bin
AM_CPPFLAGS += $(BOOST_INCLUDES) $(CRYPTO_CFLAGS) $(CRYPTO_INCLUDES)
AM_CPPFLAGS += $(NGHTTP2_INCLUDES)
AM_CXXFLAGS = $(KEA_CXXFLAGS)

if USE_STATIC_LINK
AM_LDFLAGS = -static
endif

CLEANFILES  = *.gcno *.gcda

# convenience archive

noinst_LTLIBRARIES = libngrv.la

libngrv_la_SOURCES  = 

#EXTRA_DIST = ngrv_messages.mes
#EXTRA_DIST += ngrv_lexer.ll
#EXTRA_DIST += ngrv_parser.yy

sbin_PROGRAMS = kea-ngrv

kea_ngrv_SOURCES  = ngrv.cc

kea_ngrv_LDADD  = #libngrv.la
kea_ngrv_LDADD += $(top_builddir)/src/lib/process/libkea-process.la
kea_ngrv_LDADD += $(top_builddir)/src/lib/cfgrpt/libcfgrpt.la
kea_ngrv_LDADD += $(top_builddir)/src/lib/config/libkea-cfgclient.la
kea_ngrv_LDADD += $(top_builddir)/src/lib/cc/libkea-cc.la
kea_ngrv_LDADD += $(top_builddir)/src/lib/log/libkea-log.la
kea_ngrv_LDADD += $(top_builddir)/src/lib/util/libkea-util.la
kea_ngrv_LDADD += $(top_builddir)/src/lib/exceptions/libkea-exceptions.la
kea_ngrv_LDADD += $(LOG4CPLUS_LIBS) $(CRYPTO_LIBS) $(BOOST_LIBS) $(NGHTTP2_LIBS)

kea_ngrv_LDFLAGS = $(AM_LDFLAGS) $(CRYPTO_LDFLAGS)

# If we want to get rid of all generated messages files, we need to use
# make maintainer-clean. The proper way to introduce custom commands for
# that operation is to define maintainer-clean-local target. However,
# make maintainer-clean also removes Makefile, so running configure script
# is required.  To make it easy to rebuild messages without going through
# reconfigure, a new target messages-clean has been added.
maintainer-clean-local:
	rm -f ngrv_messages.h ngrv_messages.cc

# To regenerate messages files, one can do:
#
# make messages-clean
# make messages
#
# This is needed only when a .mes file is modified.
messages-clean: maintainer-clean-local

if GENERATE_MESSAGES

# Define rule to build logging source files from message file
messages: ngrv_messages.h ngrv_messages.cc
	@echo Message files regenerated

ngrv_messages.h ngrv_messages.cc: ngrv_messages.mes
	$(top_builddir)/src/lib/log/compiler/kea-msg-compiler $(top_srcdir)/src/bin/ngrv/ngrv_messages.mes

else

messages ngrv_messages.h ngrv_messages.cc:
	@echo Messages generation disabled. Configure with --enable-generate-messages to enable it.

endif

if GENERATE_PARSER

parser: ngrv_lexer.cc location.hh position.hh stack.hh ngrv_parser.cc ngrv_parser.h
	@echo "Flex/bison files regenerated"

# --- Flex/Bison stuff below --------------------------------------------------
# When debugging grammar issues, it's useful to add -v to bison parameters.
# bison will generate parser.output file that explains the whole grammar.
# It can be used to manually follow what's going on in the parser.
# This is especially useful if yydebug_ is set to 1 as that variable
# will cause parser to print out its internal state.
# Call flex with -s to check that the default rule can be suppressed
# Call bison with -W to get warnings like unmarked empty rules
# Note C++11 deprecated register still used by flex < 2.6.0
location.hh position.hh stack.hh ngrv_parser.cc ngrv_parser.h: ngrv_parser.yy
	$(YACC) --defines=ngrv_parser.h --report=all --report-file=ngrv_parser.report -o ngrv_parser.cc ngrv_parser.yy

ngrv_lexer.cc: ngrv_lexer.ll
	$(LEX) --prefix ngrv_ -o ngrv_lexer.cc ngrv_lexer.ll

else

parser location.hh position.hh stack.hh ngrv_parser.cc ngrv_parser.h ngrv_lexer.cc:
	@echo Parser generation disabled. Configure with --enable-generate-parser to enable it.

endif
