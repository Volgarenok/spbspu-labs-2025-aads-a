#ifndef RNL_ITERATOR_HPP
#define RNL_ITERATOR_HPP
#include <functional>
#include "avltree_node.hpp"
#include <stack.hpp>
namespace karnauhova
{
  template< typename Key, typename Value, typename Compare >
  class AvlTree;

  template< typename Key, typename Value, typename Compare = std::less< Key > >
  struct RnlIterator: public std::iterator< std::forward_iterator_tag, Key, Value, Compare >
  {
    friend class AvlTree< Key, Value, Compare >;
  public:
    using Node = detail::AvlTreeNode< Key, Value >;
    using this_t = RnlIterator< Key, Value, Compare >;
    using data_t = std::pair< Key, Value >;

    RnlIterator() noexcept;
    RnlIterator(Node*, Node*) noexcept;

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;

    data_t& operator*() const noexcept;
    data_t* operator->() const noexcept;

    bool operator==(const this_t&) const noexcept;
    bool operator!=(const this_t&) const noexcept;
  private:
    Stack< Node* > stack_;
    Node* node_;
    Node* fake_;
  };

  template< typename Key, typename Value, typename Compare>
  RnlIterator< Key, Value, Compare >::RnlIterator() noexcept:
    stack_(),
    node_(nullptr),
    fake_(nullptr)
  {}

  template< typename Key, typename Value, typename Compare>
  RnlIterator< Key, Value, Compare >::RnlIterator(Node* node, Node* fake) noexcept:
    stack_(),
    node_(node),
    fake_(fake)
  {}

  template< typename Key, typename Value, typename Compare>
  typename RnlIterator< Key, Value, Compare >::this_t& RnlIterator< Key, Value, Compare >::operator++() noexcept
  {
    if (node_ == nullptr || node_ == fake_)
    {
      return *this;
    }
    if (node_->left != fake_)
    {
      stack_.push(node_);
      node_ = node_->left;
      while (node_->right != fake_)
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
      if (stack_.empty())
      {
        node_ = fake_;
      }
      else
      {
        node_ = stack_.top();
        stack_.pop();
      }
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  RnlIterator< Key, Value, Compare >  RnlIterator< Key, Value, Compare >::operator++(int) noexcept
  {
    this_t res(*this);
    ++(*this);
    return res;
  }

  template< typename Key, typename Value, typename Compare >
  std::pair< Key, Value >& RnlIterator< Key, Value, Compare >::operator*() const noexcept
  {
    return node_->data;
  }

  template< typename Key, typename Value, typename Compare >
  std::pair< Key, Value >* RnlIterator< Key, Value, Compare >::operator->() const noexcept
  {
    return std::addressof(node_->data);
  }

  template< typename Key, typename Value, typename Compare >
  bool RnlIterator< Key, Value, Compare >::operator==(const this_t& oth) const noexcept
  {
    return node_ == oth.node_;
  }

  template< typename Key, typename Value, typename Compare >
  bool RnlIterator< Key, Value, Compare >::operator!=(const this_t& oth) const noexcept
  {
    return !(*this == oth);
  }
}

#endif
