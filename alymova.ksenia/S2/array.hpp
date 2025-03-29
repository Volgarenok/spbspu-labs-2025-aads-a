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
    Array(const T& other);
    Array(T&& other);
    ~Array();

    Array& operator=(const T& other);
    Array& operator=(T&& other);

    size_t size() const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_front();
    void pop_back();
    template< typename... Args >
    void emplace(Args&&... args);
  private:
    size_t size_;
    size_t capacity_;
    T* array_;

    void clear();
  };

  template< typename T >
  Array< T >::Array():
    size_(0),
    capacity_(10),
    array_(new T[capacity_])
  {}

  template< typename T >
  Array< T >::Array(const T& other):
    size_(other.size_),
    capacity_(other.capacity_),
    array_(new T[capacity_])
  {
    try
    {
      for (size_t i = 0; i < size_; i++)
      {
        array_[i] = other.array_[i];
      }
    }
    catch(const std::bad_alloc& e)
    {
      clear();
      throw;
    }
  }

  template< typename T >
  Array< T >::Array(T&& other):
    size_(std::exchange(other.size_, 0)),
    capacity_(std::exchange(other.capacity_, 10)),
    array_(std::exchange(other.array_, nullptr))
  {}

  template< typename T >
  Array< T >::~Array()
  {
    clear();
  }

  template< typename T >
  Array< T >& Array< T >::operator=(const T& other)
  {
    Array copy(other);
    std::swap(size_, copy.size_);
    std::swap(capacity_, copy.capacity_);
    std::swap(array_, copy.array_);
    return *this;
  }

  template< typename T >
  Array< T >& Array< T >::operator=(T&& other)
  {
    clear();
    size_ = std::exchange(other.size_, 0);
    capacity_ = std::exchange(other.capacity_, 10);
    array_ = std::exchange(other.array_, nullptr);
    return *this;
  }

  template< typename T >
  size_t Array< T >::size() const
  {
    return size_;
  }

  template< typename T >
  T& Array< T >::front()
  {
    assert(size_ != 0);
    return array_[0];
  }

  template< typename T >
  const T& Array< T >::front() const
  {
    assert(size_ != 0);
    return array_[0];
  }

  template< typename T >
  T& Array< T >::back()
  {
    assert(size_ != 0);
    return array_[size_ - 1];
  }

  template< typename T >
  const T& Array< T >::back() const
  {
    assert(size_ != 0);
    return array_[size_ - 1];
  }

  template< typename T >
  void Array< T >::push_back(const T& value)
  {
    emplace(value);
  }

  template< typename T >
  void Array< T >::push_back(T&& value)
  {
    emplace(std::forward< T >(value));
  }

  template< typename T >
  void Array< T >::pop_front()
  {
    assert(size_ != 0);
    for (size_t i = 0; i < size_ - 1; i++)
    {
      array_[i] = array_[i + 1];
    }
    size_--;
  }

  template< typename T >
  void Array< T >::pop_back()
  {
    assert(size_ != 0);
    size_--;
  }

  template< typename T >
  template< typename... Args >
  void Array< T >::emplace(Args&&... args)
  {
    if (size_ == capacity_)
    {
      int ratio = 2;
      T* array_new = new T[capacity_ * ratio];
      for (size_t i = 0; i < size_; i++)
      {
        array_new[i] = array_[i];
      }
      clear();
      array_ = array_new;
      capacity_ *= ratio;
    }
    array_[size_] = T{std::forward< Args >(args)...};
    size_++;
  }

  template< typename T >
  void Array< T >::clear()
  {
    delete[] array_;
  }
}
#endif
