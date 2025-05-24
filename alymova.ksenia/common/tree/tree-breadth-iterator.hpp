#ifndef TREE_BREADTH_ITERATOR_HPP
#define TREE_BREADTH_ITERATOR_HPP
#include <iterator>
#include <queue.hpp>
#include "tree-node.hpp"

#include <iostream>

namespace alymova
{
  template< class Key, class Value, class Comparator >
  struct TTTConstBreadthIterator:
    public std::iterator< std::forward_iterator_tag, std::pair< Key, Value > >
  {
  public:
    using ConstBreadthIterator = TTTConstBreadthIterator< Key, Value, Comparator >;
    using Node = typename detail::TTTNode< Key, Value, Comparator >;
    using NodeType = typename Node::NodeType;
    using NodePoint = typename detail::NodePoint;

    TTTConstBreadthIterator();
    ConstBreadthIterator& operator++();
    ConstBreadthIterator operator++(int);
    bool operator==(const ConstBreadthIterator& other) const noexcept;
    bool operator!=(const ConstBreadthIterator& other) const noexcept;
    const std::pair< Key, Value >& operator*() const noexcept;
    const std::pair< Key, Value >* operator->() const noexcept;
  protected:
    std::pair< Node*, NodePoint > node_;
    Queue< std::pair< Node*, NodePoint > > nexts_;

    TTTConstBreadthIterator(Node* node, NodePoint point);

    friend class TwoThreeTree< Key, Value, Comparator >;
  };

  template< class Key, class Value, class Comparator >
  struct TTTBreadthIterator final:
    public TTTConstBreadthIterator< Key, Value, Comparator >
  {
    using Base = TTTConstBreadthIterator< Key, Value, Comparator >;
    using Node = typename detail::TTTNode< Key, Value, Comparator >;
    using NodePoint = typename detail::NodePoint;

    TTTBreadthIterator(Node* root, NodePoint);

    std::pair< Key, Value >& operator*() noexcept;
    std::pair< Key, Value >* operator->() noexcept;
  };

  template < class Key, class Value, class Comparator >
  TTTConstBreadthIterator< Key, Value, Comparator >::TTTConstBreadthIterator():
    node_({nullptr, NodeType::Empty}),
    nexts_()
  {}

  template< class Key, class Value, class Comparator >
  TTTConstBreadthIterator< Key, Value, Comparator >::TTTConstBreadthIterator(Node* node, NodePoint point):
    node_({node, point}),
    nexts_()
  {
    if (point != NodePoint::Fake && node)
    {
      nexts_.push(node_);
      if (node_.first->type == NodeType::Triple)
      {
        nexts_.push({node_.first, NodePoint::Second});
      }
      ++(*this);
    }
  }

  template< class Key, class Value, class Comparator >
  TTTConstBreadthIterator< Key, Value, Comparator >& TTTConstBreadthIterator< Key, Value, Comparator >::operator++()
  {
    assert(node_.first != nullptr && "You are trying to access beyond tree's bounds");
    assert(node_.second != NodePoint::Fake && "You are trying to access beyond tree's bounds");

    node_ = nexts_.front();
    nexts_.pop();
    if (node_.second == NodePoint::Second)
    {
      return (*this);
    }
    Node* node = node_.first;
    if (node->left)
    {
      if (node->left->type == NodeType::Fake)
      {
        nexts_.push({node->left, NodePoint::Fake});
      }
      else
      {
        nexts_.push({node->left, NodePoint::First});
        if (node->left->type == NodeType::Triple)
        {
          nexts_.push({node->left, NodePoint::Second});
        }
      }
    }
    if (node->mid)
    {
      nexts_.push({node->mid, NodePoint::First});
      if (node->mid->type == NodeType::Triple)
      {
        nexts_.push({node->mid, NodePoint::Second});
      }
    }
    if (node->right)
    {
      if (node->right->type == NodeType::Fake)
      {
        nexts_.push({node->right, NodePoint::Fake});
      }
      else
      {
        nexts_.push({node->right, NodePoint::First});
        if (node->right->type == NodeType::Triple)
        {
          nexts_.push({node->right, NodePoint::Second});
        }
      }
    }
    return (*this);
  }

  template< class Key, class Value, class Comparator >
  TTTConstBreadthIterator< Key, Value, Comparator > TTTConstBreadthIterator< Key, Value, Comparator >::operator++(int)
  {
    ConstBreadthIterator old = *this;
    ++(*this);
    return old;
  }

  template< class Key, class Value, class Comparator >
  bool TTTConstBreadthIterator< Key, Value, Comparator >::operator==(const ConstBreadthIterator& other) const noexcept
  {
    if (node_.second == NodePoint::Fake && other.node_.second == NodePoint::Fake)
    {
      return true;
    }
    return (node_ == other.node_);
  }

  template< class Key, class Value, class Comparator >
  bool TTTConstBreadthIterator< Key, Value, Comparator >::operator!=(const ConstBreadthIterator& other) const noexcept
  {
    return !(*this == other);
  }

  template< class Key, class Value, class Comparator >
  const std::pair< Key, Value >& TTTConstBreadthIterator< Key, Value, Comparator >::operator*() const noexcept
  {
    assert(node_.first != nullptr && "You are trying to access beyond tree's bounds");
    assert(node_.second != NodePoint::Fake && "You are trying to access beyond tree's bounds");

    return node_.first->data[node_.second - 1];
  }

  template< class Key, class Value, class Comparator >
  const std::pair< Key, Value >* TTTConstBreadthIterator< Key, Value, Comparator >::operator->() const noexcept
  {
    assert(node_.first != nullptr && "You are trying to access beyond tree's bounds");
    assert(node_.second != NodePoint::Fake && "You are trying to access beyond tree's bounds");

    return std::addressof(node_.first->data[node_.second - 1]);
  }

  template< class Key, class Value, class Comparator >
  TTTBreadthIterator< Key, Value, Comparator >::TTTBreadthIterator(Node* node, NodePoint point):
    Base(node, point)
  {}

  template< class Key, class Value, class Comparator >
  std::pair< Key, Value >& TTTBreadthIterator< Key, Value, Comparator >::operator*() noexcept
  {
    assert(Base::node_.first != nullptr && "You are trying to access beyond tree's bounds");
    assert(Base::node_.second != NodePoint::Fake && "You are trying to access beyond tree's bounds");

    return Base::node_.first->data[Base::node_.second - 1];
  }

  template< class Key, class Value, class Comparator >
  std::pair< Key, Value >* TTTBreadthIterator< Key, Value, Comparator >::operator->() noexcept
  {
    assert(Base::node_.first != nullptr && "You are trying to access beyond tree's bounds");
    assert(Base::node_.second != NodePoint::Fake && "You are trying to access beyond tree's bounds");

    return std::addressof(Base::node_.first->data[Base::node_.second - 1]);
  }
}
#endif
