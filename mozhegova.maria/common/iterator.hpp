#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <cassert>
#include <iterator>
#include "node.hpp"

namespace mozhegova
{
  template< typename Key, typename T, typename Cmp >
  class BiTree;

  template< typename Key, typename T, typename Cmp >
  class TreeIterator:
    public std::iterator< std::bidirectional_iterator_tag, std::pair< Key, T > >
  {
    friend class BiTree< Key, T, Cmp >;
  public:
    using this_t = TreeIterator< Key, T, Cmp >;

    TreeIterator();
    TreeIterator(const this_t &) = default;
    ~TreeIterator() = default;
    this_t & operator=(const this_t &) = default;

    this_t & operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t & operator--() noexcept;
    this_t operator--(int) noexcept;

    std::pair< Key, T > & operator*() const noexcept;
    std::pair< Key, T > * operator->() const noexcept;

    bool operator==(const this_t &) const noexcept;
    bool operator!=(const this_t &) const noexcept;
  private:
    detail::TreeNode< Key, T > * node_;

    explicit TreeIterator(detail::TreeNode< Key, T > *);
  };

  template< typename Key, typename T, typename Cmp >
  TreeIterator< Key, T, Cmp >::TreeIterator():
    node_(nullptr)
  {}

  template< typename Key, typename T, typename Cmp >
  TreeIterator< Key, T, Cmp >::TreeIterator(detail::TreeNode< Key, T > * node):
    node_(node)
  {}

  template< typename Key, typename T, typename Cmp >
  TreeIterator< Key, T, Cmp > & TreeIterator< Key, T, Cmp >::operator++() noexcept
  {
    assert(node_ != nullptr);
    if (node_->right->h != 0)
    {
      node_ = node_->right;
      while (node_->left->h != 0)
      {
        node_ = node_->left;
      }
    }
    else
    {
      while (node_->parent && node_ == node_->parent->right)
      {
        node_ = node_->parent;
      }
      node_ = node_->parent;
    }
    return *this;
  }

  template< typename Key, typename T, typename Cmp >
  TreeIterator< Key, T, Cmp > TreeIterator< Key, T, Cmp >::operator++(int) noexcept
  {
    assert(node_ != nullptr);
    TreeIterator< Key, T, Cmp > result(*this);
    ++(*this);
    return result;
  }

  template< typename Key, typename T, typename Cmp >
  TreeIterator< Key, T, Cmp > & TreeIterator< Key, T, Cmp >::operator--() noexcept
  {
    assert(node_ != nullptr);
    if (node_->left->h != 0)
    {
      node_ = node_->left;
      while (node_->right->h != 0)
      {
        node_ = node_->right;
      }
    }
    else
    {
      while (node_->parent && node_ == node_->parent->left)
      {
        node_ = node_->parent;
      }
      node_ = node_->parent;
    }
    return *this;
  }

  template< typename Key, typename T, typename Cmp >
  TreeIterator< Key, T, Cmp > TreeIterator< Key, T, Cmp >::operator--(int) noexcept
  {
    assert(node_ != nullptr);
    TreeIterator< Key, T, Cmp > result(*this);
    --(*this);
    return result;
  }

  template< typename Key, typename T, typename Cmp >
  std::pair< Key, T > & TreeIterator< Key, T, Cmp >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< typename Key, typename T, typename Cmp >
  std::pair< Key, T > * TreeIterator< Key, T, Cmp >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename Key, typename T, typename Cmp >
  bool TreeIterator< Key, T, Cmp >::operator==(const this_t & rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< typename Key, typename T, typename Cmp >
  bool TreeIterator< Key, T, Cmp >::operator!=(const this_t & rhs) const noexcept
  {
    return !(rhs == *this);
  }
}

#endif
