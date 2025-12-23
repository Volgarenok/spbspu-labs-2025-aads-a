#ifndef TREE_CITER_HPP
#define TREE_CITER_HPP
#include <iterator>
#include <cassert>
#include "tree-node.hpp"

namespace sveshnikov
{
  template< typename Key, typename T, typename Cmp >
  class AvlTree;

  template< typename Key, typename T >
  class ConstIter: public std::iterator< std::bidirectional_iterator_tag, std::pair< Key, T > >
  {
  public:
    using value_t = std::pair< Key, T >;
    ConstIter();
    ConstIter(const ConstIter &rhs) = default;
    ~ConstIter() = default;

    ConstIter &operator=(const ConstIter &rhs) = default;
    ConstIter &operator++() noexcept;
    ConstIter operator++(int) noexcept;
    ConstIter &operator--() noexcept;
    ConstIter operator--(int) noexcept;

    const value_t &operator*() const noexcept;
    const value_t *operator->() const noexcept;

    bool operator!=(const ConstIter &rhs) const noexcept;
    bool operator==(const ConstIter &rhs) const noexcept;

  private:
    const tree_node_t< Key, T > *node_;

    ConstIter(const tree_node_t< Key, T > *node);

    template< typename K, typename V, typename C >
    friend class AvlTree;
  };

  template< typename Key, typename T >
  ConstIter< Key, T >::ConstIter():
    node_(nullptr)
  {}

  template< typename Key, typename T >
  ConstIter< Key, T >::ConstIter(const tree_node_t< Key, T > *node):
    node_(node)
  {}

  template< typename Key, typename T >
  ConstIter< Key, T > &ConstIter< Key, T >::operator++() noexcept
  {
    assert(node_ != nullptr);
    if (node_->right_)
    {
      node_ = node_->right_;
      while (node_->left_)
      {
        node_ = node_->left_;
      }
    }
    else
    {
      while (node_->parent_ && node_ != node_->parent_->left_)
      {
        node_ = node_->parent_;
      }
      node_ = node_->parent_;
    }
    return *this;
  }

  template< typename Key, typename T >
  ConstIter< Key, T > ConstIter< Key, T >::operator++(int) noexcept
  {
    assert(node_ != nullptr);
    ConstIter result(*this);
    ++(*this);
    return result;
  }

  template< typename Key, typename T >
  ConstIter< Key, T > &ConstIter< Key, T >::operator--() noexcept
  {
    assert(node_ != nullptr);
    if (node_->left_)
    {
      node_ = node_->left_;
      while (node_->right_)
      {
        node_ = node_->right_;
      }
    }
    else
    {
      while (node_->parent_ && node_ == node_->parent_->left_)
      {
        node_ = node_->parent_;
      }
      node_ = node_->parent_;
    }
    return *this;
  }

  template< typename Key, typename T >
  ConstIter< Key, T > ConstIter< Key, T >::operator--(int) noexcept
  {
    assert(node_ != nullptr);
    ConstIter result(*this);
    --(*this);
    return result;
  }

  template< typename Key, typename T >
  const typename ConstIter< Key, T >::value_t &ConstIter< Key, T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename Key, typename T >
  const typename ConstIter< Key, T >::value_t *ConstIter< Key, T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename Key, typename T >
  bool ConstIter< Key, T >::operator!=(const ConstIter &rhs) const noexcept
  {
    return node_ != rhs.node_;
  }

  template< typename Key, typename T >
  bool ConstIter< Key, T >::operator==(const ConstIter &rhs) const noexcept
  {
    return node_ == rhs.node_;
  }
}

#endif
