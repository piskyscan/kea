SUBDIRS = . tests

AM_CPPFLAGS = -I$(top_srcdir)/src/lib -I$(top_builddir)/src/lib
AM_CPPFLAGS += $(BOOST_INCLUDES)
AM_CXXFLAGS = $(KEA_CXXFLAGS)

lib_LTLIBRARIES = libkea-cc.la
libkea_cc_la_SOURCES = base_stamped_element.cc base_stamped_element.h
libkea_cc_la_SOURCES += data.cc data.h
libkea_cc_la_SOURCES += element_value.h
libkea_cc_la_SOURCES += cfg_to_element.h dhcp_config_error.h
libkea_cc_la_SOURCES += command_interpreter.cc command_interpreter.h
libkea_cc_la_SOURCES += json_feed.cc json_feed.h
libkea_cc_la_SOURCES += server_tag.cc server_tag.h
libkea_cc_la_SOURCES += simple_parser.cc simple_parser.h
libkea_cc_la_SOURCES += stamped_element.cc stamped_element.h
libkea_cc_la_SOURCES += stamped_value.cc stamped_value.h
libkea_cc_la_SOURCES += user_context.cc user_context.h

libkea_cc_la_LIBADD  = $(top_builddir)/src/lib/asiolink/libkea-asiolink.la
libkea_cc_la_LIBADD += $(top_builddir)/src/lib/util/libkea-util.la
libkea_cc_la_LIBADD += $(top_builddir)/src/lib/exceptions/libkea-exceptions.la
libkea_cc_la_LIBADD += $(BOOST_LIBS)

libkea_cc_la_LDFLAGS = -no-undefined -version-info 9:5:1

# Specify the headers for copying into the installation directory tree.
libkea_cc_includedir = $(pkgincludedir)/cc
libkea_cc_include_HEADERS = \
	base_stamped_element.h \
	cfg_to_element.h \
	command_interpreter.h \
	data.h \
	dhcp_config_error.h \
	element_value.h \
	json_feed.h \
	server_tag.h \
	simple_parser.h \
	stamped_element.h \
	stamped_value.h \
	user_context.h

EXTRA_DIST = cc.dox

CLEANFILES = *.gcno *.gcda

if GENERATE_PARSER

parser: lexer.cc location.hh position.hh stack.hh parser.cc parser.h
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
location.hh position.hh stack.hh parser.cc parser.h: parser.yy
	$(YACC) --defines=parser.h --report=all --report_file=parser.report -o parser.cc parser.yy

lexer.cc: lexer.ll
	$(LEX) --prefix parser -o lexer.cc lexer.ll

else

parser location.hh position.hh stack.hh parser.cc parser.h lexer.cc:
	@echo Parser generation disabled. Configure with --enable-generate-parser to enable it.

endif

