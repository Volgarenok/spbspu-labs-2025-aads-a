#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <utility>
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

    void swap(DynamicArray& other) noexcept;

    DynamicArray& operator=(const DynamicArray&);
    DynamicArray& operator=(DynamicArray&& other) noexcept;

    void push_back(const T& value);
    void pop_back();
    void pop_front();

    size_t size() const noexcept;
    bool empty() const noexcept;

    const T& front() const;
    T& front();

    const T& back() const;
    T& back();

   private:
    size_t capacity_;
    size_t length_;
    size_t head_;
    T* data_;
    void resize();
  };

  template < typename T >
  DynamicArray< T >::DynamicArray() noexcept:
    capacity_(50),
    length_(0),
    head_(0),
    data_(new T[capacity_])
  {}

  template < typename T >
  DynamicArray< T >::~DynamicArray()
  {
    delete[] data_;
  }

  template< typename T >
  DynamicArray< T >::DynamicArray(const DynamicArray& other):
    capacity_(other.capacity_),
    length_(other.length_),
    head_(0),
    data_(new T[other.capacity_])
  {
    try
    {
      for (size_t i = 0; i < other.length_; ++i)
      {
        data_[i] = other.data_[i + other.head_];
      }
    }
    catch (...)
    {
      delete[] data_;
      throw;
    }
  }

  template < typename T >
  DynamicArray< T >::DynamicArray(DynamicArray&& other) noexcept:
    capacity_(other.capacity_),
    length_(other.length_),
    head_(0),
    data_(other.data_)
  {
    other.data_ = nullptr;
    other.capacity_ = 0;
    other.length_ = 0;
    other.head_ = 0;
  }

  template< typename T >
  void DynamicArray< T >::swap(DynamicArray< T >& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(capacity_, other.capacity_);
    std::swap(length_, other.length_);
    std::swap(head_, other.head_);
  }

  template < typename T >
  DynamicArray< T >& DynamicArray< T >::operator=(const DynamicArray& other)
  {
    DynamicArray< T > temp(other);
    swap(temp);
    return *this;
  }

  template < typename T >
  DynamicArray< T >& DynamicArray< T >::operator=(DynamicArray&& other) noexcept
  {
    DynamicArray< T > temp(std::move(other));
    swap(temp);
    return *this;
  }

  template < typename T >
  void DynamicArray< T >::resize()
  {
    size_t new_capacity = capacity_ * 2;
    T* new_data = nullptr;
    try
    {
      new_data = new T[new_capacity];
      for (size_t i = 0; i < length_; ++i)
      {
        new_data[i + head_] = data_[i + head_];
      }
    }
    catch (...)
    {
      delete[] new_data;
      throw;
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
  }

  template < typename T >
  void DynamicArray< T >::push_back(const T& value)
  {
    if (length_ == capacity_)
    {
      resize();
    }
    data_[length_ + head_] = value;
    ++length_;
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
    head_ = (head_ + 1) % capacity_;
    --length_;
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
  const T& DynamicArray< T >::front() const
  {
    return data_[head_];
  }

  template < typename T >
  T& DynamicArray< T >::front()
  {
    return data_[head_];
  }

  template < typename T >
  const T& DynamicArray< T >::back() const
  {
    return data_[length_ + head_ - 1];
  }

  template < typename T >
  T& DynamicArray< T >::back()
  {
    return data_[length_ + head_ - 1];
  }
}

#endif
