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

    Queue() = default;
  
    Queue(const Queue& other)
      : _head(other._head), _tail(other._tail), _size(other._size), _buffer(other._buffer) {}

    Queue(Queue&& other) { swap(other); }

    ~Queue() = default;

    Queue& operator=(const Queue& other)
    {
    }

    Queue& operator=(Queue&& other)
    {
    }

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
