// Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <util/patricia_tree.h>
#include <gtest/gtest.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <sstream>

using namespace isc;
using namespace isc::util;
using namespace std;

namespace {

/// @brief Test Fixtire for testing patricia trees.
class PatriciaTreeTest : public ::testing::Test {
public:
    /// @brief Dummy struct for template argument.
    struct A { };

    /// @brief Pointer to nodes.
    typedef typename Node<A>::NodePtr NodePtr;

    /// @brief Constructor.
    ///
    /// Build a patricia tree for each family.
    PatriciaTreeTest()
        : tree4_(PatriciaTree<Node<A> >(4)),
          tree6_(PatriciaTree<Node<A> >(16)) {
    }

    /// @brief Text to binary address.
    ///
    /// @param family Address family i.e. AF_INET or AF_INET6.
    /// @param addr Address in text form.
    /// @return Vector with the binary address.
    static vector<uint8_t> pton(int family, const string& addr) {
        vector<uint8_t> bin;
        socklen_t len;
        switch (family) {
        case AF_INET:
            len = 4;
            break;
        case AF_INET6:
            len = 16;
            break;
        default:
            isc_throw(BadValue, "unknown family " << family);
        }
        bin.resize(len);
        if (inet_pton(family, addr.c_str(), &bin[0]) != 1) {
            isc_throw(BadValue, "bad address " << addr);
        }
        return (bin);
    }

    /// @brief Binary address to text.
    ///
    /// @param family Address family i.e. AF_INET or AF_INET6.
    /// @param addr Address in binary form.
    /// @return String with the text address.
    static string ntop(int family, const vector<uint8_t>& addr) {
        char buf[INET6_ADDRSTRLEN];
        socklen_t len;
        switch (family) {
        case AF_INET:
            len = 4;
            if (addr.size() != len) {
                isc_throw(BadValue, "bad IPv4 address size " << addr.size());
            }
            break;
        case AF_INET6:
            len = 16;
            if (addr.size() != len) {
                isc_throw(BadValue, "bad IPv6 address size " << addr.size());
            }
            break;
        default:
            isc_throw(BadValue, "unknown family " << family);
        }
        return (string(inet_ntop(family, &addr[0], buf, len)));
    }

    /// @brief Node prefix to text.
    ///
    /// @param family Address family i.e. AF_INET or AF_INET6.
    /// @param node Node with the prefix to display.
    static string ntop(int family, const NodePtr& node) {
        if (!node || node->addr_.empty()) {
            return ("");
        }
        ostringstream oss;
        oss << ntop(family, node->addr_)
            << "/" << static_cast<unsigned>(node->len_);
        return (oss.str());
    }

    /// @brief Search and remove.
    ///
    /// @param family Address family i.e. AF_INET or AF_INET6.
    /// @param addr Prefix address part in text form.
    /// @param len Prefix length.
    /// @return When the node was found and removed true, false otherwise.
    bool searchRemove(int family, const string& addr, uint8_t len) {
        auto pref = pton(family, addr);
        if (family == AF_INET) {
            auto node = tree4_.findExact(pton(AF_INET, addr), len);
            if (node) {
                tree4_.remove(node);
                return (true);
            }
        } else {
            auto node = tree6_.findExact(pton(AF_INET6, addr), len);
            if (node) {
                tree6_.remove(node);
                return (true);
            }
        }
        return (false);
    }

    /// @brief IPv4 patricia tree.
    PatriciaTree<Node<A> > tree4_;

    /// @brief IPv6 patricia tree.
    PatriciaTree<Node<A> > tree6_;
};

TEST(PatriciaTest, empty4) {
    class X { };
    PatriciaTree<Node<X> > pt4(4);
}

TEST(PatriciaTest, empty6) {
    class X { };
    PatriciaTree<Node<X> > pt6(16);
}

TEST_F(PatriciaTreeTest, basic4) {
    auto pref = pton(AF_INET, "127.0.0.0");
    auto node = tree4_.insert(pref, 8);
    ASSERT_TRUE(node);
    EXPECT_EQ(pref, node->addr_);
    EXPECT_EQ(8, node->len_);
    auto addr = pton(AF_INET, "127.0.0.1");
    EXPECT_TRUE(tree4_.find(addr, 32));
    addr = pton(AF_INET, "10.0.0.1");
    EXPECT_FALSE(tree4_.find(addr, 32));
}

TEST_F(PatriciaTreeTest, basic6) {
    auto addr = pton(AF_INET6, "2001:db8:0:1::");
    auto node = tree6_.insert(addr, 64);
    EXPECT_TRUE(node);
}

TEST_F(PatriciaTreeTest, demo) {
    EXPECT_TRUE(tree4_.insert(pton(AF_INET, "127.0.0.0"), 8));

    EXPECT_TRUE(tree4_.find(pton(AF_INET, "127.0.0.1"), 32));
    EXPECT_FALSE(tree4_.find(pton(AF_INET, "10.0.0.1"), 32));

    EXPECT_TRUE(tree4_.insert(pton(AF_INET, "10.42.42.0"), 24));
    EXPECT_TRUE(tree4_.insert(pton(AF_INET, "10.42.69.0"), 24));
    EXPECT_TRUE(tree4_.insert(pton(AF_INET, "10.0.0.0"), 8));
    EXPECT_TRUE(tree4_.insert(pton(AF_INET, "10.0.0.0"), 9));

    EXPECT_TRUE(tree4_.find(pton(AF_INET, "10.42.42.0"), 24));
    EXPECT_TRUE(tree4_.find(pton(AF_INET, "10.10.10.10"), 32));
    EXPECT_TRUE(tree4_.find(pton(AF_INET, "10.10.10.1"), 32));
    EXPECT_FALSE(tree4_.findExact(pton(AF_INET, "10.0.0.0"), 32));
    EXPECT_TRUE(tree4_.findExact(pton(AF_INET, "10.0.0.0"), 8));

    size_t cnt = tree4_.walkInOrder(NodePtr(), [] (const NodePtr&) { });
    EXPECT_LE(cnt, tree4_.getNodeCount());

    EXPECT_FALSE(searchRemove(AF_INET, "42.0.0.0", 8));
    EXPECT_TRUE(searchRemove(AF_INET, "10.0.0.0", 8));
    EXPECT_FALSE(tree4_.findExact(pton(AF_INET, "10.0.0.0"), 32));

    cnt = tree4_.walkInOrder(NodePtr(), [] (const NodePtr&) { });
    EXPECT_LE(cnt, tree4_.getNodeCount());

    tree4_.clear();
}

} // end of anonymous space.

/*
add 127.0.0.0/8
search 127.0.0.1 -> 127.0.0.0/8
search 10.0.0.1 -> null
search 0.0.0.42 -> null
add 10.0.0.0/8
add 10.42.42.0/31 10.42.42.0/26 10.42.42.0/24 10.42.42.0/32 10.42.69.0/24
search 10.42.42.0/24 10.42.69.0/24 -> them
search 10.10.10.10 -> 10.0.0.0/8
search 10.0.0.1 -> 10.0.0.0/8
search exact 10.0.0.0/8
remove 42.0.0.0/8 -> not in
remove 10.0.0.0/8 -> removed
search exact 10.0.0.0/8 -> null
clim inorder
clim
add 0.0.0.0/0
search 10.0.0.1 -> 0.0.0.0/0
add cidr 211.200.0.0-211.205.255.255 (211.200.0.0/14 & 211.204.0.0/15)
search 211.202.0.1 -> 211.200.0.0/14
remove cidr 211.200.0.0-211.205.255.255
add 2001:220::/35
search 2001:220:: -> 2001:220::/35
*/
