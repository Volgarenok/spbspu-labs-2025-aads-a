#ifndef AVLTREE_CITERATOR_HPP
#define AVLTREE_CITERATOR_HPP
#include <functional>
#include <cstddef>
#include "avltree_node.hpp"

namespace karnauhova
{
  template<typename Key, typename Value, typename Compare >
  class AvlTree;

  template<typename Key, typename Value, typename Compare >
  class AvlTreeIterator;

  template< typename Key, typename Value, typename Compare = std::less< Key > >
  struct AvlTreeCIterator
  {
    friend class AvlTree< Key, Value, Compare >;
    friend class AvlTreeIterator< Key, Value, Compare >;
  public:
    using this_t = AvlTreeCIterator< Key, Value, Compare>;
    using Node = detail::AvlTreeNode< Key, Value >;
    AvlTreeCIterator() noexcept;
    AvlTreeCIterator(const AvlTreeIterator< Key, Value, Compare >&) noexcept;
    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t& operator--() noexcept;
    this_t operator--(int) noexcept;

    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const noexcept;

    bool operator==(const this_t& rhs) const noexcept;
    bool operator!=(const this_t& rhs) const noexcept;
  private:
    Node* node_;
    Node* fake_;
    explicit AvlTreeCIterator(Node* node, Node* fake) noexcept;
  };

  template< typename Key, typename Value, typename Compare >
  AvlTreeCIterator< Key, Value, Compare >::AvlTreeCIterator() noexcept:
    node_(nullptr),
    fake_(nullptr)
  {}

  template< typename Key, typename Value, typename Compare >
  AvlTreeCIterator< Key, Value, Compare >::AvlTreeCIterator(const AvlTreeIterator< Key, Value, Compare >& oth) noexcept:
    node_(oth.node_),
    fake_(oth.fake_)
  {}

  template< typename Key, typename Value, typename Compare >
  AvlTreeCIterator< Key, Value, Compare >::AvlTreeCIterator(Node* node, Node* fake) noexcept:
    node_(node),
    fake_(fake)
  {}

  template< typename Key, typename Value, typename Compare >
  AvlTreeCIterator< Key, Value, Compare >&  AvlTreeCIterator< Key, Value, Compare >::operator++() noexcept
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
  AvlTreeCIterator< Key, Value, Compare >  AvlTreeCIterator< Key, Value, Compare >::operator++(int) noexcept
  {
    this_t res(*this);
    ++(*this);
    return res;
  }

  template< typename Key, typename Value, typename Compare >
  AvlTreeCIterator< Key, Value, Compare >&  AvlTreeCIterator< Key, Value, Compare >::operator--() noexcept
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
  AvlTreeCIterator< Key, Value, Compare >  AvlTreeCIterator< Key, Value, Compare >::operator--(int) noexcept
  {
    this_t res(*this);
    --(*this);
    return res;
  }

  template< typename Key, typename Value, typename Compare >
  const std::pair< Key, Value >& AvlTreeCIterator< Key, Value, Compare >::operator*() const
  {
    return node_->data;
  }

  template< typename Key, typename Value, typename Compare >
  const std::pair< Key, Value >* AvlTreeCIterator< Key, Value, Compare >::operator->() const noexcept
  {
    return std::addressof(node_->data);
  }

  template< typename Key, typename Value, typename Compare >
  bool AvlTreeCIterator< Key, Value, Compare >::operator==(const this_t& rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< typename Key, typename Value, typename Compare >
  bool AvlTreeCIterator< Key, Value, Compare >::operator!=(const this_t& rhs) const noexcept
  {
    return !(*this == rhs);
  }
}
#endif
