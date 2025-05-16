#ifndef LNR_ITERATOR_HPP
#define LNR_ITERATOR_HPP
#include <cassert>
#include <tree/node.hpp>
#include <stack.hpp>

namespace demehin
{
  template< typename Key, typename T, typename Cmp >
  class Tree;

  template< typename Key, typename T, typename Cmp, bool isConst >
  class LnrIterator
  {
    friend class LnrIterator< Key, T, Cmp, true >;
    friend class LnrIterator< Key, T, Cmp, false >;
    friend class Tree< Key, T, Cmp >;
  public:
    using Node = demehin::TreeNode< Key, T >;
    using this_t = LnrIterator< Key, T, Cmp, isConst >;
    using node_t = typename std::conditional< isConst, const Node, Node >::type;
    using stack_t = typename std::conditional< isConst, Stack< const Node* >, Stack< Node* > >::type;
    using data_t = typename std::conditional< isConst, const std::pair< Key, T >, std::pair< Key, T > >::type;

    LnrIterator() noexcept;

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;

    data_t& operator*() const noexcept;
    data_t* operator->() const noexcept;

    bool operator==(const this_t&) const noexcept;
    bool operator!=(const this_t&) const noexcept;

  private:
    node_t* node_;
    stack_t stack_;

    explicit LnrIterator(node_t*) noexcept;
  };

  template< typename Key, typename T, typename Cmp, bool isConst >
  LnrIterator< Key, T, Cmp, isConst >::LnrIterator() noexcept:
    node_(nullptr),
    stack_()
  {}

  template< typename Key, typename T, typename Cmp, bool isConst >
  LnrIterator< Key, T, Cmp, isConst >::LnrIterator(node_t* node) noexcept:
    node_(node),
    stack_()
  {}

  template< typename Key, typename T, typename Cmp, bool isConst >
  typename LnrIterator< Key, T, Cmp, isConst >::this_t& LnrIterator< Key, T, Cmp, isConst >::operator++() noexcept
  {
    if (node_ == nullptr)
    {
      return *this;
    }

    if (node_->right != nullptr)
    {
      stack_.push(node_);
      node_ = node_->right;
      while (node_->left != nullptr)
      {
        stack_.push(node_);
        node_ = node_->left;
      }
    }
    else
    {
      while (!stack_.empty() && stack_.top()->right == node_)
      {
        node_ = stack_.top();
        stack_.pop();
      }
      node_ = stack_.empty() ? nullptr : stack_.top();
      if (!stack_.empty())
      {
        stack_.pop();
      }
    }
    return *this;
  }

  template< typename Key, typename T, typename Cmp, bool isConst >
  typename LnrIterator< Key, T, Cmp, isConst >::this_t LnrIterator< Key, T, Cmp, isConst >::operator++(int) noexcept
  {
    this_t res(*this);
    ++(*this);
    return res;
  }

  template< typename Key, typename T, typename Cmp, bool isConst >
  typename LnrIterator< Key, T, Cmp, isConst >::data_t& LnrIterator< Key, T, Cmp, isConst >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< typename Key, typename T, typename Cmp, bool isConst >
  typename LnrIterator< Key, T, Cmp, isConst >::data_t* LnrIterator< Key, T, Cmp, isConst >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename Key, typename T, typename Cmp, bool isConst >
  bool LnrIterator< Key, T, Cmp, isConst >::operator==(const this_t& rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< typename Key, typename T, typename Cmp, bool isConst >
  bool LnrIterator< Key, T, Cmp, isConst >::operator!=(const this_t& rhs) const noexcept
  {
    return !(*this == rhs);
  }
}

#endif

