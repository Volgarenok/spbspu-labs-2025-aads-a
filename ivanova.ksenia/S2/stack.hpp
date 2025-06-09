#pragma once
#include <algorithm>
#include <cstddef>
#include <memory>

namespace ivanova
{
  template <typename T>
  class Stack
  {
  public:

    Stack() = default;

    Stack(const Stack& other) : _buffer(other._buffer) {}

    Stack(Stack&& other) { swap(other); }

    ~Stack() = default;

    Stack& operator=(const Stack& other)
    {
    }

    Stack& operator=(Stack&& other)
    {
    }

    void swap(Stack& other)
    {
      _buffer.swap(other._buffer);
    }

  private:
    Array<T> _buffer;
  };
}
