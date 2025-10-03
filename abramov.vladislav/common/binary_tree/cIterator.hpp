#ifndef CITERATOR_HPP
#define CITERATOR_HPP
#include <iterator>
#include "node.hpp"

namespace abramov
{
  template< class Key, class Value >
  struct ConstIterator: std::iterator< std::bidirectional_iterator_tag, std::pair< Key, Value > >
  {
    using cnode_t = const Node< Key, Value >;

    ConstIterator();
    ConstIterator(const Node< Key, Value > *node, const Node< Key, Value > *fake, const Node< Key, Value > *root);
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
    const Node< Key, Value > *node_;
    const Node< Key, Value > *fake_;
    const Node< Key, Value > *root_;

    const Node< Key, Value > *cgetMax(const Node< Key, Value > *root) noexcept;
  };

  template< class Key, class Value >
  ConstIterator< Key, Value >::ConstIterator():
    node_(nullptr),
    fake_(nullptr),
    root_(nullptr)
  {}

  template< class Key, class Value >
  ConstIterator< Key, Value >::ConstIterator(cnode_t *node, cnode_t *fake, cnode_t *root):
    node_(node),
    fake_(fake),
    root_(root)
  {}

  template< class Key, class Value >
  ConstIterator< Key, Value > &ConstIterator< Key, Value >::operator++() noexcept
  {
    if (node_ == cgetMax(root_))
    {
      node_ = fake_;
    }
    else if (node_->right_ != fake_)
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
  ConstIterator< Key, Value > ConstIterator< Key, Value >::operator++(int) noexcept
  {
    ConstIterator< Key, Value > c_iter(*this);
    ++(*this);
    return c_iter;
  }

  template< class Key, class Value >
  ConstIterator< Key, Value > &ConstIterator< Key, Value >::operator--() noexcept
  {
    if (node_ == fake_)
    {
      node_ = cgetMax(root_);
    }
    else if (node_->left_ != fake_)
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

  template< class Key, class Value >
  const Node< Key, Value > *ConstIterator< Key, Value >::cgetMax(const Node< Key, Value > *root) noexcept
  {
    if (root == fake_ || !root)
    {
      return fake_;
    }
    while (root->right_ != fake_)
    {
      root = root->right_;
    }
    return root;
  }
}
#endif
