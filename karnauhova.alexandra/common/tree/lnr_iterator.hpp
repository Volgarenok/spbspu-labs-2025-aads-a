#ifndef LNR_ITERATOR_HPP
#define LNR_ITERATOR_HPP
#include <functional>
#include "avltree_node.hpp"
#include <stack.hpp>
namespace karnauhova
{
  template< typename Key, typename Value, typename Compare >
  class AvlTree;

  template< typename Key, typename Value, typename Compare = std::less< Key > >
  struct LnrIterator: public std::iterator< std::forward_iterator_tag, Key, Value, Compare >
  {
    friend class AvlTree< Key, Value, Compare >;
  public:
    using Node = detail::AvlTreeNode< Key, Value >;
    using this_t = LnrIterator< Key, Value, Compare >;
    using data_t = std::pair< Key, Value >;

    LnrIterator() noexcept;
    LnrIterator(Node*, Node*) noexcept;

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
  LnrIterator< Key, Value, Compare >::LnrIterator() noexcept:
    stack_(),
    node_(nullptr),
    fake_(nullptr)
  {}

  template< typename Key, typename Value, typename Compare>
  LnrIterator< Key, Value, Compare >::LnrIterator(Node* node, Node* fake) noexcept:
    stack_(),
    node_(node),
    fake_(fake)
  {}

  template< typename Key, typename Value, typename Compare>
  typename LnrIterator< Key, Value, Compare >::this_t& LnrIterator< Key, Value, Compare >::operator++() noexcept
  {
    if (node_ == nullptr || node_ == fake_)
    {
      return *this;
    }
    if (node_->right != fake_)
    {
      stack_.push(node_);
      node_ = node_->right;
      while (node_->left != fake_)
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
  LnrIterator< Key, Value, Compare >  LnrIterator< Key, Value, Compare >::operator++(int) noexcept
  {
    this_t res(*this);
    ++(*this);
    return res;
  }

  template< typename Key, typename Value, typename Compare >
  std::pair< Key, Value >& LnrIterator< Key, Value, Compare >::operator*() const noexcept
  {
    return node_->data;
  }

  template< typename Key, typename Value, typename Compare >
  std::pair< Key, Value >* LnrIterator< Key, Value, Compare >::operator->() const noexcept
  {
    return std::addressof(node_->data);
  }

  template< typename Key, typename Value, typename Compare >
  bool LnrIterator< Key, Value, Compare >::operator==(const this_t& oth) const noexcept
  {
    return node_ == oth.node_;
  }

  template< typename Key, typename Value, typename Compare >
  bool LnrIterator< Key, Value, Compare >::operator!=(const this_t& oth) const noexcept
  {
    return !(*this == oth);
  }
}

#endif
