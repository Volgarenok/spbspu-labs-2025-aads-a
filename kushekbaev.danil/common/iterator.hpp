#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include "treenode.hpp"

namespace kushekbaev
{
  template< typename Key, typename Value, typename Cmp >
  struct Tree;

  template< typename Key, typename Value, typename Cmp >
  struct ConstIterator;

  template< typename Key, typename Value, typename Cmp >
  struct Iterator
  {
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::pair< const Key, Value >;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using this_t = Iterator< Key, Value, Cmp >;

    Iterator() noexcept;
    Iterator(const this_t&) = default;
    ~Iterator() = default;

    this_t& operator=(const this_t& other) = default;
    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t& operator--() noexcept;
    this_t operator--(int) noexcept;

    std::pair< Key, Value >& operator*() noexcept;
    std::pair< Key, Value >* operator->() noexcept;
    bool operator!=(const this_t& other) const noexcept;
    bool operator==(const this_t& other) const noexcept;

    private:
      friend struct Tree< Key, Value, Cmp >;
      friend struct ConstIterator< Key, Value, Cmp >;
      TreeNode< Key, Value, Cmp >* node_;
      explicit Iterator(TreeNode< Key, Value, Cmp >* node) noexcept;
  };

  template< typename Key, typename Value, typename Cmp >
  Iterator< Key, Value, Cmp >::Iterator() noexcept:
    node_(nullptr)
  {}

  template< typename Key, typename Value, typename Cmp >
  Iterator< Key, Value, Cmp >::Iterator(TreeNode< Key, Value, Cmp >* node) noexcept:
    node_(node)
  {}

  template< typename Key, typename Value, typename Cmp >
  Iterator< Key, Value, Cmp >& Iterator< Key, Value, Cmp >::operator++() noexcept
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
      while (parent != parent->parent && parent->right == node_)
      {
        node_ = parent;
        parent = parent->parent;
      }
      node_ = parent;
    }
    return *this;
  }

  template< typename Key, typename Value, typename Cmp >
  Iterator< Key, Value, Cmp > Iterator< Key, Value, Cmp >::operator++(int) noexcept
  {
    this_t tmp(*this);
    ++(*this);
    return tmp;
  }

  template< typename Key, typename Value, typename Cmp >
  Iterator< Key, Value, Cmp >& Iterator< Key, Value, Cmp >::operator--() noexcept
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
      while (node_->parent && node_->parent->left == node_)
      {
        node_ = parent;
        parent = parent->parent;
      }
      node_ = parent;
    }
    return (*this);
  }

  template< typename Key, typename Value, typename Cmp >
  Iterator< Key, Value, Cmp > Iterator< Key, Value, Cmp >::operator--(int) noexcept
  {
    this_t tmp(*this);
    --(*this);
    return tmp;
  }

  template< typename Key, typename Value, typename Cmp >
  std::pair< Key, Value >& Iterator< Key, Value, Cmp >::operator*() noexcept
  {
    return node_->data;
  }

  template< typename Key, typename Value, typename Cmp >
  std::pair< Key, Value >* Iterator< Key, Value, Cmp >::operator->() noexcept
  {
    return std::addressof(node_->data);
  }

  template< typename Key, typename Value, typename Cmp >
  bool Iterator< Key, Value, Cmp >::operator!=(const this_t& other) const noexcept
  {
    return node_ != other.node_;
  }

  template< typename Key, typename Value, typename Cmp >
  bool Iterator< Key, Value, Cmp >::operator==(const this_t& other) const noexcept
  {
    return !(*this != other);
  }
}

#endif
