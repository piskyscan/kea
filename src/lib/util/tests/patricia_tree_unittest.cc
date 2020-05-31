// Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <util/patricia_tree.h>

#include <gtest/gtest.h>

using namespace isc::util;
using namespace std;

namespace {

struct A;
typedef boost::shared_ptr<A> APtr;

TEST(PatriciaTreeTest, basic4) {
    PatriciaTree<APtr, 4> pt4;
}

TEST(PatriciaTreeTest, basic6) {
    PatriciaTree<APtr, 16> pt6;
}

TEST(PatriciaTreeTest, key4) {
    vector<uint8_t> addr = { 127, 0, 0, 0 };
    PatriciaTree<APtr, 4>::Key key(addr);
}

TEST(PatriciaTreeTest, key6) {
    vector<uint8_t> addr = { 0x20, 1, 0xd, 0xb8, 0, 0, 0, 1,
                             0, 0, 0, 0, 0, 0, 0, 0 };
    EXPECT_EQ(16, addr.size());
    PatriciaTree<APtr, 16>::Key key(addr);
}

TEST(PatriciaTreeTest, insert4) {
    vector<uint8_t> addr = { 127, 0, 0, 0 };
    typedef PatriciaTree<APtr, 4> PT4;
    PT4::Key key(addr);
    PT4 pt4;
    EXPECT_NO_THROW(pt4.insert(key, 8));
}

TEST(PatriciaTreeTest, insert6) {
    vector<uint8_t> addr = { 0x20, 1, 0xd, 0xb8, 0, 0, 0, 1,
                             0, 0, 0, 0, 0, 0, 0, 0 };
    typedef PatriciaTree<APtr, 16> PT6;
    PT6::Key key(addr);
    PT6 pt6;
    EXPECT_NO_THROW(pt6.insert(key, 64));
}

TEST(PatriciaTreeTest, find4) {
    vector<uint8_t> pref = { 127, 0, 0, 0 };
    typedef PatriciaTree<APtr, 4> PT4;
    PT4::Key key(pref);
    PT4 pt4;
    EXPECT_NO_THROW(pt4.insert(key, 8));
    vector<uint8_t> addr = { 127, 0, 0, 1 };
    auto n4 = pt4.find(PT4::Key(addr), 32);
    ASSERT_TRUE(n4);
    ASSERT_TRUE(n4->key_);
    EXPECT_TRUE(n4->key_->compare(key));
    EXPECT_EQ(8, n4->len_);
}

}
