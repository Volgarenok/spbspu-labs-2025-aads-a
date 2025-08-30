#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <cassert>
#include <utility>
#include <cstddef>

namespace lanovenko
{
  template< typename T >
  class Array
  {
  public:
    ~Array() noexcept;
    Array(size_t max_size = 1'000);
    Array(const Array& rhs);
    Array(Array&& rhs) noexcept;
    Array& operator=(const Array& rhs);
    Array& operator=(Array&& rhs) noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;
    T& front() noexcept;
    T& back() noexcept;
    const T& front() const noexcept;
    const T& back() const noexcept;
    template< typename U >
    void push(U&& value);
    void pop_front() noexcept;
    void pop_back() noexcept;
    void swap(Array& rhs) noexcept;
  private:
    size_t max_size_;
    size_t size_;
    size_t first_;
    T* data_;
  };

  template< typename T >
  Array< T >::~Array() noexcept
  {
    delete[] data_;
  }

  template< typename T >
  Array< T >::Array(size_t max_size):
    max_size_(max_size),
    size_(0),
    first_(0),
    data_(new T[max_size]{})
  {}

  template< typename T >
  Array< T >::Array(const Array& rhs):
    Array(rhs.max_size_)
  {
    for (size_t i = 0; i < rhs.size_; i++)
    {
      push(rhs.data_[i]);
    }
    first_ = rhs.first_;
  }

  template< typename T >
  Array< T >::Array(Array&& rhs) noexcept:
    max_size_(rhs.max_size_),
    size_(rhs.size_),
    first_(rhs.first_),
    data_(rhs.data_)
  {
    rhs.first_ = rhs.size_ = 0;
    rhs.data_ = nullptr;
  }

  template< typename T >
  Array< T >& Array< T >::operator=(const Array& rhs)
  {
    if (this != std::addressof(rhs))
    {
      Array< T > temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  Array< T >& Array< T >::operator=(Array&& rhs) noexcept
  {
    if (this != std::addressof(rhs))
    {
      Array< T > temp(std::move(rhs));
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  inline bool Array< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  inline size_t Array< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  inline T& Array< T >::front() noexcept
  {
    assert(!empty());
    return data_[first_];
  }

  template< typename T >
  inline T& Array< T >::back() noexcept
  {
    assert(!empty());
    return data_[size_ - 1];
  }

  template< typename T >
  inline const T& Array< T >::front() const noexcept
  {
    assert(!empty());
    return data_[first_];
  }

  template< typename T >
  inline const T& Array< T >::back() const noexcept
  {
    assert(!empty());
    return data_[size_ - 1];
  }

  template< typename T >
  inline void Array< T >::pop_front() noexcept
  {
    assert(!empty());
    size_--;
    if (size_ != 0)
    {
      first_++;
    }
  }

  template< typename T >
  inline void Array< T >::pop_back() noexcept
  {
    assert(!empty());
    size_--;
  }

  template< typename T >
  inline void Array< T >::swap(Array& rhs) noexcept
  {
    using std::swap;
    swap(max_size_, rhs.max_size_);
    swap(size_, rhs.size_);
    swap(first_, rhs.first_);
    swap(data_, rhs.data_);
  }

  template< typename T >
  template< typename U >
  inline void Array< T >::push(U&& value)
  {
    assert(size_ < max_size_);
    data_[size_++] = std::forward< U >(value);
  }
}

#endif
