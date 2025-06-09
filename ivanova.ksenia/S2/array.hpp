#pragma once
#include <algorithm>
#include <cstddef>
#include <memory>

namespace ivanova
{
  template <typename T>
  class Array
  {
  public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using iterator = pointer;
    using const_iterator = const_pointer;

    Array() noexcept : _size(0), _capacity(0), _data(nullptr) {}

    Array(const Array& other) : _size(other._size), _capacity(other._capacity), _data(allocate(_capacity))
    {
      size_t constructed = 0;
      try
      {
        for (; constructed < _size; ++constructed)
        {
          new (_data + constructed) value_type(other._data[constructed]);
        }
      }
      catch (...)
      {
        destruct(_data, constructed);
        deallocate(_data);
        throw;
      }
    }

    Array(Array&& other) noexcept : Array() { swap(other); }

    ~Array()
    {
      destruct(_data, _size);
      deallocate(_data);
    }

    Array& operator=(const Array& other)
    {
      if (this != &other)
      {
        Array temp(other);
        swap(temp);
      }
      return *this;
    }

    Array& operator=(Array&& other) noexcept
    {
      Array temp(std::move(other));
      swap(temp);
      return *this;
    }

    void push_back(const_reference value)
    {
      if (_size != _capacity)
      {
        new (_data + _size) value_type(value);
        ++_size;
        return;
      }
      size_type new_capacity = _size * 2 + 1;
      pointer new_data = allocate(new_capacity);
      try
      {
        new (new_data + _size) value_type(value);
      }
      catch (...)
      {
        deallocate(new_data);
        throw;
      }
      size_type constructed = 0;
      try
      {
        for (; constructed < _size; ++constructed)
        {
          new (new_data + constructed) value_type(_data[constructed]);
        }
      }
      catch (...)
      {
        destruct(new_data, constructed);
        deallocate(new_data);
        throw;
      }
      deallocate(_data);
      _capacity = new_capacity;
      _data = new_data;
      ++_size;
    }

    void pop_back()
    {
      if (_size > 0)
      {
        back().~value_type();
        --_size;
      }
    }

    reference front() { return _data[0]; }
    reference back() { return _data[_size - 1]; }
    const_reference front() const { return _data[0]; }
    const_reference back() const { return _data[_size - 1]; }

    reference operator[](size_type index) { return _data[index]; }
    const_reference operator[](size_type index) const { return _data[index]; }

    pointer data() noexcept { return _data; }
    const_pointer data() const noexcept { return _data; }

    size_type size() const noexcept { return _size; }
    size_type capacity() const noexcept { return _capacity; }
    bool empty() const noexcept { return _size == 0; }

    void swap(Array& other) noexcept
    {
      std::swap(_size, other._size);
      std::swap(_capacity, other._capacity);
      std::swap(_data, other._data);
    }

    iterator begin() noexcept { return _data; }
    iterator end() noexcept { return _data + _size; }
    const_iterator begin() const noexcept { return _data; }
    const_iterator end() const noexcept { return _data + _size; }

  private:
    size_type _size;
    size_type _capacity;
    pointer _data;

    static pointer allocate(size_type size)
    {
      return size > 0 ? static_cast<pointer>(operator new(size * sizeof(value_type))) : nullptr;
    }

    static void deallocate(pointer data)
    {
      if (data != nullptr)
      {
        operator delete(data);
      }
    }

    static void destruct(pointer data, size_type size)
    {
      for (size_type i = 0; i < size; ++i)
      {
        data[i].~value_type();
      }
    }
  };
}
