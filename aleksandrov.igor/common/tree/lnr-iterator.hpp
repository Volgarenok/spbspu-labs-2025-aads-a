#ifndef LNR_ITERATOR_HPP
#define LNR_ITERATOR_HPP

#include "heavy-iterator.hpp"
#include "../stack.hpp"

namespace aleksandrov
{
  template< class K, class V, class C, bool isConst >
  class LnrIterator final: public HeavyIterator< K, V, C, isConst >
  {
  public:
    LnrIterator();

    LnrIterator& operator++();
    LnrIterator& operator++(int);

  private:
    friend class Tree< K, V, C >;
    using Node = detail::Node< K, V >;
    using Base = HeavyIterator< K, V, C, isConst >;

    Stack< std::pair< Node*, PointsTo > > stack_;
    Node* curr_;

    explicit LnrIterator(Node*);
  };

  template< class K, class V, class C, bool isConst >
  LnrIterator< K, V, C, isConst >::LnrIterator():
    curr_(nullptr)
  {}

  template< class K, class V, class C, bool isConst >
  LnrIterator< K, V, C, isConst >::LnrIterator(Node* node):
    Base(node),
    curr_(node)
  {}

  template< class K, class V, class C, bool isConst >
  auto LnrIterator< K, V, C, isConst >::operator++() -> LnrIterator&
  {
    assert(this->node_ != nullptr && "ERROR: Trying to iterate from null-node!");

    while (curr_)
    {
      stack_.push({ curr_, PointsTo::Left });
      curr_ = curr_->left;
    }

    if (stack_.empty())
    {
      this->node_ = nullptr;
      this->dir_ = PointsTo::None;
      return *this;
    }
    auto node = stack_.top().first;
    auto dir = stack_.top().second;

    if (dir == PointsTo::Left)
    {
      this->node_ = node;
      this->dir_ = PointsTo::Left;
      if (node->isTriple())
      {
        stack_.top().second = PointsTo::Right;
        curr_ = node->middle;
      }
      else
      {
        stack_.pop();
        curr_ = node->right;
      }
    }
    else if (dir == PointsTo::Right)
    {
      this->node_ = node;
      this->dir_ = PointsTo::Right;
      stack_.pop();
      curr_ = node->right;
    }
    return *this;
  }

  template< class K, class V, class C, bool isConst >
  auto LnrIterator< K, V, C, isConst >::operator++(int) -> LnrIterator&
  {
    assert(this->node_ != nullptr && "ERROR: Trying to iterate from null-node!");
    auto result(*this);
    ++(*this);
    return result;
  }
}

#endif

