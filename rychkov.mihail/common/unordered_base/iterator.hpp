#ifndef UNORDERED_BASE_ITERATOR_HPP
#define UNORDERED_BASE_ITERATOR_HPP

#include <utility>
#include <memory>
#include <iterator>

namespace rychkov
{
  template< class V, bool IsConst >
  class UnorderedBaseIterator
  {
  public:
    using value_type = V;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

    UnorderedBaseIterator() noexcept:
      data_(nullptr),
      end_(nullptr)
    {};
    template< bool IsConst1 = IsConst >
    UnorderedBaseIterator(typename std::enable_if_t< IsConst && IsConst1,
          UnorderedBaseIterator< V, false > > rhs) noexcept:
      data_(rhs.data_),
      end_(rhs.end_)
    {}

    bool operator==(UnorderedBaseIterator rhs) const noexcept
    {
      return data_ == rhs.data_;
    }
    bool operator!=(UnorderedBaseIterator rhs) const noexcept
    {
      return !operator==(rhs);
    }

    UnorderedBaseIterator& operator++() noexcept
    {
      shift_right();
      return *this;
    }
    UnorderedBaseIterator operator++(int) noexcept
    {
      UnorderedBaseIterator temp = *this;
      operator++();
      return temp;
    }
    UnorderedBaseIterator& operator--() noexcept
    {
      shift_left();
      return *this;
    }
    UnorderedBaseIterator operator--(int) noexcept
    {
      UnorderedBaseIterator temp = *this;
      operator--();
      return temp;
    }

    template< bool IsConst1 = IsConst >
    typename std::enable_if_t< !IsConst && !IsConst1, value_type& > operator*() noexcept
    {
      return data_->second;
    }
    const value_type& operator*() const noexcept
    {
      return data_->second;
    }

    template< bool IsConst1 = IsConst >
    typename std::enable_if_t< !IsConst && !IsConst1, value_type* > operator->() noexcept
    {
      return std::addressof(operator*());
    }
    const value_type* operator->() const noexcept
    {
      return std::addressof(operator*());
    }

  private:
    template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
    friend class UnorderedBase;
    friend class UnorderedBaseIterator< V, true >;

    using stored_value = std::pair< size_t, value_type >;

    stored_value* data_;
    stored_value* end_;

    UnorderedBaseIterator(stored_value* data, stored_value* end) noexcept:
      data_(data),
      end_(end)
    {};

    void shift_left() noexcept
    {
      for (--data_; data_->first == ~0ULL; --data_)
      {}
    }
    void shift_right() noexcept
    {
      for (++data_; (data_ != end_) && (data_->first == ~0ULL); ++data_)
      {}
    }
  };
}

#endif
