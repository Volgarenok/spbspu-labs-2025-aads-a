#ifndef CONST_ITERATOR_HPP
#define CONST_ITERATOR_HPP

#include <cassert>
#include "node.hpp"

namespace aleksandrov
{
  template< typename Key, typename Value, typename Compare >
  class Tree;

  template< typename Key, typename Value, typename Compare >
  class ConstIterator
  {
  public:
    ConstIterator():
      node_(nullptr),
      type_(NodeType::Double),
      pos_(PointsTo::Left)
    {}
    explicit ConstIterator(detail::Node< Key, Value >* node):
      node_(node),
      type_(node->type_),
      pos_(node->pos_)
    {}

    ConstIterator& operator++() noexcept;
    ConstIterator operator++(int) noexcept;
    ConstIterator& operator--() noexcept;
    ConstIterator operator--(int) noexcept;

    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;

    bool operator==(const ConstIterator&) const noexcept;
    bool operator!=(const ConstIterator&) const noexcept;
  private:
    friend Tree< Key, Value, Compare >;

    detail::Node< Key, Value >* node_;

    enum class NodeType
    {
      Double,
      Triple
    } type_;

    enum class PointsTo
    {
      Left,
      Right
    } pos_;

    ConstIterator fallLeft(ConstIterator);
    ConstIterator fallRight(ConstIterator);
    bool isTriple() const noexcept;
  };
}

#endif

