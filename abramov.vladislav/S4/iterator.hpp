#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
#include "node.hpp"

namespace abramov
{
  template< class Key, class Value >
  struct Iterator: std::iterator< std::bidirectional_iterator_tag, std::pair< Key, Value > >
  {
    Iterator();
    Iterator(Node< Key, Value > *node, Node< Key, Value > *fake);
    Iterator(const Iterator< Key, Value > &iter) = default;
    ~Iterator() = default;
    Iterator< Key, Value > &operator=(const Iterator< Key, Value > &iter) = default;
    Iterator< Key, Value > &operator++() noexcept;
    Iterator< Key, Value > operator++(int) noexcept;
    Iterator< Key, Value > &operator--() noexcept;
    Iterator< Key, Value > operator--(int) noexcept;
    bool operator==(const Iterator< Key, Value > &iter) const noexcept;
    bool operator!=(const Iterator< Key, Value > &iter) const noexcept;
    std::pair< Key, Value > &operator*() noexcept;
    std::pair< Key, Value > *operator->() noexcept;
  private:
    Node< Key, Value > *node_;
    Node< Key, Value > *fake_;
  };

  template< class Key, class Value >
  Iterator< Key, Value >::Iterator():
    node_(nullptr)
  {}

  template< class Key, class Value >
  Iterator< Key, Value >::Iterator(Node< Key, Value > *node, Node< Key, Value > *fake):
    node_(node),
    fake_(fake)
  {}

  template< class Key, class Value >
  Iterator< Key, Value > &Iterator< Key, Value >::operator++() noexcept
  {
    if (node_->right_ != fake_)
    {
      node_ = node_->right_;
      while (node_->left_ != fake_)
      {
        node_ = node_->left_;
      }
    }
    else
    {
      Node< Key, Value > *parent = node_->parent_;
      while (node_->parent_ && node_ == parent->right_)
      {
        node_ = parent;
        parent = parent->parent_;
      }
      node_ = parent;
    }
    return *this;
  }

  template< class Key, class Value >
  Iterator< Key, Value > Iterator< Key, Value >::operator++(int) noexcept
  {
    Iterator< Key, Value > iter(*this);
    ++(*this);
    return iter;
  }

  template< class Key, class Value >
  Iterator< Key, Value > &Iterator< Key, Value >::operator--() noexcept
  {
    if (node_->left_ != fake_)
    {
      node_ = node_->left_;
      while (node_->right_ != fake_)
      {
        node_ = node_->right_;
      }
    }
    else
    {
      Node< Key, Value > *parent = node_->parent_;
      while (node_->parent_ && node_ == parent->left_)
      {
        node_ = parent;
        parent = parent->parent_;
      }
      node_ = parent;
    }
    return *this;
  }

  template< class Key, class Value >
  Iterator< Key, Value > Iterator< Key, Value >::operator--(int) noexcept
  {
    Iterator< Key, Value > iter(*this);
    --(*this);
    return iter;
  }

  template< class Key, class Value >
  bool Iterator< Key, Value >::operator==(const Iterator< Key, Value > &iter) const noexcept
  {
    return node_ == iter.node_;
  }

  template< class Key, class Value >
  bool Iterator< Key, Value >::operator!=(const Iterator< Key, Value > &iter) const noexcept
  {
    return !(iter == *this);
  }

  template< class Key, class Value >
  std::pair< Key, Value > &Iterator< Key, Value >::operator*() noexcept
  {
    return node_->data_;
  }

  template< class Key, class Value >
  std::pair< Key, Value > *Iterator< Key, Value >::operator->() noexcept
  {
    return std::addressof(node_->data_);
  }
}
#endif
