#ifndef CRNL_ITERATOR_HPP
#define CRNL_ITERATOR_HPP
#include <functional>
#include <stack.hpp>
#include "avltree_node.hpp"

namespace karnauhova
{
  template< typename Key, typename Value, typename Compare >
  class AvlTree;

  template< typename Key, typename Value, typename Compare >
  class RnlIterator;

  template< typename Key, typename Value, typename Compare = std::less< Key > >
  struct CRnlIterator: public std::iterator< std::forward_iterator_tag, Key, Value, Compare >
  {
    friend class AvlTree< Key, Value, Compare >;
    friend class RnlIterator< Key, Value, Compare >;
  public:
    using Node = detail::AvlTreeNode< Key, Value >;
    using this_t = CRnlIterator< Key, Value, Compare >;
    using data_t = std::pair< Key, Value >;

    CRnlIterator() noexcept;
    CRnlIterator(Node*, Node*) noexcept;
    CRnlIterator(const RnlIterator< Key, Value, Compare >&) noexcept;

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;

    const data_t& operator*() const noexcept;
    const data_t* operator->() const noexcept;

    bool operator==(const this_t&) const noexcept;
    bool operator!=(const this_t&) const noexcept;
  private:
    Stack< Node* > stack_;
    Node* node_;
    Node* fake_;
  };

  template< typename Key, typename Value, typename Compare>
  CRnlIterator< Key, Value, Compare >::CRnlIterator() noexcept:
    stack_(),
    node_(nullptr),
    fake_(nullptr)
  {}

  template< typename Key, typename Value, typename Compare>
  CRnlIterator< Key, Value, Compare >::CRnlIterator(Node* node, Node* fake) noexcept:
    stack_(),
    node_(node),
    fake_(fake)
  {}

  template< typename Key, typename Value, typename Compare>
  CRnlIterator< Key, Value, Compare >::CRnlIterator(const RnlIterator< Key, Value, Compare >& copy) noexcept:
    stack_(copy.stack_),
    node_(copy.node),
    fake_(copy.fake)
  {}

  template< typename Key, typename Value, typename Compare>
  typename CRnlIterator< Key, Value, Compare >::this_t& CRnlIterator< Key, Value, Compare >::operator++() noexcept
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
  CRnlIterator< Key, Value, Compare >  CRnlIterator< Key, Value, Compare >::operator++(int) noexcept
  {
    this_t res(*this);
    ++(*this);
    return res;
  }

  template< typename Key, typename Value, typename Compare >
  const std::pair< Key, Value >& CRnlIterator< Key, Value, Compare >::operator*() const noexcept
  {
    return node_->data;
  }

  template< typename Key, typename Value, typename Compare >
  const std::pair< Key, Value >* CRnlIterator< Key, Value, Compare >::operator->() const noexcept
  {
    return std::addressof(node_->data);
  }

  template< typename Key, typename Value, typename Compare >
  bool CRnlIterator< Key, Value, Compare >::operator==(const this_t& oth) const noexcept
  {
    return node_ == oth.node_;
  }

  template< typename Key, typename Value, typename Compare >
  bool CRnlIterator< Key, Value, Compare >::operator!=(const this_t& oth) const noexcept
  {
    return !(*this == oth);
  }
}

#endif
