#ifndef CONST_ITERATOR_HPP
#define CONST_ITERATOR_HPP
#include <memory>
#include <iterator>
#include "node.hpp"

namespace smirnov
{
  template < typename Key, typename Value, typename Compare >
  class AvlTree;
  template < typename Key, typename Value, typename Compare = std::less< Key > >
  class ConstIterator: public std::iterator< std::bidirectional_iterator_tag, const std::pair< Key, Value > >
  {
    friend class AvlTree< Key, Value, Compare >;
  public:
    ConstIterator();
    ~ConstIterator() = default;
    const std::pair< const Key, Value > & operator*() const;
    const std::pair< const Key, Value > * operator->() const;
    ConstIterator & operator++();
    ConstIterator operator++(int);
    ConstIterator & operator--();
    ConstIterator operator--(int);
    bool operator==(const ConstIterator & rhs) const;
    bool operator!=(const ConstIterator & rhs) const;
  private:
    explicit ConstIterator(Node< Key, Value > * node);
    Node< Key, Value > * node_;
  };

  template < typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare >::ConstIterator():
    node_(nullptr)
  {}

  template < typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare >::ConstIterator(Node< Key, Value > * node):
    node_(node)
  {}

  template < typename Key, typename Value, typename Compare >
  const std::pair< const Key, Value > & ConstIterator< Key, Value, Compare >::operator*() const
  {
    return node_->data;
  }

  template < typename Key, typename Value, typename Compare >
  const std::pair< const Key, Value > * ConstIterator< Key, Value, Compare >::operator->() const
  {
    return std::addressof(node_->data);
  }

  template < typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare > & ConstIterator< Key, Value, Compare >::operator++()
  {
    if (!node_)
    {
      return *this;
    }
    if (node_->right)
    {
      node_ = node_->right;
      while (node_->left)
      {
        node_ = node_->left;
      }
      return *this;
    }
    Node< Key, Value > * parent = node_->parent;
    while (parent && node_ == parent->right)
    {
      node_ = parent;
      parent = parent->parent;
    }
    node_ = parent;
    return *this;
  }

  template < typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare > ConstIterator< Key, Value, Compare >::operator++(int)
  {
    ConstIterator tmp(*this);
    ++(*this);
    return tmp;
  }

  template < typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare > & ConstIterator< Key, Value, Compare >::operator--()
  {
    if (!node_)
    {
      return *this;
    }
    if (node_->left)
    {
      node_ = node_->left;
      while (node_->right)
      {
        node_ = node_->right;
      }
      return *this;
    }
    Node< Key, Value > * parent = node_->parent;
    while (parent && node_ == parent->left)
    {
      node_ = parent;
      parent = parent->parent;
    }
    node_ = parent;
    return *this;
  }

  template < typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare > ConstIterator< Key, Value, Compare >::operator--(int)
  {
    ConstIterator tmp(*this);
    --(*this);
    return tmp;
  }

  template < typename Key, typename Value, typename Compare >
  bool ConstIterator< Key, Value, Compare >::operator==(const ConstIterator & rhs) const
  {
    return node_ == rhs.node_;
  }

  template < typename Key, typename Value, typename Compare >
  bool ConstIterator< Key, Value, Compare >::operator!=(const ConstIterator & rhs) const
  {
    return !(*this == rhs);
  }
}
#endif
