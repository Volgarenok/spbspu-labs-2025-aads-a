#ifndef BRITERATOR_HPP
#define BRITERATOR_HPP
#include <cassert>
#include <iterator>
#include <utility>
#include "queue.hpp"
#include "treeNode.hpp"

namespace brevnov
{
  template< typename Key, typename Value, typename Cmp >
  class AVLTree;

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  class BrIterator: public std::iterator< std::forward_iterator_tag, std::pair< Key, Value > >
  {
  public:
    using value = std::pair< Key, Value >;
    using ref = std::conditional_t< IsConst, const value&, value& >;
    using point = std::conditional_t< IsConst, const value*, value* >;
    using Node = TreeNode< Key, Value >;

    BrIterator() noexcept;
    template < bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
    BrIterator(const BrIterator< Key, Value, Cmp, OtherIsConst >&);
    template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
    BrIterator< Key, Value, Cmp, IsConst > operator=(const BrIterator< Key, Value, Cmp, OtherIsConst >&);

    BrIterator< Key, Value, Cmp, IsConst > operator++();
    BrIterator< Key, Value, Cmp, IsConst > operator++(int);


    ref operator*() const noexcept;
    point operator->() const noexcept;

    bool operator==(const BrIterator< Key, Value, Cmp, IsConst >&) const noexcept;
    bool operator!=(const BrIterator< Key, Value, Cmp, IsConst >&) const noexcept;
  private:
    Node* node_;
    Queue< Node* > queue_;
    explicit BrIterator(Node*) noexcept;
    friend class BrIterator< Key, Value, Cmp, !IsConst >;
    friend class AVLTree< Key, Value, Cmp >;
  };

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  BrIterator< Key, Value, Cmp, IsConst >::BrIterator() noexcept:
    node_(nullptr),
    queue_()
  {}

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > >
  BrIterator< Key, Value, Cmp, IsConst >::BrIterator(const BrIterator< Key, Value, Cmp, OtherIsConst >& copy):
    node_(copy.node_),
    queue_(copy.queue_)
  {}

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > >
  BrIterator< Key, Value, Cmp, IsConst > BrIterator< Key, Value, Cmp, IsConst >::operator=(
    const BrIterator< Key, Value, Cmp, OtherIsConst >& copy)
  {
    queue_ = copy.queue_;
    node_ = copy.node_;
  }

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  BrIterator< Key, Value, Cmp, IsConst >::BrIterator(Node* node) noexcept:
    node_(node),
    queue_()
  {
    if (node)
    {
      if (node_->left)
      {
        queue_.push(node_->left);
      }
      if (node_->right)
      {
        queue_.push(node_->right);
      }
    }
  }

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  BrIterator< Key, Value, Cmp, IsConst > BrIterator< Key, Value, Cmp, IsConst >::operator++()
  {
    if (queue_.empty())
    {
      node_ = nullptr;
      return *this;
    }
    node_ = queue_.front();
    queue_.pop();
    if (node_->left)
    {
      queue_.push(node_->left);
    }
    if (node_->right)
    {
      queue_.push(node_->right);
    }
    return *this;
  }

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  BrIterator< Key, Value, Cmp, IsConst > BrIterator< Key, Value, Cmp, IsConst >::operator++(int)
  {
    BrIterator< Key, Value, Cmp, IsConst > result(*this);
    ++(*this);
    return result;
  }

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  typename BrIterator< Key, Value, Cmp, IsConst >::ref BrIterator< Key, Value, Cmp, IsConst >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  typename BrIterator< Key, Value, Cmp, IsConst >::point BrIterator< Key, Value, Cmp, IsConst >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  bool BrIterator< Key, Value, Cmp, IsConst >::operator==(const BrIterator<Key, Value, Cmp, IsConst>& bri) const noexcept
  {
    return node_ == bri.node_;
  }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    bool BrIterator< Key, Value, Cmp, IsConst >::operator!=(const BrIterator< Key, Value, Cmp, IsConst >& bri) const noexcept
    {
      return !(*this == bri);
    }
}
#endif
