#ifndef CITERATOR_HPP
#define CITERATOR_HPP
#include <iterator>
#include "node.hpp"

namespace abramov
{
  template< class Key, class Value >
  struct ConstIterator: std::iterator< std::bidirectional_iterator_tag, std::pair< Key, Value > >
  {
    ConstIterator();
    ConstIterator(Node< Key, Value > *node);
    ConstIterator(const ConstIterator< Key, Value > &c_iter) = default;
    ~ConstIterator() = default;
    ConstIterator< Key, Value > &operator=(const ConstIterator< Key, Value > &c_iter) = default;
    ConstIterator< Key, Value > &operator++() noexcept;
    ConstIterator< Key, Value > operator++(int) noexcept;
    ConstIterator< Key, Value > &operator--() noexcept;
    ConstIterator< Key, Value > operator--(int) noexcept;
    bool operator==(const ConstIterator< Key, Value > &c_iter) const noexcept;
    bool operator!=(const ConstIterator< Key, Value > &c_iter) const noexcept;
    const std::pair< Key, Value > &operator*() const noexcept;
    const std::pair< Key, Value > *operator->() const noexcept;
  private:
    Node< Key, Value > *node_;
  };

  template< class Key, class Value >
  ConstIterator< Key, Value >::ConstIterator():
    node_(nullptr)
  {}

  template< class Key, class Value >
  ConstIterator< Key, Value >::ConstIterator(Node< Key, Value > *node):
    node_(node)
  {}

  template< class Key, class Value >
  ConstIterator< Key, Value > &ConstIterator< Key, Value >::operator++() noexcept
  {
    if (node_->right_)
    {
      node_ = node_->right_;
      while (node_->left_)
      {
        node_ = node_->left_;
      }
    }
    else
    {
      while (node_->parent_ && node_->parent_->left_ != node_)
      {
        node_ = node_->parent_;
      }
      node_ = node_->parent_;
    }
    return *this;
  }

  template< class Key, class Value >
  ConstIterator< Key, Value > ConstIterator< Key, Value >::operator++(int) noexcept
  {
    ConstIterator< Key, Value > c_iter(*this);
    ++(*this);
    return c_iter;
  }

  template< class Key, class Value >
  ConstIterator< Key, Value > &ConstIterator< Key, Value >::operator--() noexcept
  {
    if (node_->left_)
    {
      node_ = node_->left_;
      while (node_->right_)
      {
        node_ = node_->right;
      }
    }
    else
    {
      while (node_->parent_ && node_->parent_->right != node_)
      {
        node_ = node_->parent_;
      }
      node_ = node_->parent_;
    }
    return *this;
  }

  template< class Key, class Value >
  ConstIterator< Key, Value > ConstIterator< Key, Value >::operator--(int) noexcept
  {
    ConstIterator< Key, Value > c_iter(*this);
    --(*this);
    return c_iter;
  }

  template< class Key, class Value >
  bool ConstIterator< Key, Value >::operator==(const ConstIterator< Key, Value > &c_iter) const noexcept
  {
    return node_ == c_iter.node_;
  }

  template< class Key, class Value >
  bool ConstIterator< Key, Value >::operator!=(const ConstIterator< Key, Value > &c_iter) const noexcept
  {
    return !(c_iter == *this);
  }

  template< class Key, class Value >
  const std::pair< Key, Value > &ConstIterator< Key, Value >::operator*() const noexcept
  {
    return node_->data_;
  }

  template< class Key, class Value >
  const std::pair< Key, Value > *ConstIterator< Key, Value >::operator->() const noexcept
  {
    return std::addressof(node_->data_);
  }
}
#endif
