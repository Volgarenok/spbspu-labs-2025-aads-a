#ifndef ITERATOR_TTT_HPP
#define ITERATOR_TTT_HPP

#include <cassert>
#include <iterator>
#include "TwoThreeTree.hpp"

namespace gavrilova {

  template < class Key, class Value, class Cmp >
  struct IteratorTTT: public std::iterator< std::bidirectional_iterator_tag, std::pair< Key, Value > > {
    using this_t = IteratorTTT< Key, Value, Cmp >;
    using Node = NodeTwoThreeTree< Key, Value >;
    using value_type = std::pair< Key, Value >;

    IteratorTTT();
    ~IteratorTTT() = default;
    IteratorTTT(const this_t&) = default;
    this_t& operator=(const this_t&) = default;

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t& operator--() noexcept;
    this_t operator--(int) noexcept;

    value_type& operator*() const;
    value_type* operator->() const;

    bool operator!=(const this_t&) const;
    bool operator==(const this_t&) const;

  private:
    Node* node_;
    int key_pos_;
    Node* fake_;

    friend class TwoThreeTree< Key, Value, Cmp >;
    friend class ConstIterator< Key, Value, Cmp >;
    explicit IteratorTTT(Node* node, int key_pos, Node* fake);

    Node* go_min(Node* start) const;
    Node* go_max(Node* start) const;
  };

  template < class Key, class Value, class Cmp >
  IteratorTTT< Key, Value, Cmp >::IteratorTTT():
    node_(nullptr),
    key_pos_(0),
    fake_(nullptr)
  {}

  template < class Key, class Value, class Cmp >
  IteratorTTT< Key, Value, Cmp >::IteratorTTT(Node* node, int key_pos, Node* fake):
    node_(node),
    key_pos_(key_pos),
    fake_(fake)
  {}

  template < class Key, class Value, class Cmp >
  typename IteratorTTT< Key, Value, Cmp >::this_t& IteratorTTT< Key, Value, Cmp >::operator++() noexcept
  {
    if (node_->is_fake) {
      key_pos_ = 0;
      return *this;
    }

    if (node_->is_leaf()) {
      if (node_->is_3_node && key_pos_ == 0) {
        key_pos_ = 1;
      } else {
        Node* parent = node_->parent;
        Node* child = node_;
        while (parent && !parent->is_fake && parent->children[0] != child) {
          child = parent;
          parent = parent->parent;
        }
        if (!parent || parent->is_fake) {
          node_ = fake_;
          key_pos_ = 0;
        } else {
          node_ = parent;
          key_pos_ = (parent->children[0] == child) ? 0 : 1;
        }
      }
    } else {
      if (key_pos_ == 0) {
        node_ = go_min(node_->children[1]);
      } else {
        node_ = go_min(node_->children[2]);
      }
      key_pos_ = 0;
    }
    return *this;
  }

  template < class Key, class Value, class Cmp >
  typename IteratorTTT< Key, Value, Cmp >::this_t IteratorTTT< Key, Value, Cmp >::operator++(int) noexcept
  {
    this_t temp(*this);
    ++(*this);
    return temp;
  }

  template < class Key, class Value, class Cmp >
  typename IteratorTTT< Key, Value, Cmp >::this_t& IteratorTTT< Key, Value, Cmp >::operator--() noexcept
  {
    assert(node_);
    if (node_->is_fake) {
      node_ = go_max(fake_->children[0]);
      assert(node_);
      key_pos_ = node_->is_3_node ? 1 : 0;
      return *this;
    }

    if (node_->is_leaf()) {
      if (node_->is_3_node && key_pos_ == 1) {
        key_pos_ = 0;
      } else {
        Node* parent = node_->parent;
        Node* child = node_;
        while (parent && !parent->is_fake && parent->children[0] == child) {
          child = parent;
          parent = parent->parent;
        }
        if (!parent || parent->is_fake) {
          node_ = fake_;
          key_pos_ = 0;
        } else {
          node_ = parent;
          key_pos_ = (parent->children[2] == child) ? 1 : 0;
        }
      }
    } else {
      if (key_pos_ == 0) {
        node_ = go_max(node_->children[0]);
      } else {
        node_ = go_max(node_->children[1]);
      }
      key_pos_ = node_->is_3_node ? 1 : 0;
    }
    return *this;
  }

  template < class Key, class Value, class Cmp >
  typename IteratorTTT< Key, Value, Cmp >::this_t IteratorTTT< Key, Value, Cmp >::operator--(int) noexcept
  {
    this_t temp(*this);
    --(*this);
    return temp;
  }

  template < class Key, class Value, class Cmp >
  typename IteratorTTT< Key, Value, Cmp >::value_type& IteratorTTT< Key, Value, Cmp >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->data[key_pos_];
  }

  template < class Key, class Value, class Cmp >
  typename IteratorTTT< Key, Value, Cmp >::value_type* IteratorTTT< Key, Value, Cmp >::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data[key_pos_]);
  }

  template < class Key, class Value, class Cmp >
  bool IteratorTTT< Key, Value, Cmp >::operator==(const this_t& rhs) const
  {
    return node_ == rhs.node_ && key_pos_ == rhs.key_pos_;
  }

  template < class Key, class Value, class Cmp >
  bool IteratorTTT< Key, Value, Cmp >::operator!=(const this_t& rhs) const
  {
    return !(*this == rhs);
  }

  template < class Key, class Value, class Cmp >
  typename IteratorTTT< Key, Value, Cmp >::Node* IteratorTTT< Key, Value, Cmp >::go_min(Node* start) const
  {
    Node* cur = start;
    while (!cur->is_leaf()) {
      cur = cur->children[0];
    }
    return cur;
  }

  template < class Key, class Value, class Cmp >
  typename IteratorTTT< Key, Value, Cmp >::Node* IteratorTTT< Key, Value, Cmp >::go_max(Node* start) const
  {
    Node* cur = start;
    while (!cur->is_leaf()) {
      cur = cur->children[cur->is_3_node ? 2 : 1];
    }
    return cur;
  }

}

#endif
