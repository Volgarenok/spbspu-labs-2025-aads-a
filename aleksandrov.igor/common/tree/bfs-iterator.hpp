#ifndef BFS_ITERATOR_HPP
#define BFS_ITERATOR_HPP

#include "heavy-iterator.hpp"
#include "../queue.hpp"
#include "../../S5/vector.hpp"

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

    Vector< std::pair< Node*, PointsTo > > vector_;
    size_t index_;

    explicit BfsIterator(Node*, bool isEnd = false);

    BfsIterator& shiftForward();
    BfsIterator& shiftBackward();
  };

  template< class K, class V, class C, bool isConst, bool isReversive >
  BfsIterator< K, V, C, isConst, isReversive >::BfsIterator():
    Base(),
    index_(0)
  {}

  template< class K, class V, class C, bool isConst, bool isReversive >
  BfsIterator< K, V, C, isConst, isReversive >::BfsIterator(Node* node, bool isEnd)
  {
    if (!node)
    {
      this->node_ = nullptr;
      this->dir_ = PointsTo::None;
      index_ = 0;
      return;
    }
    Queue< Node* > queue;
    queue.push(node);
    while (!queue.empty())
    {
      Node* curr = queue.front();
      queue.pop();
      vector_.pushBack({ curr, PointsTo::Left });
      if (curr->isTriple())
      {
        vector_.pushBack({ curr, PointsTo::Right });
      }
      if (!curr->isLeaf())
      {
        queue.push(curr->left);
        if (curr->isTriple())
        {
          queue.push(curr->middle);
        }
        queue.push(curr->right);
      }
    }
    if (isReversive)
    {
      index_ = isEnd ? vector_.size() : vector_.size() - 1;
    }
    else
    {
      index_ = isEnd ? vector_.size() : 0;
    }
    if (index_ < vector_.size())
    {
      this->node_ = vector_[index_].first;
      this->dir_ = vector_[index_].second;
    }
    else
    {
      this->node_ = nullptr;
      this->dir_ = PointsTo::None;
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
    if (index_ < vector_.size() - 1)
    {
      ++index_;
      this->node_ = vector_[index_].first;
      this->dir_ = vector_[index_].second;
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
    if (index_ > 0)
    {
      --index_;
      this->node_ = vector_[index_].first;
      this->dir_ = vector_[index_].second;
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

