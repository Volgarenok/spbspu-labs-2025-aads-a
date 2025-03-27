#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <stdexcept>

namespace kushekbaev
{
  template< typename T >
  class Array
  {
    public:
      Array();
      Array(const Array& other);
      Array(Array&& other);
      ~Array();

      Array& operator=(const Array& other);

      T& front() noexcept;
      T& back() noexcept;
      const T& front() const noexcept;
      const T& back() const noexcept;

      bool empty() const noexcept;
      size_t size() const noexcept;

      void push_back(const T& value);
      void pop_front();
      void pop_back();

    private:
      T* data_;
      size_t size_;
      size_t capacity_;
      void resizeArray();
  };

  template< typename T >
  Array< T >::Array():
    data_(new T[1]),
    size_(0),
    capacity_(1)
  {}

  template< typename T >
  Array< T >::Array(const Array& other):
    data_(new T[other.capacity_]),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i] = other.data_[i];
    }
  }

  template< typename T >
  Array< T >::Array(Array&& other):
    data_(other.data_),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  template< typename T >
  Array< T >::~Array()
  {
    delete[] data_;
  }

  template< typename T >
  Array< T >& Array< T >::operator=(const Array& other)
  {
    if (this != &other)
    {
      T* newData = new T[other.capacity_];
      for (size_t i = 0; i < other.size_; ++i)
      {
        newData[i] = other.data_[i];
      }
      delete[] data_;
      data_ = newData;
      size_ = other.size_;
      capacity_ = other.capacity_;
    }
    return *this;
  }

  template< typename T >
  bool Array< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t Array< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  T& Array< T >::front() noexcept
  {
    return data_[0];
  }

  template< typename T >
  T& Array< T >::back() noexcept
  {
    return data_[size_ - 1];
  }

  template< typename T >
  const T& Array< T >::front() const noexcept
  {
    return data_[0];
  }

  template< typename T >
  const T& Array< T >::back() const noexcept
  {
    return data_[size_ - 1];
  }

  template< typename T >
  void Array< T >::push_back(const T& value)
  {
    if (size_ == capacity_)
    {
      resizeArray();
    }
    data_[++size_] = value;
  }

  template< typename T >
  void Array< T >::pop_front()
  {
    if (empty())
    {
      throw std::out_of_range("The array is empty!");
    }
    for (size_t i = 1; i < size_; ++i)
    {
      data_[i - 1] = data_[i];
    }
    --size_;
  }

  template< typename T >
  void Array< T >::pop_back()
  {
    if (!empty())
    {
      --size_;
    }
  }

  template< typename T >
  void Array< T >::resizeArray()
  {
    size_t newCapacity = capacity_ * 2;
    T* newData = new T[newCapacity];
    for (size_t i = 0; i < size_; ++i)
    {
      newData[i] = data_[i];
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
  }
}

#endif
