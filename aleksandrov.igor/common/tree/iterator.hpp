#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include <cassert>
#include "node.hpp"

namespace aleksandrov
{
  template< class K, class V, class C >
  class Tree;

  enum class PointsTo
  {
    Left,
    Right,
    None
  };

  template< class K, class V, class C, bool isConst >
  class Iterator: public std::iterator< std::bidirectional_iterator_tag, std::pair< K, V > >
  {
  public:
    using ValueType = std::pair< K, V >;
    using NodeType = detail::NodeType;
    using Reference = std::conditional_t< isConst, const ValueType&, ValueType& >;
    using Pointer = std::conditional_t< isConst, const ValueType*, ValueType* >;

    Iterator();

    Iterator& operator++() noexcept;
    Iterator operator++(int) noexcept;
    Iterator& operator--() noexcept;
    Iterator operator--(int) noexcept;

    Reference operator*() const noexcept;
    Pointer operator->() const noexcept;

    bool operator==(const Iterator&) const noexcept;
    bool operator!=(const Iterator&) const noexcept;

  private:
    friend class Tree< K, V, C >;
    using Node = detail::Node< K, V >;

    Node* node_;
    PointsTo dir_;

    explicit Iterator(Node*);
    explicit Iterator(Node*, PointsTo);

    Iterator fallLeft() noexcept;
    Iterator fallRight() noexcept;
  };

  template< class K, class V, class C, bool isConst >
  Iterator< K, V, C, isConst >::Iterator():
    node_(nullptr),
    dir_(PointsTo::None)
  {}

  template< class K, class V, class C, bool isConst >
  Iterator< K, V, C, isConst >::Iterator(Node* node):
    node_(node),
    dir_(PointsTo::Left)
  {}

  template< class K, class V, class C, bool isConst >
  Iterator< K, V, C, isConst >::Iterator(Node* node, PointsTo dir):
    node_(node),
    dir_(dir)
  {}

  template< class K, class V, class C, bool isConst >
  Iterator< K, V, C, isConst >& Iterator< K, V, C, isConst >::operator++() noexcept
  {
    assert(node_ != nullptr && "ERROR: Trying to iterate from null-node!");

    if (node_->isTriple())
    {
      if (dir_ == PointsTo::Left && node_->isLeaf())
      {
        dir_ = PointsTo::Right;
        return *this;
      }
      if (dir_ == PointsTo::Left)
      {
        node_ = node_->middle;
        fallLeft();
        return *this;
      }
      if (dir_ == PointsTo::Right && !node_->isLeaf())
      {
        node_ = node_->right;
        dir_ = PointsTo::Left;
        fallLeft();
        return *this;
      }
    }
    else if (node_->isDouble() && node_->right)
    {
      node_ = node_->right;
      fallLeft();
      return *this;
    }

    while (node_->parent)
    {
      if (node_->parent->left == node_)
      {
        node_ = node_->parent;
        dir_ = PointsTo::Left;
        return *this;
      }
      if (node_->parent->isTriple() && node_->parent->middle == node_)
      {
        node_ = node_->parent;
        dir_ = PointsTo::Right;
        return *this;
      }
      node_ = node_->parent;
    }
    node_ = nullptr;
    dir_ = PointsTo::None;
    return *this;
  }

  template< class K, class V, class C, bool isConst >
  Iterator< K, V, C, isConst > Iterator< K, V, C, isConst >::operator++(int) noexcept
  {
    assert(node_ != nullptr && "ERROR: Trying to iterate from null-node!");
    auto result(*this);
    ++(*this);
    return result;
  }

  template< class K, class V, class C, bool isConst >
  Iterator< K, V, C, isConst >& Iterator< K, V, C, isConst >::operator--() noexcept
  {
    assert(node_ != nullptr && "ERROR: Trying to iterate from null-node!");

    if (node_->isTriple())
    {
      if (dir_ == PointsTo::Right && node_->isLeaf())
      {
        dir_ = PointsTo::Left;
        return *this;
      }
      if (dir_ == PointsTo::Right)
      {
        node_ = node_->middle;
        fallRight();
        if (node_->isDouble())
        {
          dir_ = PointsTo::Left;
        }
        return *this;
      }
      if (dir_ == PointsTo::Left && !node_->isLeaf())
      {
        node_ = node_->left;
        fallRight();
        if (node_->isTriple())
        {
          dir_ = PointsTo::Right;
        }
        return *this;
      }
    }
    else if (node_->isDouble() && node_->left)
    {
      node_ = node_->left;
      fallRight();
      if (node_->isTriple())
      {
        dir_ = PointsTo::Right;
      }
      return *this;
    }

    while (node_->parent)
    {
      if (node_->parent->right == node_)
      {
        node_ = node_->parent;
        if (node_->parent->isTriple())
        {
          dir_ = PointsTo::Right;
        }
        return *this;
      }
      if (node_->parent->isTriple() && node_->parent->middle == node_)
      {
        node_ = node_->parent;
        dir_ = PointsTo::Left;
        return *this;
      }
      node_ = node_->parent;
    }
    node_ = node_->parent;
    dir_ = PointsTo::None;
    return *this;
  }

  template< class K, class V, class C, bool isConst >
  Iterator< K, V, C, isConst > Iterator< K, V, C, isConst >::operator--(int) noexcept
  {
    assert(node_ != nullptr && "ERROR: Trying to iterate from null-node!");
    auto result(*this);
    --(*this);
    return result;
  }

  template< class K, class V, class C, bool isConst >
  typename Iterator< K, V, C, isConst >::Reference Iterator< K, V, C, isConst >::operator*() const noexcept
  {
    assert(dir_ != PointsTo::None && "ERROR: Access to null-node!");
    return dir_ == PointsTo::Left ? node_->data[0] : node_->data[1];
  }

  template< class K, class V, class C, bool isConst >
  typename Iterator< K, V, C, isConst >::Pointer Iterator< K, V, C, isConst >::operator->() const noexcept
  {
    assert(dir_ != PointsTo::None && "ERROR: Access to null-node!");
    return std::addressof(dir_ == PointsTo::Left ? node_->data[0] : node_->data[1]);
  }

  template< class K, class V, class C, bool isConst >
  bool Iterator< K, V, C, isConst >::operator==(const Iterator& rhs) const noexcept
  {
    return node_ == rhs.node_ && dir_ == rhs.dir_;
  }

  template< class K, class V, class C, bool isConst >
  bool Iterator< K, V, C, isConst >::operator!=(const Iterator& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< class K, class V, class C, bool isConst >
  Iterator< K, V, C, isConst > Iterator< K, V, C, isConst >::fallLeft() noexcept
  {
    assert(node_ != nullptr && "ERROR: Trying to iterate from null-node!");
    while (node_->left)
    {
      node_ = node_->left;
    }
    return *this;
  }

  template< class K, class V, class C, bool isConst >
  Iterator< K, V, C, isConst > Iterator< K, V, C, isConst >::fallRight() noexcept
  {
    assert(node_ != nullptr && "ERROR: Trying to iterate from null-node!");
    while (node_->right)
    {
      node_ = node_->right;
    }
    return *this;
  }
}

#endif

