#ifndef LNRITERATOR_HPP
#define LNRITERATOR_HPP
#include <cassert>
#include <iterator>
#include <utility>
#include "iterator.hpp"
#include "stack.hpp"
#include "treeNode.hpp"
namespace brevnov
{
  template< typename Key, typename Value, typename Cmp >
  class AVLTree;


  template< typename Key, typename Value, typename Cmp, bool IsConst >
  class LnrIterator: public std::iterator< std::forward_iterator_tag, std::pair< Key, Value > >
  {
  public:
    using value = std::pair< Key, Value >;
    using ref = std::conditional_t< IsConst, const value&, value& >;
    using point = std::conditional_t< IsConst, const value*, value* >;
    using Node = TreeNode< Key, Value >;

    LnrIterator() noexcept;
    template < bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
    LnrIterator(const LnrIterator< Key, Value, Cmp, OtherIsConst >&);
    template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
    LnrIterator< Key, Value, Cmp, IsConst > operator=(const LnrIterator< Key, Value, Cmp, OtherIsConst >&);

    LnrIterator< Key, Value, Cmp, IsConst > operator++();
    LnrIterator< Key, Value, Cmp, IsConst > operator++(int);
    bool operator==(const LnrIterator< Key, Value, Cmp, IsConst >&) const noexcept;
    bool operator!=(const LnrIterator< Key, Value, Cmp, IsConst >&) const noexcept;

    ref operator*() const noexcept;
    point operator->() const noexcept;
  private:
    Node* node_;
    Stack< Node* > stack_;
    explicit LnrIterator(Node*) noexcept;
    friend class LnrIterator< Key, Value, Cmp, !IsConst >;
    friend class AVLTree< Key, Value, Cmp >;
  };

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  LnrIterator< Key, Value, Cmp, IsConst >::LnrIterator() noexcept:
    node_(nullptr),
    stack_()
  {}

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > >
  LnrIterator< Key, Value, Cmp, IsConst >::LnrIterator(const LnrIterator< Key, Value, Cmp, OtherIsConst >& copy):
    node_(copy.node_),
    stack_(copy.stack_)
  {}

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > >
  LnrIterator< Key, Value, Cmp, IsConst > LnrIterator< Key, Value, Cmp, IsConst >::operator=(
    const LnrIterator< Key, Value, Cmp, OtherIsConst >& copy)
  {
    stack_ = copy.stack_;
    node_ = copy.node_;
  }

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  LnrIterator< Key, Value, Cmp, IsConst >::LnrIterator(Node* node) noexcept:
    node_(node),
    stack_()
  {}

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  LnrIterator< Key, Value, Cmp, IsConst > LnrIterator< Key, Value, Cmp, IsConst >::operator++()
  {
    if (!node_)
    {
      return *this;
    }
    if (node_->right)
    {
      stack_.push(node_);
      node_ = node_->right;
      while (node_->left)
      {
        stack_.push(node_);
        node_ = node_->left;
      }
    }
    else
    {
      if (!stack_.empty() && stack_.top()->right == node_)
      {
        node_ = stack_.top();
        stack_.pop();
      }
      if (stack_.empty())
      {
        node_ = nullptr;
      }
      else
      {
        node_ = stack_.top();
        stack_.pop();
      }
    }
    return *this;
  }

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  LnrIterator< Key, Value, Cmp, IsConst > LnrIterator< Key, Value, Cmp, IsConst >::operator++(int)
  {
    LnrIterator< Key, Value, Cmp, IsConst > result(*this);
    ++(*this);
    return result;
  }

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  typename LnrIterator< Key, Value, Cmp, IsConst >::ref LnrIterator< Key, Value, Cmp, IsConst >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  typename LnrIterator< Key, Value, Cmp, IsConst >::point LnrIterator< Key, Value, Cmp, IsConst >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  bool LnrIterator< Key, Value, Cmp, IsConst >::operator==(const LnrIterator<Key, Value, Cmp, IsConst>& lhr) const noexcept
  {
    return node_ == lhr.node_;
  }

  template< typename Key, typename Value, typename Cmp, bool IsConst >
  bool LnrIterator< Key, Value, Cmp, IsConst >::operator!=(const LnrIterator<Key, Value, Cmp, IsConst>& lhr) const noexcept
  {
    return !(*this == lhr);
  }
}
#endif
