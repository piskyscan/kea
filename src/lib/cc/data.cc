// Copyright (C) 2010-2020 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <cc/data.h>
#include <cc/parser_context.h>

#include <cstring>
#include <cassert>
#include <climits>
#include <list>
#include <map>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <cerrno>

#include <boost/lexical_cast.hpp>

#include <cmath>

using namespace std;

namespace {
const char* const WHITESPACE = " \b\f\n\r\t";
} // end anonymous namespace

namespace isc {
namespace data {

std::string
Element::Position::str() const {
    std::ostringstream ss;
    ss << file_ << ":" << line_ << ":" << pos_;
    return (ss.str());
}

std::ostream&
operator<<(std::ostream& out, const Element::Position& pos) {
    out << pos.str();
    return (out);
}

std::string
Element::str() const {
    std::stringstream ss;
    toJSON(ss);
    return (ss.str());
}

std::string
Element::toWire() const {
    std::stringstream ss;
    toJSON(ss);
    return (ss.str());
}

void
Element::toWire(std::ostream& ss) const {
    toJSON(ss);
}

bool
Element::getValue(int64_t&) const {
    return (false);
}

bool
Element::getValue(double&) const {
    return (false);
}

bool
Element::getValue(bool&) const {
    return (false);
}

bool
Element::getValue(std::string&) const {
    return (false);
}

bool
Element::getValue(std::vector<ElementPtr>&) const {
    return (false);
}

bool
Element::getValue(std::map<std::string, ConstElementPtr>&) const {
    return (false);
}

bool
Element::setValue(const long long int) {
    return (false);
}

bool
Element::setValue(const double) {
    return (false);
}

bool
Element::setValue(const bool) {
    return (false);
}

bool
Element::setValue(const std::string&) {
    return (false);
}

bool
Element::setValue(const std::vector<ElementPtr>&) {
    return (false);
}

bool
Element::setValue(const std::map<std::string, ConstElementPtr>&) {
    return (false);
}

ConstElementPtr
Element::get(const int) const {
    throwTypeError("get(int) called on a non-list Element");
}

ElementPtr
Element::getNonConst(const int) const {
    throwTypeError("get(int) called on a non-list Element");
}

void
Element::set(const size_t, ElementPtr) {
    throwTypeError("set(int, element) called on a non-list Element");
}

void
Element::add(ElementPtr) {
    throwTypeError("add() called on a non-list Element");
}

void
Element::remove(const int) {
    throwTypeError("remove(int) called on a non-list Element");
}

size_t
Element::size() const {
    throwTypeError("size() called on a non-list Element");
}

bool
Element::empty() const {
    throwTypeError("empty() called on a non-list Element");
}

ConstElementPtr
Element::get(const std::string&) const {
    throwTypeError("get(string) called on a non-map Element");
}

void
Element::set(const std::string&, ConstElementPtr) {
    throwTypeError("set(name, element) called on a non-map Element");
}

void
Element::remove(const std::string&) {
    throwTypeError("remove(string) called on a non-map Element");
}

bool
Element::contains(const std::string&) const {
    throwTypeError("contains(string) called on a non-map Element");
}

ConstElementPtr
Element::find(const std::string&) const {
    throwTypeError("find(string) called on a non-map Element");
}

bool
Element::find(const std::string&, ConstElementPtr&) const {
    return (false);
}

std::ostream&
operator<<(std::ostream& out, const Element& e) {
    return (out << e.str());
}

bool
operator==(const Element& a, const Element& b) {
    return (a.equals(b));
}

bool operator!=(const Element& a, const Element& b) {
    return (!a.equals(b));
}

//
// factory functions
//
ElementPtr
Element::create(const Position& pos) {
    return (ElementPtr(new NullElement(pos)));
}

ElementPtr
Element::create(const long long int i, const Position& pos) {
    return (ElementPtr(new IntElement(static_cast<int64_t>(i), pos)));
}

ElementPtr
Element::create(const int i, const Position& pos) {
    return (create(static_cast<long long int>(i), pos));
}

ElementPtr
Element::create(const long int i, const Position& pos) {
    return (create(static_cast<long long int>(i), pos));
}

ElementPtr
Element::create(const double d, const Position& pos) {
    return (ElementPtr(new DoubleElement(d, pos)));
}

ElementPtr
Element::create(const bool b, const Position& pos) {
    return (ElementPtr(new BoolElement(b, pos)));
}

ElementPtr
Element::create(const std::string& s, const Position& pos) {
    return (ElementPtr(new StringElement(s, pos)));
}

ElementPtr
Element::create(const char *s, const Position& pos) {
    return (create(std::string(s), pos));
}

ElementPtr
Element::createList(const Position& pos) {
    return (ElementPtr(new ListElement(pos)));
}

ElementPtr
Element::createMap(const Position& pos) {
    return (ElementPtr(new MapElement(pos)));
}

std::string
Element::typeToName(Element::types type) {
    switch (type) {
    case Element::integer:
        return (std::string("integer"));
    case Element::real:
        return (std::string("real"));
    case Element::boolean:
        return (std::string("boolean"));
    case Element::string:
        return (std::string("string"));
    case Element::list:
        return (std::string("list"));
    case Element::map:
        return (std::string("map"));
    case Element::null:
        return (std::string("null"));
    case Element::any:
        return (std::string("any"));
    default:
        return (std::string("unknown"));
    }
}

Element::types
Element::nameToType(const std::string& type_name) {
    if (type_name == "integer") {
        return (Element::integer);
    } else if (type_name == "real") {
        return (Element::real);
    } else if (type_name == "boolean") {
        return (Element::boolean);
    } else if (type_name == "string") {
        return (Element::string);
    } else if (type_name == "list") {
        return (Element::list);
    } else if (type_name == "map") {
        return (Element::map);
    } else if (type_name == "named_set") {
        return (Element::map);
    } else if (type_name == "null") {
        return (Element::null);
    } else if (type_name == "any") {
        return (Element::any);
    } else {
        isc_throw(TypeError, type_name + " is not a valid type name");
    }
}

ElementPtr
Element::fromJSON(std::istream& in, bool) {
    std::ostringstream ss;
    ss << in.rdbuf();
    return (fromJSON(ss.str()));
}

ElementPtr
Element::fromJSON(const std::string& in, bool) {
    ParserContext ctx;
    try {
        return (ctx.parseString(in));
    } catch (const JSONError& ex) {
        isc_throw(JSONError, ex.what());
    }
    return (ElementPtr());
}

ElementPtr
Element::fromJSONFile(const std::string& file_name, bool) {
    ParserContext ctx;
    try {
        return (ctx.parseFile(file_name));
    } catch (const JSONError& ex) {
        isc_throw(JSONError, ex.what());
    }
    return (ElementPtr());
}

// to JSON format

void
IntElement::toJSON(std::ostream& ss) const {
    ss << intValue();
}

void
DoubleElement::toJSON(std::ostream& ss) const {
    // The default output for doubles nicely drops off trailing
    // zeros, however this produces strings without decimal points
    // for whole number values.  When reparsed this will create
    // IntElements not DoubleElements.  Rather than used a fixed
    // precision, we'll just tack on an ".0" when the decimal point
    // is missing.
    ostringstream val_ss;
    val_ss << doubleValue();
    ss << val_ss.str();
    if (val_ss.str().find_first_of('.') == string::npos) {
        ss << ".0";
    }
}

void
BoolElement::toJSON(std::ostream& ss) const {
    if (boolValue()) {
        ss << "true";
    } else {
        ss << "false";
    }
}

void
NullElement::toJSON(std::ostream& ss) const {
    ss << "null";
}

void
StringElement::toJSON(std::ostream& ss) const {
    ss << "\"";
    const std::string& str = stringValue();
    for (size_t i = 0; i < str.size(); ++i) {
        const char c = str[i];
        // Escape characters as defined in JSON spec
        // Note that we do not escape forward slash; this
        // is allowed, but not mandatory.
        switch (c) {
        case '"':
            ss << '\\' << c;
            break;
        case '\\':
            ss << '\\' << c;
            break;
        case '\b':
            ss << '\\' << 'b';
            break;
        case '\f':
            ss << '\\' << 'f';
            break;
        case '\n':
            ss << '\\' << 'n';
            break;
        case '\r':
            ss << '\\' << 'r';
            break;
        case '\t':
            ss << '\\' << 't';
            break;
        default:
            if (((c >= 0) && (c < 0x20)) || (c < 0) || (c >= 0x7f)) {
                std::ostringstream esc;
                esc << "\\u"
                    << hex
                    << setw(4)
                    << setfill('0')
                    << (static_cast<unsigned>(c) & 0xff);
                ss << esc.str();
            } else {
                ss << c;
            }
        }
    }
    ss << "\"";
}

void
ListElement::toJSON(std::ostream& ss) const {
    ss << "[ ";

    const std::vector<ElementPtr>& v = listValue();
    for (auto it = v.begin(); it != v.end(); ++it) {
        if (it != v.begin()) {
            ss << ", ";
        }
        (*it)->toJSON(ss);
    }
    ss << " ]";
}

void
MapElement::toJSON(std::ostream& ss) const {
    ss << "{ ";

    const std::map<std::string, ConstElementPtr>& m = mapValue();
    for (auto it = m.begin(); it != m.end(); ++it) {
        if (it != m.begin()) {
            ss << ", ";
        }
        ss << "\"" << (*it).first << "\": ";
        if ((*it).second) {
            (*it).second->toJSON(ss);
        } else {
            ss << "None";
        }
    }
    ss << " }";
}

// throws when one of the types in the path (except the one
// we're looking for) is not a MapElement
// returns 0 if it could simply not be found
// should that also be an exception?
ConstElementPtr
MapElement::find(const std::string& id) const {
    const size_t sep = id.find('/');
    if (sep == std::string::npos) {
        return (get(id));
    } else {
        ConstElementPtr ce = get(id.substr(0, sep));
        if (ce) {
            // ignore trailing slash
            if  (sep + 1 != id.size()) {
                return (ce->find(id.substr(sep + 1)));
            } else {
                return (ce);
            }
        } else {
            return (ElementPtr());
        }
    }
}

void
MapElement::set(const std::string& key, ConstElementPtr value) {
    m[key] = value;
}

bool
MapElement::find(const std::string& id, ConstElementPtr& t) const {
    try {
        ConstElementPtr p = find(id);
        if (p) {
            t = p;
            return (true);
        }
    } catch (const TypeError&) {
        // ignore
    }
    return (false);
}

bool
IntElement::equals(const Element& other) const {
    return (other.getType() == Element::integer) &&
           (i == other.intValue());
}

bool
DoubleElement::equals(const Element& other) const {
    return (other.getType() == Element::real) &&
           (fabs(d - other.doubleValue()) < 1e-14);
}

bool
BoolElement::equals(const Element& other) const {
    return (other.getType() == Element::boolean) &&
           (b == other.boolValue());
}

bool
NullElement::equals(const Element& other) const {
    return (other.getType() == Element::null);
}

bool
StringElement::equals(const Element& other) const {
    return (other.getType() == Element::string) &&
           (s == other.stringValue());
}

bool
ListElement::equals(const Element& other) const {
    if (other.getType() == Element::list) {
        const size_t s = size();
        if (s != other.size()) {
            return (false);
        }
        for (size_t i = 0; i < s; ++i) {
            if (!get(i)->equals(*other.get(i))) {
                return (false);
            }
        }
        return (true);
    } else {
        return (false);
    }
}

bool
MapElement::equals(const Element& other) const {
    if (other.getType() == Element::map) {
        if (size() != other.size()) {
            return (false);
        }
        for (auto kv : mapValue()) {
            auto key = kv.first;
            if (other.contains(key)) {
                if (!get(key)->equals(*other.get(key))) {
                    return (false);
                }
            } else {
                return (false);
            }
        }
        return (true);
    } else {
        return (false);
    }
}

bool
isNull(ConstElementPtr p) {
    return (!p);
}

void
removeIdentical(ElementPtr a, ConstElementPtr b) {
    if (!b) {
        return;
    }
    if (a->getType() != Element::map || b->getType() != Element::map) {
        isc_throw(TypeError, "Non-map Elements passed to removeIdentical");
    }

    // As maps do not allow entries with multiple keys, we can either iterate
    // over a checking for identical entries in b or vice-versa.  As elements
    // are removed from a if a match is found, we choose to iterate over b to
    // avoid problems with element removal affecting the iterator.
    for (auto kv : b->mapValue()) {
        auto key = kv.first;
        if (a->contains(key)) {
            if (a->get(key)->equals(*b->get(key))) {
                a->remove(key);
            }
        }
    }
}

ConstElementPtr
removeIdentical(ConstElementPtr a, ConstElementPtr b) {
    ElementPtr result = Element::createMap();

    if (!b) {
        return (result);
    }

    if (a->getType() != Element::map || b->getType() != Element::map) {
        isc_throw(TypeError, "Non-map Elements passed to removeIdentical");
    }

    for (auto kv : a->mapValue()) {
        auto key = kv.first;
        if (!b->contains(key) ||
            !a->get(key)->equals(*b->get(key))) {
            result->set(key, kv.second);
        }
    }

    return (result);
}

void
merge(ElementPtr element, ConstElementPtr other) {
    if (element->getType() != Element::map ||
        other->getType() != Element::map) {
        isc_throw(TypeError, "merge arguments not MapElements");
    }

    for (auto kv : other->mapValue()) {
        auto key = kv.first;
        auto value = kv.second;
        if (value && value->getType() != Element::null) {
            element->set(key, value);
        } else if (element->contains(key)) {
            element->remove(key);
        }
    }
}

ElementPtr
copy(ConstElementPtr from, int level) {
    if (!from) {
        isc_throw(BadValue, "copy got a null pointer");
    }
    int from_type = from->getType();
    if (from_type == Element::integer) {
        return (ElementPtr(new IntElement(from->intValue())));
    } else if (from_type == Element::real) {
        return (ElementPtr(new DoubleElement(from->doubleValue())));
    } else if (from_type == Element::boolean) {
        return (ElementPtr(new BoolElement(from->boolValue())));
    } else if (from_type == Element::null) {
        return (ElementPtr(new NullElement()));
    } else if (from_type == Element::string) {
        return (ElementPtr(new StringElement(from->stringValue())));
    } else if (from_type == Element::list) {
        ElementPtr result = ElementPtr(new ListElement());
        for (auto elem : from->listValue()) {
            if (level == 0) {
                result->add(elem);
            } else {
                result->add(copy(elem, level - 1));
            }
        }
        return (result);
    } else if (from_type == Element::map) {
        ElementPtr result = ElementPtr(new MapElement());
        for (auto kv : from->mapValue()) {
            auto key = kv.first;
            auto value = kv.second;
            if (level == 0) {
                result->set(key, value);
            } else {
                result->set(key, copy(value, level - 1));
            }
        }
        return (result);
    } else {
        isc_throw(BadValue, "copy got an element of type: " << from_type);
    }
}

namespace {

// Helper function which blocks infinite recursion
bool
isEquivalent0(ConstElementPtr a, ConstElementPtr b, unsigned level) {
    // check looping forever on cycles
    if (!level) {
        isc_throw(BadValue, "isEquivalent got infinite recursion: "
                  "arguments include cycles");
    }
    if (!a || !b) {
        isc_throw(BadValue, "isEquivalent got a null pointer");
    }
    // check types
    if (a->getType() != b->getType()) {
        return (false);
    }
    if (a->getType() == Element::list) {
        // check empty
        if (a->empty()) {
            return (b->empty());
        }
        // check size
        if (a->size() != b->size()) {
            return (false);
        }

        // copy b into a list
        const size_t s = a->size();
        std::list<ConstElementPtr> l;
        for (size_t i = 0; i < s; ++i) {
            l.push_back(b->get(i));
        }

        // iterate on a
        for (size_t i = 0; i < s; ++i) {
            ConstElementPtr item = a->get(i);
            // lookup this item in the list
            bool found = false;
            for (auto it = l.begin(); it != l.end(); ++it) {
                // if found in the list remove it
                if (isEquivalent0(item, *it, level - 1)) {
                    found = true;
                    l.erase(it);
                    break;
                }
            }
            // if not found argument differs
            if (!found) {
                return (false);
            }
        }

        // sanity check: the list must be empty
        if (!l.empty()) {
            isc_throw(Unexpected, "isEquivalent internal error");
        }
        return (true);
    } else if (a->getType() == Element::map) {
        // check sizes
        if (a->size() != b->size()) {
            return (false);
        }
        // iterate on the first map
        for (auto kv : a->mapValue()) {
            // get the b value for the given keyword and recurse
            ConstElementPtr item = b->get(kv.first);
            if (!item || !isEquivalent0(kv.second, item, level - 1)) {
                return (false);
            }
        }
        return (true);
    } else {
        return (a->equals(*b));
    }
}

} // end anonymous namespace

bool
isEquivalent(ConstElementPtr a, ConstElementPtr b) {
    return (isEquivalent0(a, b, 100));
}

void
prettyPrint(ConstElementPtr element, std::ostream& out,
            unsigned indent, unsigned step) {
    if (!element) {
        isc_throw(BadValue, "prettyPrint got a null pointer");
    }
    if (element->getType() == Element::list) {
        // empty list case
        if (element->empty()) {
            out << "[ ]";
            return;
        }

        // complex ? multiline : oneline
        if (!element->get(0)) {
            isc_throw(BadValue, "prettyPrint got a null pointer");
        }
        int first_type = element->get(0)->getType();
        bool complex = false;
        if ((first_type == Element::list) || (first_type == Element::map)) {
            complex = true;
        }
        std::string separator = complex ? ",\n" : ", ";

        // open the list
        out << "[" << (complex ? "\n" : " ");

        // iterate on items
        const auto& l = element->listValue();
        for (auto it = l.begin(); it != l.end(); ++it) {
            // add the separator if not the first item
            if (it != l.begin()) {
                out << separator;
            }
            // add indentation
            if (complex) {
                out << std::string(indent + step, ' ');
            }
            // recursive call
            prettyPrint(*it, out, indent + step, step);
        }

        // close the list
        if (complex) {
            out << "\n" << std::string(indent, ' ');
        } else {
            out << " ";
        }
        out << "]";
    } else if (element->getType() == Element::map) {
        // empty map case
        if (element->size() == 0) {
            out << "{ }";
            return;
        }

        // open the map
        out << "{\n";

        // iterate on keyword: value
        const auto& m = element->mapValue();
        bool first = true;
        for (auto it = m.begin(); it != m.end(); ++it) {
            // add the separator if not the first item
            if (first) {
                first = false;
            } else {
                out << ",\n";
            }
            // add indentation
            out << std::string(indent + step, ' ');
            // add keyword:
            out << "\"" << it->first << "\": ";
            // recursive call
            prettyPrint(it->second, out, indent + step, step);
        }

        // close the map
        out << "\n" << std::string(indent, ' ') << "}";
    } else {
        // not a list or a map
        element->toJSON(out);
    }
}

std::string
prettyPrint(ConstElementPtr element, unsigned indent, unsigned step) {
    std::stringstream ss;
    prettyPrint(element, ss, indent, step);
    return (ss.str());
}

} // end of isc::data namespace
} // end of isc namespace
