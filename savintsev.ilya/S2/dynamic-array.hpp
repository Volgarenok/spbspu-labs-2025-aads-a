#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H
#include <cstddef>
#include <lrgcpy.hpp>

namespace savintsev
{
  template< typename T >
  class Array
  {
  public:
    Array(size_t n);
    Array(const Array & rhs);
    Array(Array && rhs) noexcept;
    ~Array();

    bool empty() const noexcept;
    size_t size() const noexcept;

    const T & front() const noexcept;
    T & front() noexcept;
    const T & back() const noexcept;
    T & back() noexcept;

    void push_back(T rhs);
    void pop_front();
    void pop_back() noexcept;

    template< typename U >
    friend void swap(Array< U > & x, Array< U > & y) noexcept;
  private:
    T * data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
  };

  template< typename T >
  void swap(Array< T > & x, Array< T > & y) noexcept
  {
    std::swap(x.data_, y.data_);
    std::swap(x.size_, y.size_);
    std::swap(x.capacity_, y.capacity_);
  }

  template< typename T >
  Array< T >::Array(size_t n):
    data_(new T[n]),
    size_(0),
    capacity_(n)
  {}

  template< typename T >
  Array< T >::Array(const Array & rhs):
    data_(new T[rhs.capacity_]),
    size_(rhs.size_),
    capacity_(rhs.capacity_)
  {
    size_t created = 0;
    try
    {
      for (; created < rhs.size_; ++created)
      {
        data_[created] = rhs.data_[created];
      }
    }
    catch (const std::exception & e)
    {
      delete[] data_;
      data_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
  }

  template< typename T >
  Array< T >::Array(Array && rhs) noexcept:
    data_(rhs.data_),
    size_(rhs.size_),
    capacity_(rhs.capacity_)
  {
    rhs.data_ = nullptr;
    rhs.size_ = 0;
    rhs.capacity_ = 0;
  }

  template< typename T >
  Array< T >::~Array()
  {
    delete[] data_;
  }

  template< typename T >
  bool Array< T >::empty() const noexcept
  {
    return !size_;
  }

  template< typename T >
  size_t Array<T>::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  const T & Array< T >::front() const noexcept
  {
    return data_[0];
  }

  template< typename T >
  T & Array< T >::front() noexcept
  {
    return data_[0];
  }

  template< typename T >
  const T & Array< T >::back() const noexcept
  {
    return data_[size_ - 1];
  }

  template< typename T >
  T & Array< T >::back() noexcept
  {
    return data_[size_ - 1];
  }

  template< typename T >
  void Array< T >::push_back(T rhs)
  {
    if (size_ < capacity_)
    {
      data_[size_] = rhs;
      size_++;
      return;
    }
    T * arr = createExpandCopy(data_, capacity_, capacity_ + capacity_);
    try
    {
      arr[size_] = rhs;
    }
    catch (const std::exception & e)
    {
      delete[] arr;
      throw;
    }
    delete[] data_;
    data_ = arr;
    capacity_ += capacity_;
    size_++;
  }

  template< typename T >
  void Array< T >::pop_back() noexcept
  {
    size_--;
  }

  template< typename T >
  void Array< T >::pop_front()
  {
    for (size_t i = 0; i < size_ - 1; ++i)
    {
      data_[i] = data_[i + 1];
    }
    size_--;
  }
}

#endif
