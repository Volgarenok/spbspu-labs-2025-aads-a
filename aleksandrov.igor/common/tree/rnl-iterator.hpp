#ifndef RNL_ITERATOR_HPP
#define RNL_ITERATOR_HPP

#include "heavy-iterator.hpp"
#include "../stack.hpp"

namespace aleksandrov
{
  template< class K, class V, class C, bool isConst >
  class RnlIterator final: public HeavyIterator< K, V, C, isConst >
  {
  public:
    RnlIterator();

    RnlIterator& operator++();
    RnlIterator& operator++(int);

  private:
    friend class Tree< K, V, C >;
    using Node = detail::Node< K, V >;
    using Base = HeavyIterator< K, V, C, isConst >;

    Stack< std::pair< Node*, PointsTo > > stack_;
    Node* curr_;

    explicit RnlIterator(Node*);
  };

  template< class K, class V, class C, bool isConst >
  RnlIterator< K, V, C, isConst >::RnlIterator():
    curr_(nullptr)
  {}

  template< class K, class V, class C, bool isConst >
  RnlIterator< K, V, C, isConst >::RnlIterator(Node* node):
    Base(node),
    curr_(node)
  {}

  template< class K, class V, class C, bool isConst >
  auto RnlIterator< K, V, C, isConst >::operator++() -> RnlIterator&
  {
    assert(this->node_ != nullptr && "ERROR: Trying to iterate from null-node!");

    while (curr_)
    {
      stack_.push({ curr_, curr_->isTriple() ? PointsTo::Right : PointsTo::Left });
      curr_ = curr_->right;
    }

    if (stack_.empty())
    {
      this->node_ = nullptr;
      this->dir_ = PointsTo::None;
      return *this;
    }
    auto node = stack_.top().first;
    auto dir = stack_.top().second;

    this->node_ = node;
    if (node->isTriple() && dir == PointsTo::Right)
    {
      this->dir_ = PointsTo::Right;
      stack_.top().second = PointsTo::Left;
      curr_ = node->middle;
    }
    else
    {
      this->dir_ = PointsTo::Left;
      stack_.pop();
      curr_ = node->left;
    }
    return *this;
  }

  template< class K, class V, class C, bool isConst >
  auto RnlIterator< K, V, C, isConst >::operator++(int) -> RnlIterator&
  {
    assert(this->node_ != nullptr && "ERROR: Trying to iterate from null-node!");
    auto result(*this);
    ++(*this);
    return result;
  }
}

#endif

