// Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <cc/parser_context.h>
#include <cc/parser.h>
#include <exceptions/exceptions.h>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <limits>

namespace isc {
namespace data {

ParserContext::~ParserContext()
{
}

ElementPtr
ParserContext::parseString(const std::string& str)
{
    scanStringBegin(str);
    return (parseCommon());
}

ElementPtr
ParserContext::parseFile(const std::string& filename)
{
    FILE* f = fopen(filename.c_str(), "r");
    if (!f) {
	isc_throw(ParseError, "Unable to open file " << filename);
    }
    scanFileBegin(f, filename);
    return (parseCommon());
}

ElementPtr
ParserContext::parseCommon() {
    Parser parser(*this);
    try {
	ElementPtr res = parser.parse();
	if (!res) {
	    isc_throw(ParseError, "Parser abort");
	}
	scanEnd();
    }
    catch (...) {
	scanEnd();
	throw;
    }
    return (res);
}

void
ParserContext::error(const location& loc,
		     const std::string& what,
		     size_t pos)
{
    if (pos == 0) {
	isc_throw(EvalParseError, loc << ": " << what);
    } else {
	isc_throw(EvalParseError, loc << " (near " << pos << "): " << what);
    }
}

void
ParserContext::error (const std::string& what)
{
    isc_throw(EvalParseError, what);
}

void
ParserContext::fatal (const std::string& what)
{
    isc_throw(Unexpected, what);
}
