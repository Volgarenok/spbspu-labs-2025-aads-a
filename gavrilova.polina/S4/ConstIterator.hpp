#ifndef CONST_ITERATOR_TTT_HPP
#define CONST_ITERATOR_TTT_HPP

#include <cassert>
#include <iterator>
#include "TwoThreeTree.hpp"

namespace gavrilova {

  template < class Key, class Value, class Cmp >
  struct ConstIterator: public std::iterator< std::bidirectional_iterator_tag, const std::pair< Key, Value > > {
    using this_t = ConstIterator< Key, Value, Cmp >;
    using Node = NodeTwoThreeTree< Key, Value >;
    using value_type = const std::pair< Key, Value >;
    using Iterator = IteratorTTT< Key, Value, Cmp >;

    ConstIterator();
    ConstIterator(const this_t&) = default;
    ConstIterator(const Iterator& other) noexcept;
    ~ConstIterator() = default;
    this_t& operator=(const this_t&) = default;

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t& operator--() noexcept;
    this_t operator--(int) noexcept;

    const value_type& operator*() const;
    const value_type* operator->() const;

    bool operator!=(const this_t&) const;
    bool operator==(const this_t&) const;

  private:
    const Node* node_;
    int key_pos_;
    const Node* fake_;

    friend class TwoThreeTree< Key, Value, Cmp >;
    friend class IteratorTTT< Key, Value, Cmp >;
    explicit ConstIterator(const Node* node, int key_pos, const Node* fake);

    const Node* go_min(const Node* start) const;
    const Node* go_max(const Node* start) const;
  };

  template < class Key, class Value, class Cmp >
  ConstIterator< Key, Value, Cmp >::ConstIterator():
    node_(nullptr),
    key_pos_(0),
    fake_(nullptr)
  {}

  template < class Key, class Value, class Cmp >
  ConstIterator< Key, Value, Cmp >::ConstIterator(const Node* node, int key_pos, const Node* fake):
    node_(node),
    key_pos_(key_pos),
    fake_(fake)
  {}

  template < class Key, class Value, class Cmp >
  ConstIterator< Key, Value, Cmp >::ConstIterator(const Iterator& other) noexcept:
    node_(other.node_),
    key_pos_(other.key_pos_),
    fake_(other.fake_)
  {}

  template < class Key, class Value, class Cmp >
  typename ConstIterator< Key, Value, Cmp >::this_t&
  ConstIterator< Key, Value, Cmp >::operator++() noexcept
  {
    if (node_->is_fake) {
      key_pos_ = 0;
      return *this;
    }

    if (node_->is_leaf()) {
      if (node_->is_3_node && key_pos_ == 0) {
        key_pos_ = 1;
      } else {
        const Node* parent = node_->parent;
        const Node* child = node_;
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
  typename ConstIterator< Key, Value, Cmp >::this_t
  ConstIterator< Key, Value, Cmp >::operator++(int) noexcept
  {
    this_t temp(*this);
    ++(*this);
    return temp;
  }

  template < class Key, class Value, class Cmp >
  typename ConstIterator< Key, Value, Cmp >::this_t&
  ConstIterator< Key, Value, Cmp >::operator--() noexcept
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
        const Node* parent = node_->parent;
        const Node* child = node_;
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
  typename ConstIterator< Key, Value, Cmp >::this_t
  ConstIterator< Key, Value, Cmp >::operator--(int) noexcept
  {
    this_t temp(*this);
    --(*this);
    return temp;
  }

  template < class Key, class Value, class Cmp >
  const typename ConstIterator< Key, Value, Cmp >::value_type&
  ConstIterator< Key, Value, Cmp >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->data[key_pos_];
  }

  template < class Key, class Value, class Cmp >
  const typename ConstIterator< Key, Value, Cmp >::value_type*
  ConstIterator< Key, Value, Cmp >::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data[key_pos_]);
  }

  template < class Key, class Value, class Cmp >
  bool ConstIterator< Key, Value, Cmp >::operator==(const this_t& rhs) const
  {
    return node_ == rhs.node_ && key_pos_ == rhs.key_pos_;
  }

  template < class Key, class Value, class Cmp >
  bool ConstIterator< Key, Value, Cmp >::operator!=(const this_t& rhs) const
  {
    return !(*this == rhs);
  }

  template < class Key, class Value, class Cmp >
  const typename ConstIterator< Key, Value, Cmp >::Node*
  ConstIterator< Key, Value, Cmp >::go_min(const Node* start) const
  {
    const Node* cur = start;
    while (!cur->is_leaf()) {
      cur = cur->children[0];
    }
    return cur;
  }

  template < class Key, class Value, class Cmp >
  const typename ConstIterator< Key, Value, Cmp >::Node*
  ConstIterator< Key, Value, Cmp >::go_max(const Node* start) const
  {
    const Node* cur = start;
    while (!cur->is_leaf()) {
      cur = cur->children[cur->is_3_node ? 2 : 1];
    }
    return cur;
  }

}

#endif
