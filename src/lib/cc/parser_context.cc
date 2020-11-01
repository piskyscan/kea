// Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <cc/parser_context.h>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <limits>

namespace isc {
namespace data {

ElementPtr
ParserContext::parseString(const std::string& str) {
    scanStringBegin(str);
    return (parseCommon());
}

ElementPtr
ParserContext::parseFile(const std::string& filename) {
    // zero out the errno to be safe.
    errno = 0;

    FILE* f = fopen(filename.c_str(), "r");
    if (!f) {
        isc_throw(InvalidOperation, "failed to read file '" << filename
                  << ": " << strerror(errno));
    }
    scanFileBegin(f, filename);
    return (parseCommon());
}

ElementPtr
ParserContext::parseCommon() {
    Parser parser(*this);
    try {
        int res = parser.parse();
        if (res != 0) {
            isc_throw(JSONError, "Parser abort");
        }
        scanEnd();
    }
    catch (...) {
        scanEnd();
        throw;
    }
    if (stack_.size() == 1) {
        return (stack_[0]);
    } else {
        isc_throw(JSONError, "Expected exactly one terminal Element expected, found "
                  << stack_.size());
    }
}

void
ParserContext::error(const location& loc,
                     const std::string& what,
                     size_t pos) {
    const std::string& file = *loc.begin.filename;
    const uint32_t line = loc.begin.line;
    const uint32_t column = loc.begin.column;
    if (pos == 0) {
        isc_throw(JSONError, what << " in " << file << ":" << line
                  << ":" << column);
    } else {
        isc_throw(JSONError, what << " in " << file << ":" << line
                  << ":" << column << " (near " << pos << ")");
    }
}

void
ParserContext::error (const std::string& what) {
    isc_throw(JSONError, what);
}

void
ParserContext::fatal (const std::string& what) {
    isc_throw(Unexpected, what);
}

Element::Position
ParserContext::loc2pos(location& loc) {
    const std::string& file = *loc.begin.filename;
    const uint32_t line = loc.begin.line;
    const uint32_t pos = loc.begin.column;
    return (Element::Position(file, line, pos));
}

void
ParserContext::unique(const std::string& name, Element::Position loc) {
    ConstElementPtr value = stack_.back()->get(name);
    if (value) {
        isc_throw(JSONError, loc << ": duplicate " << name
                  << " entries in JSON"
                  << " map (previous at " << value->getPosition() << ")");
    }
}

} // namespace data
} // namespace isc
