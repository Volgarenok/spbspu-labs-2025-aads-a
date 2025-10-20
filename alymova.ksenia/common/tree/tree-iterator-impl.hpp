#ifndef TREE_ITERATOR_IMPL_HPP
#define TREE_ITERATOR_IMPL_HPP
#include <cassert>
#include "tree-iterators.hpp"

namespace alymova
{
  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator >::TTTIterator():
    node_(nullptr),
    point_(NodePoint::Empty)
  {}

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator >::TTTIterator(Node* node, NodePoint point):
    node_(node),
    point_(point)
  {}

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator >::TTTIterator(TTTConstIterator< Key, Value, Comparator> it):
    Iterator(const_cast< Node* >(it.node_), it.point_)
  {}

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator>& TTTIterator< Key, Value, Comparator >::operator++() noexcept
  {
    assert(node_ != nullptr && "You are trying to access beyond tree's bounds");
    assert(node_->type != NodeType::Empty && "Incorrect node index");

    if (node_->type == NodeType::Double)
    {
      if (node_->right)
      {
        node_ = node_->right;
        fall_left();
        point_ = (node_->type == NodeType::Fake) ? NodePoint::Fake : NodePoint::First;
        return *this;
      }
    }
    else if (node_->type == NodeType::Triple)
    {
      if (point_ == NodePoint::First)
      {
        if (node_->mid)
        {
          node_ = node_->mid;
          fall_left();
          return *this;
        }
        point_ = NodePoint::Second;
        return *this;
      }
      else if (point_ == NodePoint::Second)
      {
        if (node_->right)
        {
          node_ = node_->right;
          fall_left();
          point_ = (node_->type == NodeType::Fake) ? NodePoint::Fake : NodePoint::First;
          return *this;
        }
      }
    }
    while (node_->parent)
    {
      if (node_->parent->left == node_)
      {
        node_ = node_->parent;
        point_ = NodePoint::First;
        return *this;
      }
      if (node_->parent->mid == node_)
      {
        node_ = node_->parent;
        point_ = NodePoint::Second;
        return *this;
      }
      node_ = node_->parent;
    }
    node_ = nullptr;
    point_ = NodePoint::Fake;
    return *this;
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator> TTTIterator< Key, Value, Comparator >::operator++(int) noexcept
  {
    Iterator old = *this;
    ++(*this);
    return old;
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator>& TTTIterator< Key, Value, Comparator >::operator--() noexcept
  {
    assert(node_ != nullptr && "You are trying to access beyond tree's bounds");

    if (node_->type == NodeType::Double)
    {
      if (node_->left && node_->left->type != NodeType::Fake)
      {
        node_ = node_->left;
        fall_right();
        point_ = (node_->type == NodeType::Double) ? NodePoint::First : NodePoint::Second;
        return *this;
      }
    }
    else if (node_->type == NodeType::Triple)
    {
      if (point_ == NodePoint::First)
      {
        if (node_->left && node_->left->type != NodeType::Fake)
        {
          node_ = node_->left;
          fall_right();
          point_ = (node_->type == NodeType::Double) ? NodePoint::First : NodePoint::Second;
          return *this;
        }
      }
      else if (point_ == NodePoint::Second)
      {
        if (node_->mid)
        {
          node_ = node_->mid;
          fall_right();
          point_ = (node_->type == NodeType::Double) ? NodePoint::First : NodePoint::Second;
          return *this;
        }
        point_ = NodePoint::First;
        return *this;
      }
    }
    while (node_->parent)
    {
      if (node_->parent->right == node_)
      {
        node_ = node_->parent;
        point_ = (node_->type == NodeType::Double) ? NodePoint::First : NodePoint::Second;
        return *this;
      }
      if (node_->parent->mid == node_)
      {
        node_ = node_->parent;
        point_ = NodePoint::First;
        return *this;
      }
      node_ = node_->parent;
    }
    node_ = nullptr;
    point_ = NodePoint::Fake;
    return *this;
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator> TTTIterator< Key, Value, Comparator >::operator--(int) noexcept
  {
    Iterator old = *this;
    --(*this);
    return old;
  }

  template< class Key, class Value, class Comparator >
  bool TTTIterator< Key, Value, Comparator >::operator==(const Iterator& other) const noexcept
  {
    if (point_ == NodePoint::Fake && other.point_ == NodePoint::Fake)
    {
      return true;
    }
    return (node_ == other.node_) && (point_ == other.point_);
  }

  template< class Key, class Value, class Comparator >
  bool TTTIterator< Key, Value, Comparator >::operator!=(const Iterator& other) const noexcept
  {
    return !(*this == other);
  }

  template< class Key, class Value, class Comparator >
  std::pair< Key, Value >& TTTIterator< Key, Value, Comparator >::operator*() noexcept
  {
    assert(node_ != nullptr && "You are trying to access beyond tree's bounds");

    return node_->data[point_ - 1];
  }

  template< class Key, class Value, class Comparator >
  std::pair< Key, Value >* TTTIterator< Key, Value, Comparator >::operator->() noexcept
  {
    assert(node_ != nullptr && "You are trying to access beyond tree's bounds");

    return std::addressof(node_->data[point_ - 1]);
  }

  template< class Key, class Value, class Comparator >
  void TTTIterator< Key, Value, Comparator >::fall_left()
  {
    while (node_->left && node_->left->type != NodeType::Fake)
    {
      node_ = node_->left;
    }
  }

  template< class Key, class Value, class Comparator >
  void TTTIterator< Key, Value, Comparator >::fall_right()
  {
    while (node_->right)
    {
      node_ = node_->right;
    }
  }
}
#endif
