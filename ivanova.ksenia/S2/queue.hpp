#pragma once
#include <algorithm>
#include <cstddef>
#include "array.hpp"

namespace ivanova
{
  template <typename T>
  class Queue
  {
  public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    Queue() = default;
  
    Queue(const Queue& other)
      : _head(other._head), _tail(other._tail), _size(other._size), _buffer(other._buffer) {}

    Queue(Queue&& other) { swap(other); }

    ~Queue() = default;

    Queue& operator=(const Queue& other)
    {
      if (this != &other)
      {
        Queue temp(other);
        swap(temp);
      }
      return *this;
    }

    Queue& operator=(Queue&& other)
    {
      Queue temp(std::move(other));
      swap(temp);
      return *this;
    }

    reference front() { return _buffer[_head]; }
    reference back() { return _buffer[_tail]; }
    const_reference front() const { return _buffer[_head]; }
    const_reference back() const { return _buffer[_tail]; }

    size_type size() const noexcept { return _size; }
    bool empty() const noexcept { return _size == 0; }

    void swap(Queue& other)
    {
      std::swap(_head, other._head);
      std::swap(_tail, other._tail);
      std::swap(_size, other._size);
      _buffer.swap(other._buffer);
    }
  private:
    size_type _head = 0;
    size_type _tail = 0;
    size_type _size = 0;
    Array<T> _buffer;
  };
}
