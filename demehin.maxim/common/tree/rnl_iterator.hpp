#ifndef RNL_ITERATOR_HPP
#define RNL_ITERATOR_HPP
#include <cassert>
#include <tree/node.hpp>
#include <stack.hpp>

namespace demehin
{
  template< typename Key, typename T, typename Cmp >
  class Tree;

  template< typename Key, typename T, typename Cmp, bool isConst >
  class RnlIterator
  {
    friend class RnlIterator< Key, T, Cmp, true >;
    friend class RnlIterator< Key, T, Cmp, false >;
    friend class Tree< Key, T, Cmp >;
  public:
    using Node = demehin::TreeNode< Key, T >;
    using this_t = RnlIterator< Key, T, Cmp, isConst >;
    using node_t = typename std::conditional< isConst, const Node, Node >::type;
    using stack_t = typename std::conditional< isConst, Stack< const Node* >, Stack< Node* > >::type;
    using data_t = typename std::conditional< isConst, const std::pair< Key, T >, std::pair< Key, T > >::type;

    RnlIterator() noexcept;

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;

    data_t& operator*() const noexcept;
    data_t* operator->() const noexcept;

    bool operator==(const this_t&) const noexcept;
    bool operator!=(const this_t&) const noexcept;

  private:
    node_t* node_;
    stack_t stack_;

    explicit RnlIterator(node_t*) noexcept;
  };

  template< typename Key, typename T, typename Cmp, bool isConst >
  RnlIterator< Key, T, Cmp, isConst >::RnlIterator() noexcept:
    node_(nullptr),
    stack_()
  {}

  template< typename Key, typename T, typename Cmp, bool isConst >
  RnlIterator< Key, T, Cmp, isConst >::RnlIterator(node_t* node) noexcept:
    node_(node),
    stack_()
  {}

  template< typename Key, typename T, typename Cmp, bool isConst >
  typename RnlIterator< Key, T, Cmp, isConst >::this_t& RnlIterator< Key, T, Cmp, isConst >::operator++() noexcept
  {
    if (node_ == nullptr)
    {
      return *this;
    }

    if (node_->left != nullptr)
    {
      stack_.push(node_);
      node_ = node_->left;
      while (node_->right != nullptr)
      {
        stack_.push(node_);
        node_ = node_->right;
      }
    }
    else
    {
      while (!stack_.empty() && stack_.top()->left == node_)
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
  typename RnlIterator< Key, T, Cmp, isConst >::this_t RnlIterator< Key, T, Cmp, isConst >::operator++(int) noexcept
  {
    this_t res(*this);
    ++(*this);
    return res;
  }

  template< typename Key, typename T, typename Cmp, bool isConst >
  typename RnlIterator< Key, T, Cmp, isConst >::data_t& RnlIterator< Key, T, Cmp, isConst >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< typename Key, typename T, typename Cmp, bool isConst >
  typename RnlIterator< Key, T, Cmp, isConst >::data_t* RnlIterator< Key, T, Cmp, isConst >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename Key, typename T, typename Cmp, bool isConst >
  bool RnlIterator< Key, T, Cmp, isConst >::operator==(const this_t& rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< typename Key, typename T, typename Cmp, bool isConst >
  bool RnlIterator< Key, T, Cmp, isConst >::operator!=(const this_t& rhs) const noexcept
  {
    return !(*this == rhs);
  }
}

#endif
