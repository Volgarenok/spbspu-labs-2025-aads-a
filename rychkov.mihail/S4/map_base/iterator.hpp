#ifndef MAP_BASE_ITERATOR_HPP
#define MAP_BASE_ITERATOR_HPP

#include <iterator>
#include <type_traits>
#include "node.hpp"

namespace rychkov
{
  template< class Value, size_t N, class RealValue, bool isConst, bool isReversed >
  class MapBaseIterator
  {
  private:
    using real_value_type = RealValue;
  public:
    static constexpr size_t node_capacity = N;
    using node_type = MapBaseNode< real_value_type, node_capacity >;

    using difference_type = ptrdiff_t;
    using value_type = Value;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::bidirectional_iterator_tag;

    MapBaseIterator() noexcept:
      node_(nullptr),
      pointed_(0)
    {};
    template< bool isConst1 = isConst >
    MapBaseIterator(typename std::enable_if_t< isConst && isConst1,
          MapBaseIterator< Value, N, RealValue, false, isReversed > > rhs) noexcept:
      node_(rhs.node_),
      pointed_(rhs.pointed_)
    {}

    bool operator==(MapBaseIterator rhs) const noexcept
    {
      return (node_ == rhs.node_) && (pointed_ == rhs.pointed_);
    }
    bool operator!=(MapBaseIterator rhs) const noexcept
    {
      return !operator==(rhs);
    }

    MapBaseIterator& operator++() noexcept
    {
      isReversed ? shift_left() : shift_right();
      return *this;
    }
    MapBaseIterator operator++(int) noexcept
    {
      MapBaseIterator temp = *this;
      operator++();
      return temp;
    }
    MapBaseIterator& operator--() noexcept
    {
      isReversed ? shift_right() : shift_left();
      return *this;
    }
    MapBaseIterator operator--(int) noexcept
    {
      MapBaseIterator temp = *this;
      operator--();
      return temp;
    }

    template< bool isConst1 = isConst >
    typename std::enable_if_t< !isConst && !isConst1, value_type& > operator*() noexcept
    {
      return *operator->();
    }
    const value_type& operator*() const noexcept
    {
      return *operator->();
    }

    template< bool isConst1 = isConst >
    typename std::enable_if_t< !isConst && !isConst1, value_type* > operator->() noexcept
    {
      return reinterpret_cast< value_type* >(std::addressof(node_->operator[](pointed_)));
    }
    const value_type* operator->() const noexcept
    {
      return reinterpret_cast< value_type* >(std::addressof(node_->operator[](pointed_)));
    }
  private:
    template< class K, class M, class C, size_t N1, bool IsSet, bool IsMulti >
    friend class MapBase;
    friend class MapBaseIterator< Value, N, RealValue, true, isReversed >;

    using node_size_type = typename node_type::size_type;
    node_type* node_;
    node_size_type pointed_;

    MapBaseIterator(node_type* node, node_size_type pointed) noexcept:
      node_(node),
      pointed_(pointed)
    {}
    void shift_left() noexcept;
    void shift_right() noexcept;
    void move_up() noexcept;
  };
}

template< class Value, size_t N, class RealValue, bool isConst, bool isReversed >
void rychkov::MapBaseIterator< Value, N, RealValue, isConst, isReversed >::shift_left() noexcept
{
  if (node_->isfake() || !node_->isleaf())
  {
    node_ = (node_->isfake() ? node_->children[0] : node_->children[pointed_]);
    for (; !node_->isleaf(); node_ = node_->children[node_->size()])
    {}
    pointed_ = node_->size() - 1;
    return;
  }
  if (pointed_ > 0)
  {
    pointed_--;
  }
  else
  {
    node_type* prev = node_;
    node_ = node_->parent;
    for (; !node_->isfake() && (node_->children[0] == prev); prev = node_, node_ = node_->parent)
    {}
    for (pointed_ = node_->size(); pointed_ > 0; pointed_--)
    {
      if (node_->children[pointed_] == prev)
      {
        break;
      }
    }
  }
}
template< class Value, size_t N, class RealValue, bool isConst, bool isReversed >
void rychkov::MapBaseIterator< Value, N, RealValue, isConst, isReversed >::shift_right() noexcept
{
  if (node_->isfake() || !node_->isleaf())
  {
    node_ = (node_->isfake() ? node_->children[0] : node_->children[pointed_ + 1]);
    for (; !node_->isleaf(); node_ = node_->children[0])
    {}
    pointed_ = 0;
    return;
  }
  if (pointed_ + 1 < node_->size())
  {
    pointed_++;
  }
  else
  {
    node_type* prev = node_;
    node_ = node_->parent;
    for (; !node_->isfake() && (node_->children[node_->size()] == prev); prev = node_, node_ = node_->parent)
    {}
    for (pointed_ = 0; pointed_ < node_->size(); pointed_++)
    {
      if (node_->children[pointed_] == prev)
      {
        break;
      }
    }
  }
}
template< class Value, size_t N, class RealValue, bool isConst, bool isReversed >
void rychkov::MapBaseIterator< Value, N, RealValue, isConst, isReversed >::move_up() noexcept
{
  node_type* prev = node_;
  node_ = node_->parent;
  for (pointed_ = 0; pointed_ < node_->size(); pointed_++)
  {
    if (node_->children[pointed_] == prev)
    {
      break;
    }
  }
}

#endif
