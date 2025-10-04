#ifndef STACK_HPP
#define STACK_HPP

#include <algorithm>
#include <cstddef>
#include "array.hpp"

namespace ivanova
{
  template < typename T >
  class Stack
  {
  public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    Stack() = default;

    Stack(const Stack& other):
      buffer_(other.buffer_)
    {}

    Stack(Stack&& other) noexcept:
      buffer_(std::move(other.buffer_))
    {}

    ~Stack() = default;

    Stack& operator=(const Stack& other)
    {
      if (this != &other)
      {
        Stack temp(other);
        swap(temp);
      }
      return *this;
    }

    Stack& operator=(Stack&& other) noexcept
    {
      Stack temp(std::move(other));
      swap(temp);
      return *this;
    }

    void swap(Stack& other) noexcept
    {
      buffer_.swap(other.buffer_);
    }

    reference top()
    {
      return buffer_.back();
    }

    const_reference top() const
    {
      return buffer_.back();
    }

    void push(const_reference value)
    {
      buffer_.push_back(value);
    }

    void pop()
    {
      buffer_.pop_back();
    }

    size_type size() const noexcept
    {
      return buffer_.size();
    }

    bool empty() const noexcept
    {
      return buffer_.empty();
    }

  private:
    Array< T > buffer_;
  };
}

#endif
