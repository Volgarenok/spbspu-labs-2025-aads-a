#pragma once

namespace ivanova
{
  template <typename T>
  class Array
  {
  public:
    Array() noexcept : _size(0), _capacity(0), _data(nullptr) {}

    Array(const Array& other) : _size(other._size), _capacity(other._capacity), _data(allocate(_capacity))
    {
    }

    Array(Array&& other) noexcept : Array() { swap(other); }

    ~Array()
    {
    }

    Array& operator=(const Array& other)
    {
    }

    Array& operator=(Array&& other) noexcept
    {
    }
  private:
  };
}
