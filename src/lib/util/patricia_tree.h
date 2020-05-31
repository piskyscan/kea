// Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PATRICIA_TREE_H
#define PATRICIA_TREE_H

#include <exceptions/exceptions.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <cstdint>
#include <iostream>
#include <stack>
#include <vector>

namespace isc {
namespace util {

/// @brief Patricia tree implementation based on ISC AFTR C code.
///
/// Patricia (Practical Algorithm to Retrieve Information Coded
/// in Alphanumeric) is a standard radix tree.

/// @tparam ContentType Type of the content e.g. a subnet list.
/// @tparam KeySize Size of keys in octets i.e. 4 or 16.
template <typename ContentType, size_t KeySize>
class PatriciaTree {
public:

    /// @brief Max bit count.
    static const size_t MAXBITCOUNT = 8 * KeySize;

    /// @brief Forward declaration of key.
    class Key;

    /// @brief Pointer to the @ref Key object.
    typedef boost::shared_ptr<Key> KeyPtr;

    /// @brief Type of keys.
    class Key {
    public:

        /// @brief Constructor.
        ///
        /// @param value Value i.e. IPv4 or IPv6 address.
        /// @throw BadValue when the length is not exactly KeySize.
        Key(const std::vector<uint8_t> value) : key_(value) {
            if (value.size() != KeySize) {
                isc_throw(BadValue, "wrong key size");
            }
        }

        /// @brief Destructor.
        virtual ~Key() { }

        /// @brief Read only access.
        ///
        /// @param index Index of the octet.
        /// @throw OutOfRange when index is greater than KeySize.
        inline uint8_t operator[] (size_t index) const {
            if (index >= KeySize) {
                isc_throw(OutOfRange, "out of range access to a key");
            }
            return (key_[index]);
        }

        /// @brief Compare keys for equality.
        ///
        /// @param other The key to compare with.
        /// @return When equal true, false otherwise.
        inline bool compare(const Key& other) const {
            return (memcmp(&key_[0], &other.key_[0], KeySize) == 0);
        }

        /// @brief Compare prefixes for equality.
        ///
        /// @param other The key part to compare with.
        /// @param len The prefix length.
        /// @throw OutOfRange when the length is greater than MAXBITCOUNT.
        inline bool compare(const Key& other, uint8_t len) const {
            if (len > MAXBITCOUNT) {
                isc_throw(OutOfRange, "wrong prefix length (compare)");
            }
            uint8_t n = len / 8;
            if (memcmp(&key_[0], &other.key_[0], n) == 0) {
                uint8_t r = len % 8;
                uint8_t m = (0xff << (8 - r));
                if ((r == 0) || ((key_[n] & m) == (other.key_[n] & m))) {
                    return (true);
                }
            }
            return (false);
        }

    private:

        /// @brief The key content.
        const std::vector<uint8_t> key_;
    };

    /// @brief Forward declaration of node.
    struct Node;

    /// @brief Pointer to the @ref Node object.
    typedef boost::shared_ptr<Node> NodePtr;

    /// @brief Weak pointer to the @ref Node object.
    typedef boost::weak_ptr<Node> WeakNodePtr;

    /// @brief The Patricia tree node.
    struct Node {
        /// @brief Pointer to parent node.
        NodePtr parent_;

        /// @brief Pointer to left node.
        NodePtr left_;

        /// @brief Pointer to right node.
        NodePtr right_;

        /// @brief Content.
        ContentType content_;

        /// @brief Internal node branch bit.
        uint8_t bit_;

        /// @brief Key.
        KeyPtr key_;

        /// @brief Key length.
        ///
        /// Used only with the key. key / len is the prefix.
        uint8_t len_;
    };

    /// @brief Constructor.
    PatriciaTree() { }

    /// @brief Destructor.
    virtual ~PatriciaTree() {
        std::stack<NodePtr> stack;
        NodePtr node = root_;
        while (node) {
            NodePtr left = node->left_;
            NodePtr right = node->right_;
            node->content_ = 0;
            /// free node
            --nodes_;
            if (left) {
                if (right) {
                    stack.push(right);
                }
                node = left;
            } else if (right) {
                node = right;
            } else if (!stack.empty()) {
                node = stack.top();
                stack.pop();
            } else {
                break;
            }
        }
#ifdef ENABLE_DEBUG
        if (nodes_ != 0) {
            std::cerr << "~PatriciaTree leaks " << nodes_ << std::endl;
        }
#endif
    }

    /// @brief Search the node for an exact prefix.
    ///
    /// @param key The key part of the prefix.
    /// @param len The length of the prefix.
    /// @return The node for the exact prefix or null if not found.
    /// @throw OutOfRange when the length is greater than MAXBITCOUNT.
    NodePtr findExact(const Key& key, uint8_t len) const {
        if (len > MAXBITCOUNT) {
            isc_throw(OutOfRange, "wrong prefix length (findExact)");
        }
        NodePtr node = root_;
        if (!root_) {
            return (NodePtr());
        }
        while (node->bit_ < len) {
            if ((key[node->bit_ >> 3] & (0x80 >> (node->bit_ & 7)))) {
                node = node->right_;
            } else {
                node = node->left_;
            }
            if (!node) {
                return (NodePtr());
            }
        }
        if ((node->bit_ > len) || !node->key_) {
            return (NodePtr());
        }
#ifdef ENABLE_DEBUG
        if ((node->bit_ != len) || (node->bit_ != node->len_)) {
            std::cerr << "findExact failure" << std::endl;
        }
#endif
        if (compare(key, node->key_, len)) {
            return (node);
        }
        return (NodePtr());
    }

    /// @brief Find the node for the prefix.
    ///
    /// @param key The key part of the prefix.
    /// @param len The length of the prefix.
    /// @return The node for the prefix or null if not found.
    /// @throw OutOfRange when the length is greater than MAXBITCOUNT.
    NodePtr find(const Key& key, uint8_t len) const {
        if (len > MAXBITCOUNT) {
            isc_throw(OutOfRange, "wrong prefix length (find)");
        }
        NodePtr node = root_;
        if (!root_) {
            return (NodePtr());
        }
        std::stack<NodePtr> stack;
        while (node->bit_ < len) {
            if (node->key_) {
                stack.push(node);
            }
            if ((key[node->bit_ >> 3] & (0x80 >> (node->bit_ & 7)))) {
                node = node->right_;
            } else {
                node = node->left_;
            }
            if (!node) {
                break;
            }
        }
        if (node && node->key_) {
            stack.push(node);
        }
        if (stack.empty()) {
            return (NodePtr());
        }
        while (!stack.empty()) {
            node = stack.top();
            stack.pop();
            if (node->key_->compare(key, node->len_) && (node->len_ <= len)) {
                return (node);
            }
        }
        return (NodePtr());
    }

    /// @brief Insert the node for the prefix.
    ///
    /// @param key The key part of the prefix.
    /// @param len The length of the prefix.
    /// @return The inserted node for the prefix.
    /// @throw OutOfRange when the length is greater than MAXBITCOUNT.
    NodePtr insert(const Key& key, uint8_t len) {
        if (len > MAXBITCOUNT) {
            isc_throw(OutOfRange, "wrong prefix length (insert)");
        }
        if (!root_) {
            NodePtr node(new Node());
            node->bit_ = len;
            node->key_.reset(new Key(key));
            node->len_ = len;
            root_ = node;
            ++nodes_;
            return (node);
        }
        NodePtr node = root_;
        while ((node->bit_ < len) || !node->key_) {
            if ((node->bit_ < MAXBITCOUNT) &&
                (key[node->bit_ >> 3] & (0x80 >> (node->bit_ & 7)))) {
                if (!node->right_) {
                    break;
                }
                node = node->right_;
            } else {
                if (!node->left_) {
                    break;
                }
                node = node->left_;
            }
#ifdef ENABLE_DEBUG
            if (!node) {
                std::cerr << "insert failure 1" << std::endl;
            }
#endif
        }
#ifdef ENABLE_DEBUG
        if (!node->key_) {
            std::cerr << "insert failure 2" << std::endl;
        }
#endif
        // find the first bit different.
        uint8_t check = (node->bit_ < len ? node->bit_ : len);
        uint8_t differ = 0;
        for (uint8_t i = 0; i * 8 < check; ++i) {
          uint8_t r = key[i] ^ (*node->key_)[i];
            if (r == 0) {
                differ = (i + 1) * 8;
                continue;
            }
            uint8_t j = 0;
            for (; j < 8; ++j) {
                if (r & (0x80 >> j)) {
                    break;
                }
            }
            // must be found.
#ifdef ENABLE_DEBUG
            if (j >= 8) {
                std::cerr << "insert failure 3" << std::endl;
            }
#endif
            differ = i * 8 + j;
            break;
        }
        if (differ > check) {
            differ = check;
        }

        NodePtr parent = node->parent_;
        while (parent && (parent->bit_ >= differ)) {
            node = parent;
            parent = node->parent_;
        }

        if ((differ == len) && (node->bit_ == len)) {
            if (node->key_) {
                return (node);
            }
            node->key_.reset(new Key(key));
            node->len_ = len;
#ifdef ENABLE_DEBUG
            if (node->content_) {
                std::cerr << "insert failure 4" << std::endl;
            }
#endif
            return (node);
        }

        NodePtr new_node(new Node());
        new_node->bit_ = len;
        new_node->key_.reset(new Key(key));
        new_node->len_ = len;
        ++nodes_;

        if (node->bit_ == differ) {
            new_node->parent_ = node;
            if ((node->bit_ < MAXBITCOUNT) &&
                (key[node->bit_ >> 3] & (0x80 >> (node->bit_ & 7)))) {
#ifdef ENABLE_DEBUG
                if (node->right_) {
                    std::cerr << "insert failure 5" << std::endl;
                }
#endif
                node->right_ = new_node;
            } else {
#ifdef ENABLE_DEBUG
                if (node->left_) {
                    std::cerr << "insert failure 6" << std::endl;
                }
#endif
                node->left_ = new_node;
            }
            return (new_node);
        }

        if (len == differ) {
            if ((node->bit_ < MAXBITCOUNT) &&
                (key[node->bit_ >> 3] & (0x80 >> (node->bit_ & 7)))) {
                new_node->right_ = node;
            } else {
                new_node->left_ = node;
            }
            new_node->parent_ = node->parent_;
            if (!node->parent_) {
#ifdef ENABLE_DEBUG
                if (root_ != node) {
                    std::cerr << "insert failure 7" << std::endl;
                }
#endif
                root_ = new_node;
            } if (node->parent_->right_ == node) {
                node->parent_->right_ = new_node;
            } else {
                node->parent_->left_ = new_node;
            }
            node->parent_ = new_node;
        } else {
            NodePtr glue(new Node());
            glue->bit_ = differ;
            glue->parent_ = node->parent_;
            ++nodes_;
            if ((differ < MAXBITCOUNT) &&
                (key[node->bit_ >> 3] & (0x80 >> (node->bit_ & 7)))) {
                glue->right_ = new_node;
                glue->left_ = node;
            } else {
                glue->right_ = node;
                glue->left_ = new_node;
            }
            new_node->parent_ = glue;

            if (!node->parent_) {
#ifdef ENABLE_DEBUG
                if (root_ != node) {
                    std::cerr << "insert failure 8" << std::endl;
                }
#endif
                root_ = glue;
            } if (node->parent_->right_ == node) {
                node->parent_->right_ = glue;
            } else {
                node->parent_->left_ = glue;
            }
            node->parent_ = glue;
        }
        return (new_node);
    }

    /// @brief Remove a node.
    ///
    /// @param node The node to remove.
    /// @throw BadValue if the node is null.
    void remove(NodePtr& node) {
        if (!node) {
            isc_throw(BadValue, "null node (remove)");
        }
        if (node->right_ && node->left_) {

            // this might be a glue node.
            if (node->key_) {
                node->key_.reset();
            }
            node->content_ = 0;
            return;
        }

        NodePtr parent;
        NodePtr child;
        if (!node->right_ && !node->left_) {
            parent = node->parent_;
            node->key_.reset();
            /// free
            --nodes_;

            if (!parent) {
#ifdef ENABLE_DEBUG
                if (root_ != node) {
                    std::cerr << "remove failure 1" << std::endl;
                }
#endif
                root_ = NodePtr();
                return;
            }

            if (parent->right_ == node) {
                parent->right_ = NodePtr();
                child = parent->left_;
            } else {
#ifdef ENABLE_DEBUG
                if (parent->left_ == node) {
                    std::cerr << "remove failure 2" << std::endl;
                }
#endif
                parent->left_ = NodePtr();
                child = parent->right_;
            }

            if (parent->key_) {
                return;
            }

            // we need to remove parent too.
            if (!parent->parent_) {
#ifdef ENABLE_DEBUG
                if (root_ != parent) {
                    std::cerr << "remove failure 3" << std::endl;
                }
#endif
                root_ = child;
            } else if (parent->parent_->right_ == parent) {
                parent->parent_->right_ = child;
            } else {
#ifdef ENABLE_DEBUG
                if (parent->parent_->left_ != parent) {
                    std::cerr << "remove failure 4" << std::endl;
                }
#endif
                parent->parent_->left_ = child;
            }
            child->parent_ = parent->parent_;
            // free parent
            --nodes_;
            return;
        }

        if (node->right_) {
            child = node->right_;
        } else {
#ifdef ENABLE_DEBUG
            if (!node->left_) {
                std::cerr << "remove failure 5" << std::endl;
            }
#endif
            child = node->left_;
        }
        parent = node->parent_;
        child->parent_ = parent;

        node->key_.reset();
        /// free node
        --nodes_;

        if (!parent) {
#ifdef ENABLE_DEBUG
            if (root_ != node) {
                std::cerr << "remove failure 6" << std::endl;
            }
#endif
            root_ = child;
            return;
        }

        if (parent->right_ == node) {
            parent->right_ = child;
        } else {
#ifdef ENABLE_DEBUG
            if (parent->left_ != node) {
                std::cerr << "remove failure 7" << std::endl;
            }
#endif
            parent->left_ = child;
        }
    }

protected:

    /// @brief Root node.
    NodePtr root_;

    /// @brief Instance counter.
    size_t nodes_;
};

} // end of namespace isc::util
} // end of namespace isc

#endif // PATRICIA_TREE_H
