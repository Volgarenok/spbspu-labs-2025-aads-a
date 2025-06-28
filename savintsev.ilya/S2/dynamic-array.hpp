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

    Array & operator=(const Array & rhs);
    Array & operator=(Array && rhs) noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    const T & front() const;
    T & front();
    const T & back() const;
    T & back();

    template< typename U >
    void push_back(U && rhs);
    void pop_front() noexcept;
    void pop_back() noexcept;

    template< typename U >
    friend void swap(Array< U > & x, Array< U > & y) noexcept;
  private:
    T * data_ = nullptr;
    size_t size_ = 0;
    size_t start_ = 0;
    size_t capacity_ = 0;
  };

  template< typename T >
  void swap(Array< T > & x, Array< T > & y) noexcept
  {
    std::swap(x.data_, y.data_);
    std::swap(x.size_, y.size_);
    std::swap(x.start_, y.start_);
    std::swap(x.capacity_, y.capacity_);
  }

  template< typename T >
  Array< T >::Array(size_t n):
    data_(new T[n]),
    size_(0),
    start_(0),
    capacity_(n)
  {}

  template< typename T >
  Array< T >::Array(const Array & rhs):
    data_(createExpandCopy(rhs.data_ + rhs.start_, rhs.size_, rhs.capacity_)),
    size_(rhs.size_),
    start_(0),
    capacity_(rhs.capacity_)
  {}

  template< typename T >
  Array< T >::Array(Array && rhs) noexcept:
    data_(rhs.data_),
    size_(rhs.size_),
    start_(rhs.start_),
    capacity_(rhs.capacity_)
  {}

  template< typename T >
  Array< T >::~Array()
  {
    delete[] data_;
  }

  template< typename T >
  Array< T > & Array< T >::operator=(const Array & rhs)
  {
    Array< T > copy{rhs};
    swap(*this, copy);
    return *this;
  }

  template< typename T >
  Array< T > & Array< T >::operator=(Array && rhs) noexcept
  {
    Array< T > copy{std::move(rhs)};
    swap(*this, copy);
    return *this;
  }

  template< typename T >
  bool Array< T >::empty() const noexcept
  {
    return !size_;
  }

  template< typename T >
  size_t Array< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  const T & Array< T >::front() const
  {
    return data_[start_];
  }

  template< typename T >
  T & Array< T >::front()
  {
    return data_[start_];
  }

  template< typename T >
  const T & Array< T >::back() const
  {
    return data_[start_ + size_ - 1];
  }

  template< typename T >
  T & Array< T >::back()
  {
    return data_[start_ + size_ - 1];
  }

  template< typename T >
  template< typename U >
  void Array< T >::push_back(U && rhs)
  {
    if (size_ + start_ < capacity_)
    {
      data_[size_ + start_] = std::forward< U >(rhs);
      size_++;
      return;
    }
    T * arr = createExpandCopy(data_ + start_, size_, capacity_ + capacity_);
    try
    {
      arr[size_] = std::forward< U >(rhs);
    }
    catch (...)
    {
      delete[] arr;
      throw;
    }
    delete[] data_;
    data_ = arr;
    capacity_ += capacity_;
    start_ = 0;
    size_++;
  }

  template< typename T >
  void Array< T >::pop_back() noexcept
  {
    size_--;
  }

  template< typename T >
  void Array< T >::pop_front() noexcept
  {
    ++start_;
    --size_;
  }
}

#endif
