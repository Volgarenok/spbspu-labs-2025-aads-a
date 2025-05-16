#ifndef CONST_ITERATOR_IMPL_HPP
#define CONST_ITERATOR_IMPL_HPP

#include "const-iterator.hpp"
#include "tree.hpp"

namespace aleksandrov
{
  template< typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare >& ConstIterator< Key, Value, Compare >::operator++() noexcept
  {
    assert(node_);
    if (node_->isTriple())
    {
      if (node_->middle && pos_ == PointsTo::Left)
      {
        node_ = fallLeft(node_->middle).node_;
        pos_ = PointsTo::Left;
        return *this;
      }
      else if (node_->right && pos_ == PointsTo::Right)
      {
        node_ = fallLeft(node_->right).node_;
        pos_ = PointsTo::Left;
        return *this;
      }
      if (pos_ == PointsTo::Left)
      {
        pos_ = PointsTo::Right;
        return *this;
      }
    }
    else if (node_->right)
    {
      node_ = fallLeft(node_->right).node_;
      pos_ = PointsTo::Left;
      return *this;
    }
    while (node_->parent)
    {
      if (node_->parent->left == node_)
      {
        node_ = node_->parent;
        pos_ = PointsTo::Left;
        return *this;
      }
      else if (node_->isTriple() && node_->parent->middle == node_)
      {
        node_ = node_->parent;
        pos_ = PointsTo::Right;
        return *this;
      }
      node_ = node_->parent;
    }
    node_ = nullptr;
    pos_ = PointsTo::Left;
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare > ConstIterator< Key, Value, Compare >::operator++(int) noexcept
  {
    assert(node_);
    ConstIterator< Key, Value, Compare > result(*this);
    ++(*this);
    return result;
  }

  template< typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare >& ConstIterator< Key, Value, Compare >::operator--() noexcept
  {
    assert(node_);
    if (node_->isTriple())
    {
      if (node_->middle && pos_ == PointsTo::Right)
      {
        node_ = fallRight(node_->middle).node_;
        pos_ = node_->isTriple() ? PointsTo::Right : PointsTo::Double;
        return *this;
      }
      else if (node_->left && pos_ == PointsTo::Left)
      {
        node_ = fallRight(node_->left).node_;
        pos_ = node_->isTriple() ? PointsTo::Right : PointsTo::Double;
        return *this;
      }
      if (pos_ == PointsTo::Right)
      {
        pos_ = PointsTo::Left;
        return *this;
      }
    }
    else if (node_->left)
    {
      node_ = fallRight(node_->left).node_;
      pos_ = node_->isTriple() ? PointsTo::Right : PointsTo::Double;
      return *this;
    }
    while (node_->parent)
    {
      if (node_->parent->right == node_)
      {
        node_ = node_->parent;
        pos_ = node_->isTriple() ? PointsTo::Right : PointsTo::Double;
        return *this;
      }
      else if (node_->isTriple() && node_->parent->middle == node_)
      {
        node_ = node_->parent;
        pos_ = PointsTo::Left;
        return *this;
      }
      node_ = node_->parent;
    }
    node_ = nullptr;
    pos_ = PointsTo::Double;
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare > ConstIterator< Key, Value, Compare >::operator--(int) noexcept
  {
    assert(node_);
    ConstIterator< Key, Value, Compare > result(*this);
    --(*this);
    return result;
  }

  template< typename Key, typename Value, typename Compare >
  const std::pair< Key, Value >& ConstIterator< Key, Value, Compare >::operator*() const
  {
    assert(node_);
    return pos_ == PointsTo::Left ? node_->data[0] : node_->data[1];
  }

  template< typename Key, typename Value, typename Compare >
  const std::pair< Key, Value >* ConstIterator< Key, Value, Compare >::operator->() const
  {
    assert(node_);
    return pos_ == PointsTo::Left ? std::addressof(node_->data[0]) : std::addressof(node_->data[1]);
  }

  template< typename Key, typename Value, typename Compare >
  bool ConstIterator< Key, Value, Compare >::operator==(const ConstIterator< Key, Value, Compare >& rhs) const noexcept
  {
    return node_ == rhs.node_ && type_ == rhs.type_ && pos_ == rhs.pos_;
  }

  template< typename Key, typename Value, typename Compare >
  bool ConstIterator< Key, Value, Compare >::operator!=(const ConstIterator< Key, Value, Compare >& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare > ConstIterator< Key, Value, Compare >::fallLeft(ConstIterator< Key, Value, Compare > root)
  {
    while (root->left)
    {
      root = root->left;
    }
    return root;
  }

  template< typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare > ConstIterator< Key, Value, Compare >::fallRight(ConstIterator< Key, Value, Compare > root)
  {
    while (root->right)
    {
      root = root->right;
    }
    return root;
  }

  template< typename Key, typename Value, typename Compare >
  bool ConstIterator< Key, Value, Compare >::isTriple() const noexcept
  {
    return type_ == NodeType::Triple;
  }
}

#endif
