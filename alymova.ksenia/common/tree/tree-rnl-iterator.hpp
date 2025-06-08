#ifndef TREE_RNL_ITERATORS_HPP
#define TREE_RNL_ITERATORS_HPP
#include <iterator>
#include <stack.hpp>
#include "tree-node.hpp"

#include <iostream>

namespace alymova
{
  template< class Key, class Value, class Comparator >
  struct TTTConstRnlIterator:
    public std::iterator< std::forward_iterator_tag, std::pair< Key, Value > >
  {
  public:
    using ConstRnlIterator = TTTConstRnlIterator< Key, Value, Comparator >;
    using Node = typename detail::TTTNode< Key, Value, Comparator >;
    using NodeType = typename Node::NodeType;
    using NodePoint = typename detail::NodePoint;

    TTTConstRnlIterator();
    ConstRnlIterator& operator++();
    ConstRnlIterator operator++(int);
    bool operator==(const ConstRnlIterator& other) const noexcept;
    bool operator!=(const ConstRnlIterator& other) const noexcept;
    const std::pair< Key, Value >& operator*() const noexcept;
    const std::pair< Key, Value >* operator->() const noexcept;
  protected:
    Node const* root_;
    std::pair< Node*, NodePoint > node_;
    std::pair< Node*, NodePoint > tmp_;
    Stack< std::pair< Node*, NodePoint > > nexts_;

    TTTConstRnlIterator(Node* root, NodePoint point);

    friend class TwoThreeTree< Key, Value, Comparator >;
  };

  template< class Key, class Value, class Comparator >
  struct TTTRnlIterator final:
    public TTTConstRnlIterator< Key, Value, Comparator >
  {
    using Base = TTTConstRnlIterator< Key, Value, Comparator >;
    using Node = typename detail::TTTNode< Key, Value, Comparator >;
    using NodePoint = typename detail::NodePoint;

    TTTRnlIterator(Node* root, NodePoint);

    std::pair< Key, Value >& operator*() noexcept;
    std::pair< Key, Value >* operator->() noexcept;
  };

  template< class Key, class Value, class Comparator >
  TTTConstRnlIterator< Key, Value, Comparator >::TTTConstRnlIterator():
    root_(nullptr),
    node_({nullptr, NodePoint::Empty}),
    tmp_({nullptr, NodePoint::Empty}),
    nexts_()
  {}

  template< class Key, class Value, class Comparator >
  TTTConstRnlIterator< Key, Value, Comparator >::TTTConstRnlIterator(Node* root, NodePoint point):
    root_(root),
    node_({root, point}),
    tmp_({root, point}),
    nexts_()
  {
    if (point != NodePoint::Fake && root)
    {
      while (tmp_.first)
      {
        if (tmp_.first->type == NodeType::Fake)
        {
          tmp_.first = nullptr;
          break;
        }
        else if (tmp_.first->type == NodeType::Double)
        {
          tmp_.second = NodePoint::First;
        }
        else
        {
          tmp_.second = NodePoint::Second;
        }
        nexts_.push(tmp_);
        tmp_.first = tmp_.first->right;
      }
      ++(*this);
    }
  }

  template< class Key, class Value, class Comparator >
  TTTConstRnlIterator< Key, Value, Comparator >& TTTConstRnlIterator< Key, Value, Comparator >::operator++()
  {
    assert(node_.first != nullptr && "You are trying to access beyond tree's bounds");
    assert(node_.second != NodePoint::Fake && "You are trying to access beyond tree's bounds");

    while (tmp_.first)
    {
      if (tmp_.first->type == NodeType::Double)
      {
        tmp_.second = NodePoint::First;
      }
      else if (tmp_.first->type == NodeType::Triple)
      {
        tmp_.second = NodePoint::Second;
      }
      nexts_.push(tmp_);
      tmp_.first = tmp_.first->right;
    }
    node_ = nexts_.top();
    nexts_.pop();
    if (node_.first->type == NodeType::Triple && node_.second == NodePoint::Second)
    {
      nexts_.push({node_.first, NodePoint::First});
      tmp_.first = node_.first->mid;
      return *this;
    }
    tmp_.first = node_.first->left;
    if (tmp_.first)
    {
      if (tmp_.first->type == NodeType::Fake)
      {
        tmp_.second = NodePoint::Fake;
      }
    }
    return *this;
  }

  template< class Key, class Value, class Comparator >
  TTTConstRnlIterator< Key, Value, Comparator > TTTConstRnlIterator< Key, Value, Comparator >::operator++(int)
  {
    ConstRnlIterator old = *this;
    ++(*this);
    return old;
  }

  template< class Key, class Value, class Comparator >
  bool TTTConstRnlIterator< Key, Value, Comparator >::operator==(const ConstRnlIterator& other) const noexcept
  {
    if (node_.second == NodePoint::Fake && other.node_.second == NodePoint::Fake)
    {
      return true;
    }
    return (node_ == other.node_);
  }

  template< class Key, class Value, class Comparator >
  bool TTTConstRnlIterator< Key, Value, Comparator >::operator!=(const ConstRnlIterator& other) const noexcept
  {
    return !(*this == other);
  }

  template< class Key, class Value, class Comparator >
  const std::pair< Key, Value >& TTTConstRnlIterator< Key, Value, Comparator >::operator*() const noexcept
  {
    assert(node_.first != nullptr && "You are trying to access beyond tree's bounds");
    assert(node_.second != NodePoint::Fake && "You are trying to access beyond tree's bounds");

    return node_.first->data[node_.second - 1];
  }

  template< class Key, class Value, class Comparator >
  const std::pair< Key, Value >* TTTConstRnlIterator< Key, Value, Comparator >::operator->() const noexcept
  {
    assert(node_.first != nullptr && "You are trying to access beyond tree's bounds");
    assert(node_.second != NodePoint::Fake && "You are trying to access beyond tree's bounds");

    return std::addressof(node_.first->data[node_.second - 1]);
  }

  template< class Key, class Value, class Comparator >
  TTTRnlIterator< Key, Value, Comparator >::TTTRnlIterator(Node* node, NodePoint point):
    Base(node, point)
  {}

  template< class Key, class Value, class Comparator >
  std::pair< Key, Value >& TTTRnlIterator< Key, Value, Comparator >::operator*() noexcept
  {
    assert(Base::node_.first != nullptr && "You are trying to access beyond tree's bounds");
    assert(Base::node_.second != NodePoint::Fake && "You are trying to access beyond tree's bounds");

    return Base::node_.first->data[Base::node_.second - 1];
  }

  template< class Key, class Value, class Comparator >
  std::pair< Key, Value >* TTTRnlIterator< Key, Value, Comparator >::operator->() noexcept
  {
    assert(Base::node_.first != nullptr && "You are trying to access beyond tree's bounds");
    assert(Base::node_.second != NodePoint::Fake && "You are trying to access beyond tree's bounds");

    return std::addressof(Base::node_.first->data[Base::node_.second - 1]);
  }
}

#endif
