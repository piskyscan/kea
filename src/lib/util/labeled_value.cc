// Copyright (C) 2013-2020 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <util/labeled_value.h>

namespace isc {
namespace util {

/**************************** LabeledValue ****************************/

LabeledValue::LabeledValue(const int value, const std::string& label)
    : value_(value), label_(label) {
    if (label.empty()) {
        isc_throw(LabeledValueError, "labels cannot be empty");
    }
}

LabeledValue::~LabeledValue(){
}

int
LabeledValue::getValue() const {
    return (value_);
}

std::string
LabeledValue::getLabel() const {
    return (label_);
}

bool
LabeledValue::operator==(const LabeledValue& other) const {
    return (this->value_ == other.value_);
}

bool
LabeledValue::operator!=(const LabeledValue& other) const {
    return (this->value_ != other.value_);
}

bool
LabeledValue::operator<(const LabeledValue& other) const {
    return (this->value_ < other.value_);
}

std::ostream& operator<<(std::ostream& os, const LabeledValue& vlp) {
    os << vlp.getLabel();
    return (os);
}

} // namespace isc::util
} // namespace isc
