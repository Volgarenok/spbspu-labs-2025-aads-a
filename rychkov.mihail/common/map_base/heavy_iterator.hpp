#ifndef MAP_BASE_HEAVY_ITERATOR_HPP
#define MAP_BASE_HEAVY_ITERATOR_HPP

#include <iterator>
#include <type_traits>
#include <utility>
#include <stack.hpp>
#include <queue.hpp>
#include "node.hpp"

namespace rychkov
{
  template< class V, size_t N, class RealV, bool isConst, bool isReversed >
  class MapBaseIterator;

  template< class Value, size_t N, class RealValue, bool isConst, bool isReversed, bool isBreadth >
  class MapBaseHeavyIterator
  {
  private:
    static constexpr size_t node_capacity = N;
    using real_value_type = RealValue;
    using node_type = MapBaseNode< real_value_type, node_capacity >;

    template< class V, size_t N1, class RealV, bool isConst1, bool isReversed1 >
    friend class MapBaseIterator;
  public:
    using difference_type = ptrdiff_t;
    using value_type = Value;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::conditional_t< isBreadth,
          std::forward_iterator_tag, std::bidirectional_iterator_tag >;

    MapBaseHeavyIterator():
      container_(),
      pointed_(0)
    {}
    template< bool isConst1 = isConst >
    MapBaseHeavyIterator(typename std::enable_if_t< isConst && isConst1,
          MapBaseHeavyIterator< Value, N, RealValue, false, isReversed, isBreadth > > rhs):
      container_(rhs.container_),
      pointed_(rhs.pointed_)
    {}
    MapBaseHeavyIterator(MapBaseIterator< Value, N, RealValue, isConst, isReversed > src):
      MapBaseHeavyIterator()
    {
      if (isBreadth)
      {
        if (src.node_->isfake())
        {
          if (src.node_->isleaf())
          {
            return;
          }
          container_.push(src.node_->children[0]);
        }
        else
        {
          decltype(src) fake = src;
          for (; !fake.node_->isfake(); fake.move_up())
          {}
          MapBaseHeavyIterator i = fake;
          for (; (i.top() != src.node_) || (i.pointed_ != src.pointed_); ++i)
          {}
          *this = std::move(i);
          return;
        }
        pointed_ = (isReversed ? top()->size() - 1 : 0);
        return;
      }
      pointed_ = src.pointed_;
      Stack< node_type* > temp;
      temp.push(src.node_);
      for (; !temp.top()->isfake(); src.move_up(), temp.push(src.node_))
      {}
      while (!temp.empty())
      {
        container_.push(temp.top());
        temp.pop();
      }
    }

    bool operator==(MapBaseHeavyIterator rhs) const noexcept
    {
      if (container_.empty() || rhs.container_.empty())
      {
        return container_.empty() && rhs.container_.empty();
      }
      return (top() == rhs.top()) && (pointed_ == rhs.pointed_);
    }
    bool operator!=(MapBaseHeavyIterator rhs) const noexcept
    {
      return !operator==(rhs);
    }

    MapBaseHeavyIterator& operator++()
    {
      isReversed ? shift_left() : shift_right();
      return *this;
    }
    MapBaseHeavyIterator operator++(int)
    {
      MapBaseHeavyIterator temp = *this;
      operator++();
      return temp;
    }
    template< bool isBreadth1 = isBreadth >
    std::enable_if_t< !isBreadth1 && !isBreadth, MapBaseHeavyIterator& > operator--()
    {
      isReversed ? shift_right() : shift_left();
      return *this;
    }
    template< bool isBreadth1 = isBreadth >
    std::enable_if_t< !isBreadth1 && !isBreadth, MapBaseHeavyIterator > operator--(int)
    {
      MapBaseHeavyIterator temp = *this;
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
      return reinterpret_cast< value_type* >(std::addressof(top()->operator[](pointed_)));
    }
    const value_type* operator->() const noexcept
    {
      return reinterpret_cast< value_type* >(std::addressof(top()->operator[](pointed_)));
    }
  private:
    friend class MapBaseHeavyIterator< Value, N, RealValue, true, isReversed, isBreadth >;

    using node_size_type = typename node_type::size_type;
    using container = std::conditional_t< isBreadth, Queue< node_type* >, Stack< node_type* > >;
    container container_;
    node_size_type pointed_;

    void shift_left();
    void shift_right();
    template< bool isBreadth1 = isBreadth >
    std::enable_if_t< isBreadth1 && isBreadth, node_type* > top() const noexcept
    {
      return container_.front();
    }
    template< bool isBreadth1 = isBreadth >
    std::enable_if_t< !isBreadth1 && !isBreadth, node_type* > top() const noexcept
    {
      return container_.top();
    }
  };
}

template< class Value, size_t N, class RealValue, bool isConst, bool isReversed, bool isBreadth >
void rychkov::MapBaseHeavyIterator< Value, N, RealValue, isConst, isReversed, isBreadth >::shift_left()
{
  if (isBreadth)
  {
    if (!top()->isleaf())
    {
      container_.push(top()->children[pointed_ + 1]);
    }
    if (pointed_ > 0)
    {
      pointed_--;
    }
    else
    {
      if (!top()->isleaf())
      {
        container_.push(top()->children[0]);
      }
      container_.pop();
      pointed_ = (container_.empty() ? 0 : top()->size() - 1);
    }
    return;
  }
  if (top()->isfake() || !top()->isleaf())
  {
    container_.push(top()->children[top()->isfake() ? 0 : pointed_]);
    for (; !top()->isleaf(); container_.push(top()->children[top()->size()]))
    {}
    pointed_ = top()->size() - 1;
    return;
  }
  if (pointed_ > 0)
  {
    pointed_--;
  }
  else
  {
    node_type* prev = top();
    container_.pop();
    for (; !top()->isfake() && (top()->children[0] == prev); prev = top(), container_.pop())
    {}
    for (pointed_ = top()->size(); pointed_ > 0; pointed_--)
    {
      if (top()->children[pointed_] == prev)
      {
        break;
      }
    }
    if (!top()->isfake())
    {
      pointed_--;
    }
  }
}
template< class Value, size_t N, class RealValue, bool isConst, bool isReversed, bool isBreadth >
void rychkov::MapBaseHeavyIterator< Value, N, RealValue, isConst, isReversed, isBreadth >::shift_right()
{
  if (isBreadth)
  {
    if (!top()->isleaf())
    {
      container_.push(top()->children[pointed_]);
    }
    if (pointed_ + 1 < top()->size())
    {
      pointed_++;
    }
    else
    {
      if (!top()->isleaf())
      {
        container_.push(top()->children[pointed_ + 1]);
      }
      container_.pop();
      pointed_ = 0;
    }
    return;
  }
  if (top()->isfake() || !top()->isleaf())
  {
    container_.push(top()->children[top()->isfake() ? 0 : pointed_ + 1]);
    for (; !top()->isleaf(); container_.push(top()->children[0]))
    {}
    pointed_ = 0;
    return;
  }
  if (pointed_ + 1 < top()->size())
  {
    pointed_++;
  }
  else
  {
    node_type* prev = top();
    container_.pop();
    for (; !top()->isfake() && (top()->children[top()->size()] == prev); prev = top(), container_.pop())
    {}
    for (pointed_ = 0; pointed_ < top()->size(); pointed_++)
    {
      if (top()->children[pointed_] == prev)
      {
        break;
      }
    }
  }
}

#endif
