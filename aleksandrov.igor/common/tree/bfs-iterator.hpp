#ifndef BFS_ITERATOR_HPP
#define BFS_ITERATOR_HPP

#include "heavy-iterator.hpp"
#include "../../S5/deque.hpp"

namespace aleksandrov
{
  template< class K, class V, class C, bool isConst, bool isReversive >
  class BfsIterator final: public HeavyIterator< K, V, C, isConst, isReversive >
  {
  public:
    using LiteIter = Iterator< K, V, C, isConst, isReversive >;

    BfsIterator();

    LiteIter makeLite() noexcept;

    BfsIterator& operator++();
    BfsIterator operator++(int);
    BfsIterator& operator--();
    BfsIterator operator--(int);

  private:
    friend class Tree< K, V, C >;
    friend class Iterator< K, V, C, isConst, isReversive >;
    using Node = detail::Node< K, V >;
    using Base = HeavyIterator< K, V, C, isConst, isReversive >;

    Deque< Node* > deque_;

    explicit BfsIterator(Node*);

    BfsIterator& shiftForward();
    BfsIterator& shiftBackward();
  };

  template< class K, class V, class C, bool isConst, bool isReversive >
  BfsIterator< K, V, C, isConst, isReversive >::BfsIterator():
    Base()
  {}

  template< class K, class V, class C, bool isConst, bool isReversive >
  BfsIterator< K, V, C, isConst, isReversive >::BfsIterator(Node* node):
    Base(node)
  {
    if (node && isReversive)
    {
      Deque< Node* > deque;
      deque.pushBack(node);
      while (!deque.empty())
      {
        Node* curr = deque.front();
        deque.popFront();
        deque_.pushBack(curr);
        if (!curr->isLeaf())
        {
          deque.pushBack(curr->left);
          if (curr->isTriple())
          {
            deque.pushBack(curr->middle);
          }
          deque.pushBack(curr->right);
        }
      }
    }
    if (!deque_.empty())
    {
      if (isReversive)
      {
        this->node_ = deque_.back();
        deque_.popBack();
        if (this->node_->isTriple())
        {
          this->dir_ = PointsTo::Right;
        }
        else
        {
          this->dir_ = PointsTo::Left;
        }
      }
      else
      {
        this->node_ = deque_.front();
        deque_.popFront();
        this->dir_ = PointsTo::Left;
      }
    }
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto BfsIterator< K, V, C, isConst, isReversive >::makeLite() noexcept -> LiteIter
  {
    return LiteIter(this->node_, this->dir_);
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto BfsIterator< K, V, C, isConst, isReversive >::operator++() -> BfsIterator&
  {
    assert(this->node_ != nullptr && "ERROR: Trying to iterate from null-node!");
    return isReversive ? shiftBackward() : shiftForward();
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto BfsIterator< K, V, C, isConst, isReversive >::operator++(int) -> BfsIterator
  {
    assert(this->node_ != nullptr && "ERROR: Trying to iterate from null-node!");
    auto result(*this);
    ++(*this);
    return result;
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto BfsIterator< K, V, C, isConst, isReversive >::operator--() -> BfsIterator&
  {
    assert(this->node_ != nullptr && "ERROR: Trying to iterate from null-node!");
    return isReversive ? shiftForward() : shiftBackward();
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto BfsIterator< K, V, C, isConst, isReversive >::operator--(int) -> BfsIterator
  {
    assert(this->node_ != nullptr && "ERROR: Trying to iterate from null-node!");
    auto result(*this);
    --(*this);
    return result;
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto BfsIterator< K, V, C, isConst, isReversive >::shiftForward() -> BfsIterator&
  {
    if (this->node_->isTriple() && this->dir_ == PointsTo::Left)
    {
      this->dir_ = PointsTo::Right;
      return *this;
    }
    if (!this->node_->isLeaf())
    {
      deque_.pushBack(this->node_->left);
      if (this->node_->isTriple())
      {
        deque_.pushBack(this->node_->middle);
      }
      deque_.pushBack(this->node_->right);
    }
    if (!deque_.empty())
    {
      this->node_ = deque_.front();
      deque_.popFront();
      this->dir_ = PointsTo::Left;
    }
    else
    {
      this->node_ = nullptr;
      this->dir_ = PointsTo::None;
    }
    return *this;
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto BfsIterator< K, V, C, isConst, isReversive >::shiftBackward() -> BfsIterator&
  {
    if (this->node_->isTriple() && this->dir_ == PointsTo::Right)
    {
      this->dir_ = PointsTo::Left;
      return *this;
    }
    if (!deque_.empty())
    {
      this->node_ = deque_.back();
      deque_.popBack();
      if (this->node_->isTriple())
      {
        this->dir_ = PointsTo::Right;
      }
      else
      {
        this->dir_ = PointsTo::Left;
      }
    }
    else
    {
      this->node_ = nullptr;
      this->dir_ = PointsTo::None;
    }
    return *this;
  }
}

#endif

