#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP
#include <utility>
#include "tree-iterators.hpp"

namespace alymova
{
  namespace detail
  {
    enum NodePoint: int {Empty, First, Second, Fake};

    template< class Key, class Value, class Comparator >
    struct TTTNode
    {
      using Node = TTTNode< Key, Value, Comparator >;

      std::pair< Key, Value > data[3];
      enum NodeType {Empty, Double, Triple, Overflow, Fake} type;
      Node* parent;
      Node* left;
      Node* mid;
      Node* right;
      Node* overflow;

      void insert(const std::pair< Key, Value >& value);
      void remove(NodePoint point) noexcept;
      bool isLeaf() const noexcept;
      void clear()noexcept;
    };

    template< class Key, class Value, class Comparator >
    void TTTNode< Key, Value, Comparator >::insert(const std::pair< Key, Value >& value)
    {
      Comparator cmp;
      data[type] = value;
      if (type == NodeType::Empty)
      {
        type = NodeType::Double;
        return;
      }
      if (type == NodeType::Double)
      {
        if (cmp(data[1].first, data[0].first))
        {
          std::swap(data[0], data[1]);
        }
        type = NodeType::Triple;
        return;
      }
      if (cmp(data[2].first, data[1].first))
      {
        std::swap(data[2], data[1]);
      }
      if (cmp(data[1].first, data[0].first))
      {
        std::swap(data[1], data[0]);
      }
      type = NodeType::Overflow;
    }

    template< class Key, class Value, class Comparator >
    void TTTNode< Key, Value, Comparator >::remove(NodePoint point) noexcept
    {
      assert(type != NodeType::Empty && "Removing from empty node");
      if (type == NodeType::Double)
      {
        type = NodeType::Empty;
        return;
      }
      if (point == NodePoint::First)
      {
        std::swap(data[0], data[1]);
      }
      type = NodeType::Double;
      return;
    }

    template< class Key, class Value, class Comparator >
    bool TTTNode< Key, Value, Comparator >::isLeaf() const noexcept
    {
      if (right)
      {
        if (right->type != NodeType::Fake)
        {
          return false;
        }
      }
      if (left)
      {
        if (left->type != NodeType::Fake)
        {
          return false;
        }
      }
      return (!mid && !overflow);
    }

    template< class Key, class Value, class Comparator >
    void TTTNode< Key, Value, Comparator >::clear() noexcept
    {
      parent = nullptr;
      left = nullptr;
      mid = nullptr;
      right = nullptr;
      overflow = nullptr;
    }
  }
}

#endif
