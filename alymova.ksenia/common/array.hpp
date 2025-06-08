#ifndef ARRAY_HPP
#define ARRAY_HPP
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <cassert>

namespace alymova
{
  template< typename T >
  struct Array
  {
    Array();
    Array(const Array< T >& other);
    Array(Array< T >&& other) noexcept;
    ~Array() noexcept;

    Array& operator=(const Array< T >& other);
    Array& operator=(Array< T >&& other) noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;
    T& front();
    const T& front() const noexcept;
    T& back();
    const T& back() const noexcept;
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_front() noexcept;
    void pop_back() noexcept;
    template< typename... Args >
    void emplace_back(Args&&... args);
    void swap(Array< T >& other);
  private:
    T* array_;
    T* begin_;
    T* size_ptr_;
    size_t size_;
    size_t capacity_;

    void clear() noexcept;
  };

  template< typename T >
  Array< T >::Array():
    array_(new T[10]{}),
    begin_(array_),
    size_ptr_(array_),
    size_(0),
    capacity_(10)
  {}

  template< typename T >
  Array< T >::Array(const Array< T >& other):
    array_(new T[10]{}),
    begin_(array_),
    size_ptr_(array_),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    try
    {
      T* ptr = other.begin_;
      for (size_t i = 0; i < other.size_; i++)
      {
        array_[i] = *ptr;
        ptr++;
        if (ptr == other.array_ + other.capacity_)
        {
          ptr = other.array_;
        }
        size_ptr_++;
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template< typename T >
  Array< T >::Array(Array< T >&& other) noexcept:
    array_(std::exchange(other.array_, nullptr)),
    begin_(std::exchange(other.begin_, nullptr)),
    size_ptr_(std::exchange(other.size_ptr_, nullptr)),
    size_(std::exchange(other.size_, 0)),
    capacity_(std::exchange(other.capacity_, 0))
  {}

  template< typename T >
  Array< T >::~Array() noexcept
  {
    clear();
  }

  template< typename T >
  Array< T >& Array< T >::operator=(const Array< T >& other)
  {
    Array copy(other);
    swap(copy);
    return *this;
  }

  template< typename T >
  Array< T >& Array< T >::operator=(Array< T >&& other) noexcept
  {
    Array moved(std::move(other));
    swap(moved);
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
  T& Array< T >::front()
  {
    assert(size_ != 0);
    return *begin_;
  }

  template< typename T >
  const T& Array< T >::front() const noexcept
  {
    assert(size_ != 0);
    return *begin_;
  }

  template< typename T >
  T& Array< T >::back()
  {
    assert(size_ != 0);
    if (size_ptr_ == array_ && size_ != 0)
    {
      return *(array_ + capacity_ - 1);
    }
    return *(size_ptr_ - 1);
  }

  template< typename T >
  const T& Array< T >::back() const noexcept
  {
    assert(size_ != 0);
    if (size_ptr_ == array_ && size_ != 0)
    {
      return *(array_ + capacity_ - 1);
    }
    return *(size_ptr_ - 1);
  }

  template< typename T >
  void Array< T >::push_back(const T& value)
  {
    emplace_back(value);
  }

  template< typename T >
  void Array< T >::push_back(T&& value)
  {
    emplace_back(std::forward< T >(value));
  }

  template< typename T >
  void Array< T >::pop_front() noexcept
  {
    assert(size_ != 0);
    if (begin_ == array_ + capacity_ - 1)
    {
      begin_ = array_;
    }
    else
    {
      begin_++;
    }
    size_--;
  }

  template< typename T >
  void Array< T >::pop_back() noexcept
  {
    assert(size_ != 0);
    if (size_ptr_ == array_ && size_ != 0)
    {
      size_ptr_ = array_ + capacity_;
    }
    size_ptr_--;
    size_--;
  }

  template< typename T >
  template< typename... Args >
  void Array< T >::emplace_back(Args&&... args)
  {
    T* capacity_ptr = array_ + capacity_ - 1;
    if ((size_ptr_ - 1) == capacity_ptr)
    {
      size_ptr_ = array_;
    }
    if (size_ptr_ == begin_ && size_ != 0)
    {
      int ratio = 2;
      T* array_new = new T[capacity_ * 2]{};
      size_t i = 0;
      try
      {
        for (T* ptr = begin_; ptr != capacity_ptr; ptr++)
        {
          array_new[i] = *ptr;
          i++;
        }
        array_new[i] = *capacity_ptr;
        i++;
        for (T* ptr = array_; ptr != begin_; ptr++)
        {
          array_new[i] = *ptr;
          i++;
        }
      }
      catch (...)
      {
        delete[] array_new;
        throw;
      }
      clear();
      capacity_ *= ratio;
      array_ = array_new;
      begin_ = array_;
      size_ptr_ = array_ + size_;
    }
    *size_ptr_ = T{std::forward< Args >(args)...};
    size_ptr_++;
    size_++;
  }

  template< typename T >
  void Array< T >::swap(Array< T >& other)
  {
    std::swap(array_, other.array_);
    std::swap(begin_, other.begin_);
    std::swap(size_ptr_, other.size_ptr_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  template< typename T >
  void Array< T >::clear() noexcept
  {
    delete[] array_;
  }
}
#endif
