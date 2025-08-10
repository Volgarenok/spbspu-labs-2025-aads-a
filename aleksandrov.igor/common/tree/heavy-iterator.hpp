#ifndef HEAVY_ITERATOR_HPP
#define HEAVY_ITERATOR_HPP

#include <iterator>
#include <cassert>
#include "node.hpp"
#include "iterator.hpp"

namespace aleksandrov
{
  template< class K, class V, class C >
  class Tree;

  template< class K, class V, class C, bool isConst, bool isReversive >
  class HeavyIterator: public std::iterator< std::bidirectional_iterator_tag, std::pair< K, V > >
  {
  public:
    using ValueType = std::pair< K, V >;
    using NodeType = detail::NodeType;
    using Reference = std::conditional_t< isConst, const ValueType&, ValueType& >;
    using Pointer = std::conditional_t< isConst, const ValueType*, ValueType* >;
    using LiteIter = Iterator< K, V, C, isConst, isReversive >;

    HeavyIterator();

    Reference operator*() const noexcept;
    Pointer operator->() const noexcept;

    bool operator==(const HeavyIterator&) const;
    bool operator!=(const HeavyIterator&) const;
    bool operator==(const LiteIter&) const;
    bool operator!=(const LiteIter&) const;

  protected:
    friend class Iterator< K, V, C, isConst, isReversive >;
    using Node = detail::Node< K, V >;

    Node* node_;
    PointsTo dir_;

    HeavyIterator(Node*);
  };

  template< class K, class V, class C, bool isConst, bool isReversive >
  HeavyIterator< K, V, C, isConst, isReversive >::HeavyIterator():
    node_(nullptr),
    dir_(PointsTo::None)
  {}

  template< class K, class V, class C, bool isConst, bool isReversive >
  HeavyIterator< K, V, C, isConst, isReversive >::HeavyIterator(Node* node):
    node_(node),
    dir_(PointsTo::Left)
  {}

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto HeavyIterator< K, V, C, isConst, isReversive >::operator*() const noexcept -> Reference
  {
    assert(dir_ != PointsTo::None && "ERROR: Access to null-node!");
    return dir_ == PointsTo::Left ? node_->data[0] : node_->data[1];
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  auto HeavyIterator< K, V, C, isConst, isReversive >::operator->() const noexcept -> Pointer
  {
    assert(dir_ != PointsTo::None && "ERROR: Access to null-node!");
    return std::addressof(dir_ == PointsTo::Left ? node_->data[0] : node_->data[1]);
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  bool HeavyIterator< K, V, C, isConst, isReversive >::operator==(const HeavyIterator& rhs) const
  {
    return node_ == rhs.node_ && dir_ == rhs.dir_;
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  bool HeavyIterator< K, V, C, isConst, isReversive >::operator!=(const HeavyIterator& rhs) const
  {
    return !(*this == rhs);
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  bool HeavyIterator< K, V, C, isConst, isReversive >::operator==(const LiteIter& rhs) const
  {
    return node_ == rhs.node_ && dir_ == rhs.dir_;
  }

  template< class K, class V, class C, bool isConst, bool isReversive >
  bool HeavyIterator< K, V, C, isConst, isReversive >::operator!=(const LiteIter& rhs) const
  {
    return !(*this == rhs);
  }
}

#endif

