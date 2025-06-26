#ifndef CONSTITERATOR_HPP
#define CONSTITERATOR_HPP

#include <cassert>
#include <iterator>
#include "node.hpp"

namespace mozhegova
{
  template< typename Key, typename T, typename Cmp >
  class BiTree;

  template< typename Key, typename T, typename Cmp >
  class TreeConstIterator:
    public std::iterator< std::bidirectional_iterator_tag, std::pair< Key, T > >
  {
    friend class BiTree< Key, T, Cmp >;
  public:
    using this_t = TreeConstIterator< Key, T, Cmp >;

    TreeConstIterator();
    TreeConstIterator(const this_t &) = default;
    ~TreeConstIterator() = default;
    this_t & operator=(const this_t &) = default;

    this_t & operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t & operator--() noexcept;
    this_t operator--(int) noexcept;

    const std::pair< Key, T > & operator*() const noexcept;
    const std::pair< Key, T > * operator->() const noexcept;

    bool operator==(const this_t &) const noexcept;
    bool operator!=(const this_t &) const noexcept;
  private:
    detail::TreeNode< Key, T > * node_;

    explicit TreeConstIterator(detail::TreeNode< Key, T > *);
  };

  template< typename Key, typename T, typename Cmp >
  TreeConstIterator< Key, T, Cmp >::TreeConstIterator():
    node_(nullptr)
  {}

  template< typename Key, typename T, typename Cmp >
  TreeConstIterator< Key, T, Cmp >::TreeConstIterator(detail::TreeNode< Key, T > * node):
    node_(node)
  {}

  template< typename Key, typename T, typename Cmp >
  TreeConstIterator< Key, T, Cmp > & TreeConstIterator< Key, T, Cmp >::operator++() noexcept
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
  TreeConstIterator< Key, T, Cmp > TreeConstIterator< Key, T, Cmp >::operator++(int) noexcept
  {
    assert(node_ != nullptr);
    TreeConstIterator< Key, T, Cmp > result(*this);
    ++(*this);
    return result;
  }

  template< typename Key, typename T, typename Cmp >
  TreeConstIterator< Key, T, Cmp > & TreeConstIterator< Key, T, Cmp >::operator--() noexcept
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
  TreeConstIterator< Key, T, Cmp > TreeConstIterator< Key, T, Cmp >::operator--(int) noexcept
  {
    assert(node_ != nullptr);
    TreeConstIterator< Key, T, Cmp > result(*this);
    --(*this);
    return result;
  }

  template< typename Key, typename T, typename Cmp >
  const std::pair< Key, T > & TreeConstIterator< Key, T, Cmp >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< typename Key, typename T, typename Cmp >
  const std::pair< Key, T > * TreeConstIterator< Key, T, Cmp >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename Key, typename T, typename Cmp >
  bool TreeConstIterator< Key, T, Cmp >::operator==(const this_t & rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< typename Key, typename T, typename Cmp >
  bool TreeConstIterator< Key, T, Cmp >::operator!=(const this_t & rhs) const noexcept
  {
    return !(rhs == *this);
  }
}

#endif
