#ifndef TREE_ITER_HPP
#define TREE_ITER_HPP
#include <iterator>
#include <cassert>
#include "tree-node.hpp"

namespace sveshnikov
{
  template< typename Key, typename T, typename Cmp >
  class AvlTree;

  template< typename Key, typename T >
  class Iter: public std::iterator< std::bidirectional_iterator_tag, std::pair< Key, T > >
  {
  public:
    using value_t = std::pair< Key, T >;
    Iter();
    Iter(const Iter &rhs) = default;
    ~Iter() = default;

    Iter &operator=(const Iter &rhs) = default;
    Iter &operator++() noexcept;
    Iter operator++(int) noexcept;
    Iter &operator--() noexcept;
    Iter operator--(int) noexcept;

    const value_t &operator*() const noexcept;
    value_t &operator*() noexcept;
    const value_t *operator->() const noexcept;
    value_t *operator->() noexcept;

    bool operator!=(const Iter &rhs) const noexcept;
    bool operator==(const Iter &rhs) const noexcept;

  private:
    tree_node_t< Key, T > *node_;

    Iter(tree_node_t< Key, T > *node);

    template< typename K, typename V, typename C >
    friend class AvlTree;
  };

  template< typename Key, typename T >
  Iter< Key, T >::Iter():
    node_(nullptr)
  {}

  template< typename Key, typename T >
  Iter< Key, T >::Iter(tree_node_t< Key, T > *node):
    node_(node)
  {}

  template< typename Key, typename T >
  Iter< Key, T > &Iter< Key, T >::operator++() noexcept
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
  Iter< Key, T > Iter< Key, T >::operator++(int) noexcept
  {
    assert(node_ != nullptr);
    Iter result(*this);
    ++(*this);
    return result;
  }

  template< typename Key, typename T >
  Iter< Key, T > &Iter< Key, T >::operator--() noexcept
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
  Iter< Key, T > Iter< Key, T >::operator--(int) noexcept
  {
    assert(node_ != nullptr);
    Iter result(*this);
    --(*this);
    return result;
  }

  template< typename Key, typename T >
  const typename Iter< Key, T >::value_t &Iter< Key, T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename Key, typename T >
  typename Iter< Key, T >::value_t &Iter< Key, T >::operator*() noexcept
  {
    return const_cast< value_t & >(static_cast< const Iter< Key, T > & >(*this).operator*());
  }

  template< typename Key, typename T >
  const typename Iter< Key, T >::value_t *Iter< Key, T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename Key, typename T >
  typename Iter< Key, T >::value_t *Iter< Key, T >::operator->() noexcept
  {
    return const_cast< value_t * >(static_cast< const Iter< Key, T > * >(this)->operator->());
  }

  template< typename Key, typename T >
  bool Iter< Key, T >::operator!=(const Iter &rhs) const noexcept
  {
    return node_ != rhs.node_;
  }

  template< typename Key, typename T >
  bool Iter< Key, T >::operator==(const Iter &rhs) const noexcept
  {
    return node_ == rhs.node_;
  }
}

#endif
