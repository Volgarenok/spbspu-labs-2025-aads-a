#ifndef AVLTREE_ITERATOR_HPP
#define AVLTREE_ITERATOR_HPP
#include <cstddef>
#include <functional>
#include "avltree_node.hpp"

namespace karnauhova
{
  template<typename Key, typename Value, typename Compare >
  class AvlTree;

  template<typename Key, typename Value, typename Compare >
  class AvlTreeCIterator;

  template< typename Key, typename Value, typename Compare = std::less< Key > >
  struct AvlTreeIterator
  {
    friend class AvlTree< Key, Value, Compare >;
    friend class AvlTreeCIterator< Key, Value, Compare >;
  public:
    using this_t = AvlTreeIterator< Key, Value, Compare>;
    using Node = detail::AvlTreeNode< Key, Value >;
    AvlTreeIterator() noexcept;
    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t& operator--() noexcept;
    this_t operator--(int) noexcept;

    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const noexcept;

    bool operator==(const this_t& rhs) const noexcept;
    bool operator!=(const this_t& rhs) const noexcept;
  private:
    Node* node_;
    Node* fake_;
    explicit AvlTreeIterator(Node* node, Node* fake) noexcept;
  };

  template< typename Key, typename Value, typename Compare >
  AvlTreeIterator< Key, Value, Compare >::AvlTreeIterator() noexcept:
    node_(nullptr),
    fake_(nullptr)
  {}

  template< typename Key, typename Value, typename Compare >
  AvlTreeIterator< Key, Value, Compare >::AvlTreeIterator(Node* node, Node* fake) noexcept:
    node_(node),
    fake_(fake)
  {}

  template< typename Key, typename Value, typename Compare >
  AvlTreeIterator< Key, Value, Compare >&  AvlTreeIterator< Key, Value, Compare >::operator++() noexcept
  {
    if (node_ == fake_ || node_ == nullptr)
    {
      return *this;
    }
    if (node_->right != fake_)
    {
      node_ = node_->right;
      while (node_->left != fake_)
      {
        node_ = node_->left;
      }
    }
    else
    {
      Node* parent = node_->parent;
      while (parent != fake_ && node_ == parent->right)
      {
        node_ = parent;
        parent = parent->parent;
      }
      node_ = parent;
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  AvlTreeIterator< Key, Value, Compare >  AvlTreeIterator< Key, Value, Compare >::operator++(int) noexcept
  {
    this_t res(*this);
    ++(*this);
    return res;
  }

  template< typename Key, typename Value, typename Compare >
  AvlTreeIterator< Key, Value, Compare >&  AvlTreeIterator< Key, Value, Compare >::operator--() noexcept
  {
    if (node_->left != fake_)
    {
      node_ = node_->left;
      while (node_->right != fake_)
      {
        node_ = node_->right;
      }
    }
    else
    {
      while ((node_->parent != fake_) && (node_->parent->left == node_))
      {
        node_ = node_->parent;
      }
      node_ = node_->parent;
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  AvlTreeIterator< Key, Value, Compare >  AvlTreeIterator< Key, Value, Compare >::operator--(int) noexcept
  {
    this_t res(*this);
    --(*this);
    return res;
  }

  template< typename Key, typename Value, typename Compare >
  std::pair< Key, Value >& AvlTreeIterator< Key, Value, Compare >::operator*() const
  {
    return node_->data;
  }

  template< typename Key, typename Value, typename Compare >
  std::pair< Key, Value >* AvlTreeIterator< Key, Value, Compare >::operator->() const noexcept
  {
    return std::addressof(node_->data);
  }

  template< typename Key, typename Value, typename Compare >
  bool AvlTreeIterator< Key, Value, Compare >::operator==(const this_t& rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< typename Key, typename Value, typename Compare >
  bool AvlTreeIterator< Key, Value, Compare >::operator!=(const this_t& rhs) const noexcept
  {
    return !(*this == rhs);
  }
}
#endif
