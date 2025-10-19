#ifndef RNL_ITERATOR_HPP
#define RNL_ITERATOR_HPP

#include <stack.hpp>
#include "heavy-iterator.hpp"

namespace aleksandrov
{
  template< class K, class V, class C, bool isConst, bool isReversive >
  class RnlIterator final: public HeavyIterator< K, V, C, isConst, isReversive >
  {
  public:
    using LiteIter = Iterator< K, V, C, isConst, isReversive >;

    RnlIterator();

    LiteIter makeLite() const noexcept;

    RnlIterator& operator++();
    RnlIterator operator++(int);
    RnlIterator& operator--();
    RnlIterator operator--(int);

  private:
    friend class Tree< K, V, C >;
    friend class Iterator< K, V, C, isConst, isReversive >;
    using Node = detail::Node< K, V >;
    using Base = HeavyIterator< K, V, C, isConst, isReversive >;

    Stack< std::pair< Node*, PointsTo > > stack_;

    explicit RnlIterator(Node*);

    RnlIterator& shiftForward();
    RnlIterator& shiftBackward();
  };

  template< class K, class V, class C, bool isConst, bool isReversive >
  RnlIterator< K, V, C, isConst, isReversive >::RnlIterator():
    Base()
  {}

  template< class K, class V, class C, bool isConst, bool isReversive >
  RnlIterator< K, V, C, isConst, isReversive >::RnlIterator(Node* node):
    Base(node)
  {
    if (!node)
    {
      this->node_ = nullptr;
      this->dir_ = PointsTo::None;
      return;
    }
    Node* curr = node;
    if (isReversive)
    {
      while (curr)
      {
        stack_.emplace(curr, PointsTo::Left);
        curr = curr->left;
      }
    }
    else
    {
      while (curr)
      {
        if (curr->isTriple())
        {
          stack_.emplace(curr, PointsTo::Right);
        }
        else
        {
          stack_.emplace(curr, PointsTo::Left);
        }
        curr = curr->right;
      }
    }
    if (!stack_.empty())
    {
      curr = stack_.top().first;
      if (isReversive)
      {
        this->dir_ = PointsTo::Left;
      }
      else if (curr)
      {
        if (curr->isTriple())
        {
          this->dir_ = PointsTo::Right;
        }
        else
        {
          this->dir_ = PointsTo::Left;
        }
      }
      this->node_ = curr;
      stack_.pop();
    }
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto RnlIterator< K, V, C, isConst, isReversive >::makeLite() const noexcept -> LiteIter
  {
    return LiteIter(this->node_, this->dir_);
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto RnlIterator< K, V, C, isConst, isReversive >::operator++() -> RnlIterator&
  {
    assert(this->node_ != nullptr && "Trying to iterate from null-node!");
    return isReversive ? shiftBackward() : shiftForward();
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto RnlIterator< K, V, C, isConst, isReversive >::operator++(int) -> RnlIterator
  {
    assert(this->node_ != nullptr && "Trying to iterate from null-node!");
    auto result(*this);
    ++(*this);
    return result;
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto RnlIterator< K, V, C, isConst, isReversive >::operator--() -> RnlIterator&
  {
    assert(this->node_ != nullptr && "Trying to iterate from null-node!");
    return isReversive ? shiftForward() : shiftBackward();
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto RnlIterator< K, V, C, isConst, isReversive >::operator--(int) -> RnlIterator
  {
    assert(this->node_ != nullptr && "Trying to iterate from null-node!");
    auto result(*this);
    --(*this);
    return result;
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto RnlIterator< K, V, C, isConst, isReversive >::shiftForward() -> RnlIterator&
  {
    if (this->node_->isTriple() && this->dir_ == PointsTo::Right)
    {
      if (this->node_->isLeaf())
      {
        this->dir_ = PointsTo::Left;
      }
      else
      {
        if (this->node_->isTriple())
        {
          stack_.emplace(this->node_, PointsTo::Right);
        }
        else
        {
          stack_.emplace(this->node_, PointsTo::Left);
        }
        this->node_ = this->node_->middle;
        this->dir_ = this->node_->isTriple() ? PointsTo::Right : PointsTo::Left;
        while (this->node_->right)
        {
          if (this->node_->isTriple())
          {
            stack_.emplace(this->node_, PointsTo::Right);
          }
          else
          {
            stack_.emplace(this->node_, PointsTo::Left);
          }
          this->node_ = this->node_->right;
          this->dir_ = this->node_->isTriple() ? PointsTo::Right : PointsTo::Left;
        }
      }
    }
    else if (this->node_->left)
    {
      stack_.emplace(this->node_, PointsTo::Left);
      this->node_ = this->node_->left;
      this->dir_ = this->node_->isTriple() ? PointsTo::Right : PointsTo::Left;
      while (this->node_->right)
      {
        if (this->node_->isTriple())
        {
          stack_.emplace(this->node_, PointsTo::Right);
        }
        else
        {
          stack_.emplace(this->node_, PointsTo::Left);
        }
        this->node_ = this->node_->right;
        this->dir_ = this->node_->isTriple() ? PointsTo::Right : PointsTo::Left;
      }
    }
    else
    {
      while (!stack_.empty() && stack_.top().first->left == this->node_)
      {
        this->node_ = stack_.top().first;
        this->dir_ = PointsTo::Left;
        stack_.pop();
      }
      if (!stack_.empty())
      {
        bool isPointsToRight = stack_.top().second == PointsTo::Right;
        if (stack_.top().first->right == this->node_ && isPointsToRight)
        {
          this->dir_ = PointsTo::Right;
        }
        else
        {
          this->dir_ = PointsTo::Left;
        }
        this->node_ = stack_.top().first;
        stack_.pop();
      }
      else
      {
        this->node_ = nullptr;
        this->dir_ = PointsTo::None;
      }
    }
    return *this;
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto RnlIterator< K, V, C, isConst, isReversive >::shiftBackward() -> RnlIterator&
  {
    if (this->node_->isTriple() && this->dir_ == PointsTo::Left)
    {
      if (this->node_->isLeaf())
      {
        this->dir_ = PointsTo::Right;
      }
      else
      {
        stack_.emplace(this->node_, PointsTo::Left);
        this->node_ = this->node_->middle;
        while (this->node_->left)
        {
          stack_.emplace(this->node_, PointsTo::Left);
          this->node_ = this->node_->left;
        }
      }
    }
    else if (this->node_->right)
    {
      stack_.emplace(this->node_, this->dir_);
      this->node_ = this->node_->right;
      while (this->node_->left)
      {
        stack_.emplace(this->node_, PointsTo::Left);
        this->node_ = this->node_->left;
        this->dir_ = PointsTo::Left;
      }
    }
    else
    {
      while (!stack_.empty() && stack_.top().first->right == this->node_)
      {
        this->node_ = stack_.top().first;
        this->dir_ = this->node_->isTriple() ? PointsTo::Right : PointsTo::Left;
        stack_.pop();
      }
      bool isNodeDone = this->node_->isDouble() || this->dir_ == PointsTo::Right;
      if (!stack_.empty() && isNodeDone)
      {
        if (stack_.top().first->left == this->node_)
        {
          this->dir_ = PointsTo::Left;
        }
        else
        {
          this->dir_ = PointsTo::Right;
        }
        this->node_ = stack_.top().first;
        stack_.pop();
      }
      else
      {
        this->node_ = nullptr;
        this->dir_ = PointsTo::None;
      }
    }
    return *this;
  }
}

#endif

