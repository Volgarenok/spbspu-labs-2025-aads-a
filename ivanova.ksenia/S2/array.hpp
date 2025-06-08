#pragma once

namespace ivanova
{
  template <typename T>
  class Array
  {
  public:
    using value_type = T;
    using pointer = T*;
    using size_type = std::size_t;

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

    void swap(Array& other) noexcept
    {
      std::swap(_size, other._size);
      std::swap(_capacity, other._capacity);
      std::swap(_data, other._data);
    }

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
