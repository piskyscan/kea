// Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PARSER_CONTEXT_H
#define PARSER_CONTEXT_H
#include <cc/data.h>
#include <cc/parser.h>
#include <cc/parser_context_decl.h>
#include <exceptions/exceptions.h>
#include <map>
#include <string>

// Tell Flex the lexer's prototype ...
#define YY_DECL \
    isc::data::Parser::symbol_type parserlex(ParserContext& driver)

// ... and declare it for the parser's sake.
YY_DECL;

namespace isc {
namespace data {

/// @brief Evaluation context, an interface to the data parser.
class ParserContext
{
public:

    /// @brief Destructor.
    virtual ~ParserContext() {}

    /// @brief Method called before scanning starts on a string.
    ///
    /// @param str string to be parsed.
    void scanStringBegin(const std::string& str);

    /// @brief Method called before scanning starts on a file.
    ///
    /// @param f stdio FILE pointer.
    /// @param filename file to be parsed.
    void scanFileBegin(FILE* f, const std::string& filename);

    /// @brief Method called after the last tokens are scanned.
    void scanEnd();

    /// @brief Run the parser on the string specified.
    ///
    /// @param str string to be parsed
    /// @return Element structure representing parsed text.
    ElementPtr parseString(const std::string& str);

    /// @brief Run the parser on the file specified.
    ///
    /// @param filename file to be parsed.
    /// @return Element structure representing parsed text.
    ElementPtr parseFile(const std::string& filename);

    /// @brief Error handler.
    ///
    /// @param loc location within the parsed file when experienced a problem.
    /// @param what string explaining the nature of the error.
    /// @param pos optional position for in string errors.
    /// @throw JSONError.
    static void error(const isc::data::location& loc,
                      const std::string& what,
                      size_t pos = 0);

    /// @brief Error handler.
    ///
    /// This is a simplified error reporting tool for possible future
    /// cases when the Parser is not able to handle the packet.
    ///
    /// @param what string explaining the nature of the error.
    /// @throw JSONError
    static void error(const std::string& what);

    /// @brief Fatal error handler.
    ///
    /// This is for should not happen but fatal errors.
    ///
    /// @param what string explaining the nature of the error.
    /// @throw isc::Unexpected.
    static void fatal(const std::string& what);

    /// @brief Converts bison's position to Element::Position.
    ///
    /// Convert a bison location into an element position.
    /// (take the begin, the end is lost)
    ///
    /// @param loc location in bison format.
    /// @return Position in format accepted by Element.
    Element::Position loc2pos(location& loc);

    /// @brief Check if a parameter is already present.
    ///
    /// Check if a parameter is already present in the map at the top
    /// of the stack and raise an error when it is.
    ///
    /// @param name name of the parameter to check.
    /// @param loc location of the current parameter.
    /// @throw JSONError.
    void unique(const std::string& name, Element::Position loc);

    /// @brief The string being parsed.
    std::string string_;

    /// @brief The name of the file being parsed.
    /// Used later to pass the file name to the location tracker.
    std::string file_;

    /// @brief sFile (aka FILE).
    FILE* sfile_;

    /// @brief JSON elements being parsed.
    std::vector<ElementPtr> stack_;

    /// @brief Location of the current token.
    ///
    /// The lexer will keep updating it. This variable will be useful
    /// for logging errors.
    location loc_;

private:
    /// @brief Common part of parseXXX.
    ///
    /// @return Element structure representing parsed text.
    ElementPtr parseCommon();
};

} // end of isc::data namespace
} // end of isc namespace

#endif
