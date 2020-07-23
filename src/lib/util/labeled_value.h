// Copyright (C) 2013-2020 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef LABELED_VALUE_H
#define LABELED_VALUE_H

#include <exceptions/exceptions.h>

#include <boost/shared_ptr.hpp>
#include <ostream>
#include <string>
#include <map>

/// @file labeled_value.h This file defines classes: LabeledValue and
/// LabeledValueSet.

namespace isc {
namespace util {

/// @brief Thrown if an error is encountered handling a LabeledValue.
class LabeledValueError : public isc::Exception {
public:
    LabeledValueError(const char* file, size_t line, const char* what) :
        isc::Exception(file, line, what) { };
};

/// @brief Implements the concept of a constant value with a text label.
///
/// This class implements an association between a constant integer value
/// and a text label. It provides a single constructor, accessors for both
/// the value and label, and boolean operators which treat the value as
/// the "key" for comparisons.  This allows them to be assembled into
/// dictionaries of unique values.  Note, that the labels are not required to
/// be unique but in practice it makes little sense to for them to be
/// otherwise.
class LabeledValue {
public:

    /// @brief Constructor
    ///
    /// @param value the numeric constant value to be labeled.
    /// @param label the text label to associate to this value.
    ///
    /// @throw LabeledValueError if label is empty.
    LabeledValue(const int value, const std::string& label);

    /// @brief Destructor.
    ///
    /// Destructor is virtual to permit derivations.
    virtual ~LabeledValue();

    /// @brief Gets the integer value of this instance.
    ///
    /// @return integer value of this instance.
    int getValue() const;

    /// @brief Gets the text label of this instance.
    ///
    /// @return The text label as string
    std::string getLabel() const;

    /// @brief  Equality operator
    ///
    /// @return True if a.value_ is equal to b.value_.
    bool operator==(const LabeledValue& other) const;

    /// @brief  Inequality operator
    ///
    /// @return True if a.value_ is not equal to b.value_.
    bool operator!=(const LabeledValue& other) const;

    /// @brief  Less-than operator
    ///
    /// @return True if a.value_ is less than b.value_.
    bool operator<(const LabeledValue& other) const;

private:
    /// @brief The numeric value to label.
    int value_;

    /// @brief The text label for the value.
    std::string label_;
};

/// @brief Dumps the label to ostream.
std::ostream& operator<<(std::ostream& os, const LabeledValue& vlp);

/// @brief Defines a shared pointer to a LabeledValue instance.
typedef boost::shared_ptr<LabeledValue> LabeledValuePtr;

/// @brief Implements a set of unique LabeledValues derived objects.
///
/// This class is intended to function as a dictionary of numeric
/// values and the labels associated with them.  It is essentially a
/// thin wrapper around a map of LabeledValue derived objects, keyed
/// by their values.  This is handy for defining a set of "valid"
/// constants while conveniently associating a text label with each
/// value.
///
/// This class offers an add method for adding entries to the set, and
/// accessors for finding an entry or an entry's label by value.
/// Note that the add methods may throw but all accessors are exception safe.
/// It is up to the caller to determine when and if an undefined value is
/// exception-worthy.
///
/// @tparam SubClassPtr Type of pointers to LabeledValue derived objects.
template <typename SubClassPtr>
class LabeledValueSet {
public:
    /// @brief Defines a text label returned by when value is not found.
    static const char* UNDEFINED_LABEL;

    /// @brief Destructor
    ///
    /// Destructor is virtual to permit derivations.
    virtual ~LabeledValueSet() { }

    /// @brief Adds the given entry to the set
    ///
    /// @param entry is the entry to add.
    ///
    /// @throw LabeledValueError if the entry is null or the set already
    /// contains an entry with the same value.
    void add(SubClassPtr entry) {
        if (!entry) {
            isc_throw(LabeledValueError, "cannot add an null entry to set");
        }

        const int value = entry->getValue();
        if (isDefined(value)) {
            isc_throw(LabeledValueError,
                      "value: " << value << " is already defined as: "
                      << getLabel(value));
        }

        map_[value] = entry;
    }

    /// @brief Fetches a pointer to the entry associated with value
    ///
    /// @param value is the value of the entry desired.
    ///
    /// @return A pointer to the entry if the entry was found otherwise the
    /// pointer is empty.
    const SubClassPtr& get(int value) {
        static SubClassPtr undefined;
        auto it = map_.find(value);
        if (it != map_.end()) {
            return ((*it).second);
        }

        // Return an empty pointer when not found.
        return (undefined);
    }

    /// @brief Tests if the set contains an entry for the given value.
    ///
    /// @param value is the value of the entry to test.
    ///
    /// @return True if an entry for value exists in the set, false if not.
    bool isDefined(const int value) const {
        auto const it = map_.find(value);
        return (it != map_.end());
    }

    /// @brief Fetches the label for the given value
    ///
    /// @param value is the value for which the label is desired.
    ///
    /// @return the label of the value if defined, otherwise it returns
    /// UNDEFINED_LABEL.
    std::string getLabel(const int value) const {
        auto const it = map_.find(value);
        if (it != map_.end()) {
            const SubClassPtr& ptr = (*it).second;
            return (ptr->getLabel());
        }

        return (std::string(UNDEFINED_LABEL));
    }

private:
    /// @brief The map of labeled values.
    std::map<unsigned int, SubClassPtr> map_;
};

template <typename SubClassPtr>
const char* LabeledValueSet<SubClassPtr>::UNDEFINED_LABEL = "UNDEFINED";

} // namespace isc::util
} // namespace isc
#endif
