#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <cassert>
#include <iterator>
#include "node.hpp"

namespace aleksandrov
{
  template< class K, class V, class C >
  class Tree;
  template< class K, class V, class C, bool isConst, bool isReversive >
  class HeavyIterator;
  template< class K, class V, class C, bool isConst, bool isReversive >
  class LnrIterator;
  template< class K, class V, class C, bool isConst, bool isReversive >
  class RnlIterator;
  template< class K, class V, class C, bool isConst, bool isReversive >
  class BfsIterator;

  enum class PointsTo
  {
    Left,
    Right,
    None
  };

  template< class K, class V, class C, bool isConst, bool isReversive >
  class Iterator: public std::iterator< std::bidirectional_iterator_tag, std::pair< K, V > >
  {
  public:
    using ValueType = std::pair< K, V >;
    using NodeType = detail::NodeType;
    using Reference = std::conditional_t< isConst, const ValueType&, ValueType& >;
    using Pointer = std::conditional_t< isConst, const ValueType*, ValueType* >;
    using HeavyIter = HeavyIterator< K, V, C, isConst, isReversive >;
    using LnrIter = LnrIterator< K, V, C, isConst, isReversive >;
    using RnlIter = RnlIterator< K, V, C, isConst, isReversive >;
    using BfsIter = BfsIterator< K, V, C, isConst, isReversive >;

    Iterator();

    LnrIter makeLNR();
    RnlIter makeRNL();
    BfsIter makeBFS();

    Iterator& operator++() noexcept;
    Iterator operator++(int) noexcept;
    Iterator& operator--() noexcept;
    Iterator operator--(int) noexcept;

    Reference operator*() const noexcept;
    Pointer operator->() const noexcept;

    bool operator==(const Iterator&) const;
    bool operator!=(const Iterator&) const;
    bool operator==(const HeavyIter&) const;
    bool operator!=(const HeavyIter&) const;

  private:
    friend class Tree< K, V, C >;
    friend class HeavyIterator< K, V, C, isConst, isReversive >;
    friend class LnrIterator< K, V, C, isConst, isReversive >;
    friend class RnlIterator< K, V, C, isConst, isReversive >;
    friend class BfsIterator< K, V, C, isConst, isReversive >;
    using Node = detail::Node< K, V >;

    Node* node_;
    PointsTo dir_;

    explicit Iterator(Node*);
    Iterator(Node*, PointsTo);

    template< class HeavyIt >
    HeavyIt makeHeavy();
    Iterator fallLeft() noexcept;
    Iterator fallRight() noexcept;
  };

  template< class K, class V, class C, bool isConst, bool isReversive >
  Iterator< K, V, C, isConst, isReversive >::Iterator():
    node_(nullptr),
    dir_(PointsTo::None)
  {}

  template< class K, class V, class C, bool isConst, bool isReversive >
  Iterator< K, V, C, isConst, isReversive >::Iterator(Node* node):
    node_(node),
    dir_(PointsTo::Left)
  {}

  template< class K, class V, class C, bool isConst, bool isReversive >
  Iterator< K, V, C, isConst, isReversive >::Iterator(Node* node, PointsTo dir):
    node_(node),
    dir_(dir)
  {}

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto Iterator< K, V, C, isConst, isReversive >::makeLNR() -> LnrIter
  {
    return makeHeavy< LnrIter >();
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto Iterator< K, V, C, isConst, isReversive >::makeRNL() -> RnlIter
  {
    return makeHeavy< RnlIter >();
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto Iterator< K, V, C, isConst, isReversive >::makeBFS() -> BfsIter
  {
    return makeHeavy< BfsIter >();
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto Iterator< K, V, C, isConst, isReversive >::operator++() noexcept -> Iterator&
  {
    assert(node_ != nullptr && "Cannot iterate from null-node!");
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

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto Iterator< K, V, C, isConst, isReversive >::operator++(int) noexcept -> Iterator
  {
    assert(node_ != nullptr && "Cannot iterate from null-node!");
    auto result(*this);
    ++(*this);
    return result;
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto Iterator< K, V, C, isConst, isReversive >::operator--() noexcept -> Iterator&
  {
    assert(node_ != nullptr && "Cannot iterate from null-node!");
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

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto Iterator< K, V, C, isConst, isReversive >::operator--(int) noexcept -> Iterator
  {
    assert(node_ != nullptr && "Cannot iterate from null-node!");
    auto result(*this);
    --(*this);
    return result;
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto Iterator< K, V, C, isConst, isReversive >::operator*() const noexcept -> Reference
  {
    assert(dir_ != PointsTo::None && "Cannot access to null-node!");
    return dir_ == PointsTo::Left ? node_->data[0] : node_->data[1];
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto Iterator< K, V, C, isConst, isReversive >::operator->() const noexcept -> Pointer
  {
    assert(dir_ != PointsTo::None && "Cannot access to null-node!");
    return std::addressof(dir_ == PointsTo::Left ? node_->data[0] : node_->data[1]);
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  bool Iterator< K, V, C, isConst, isReversive >::operator==(const Iterator& rhs) const
  {
    return node_ == rhs.node_ && dir_ == rhs.dir_;
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  bool Iterator< K, V, C, isConst, isReversive >::operator!=(const Iterator& rhs) const
  {
    return !(*this == rhs);
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  bool Iterator< K, V, C, isConst, isReversive >::operator==(const HeavyIter& rhs) const
  {
    return node_ == rhs.node_ && dir_ == rhs.dir_;
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  bool Iterator< K, V, C, isConst, isReversive >::operator!=(const HeavyIter& rhs) const
  {
    return !(*this == rhs);
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  template< class HeavyIt >
  auto Iterator< K, V, C, isConst, isReversive >::makeHeavy() -> HeavyIt
  {
    Node* root = this->node_;
    while (root && root->parent)
    {
      root = root->parent;
    }
    HeavyIt heavy(root);
    while (heavy.node_ != this->node_ || heavy.dir_ != this->dir_)
    {
      ++heavy;
    }
    return heavy;
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto Iterator< K, V, C, isConst, isReversive >::fallLeft() noexcept -> Iterator
  {
    assert(node_ != nullptr && "Cannot iterate from null-node!");
    while (node_->left)
    {
      node_ = node_->left;
    }
    return *this;
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto Iterator< K, V, C, isConst, isReversive >::fallRight() noexcept -> Iterator
  {
    assert(node_ != nullptr && "Cannot iterate from null-node!");
    while (node_->right)
    {
      node_ = node_->right;
    }
    return *this;
  }
}

#endif

