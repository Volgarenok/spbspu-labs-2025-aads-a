#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

#include "resize.hpp"

namespace mozhegova
{
  template< typename T >
  class DynamicArray
  {
  public:
    DynamicArray();
    DynamicArray(const DynamicArray & other);
    DynamicArray(DynamicArray && other) noexcept;
    explicit DynamicArray(size_t size);
    ~DynamicArray();
    DynamicArray< T > & operator=(DynamicArray< T > &&) noexcept;
    DynamicArray< T > & operator=(const DynamicArray< T > &);
    T & operator[](size_t i);
    const T & operator[](size_t i) const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void swap(DynamicArray & other) noexcept;
    void push_back(const T & value);
  private:
    size_t capacity_;
    size_t size_;
    T * data_;
  };

  template< typename T >
  DynamicArray< T >::DynamicArray():
    capacity_(10),
    size_(0),
    data_(new T[capacity_])
  {}

  template< typename T >
  DynamicArray< T >::DynamicArray(size_t size):
    capacity_(size + 10),
    size_(size),
    data_(new T[size])
  {}

  template< typename T >
  DynamicArray< T >::DynamicArray(const DynamicArray & other):
    capacity_(other.capacity_),
    size_(other.size_),
    data_(new T[capacity_])
  {
    try
    {
      for (size_t i = 0; i < size_; i++)
      {
        data_[i] = other.data_[i];
      }
    }
    catch (const std::exception &)
    {
      delete[] data_;
      throw;
    }
  }

  template< typename T >
  DynamicArray< T >::DynamicArray(DynamicArray && other) noexcept:
    capacity_(other.capacity_),
    size_(other.size_),
    data_(other.data_)
  {
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
  }

  template< typename T >
  DynamicArray< T >::~DynamicArray()
  {
    delete[] data_;
  }

  template< typename T >
  DynamicArray< T > & DynamicArray< T >::operator=(DynamicArray< T > && other) noexcept
  {
    if (this != std::addressof(other))
    {
      DynamicArray< T > copy(std::move(other));
      swap(copy);
    }
    return *this;
  }

  template< typename T >
  DynamicArray< T > & DynamicArray< T >::operator=(const DynamicArray< T > & other)
  {
    if (this != std::addressof(other))
    {
      DynamicArray< T > copy(other);
      swap(copy);
    }
    return *this;
  }

  template< typename T >
  T & DynamicArray< T >::operator[](size_t i)
  {
    return data_[i];
  }

  template< typename T >
  const T & DynamicArray< T >::operator[](size_t i) const
  {
    return data_[i];
  }

  template< typename T >
  bool DynamicArray< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t DynamicArray< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  void DynamicArray< T >::swap(DynamicArray & other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
  }

  template< typename T >
  void DynamicArray< T >::push_back(const T & value)
  {
    if (size_ == capacity_)
    {
      data_ = resize(data_, capacity_);
    }
    data_[size_++] = value;
  }
}

#endif
