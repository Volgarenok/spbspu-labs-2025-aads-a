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
    T& front() noexcept;
    const T& front() const noexcept;
    T& back() noexcept;
    const T& back() const noexcept;
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_front() noexcept;
    void pop_back() noexcept;
    template< typename... Args >
    void emplace_back(Args&&... args);
  private:
    size_t size_;
    size_t capacity_;
    T* array_;
    T* begin_;
    T* size_ptr_;

    void clear() noexcept;
  };

  template< typename T >
  Array< T >::Array():
    size_(0),
    capacity_(10),
    array_(new T[capacity_]{}),
    begin_(array_),
    size_ptr_(array_)
  {}

  template< typename T >
  Array< T >::Array(const Array< T >& other):
    size_(other.size_),
    capacity_(other.capacity_),
    array_(new T[capacity_]{}),
    begin_(array_),
    size_ptr_(array_)
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
    catch(const std::exception& e)
    {
      clear();
      throw;
    }
  }

  template< typename T >
  Array< T >::Array(Array< T >&& other) noexcept:
    size_(std::exchange(other.size_, 0)),
    capacity_(std::exchange(other.capacity_, 10)),
    array_(std::exchange(other.array_, nullptr)),
    begin_(std::exchange(other.begin_, nullptr)),
    size_ptr_(std::exchange(other.size_ptr_, nullptr))
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
    std::swap(size_, copy.size_);
    std::swap(capacity_, copy.capacity_);
    std::swap(array_, copy.array_);
    std::swap(begin_, copy.begin_);
    std::swap(size_ptr_, copy.size_ptr_);
    return *this;
  }

  template< typename T >
  Array< T >& Array< T >::operator=(Array< T >&& other) noexcept
  {
    clear();
    size_ = std::exchange(other.size_, 0);
    capacity_ = std::exchange(other.capacity_, 10);
    array_ = std::exchange(other.array_, nullptr);
    begin_ = std::exchange(other.begin_, nullptr);
    size_ptr_ = std::exchange(other.size_ptr_, nullptr);
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
  T& Array< T >::back() noexcept
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
      catch (const std::exception&)
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
  void Array< T >::clear() noexcept
  {
    delete[] array_;
  }
}
#endif
