#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include "node.hpp"

namespace sharifullina
{
  template< class Key, class Value, class Compare >
  class AVLtree;

  template< class Key, class Value, class Compare >
  class TreeIterator: public std::iterator< std::bidirectional_iterator_tag, std::pair< Key, Value > >
  {
    friend class AVLtree< Key, Value, Compare >;

  public:
    using this_t = TreeIterator< Key, Value, Compare >;

    TreeIterator();
    ~TreeIterator() = default;
    TreeIterator(const this_t&) = default;
    this_t& operator=(const this_t&) = default;

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t& operator--() noexcept;
    this_t operator--(int) noexcept;

    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const;

    bool operator==(const this_t&) const noexcept;
    bool operator!=(const this_t&) const noexcept;

  private:
    TreeNode< Key, Value >* node_;
    explicit TreeIterator(TreeNode< Key, Value >* node);
  };

  template< class Key, class Value, class Compare >
  TreeIterator< Key, Value, Compare >::TreeIterator():
    node_(nullptr)
  {}

  template< class Key, class Value, class Compare >
  TreeIterator< Key, Value, Compare >::TreeIterator(TreeNode< Key, Value >* node):
    node_(node)
  {}

  template< class Key, class Value, class Compare >
  TreeIterator< Key, Value, Compare >& TreeIterator< Key, Value, Compare >::operator++() noexcept
  {
    if (node_->right)
    {
      node_ = node_->right;
      while (node_->left)
      {
        node_ = node_->left;
      }
    }
    else
    {
      auto parent = node_->parent;
      while (parent && node_ == parent->right)
      {
        node_ = parent;
        parent = parent->parent;
      }
      node_ = parent;
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  TreeIterator< Key, Value, Compare > TreeIterator< Key, Value, Compare >::operator++(int) noexcept
  {
    TreeIterator< Key, Value, Compare > temp = *this;
    ++(*this);
    return temp;
  }

  template< class Key, class Value, class Compare >
  TreeIterator< Key, Value, Compare >& TreeIterator< Key, Value, Compare >::operator--() noexcept
  {
    if (node_->left)
    {
      node_ = node_->left;
      while (node_->right)
      {
        node_ = node_->right;
      }
    }
    else
    {
      auto parent = node_->parent;
      while (parent && node_ == parent->left)
      {
        node_ = parent;
        parent = parent->parent;
      }
      node_ = parent;
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  TreeIterator< Key, Value, Compare > TreeIterator< Key, Value, Compare >::operator--(int) noexcept
  {
    TreeIterator< Key, Value, Compare > result = *this;
    --(*this);
    return result;
  }

  template< class Key, class Value, class Compare >
  std::pair< Key, Value >& TreeIterator< Key, Value, Compare >::operator*() const
  {
    return node_->data;
  }

  template< class Key, class Value, class Compare >
  std::pair< Key, Value >* TreeIterator< Key, Value, Compare >::operator->() const
  {
    return std::addressof(node_->data);
  }

  template< class Key, class Value, class Compare >
  bool TreeIterator< Key, Value, Compare >::operator==(const this_t& other) const noexcept
  {
    return node_ == other.node_;
  }

  template< class Key, class Value, class Compare >
  bool TreeIterator< Key, Value, Compare >::operator!=(const this_t& other) const noexcept
  {
    return !(other == *this);
  }
}

#endif
