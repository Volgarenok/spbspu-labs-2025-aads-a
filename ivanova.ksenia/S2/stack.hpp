#pragma once
#include <algorithm>
#include <cstddef>
#include "array.hpp"

namespace ivanova
{
  template <typename T>
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

    Stack(const Stack& other) : _buffer(other._buffer) {}

    Stack(Stack&& other) { swap(other); }

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

    Stack& operator=(Stack&& other)
    {
      Stack temp(std::move(other));
      swap(temp);
      return *this;
    }

    void swap(Stack& other)
    {
      _buffer.swap(other._buffer);
    }

    reference top() { return _buffer.back(); }
    const_reference top() const { return _buffer.back(); }

    void push(const_reference value) { _buffer.push_back(value); }
    void pop() { _buffer.pop_back(); }

    size_type size() const noexcept { return _buffer.size(); }
    bool empty() const noexcept { return _buffer.empty(); }

  private:
    Array<T> _buffer;
  };
}
