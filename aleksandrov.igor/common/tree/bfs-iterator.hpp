#ifndef BFS_ITERATOR_HPP
#define BFS_ITERATOR_HPP

#include "heavy-iterator.hpp"
#include "../queue.hpp"

namespace aleksandrov
{
  template< class K, class V, class C, bool isConst >
  class BfsIterator final: public HeavyIterator< K, V, C, isConst >
  {
  public:
    BfsIterator();

    BfsIterator& operator++();
    BfsIterator& operator++(int);

  private:
    friend class Tree< K, V, C >;
    using Node = detail::Node< K, V >;
    using Base = HeavyIterator< K, V, C, isConst >;

    Queue< std::pair< Node*, PointsTo > > queue_;
    Node* curr_;

    explicit BfsIterator(Node*);
  };

  template< class K, class V, class C, bool isConst >
  BfsIterator< K, V, C, isConst >::BfsIterator():
    curr_(nullptr)
  {}

  template< class K, class V, class C, bool isConst >
  BfsIterator< K, V, C, isConst >::BfsIterator(Node* node):
    Base(node),
    curr_(node)
  {}

  template< class K, class V, class C, bool isConst >
  auto BfsIterator< K, V, C, isConst >::operator++() -> BfsIterator&
  {
    assert(this->node_ != nullptr && "ERROR: Trying to iterate from null-node!");

    Node* curr = this->node_;
    if (curr->isTriple() && this->dir_ == PointsTo::Left)
    {
      this->dir_ = PointsTo::Right;
      return *this;
    }
    if (curr->left)
    {
      queue_.push({ curr->left, PointsTo::Left });
    }
    if (curr->isTriple() && curr->middle)
    {
      queue_.push({ curr->middle, PointsTo::Left });
    }
    if (curr->right)
    {
      queue_.push({ curr->right, PointsTo::Left });
    }

    while (!queue_.empty())
    {
      auto node = queue_.front().first;
      auto dir = queue_.front().second;
      queue_.pop();
      if (node)
      {
        this->node_ = node;
        this->dir_ = dir;
        return *this;
      }
    }
    this->node_ = nullptr;
    this->dir_ = PointsTo::None;
    return *this;
  }

  template< class K, class V, class C, bool isConst >
  auto BfsIterator< K, V, C, isConst >::operator++(int) -> BfsIterator&
  {
    assert(this->node_ != nullptr && "ERROR: Trying to iterate from null-node!");
    auto result(*this);
    ++(*this);
    return result;
  }
}

#endif

