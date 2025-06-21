#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
#include "treeNode.hpp"

namespace finaev
{
  template< class Key, class Value, class Cmp >
  class AVLtree;

  template< class Key, class Value, class Cmp >
  class TreeIterator: public std::iterator< std::bidirectional_iterator_tag, std::pair< Key, Value > >
  {
    friend class AVLtree< Key, Value, Cmp >;
  public:
    using this_t = TreeIterator< Key, Value, Cmp >;
    TreeIterator();
    ~TreeIterator() = default;
    TreeIterator(const this_t &) = default;
    this_t & operator=(const this_t &) = default;
    this_t & operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t & operator--() noexcept;
    this_t operator--(int) noexcept;
    std::pair< Key, Value > & operator*() const;
    std::pair< Key, Value > * operator->() const;
    bool operator==(const this_t &) const noexcept;
    bool operator!=(const this_t &) const noexcept;
  private:
    TreeNode< Key, Value >* node_;
    explicit TreeIterator(TreeNode< Key, Value >*);
  };

  template< class Key, class Value, class Cmp >
  TreeIterator< Key, Value, Cmp >::TreeIterator():
    node_(nullptr)
  {}

  template< class Key, class Value, class Cmp >
  TreeIterator< Key, Value, Cmp >::TreeIterator(TreeNode< Key, Value >* node):
    node_(node)
  {}

  template< class Key, class Value, class Cmp >
  TreeIterator< Key, Value, Cmp >& TreeIterator< Key, Value, Cmp >::operator++() noexcept
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

  template< class Key, class Value, class Cmp >
  TreeIterator< Key, Value, Cmp > TreeIterator< Key, Value, Cmp >::operator++(int) noexcept
  {
    TreeIterator< Key, Value, Cmp > temp = *this;
    ++(*this);
    return temp;
  }

  template< class Key, class Value, class Cmp >
  TreeIterator< Key, Value, Cmp >& TreeIterator< Key, Value, Cmp >::operator--() noexcept
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

  template< class Key, class Value, class Cmp >
  TreeIterator< Key, Value, Cmp > TreeIterator< Key, Value, Cmp >::operator--(int) noexcept
  {
    TreeIterator< Key, Value, Cmp > result = *this;
    --(*this);
    return result;
  }

  template< class Key, class Value, class Cmp >
  std::pair< Key, Value >& TreeIterator< Key, Value, Cmp >::operator*() const
  {
    return node_->data;
  }

  template< class Key, class Value, class Cmp >
  std::pair< Key, Value >* TreeIterator< Key, Value, Cmp >::operator->() const
  {
    return std::addressof(node_->data);
  }

  template< class Key, class Value, class Cmp >
  bool TreeIterator< Key, Value, Cmp >::operator==(const this_t& other) const noexcept
  {
    return node_ == other.node_;
  }

  template< class Key, class Value, class Cmp >
  bool TreeIterator< Key, Value, Cmp >::operator!=(const this_t& other) const noexcept
  {
    return !(other == *this);
  }
}

#endif
