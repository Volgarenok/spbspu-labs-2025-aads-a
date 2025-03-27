#ifndef ARRAY_HPP
#define ARRAY_HPP
#include <iostream>
#include <stdexcept>


namespace averenkov
{
  template< class T >
  class Array
  {
  public:
    Array();
    Array(const Array &rhs);
    Array(Array &&rhs) noexcept;
    Array &operator=(const Array &rhs);
    ~Array();

    bool empty() const noexcept;
    size_t size() const noexcept;

    const T& front() const noexcept;
    T& front() noexcept;
    const T& back() const noexcept;
    T& back() noexcept;

    void push_back(T rhs);
    void pop_front();
    void pop_back() noexcept;

  private:
    T* data_;
    size_t size_;
    size_t capacity_;
    void resize();

  };

  template< class T >
  Array< T >::Array():
    data_(new T[1]),
    size_(0),
    capacity_(1)
  {}

  template< class T >
  Array< T >::Array(const Array& rhs):
    data_(new T[rhs.capacity_]),
    size_(rhs.size_),
    capacity_(rhs.capacity_)
  {
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i] = rhs.data_[i];
    }
  }

  template< class T >
  Array< T >::Array(Array&& rhs) noexcept:
    data_(rhs.data_),
    size_(rhs.size_),
    capacity_(rhs.capacity_)
  {
    rhs.data_ = nullptr;
    rhs.size_ = 0;
    rhs.capacity_ = 0;
  }

  template< class T >
  Array< T >& Array< T >::operator=(const Array& rhs)
  {
    if (this != &rhs)
    {
      T* new_data = new T[rhs.capacity_];
      for (size_t i = 0; i < rhs.size_; ++i)
      {
        new_data[i] = rhs.data_[i];
      }
      delete[] data_;
      data_ = new_data;
      size_ = rhs.size_;
      capacity_ = rhs.capacity_;
    }
    return *this;
  }

  template< class T >
  Array< T >::~Array()
  {
    delete[] data_;
  }

  template< class T >
  bool Array< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  size_t Array< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  const T& Array< T >::front() const noexcept
  {
    return data_[0];
  }

  template< class T >
  T& Array< T >::front() noexcept
  {
    return data_[0];
  }

  template< class T >
  const T& Array< T >::back() const noexcept
  {
    return data_[size_ - 1];
  }

  template< class T >
  T& Array< T >::back() noexcept
  {
    return data_[size_ - 1];
  }

  template< class T >
  void Array< T >::push_back(T rhs)
  {
    if (size_ == capacity_)
    {
      resize();
    }
    data_[size_++] = rhs;
  }

  template< class T >
  void Array< T >::pop_front()
  {
    if (empty())
    {
      throw std::out_of_range("empty");
    }
    for (size_t i = 1; i < size_; ++i)
    {
      data_[i - 1] = data_[i];
    }
    --size_;
  }

  template< class T >
  void Array< T >::pop_back() noexcept
  {
    if (!empty())
    {
      --size_;
    }
  }

  template< class T >
  void Array< T >::resize()
  {
    size_t new_capacity = capacity_ * 2;
    T* new_data = new T[new_capacity];
    for (size_t i = 0; i < size_; ++i)
    {
      new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
  }
}


#endif
