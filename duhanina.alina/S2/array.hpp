#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <stdexcept>
#include <cstddef>

namespace duhanina
{
  template < typename T >
  class DynamicArray
  {
  public:
    DynamicArray() noexcept;
    ~DynamicArray();

    DynamicArray(const DynamicArray& other);
    DynamicArray(DynamicArray&& other) noexcept;
    DynamicArray& operator=(DynamicArray&& other) noexcept;

    void push_back(const T& value);
    void pop_back();
    void pop_front();

    size_t size() const noexcept;
    bool empty() const noexcept;

    const T& front() const noexcept;
    T& front() noexcept;

    const T& back() const noexcept;
    T& back() noexcept;

   private:
    T* data_;
    size_t capacity_;
    size_t length_;
    void resize();
  };

  template < typename T >
  DynamicArray< T >::DynamicArray() noexcept:
    data_(nullptr),
    capacity_(50),
    length_(0)
  {
    data_ = new T[capacity_];
  }

  template < typename T >
  DynamicArray< T >::~DynamicArray()
  {
    delete[] data_;
  }

  template< typename T >
  DynamicArray< T >::DynamicArray(const DynamicArray& other):
    data_(new T[other.capacity_]),
    capacity_(other.capacity_),
    length_(other.length_)
  {
    try
    {
      for (size_t i = 0; i < other.length_; ++i)
      {
        data_[i] = other.data_[i];
      }
    }
    catch (...)
    {
      delete[] data_;
      data_ = nullptr;
      length_ = 0;
      capacity_ = 0;
      throw;
    }
  }

  template < typename T >
  DynamicArray< T >::DynamicArray(DynamicArray&& other) noexcept:
    data_(other.data_),
    capacity_(other.capacity_),
    length_(other.length_)
  {
    other.data_ = nullptr;
    other.capacity_ = 0;
    other.length_ = 0;
  }

  template < typename T >
  DynamicArray< T >& DynamicArray< T >::operator=(DynamicArray&& other) noexcept
  {
    if (this != &other)
    {
      delete[] data_;
      data_ = other.data_;
      capacity_ = other.capacity_;
      length_ = other.length_;
      other.data_ = nullptr;
      other.capacity_ = 0;
      other.length_ = 0;
    }
    return *this;
  }

  template < typename T >
  void DynamicArray< T >::resize()
  {
    capacity_ *= 2;
    T* new_data = new T[capacity_];
    for (size_t i = 0; i < length_; ++i)
    {
      new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;
  }

  template < typename T >
  void DynamicArray< T >::push_back(const T& value)
  {
    if (length_ == capacity_)
    {
      resize();
    }
    data_[length_++] = value;
  }

  template < typename T >
  void DynamicArray< T >::pop_back()
  {
    if (empty())
    {
      throw std::out_of_range("Array is empty");
    }
    --length_;
  }

  template< typename T >
  void DynamicArray< T >::pop_front()
  {
    if (empty())
    {
      throw std::out_of_range("Array is empty");
    }
    for (size_t i = 0; i < length_ - 1; ++i)
    {
      data_[i] = data_[i + 1];
    }
    length_--;
  }

  template < typename T >
  size_t DynamicArray< T >::size() const noexcept
  {
    return length_;
  }

  template < typename T >
  bool DynamicArray< T >::empty() const noexcept
  {
    return length_ == 0;
  }

  template < typename T >
  const T& DynamicArray< T >::front() const noexcept
  {
    return data_[0];
  }

  template < typename T >
  T& DynamicArray< T >::front() noexcept
  {
    return data_[0];
  }

  template < typename T >
  const T& DynamicArray< T >::back() const noexcept
  {
    return data_[length_ - 1];
  }

  template < typename T >
  T& DynamicArray< T >::back() noexcept
  {
    return data_[length_ - 1];
  }
}

#endif
