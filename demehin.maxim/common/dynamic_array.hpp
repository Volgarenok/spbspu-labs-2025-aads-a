#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP
#include <cstddef>
#include <utility>
#include <stdexcept>
#include "data_utils.hpp"
#include "dynamic_array_iterator.hpp"

namespace demehin
{
  template< typename T >
  class DynamicArray
  {
  public:
    using Iter = DynamicArrayIterator< T >;

    DynamicArray();
    explicit DynamicArray(size_t);
    DynamicArray(const DynamicArray&);
    DynamicArray(DynamicArray&&) noexcept;

    DynamicArray< T >& operator=(const DynamicArray< T >&);
    DynamicArray< T >& operator=(DynamicArray< T >&&);

    T& operator[](size_t) noexcept;
    const T& operator[](size_t) const noexcept;

    ~DynamicArray();

    void push(const T& rhs);

    void pop_back();
    void pop_front();

    const T& front() const;
    T& front();
    const T& back() const;
    T& back();

    Iter begin() const noexcept;
    Iter end() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void swap(DynamicArray&) noexcept;

    void clear();

  private:
    T* data_;
    size_t size_;
    size_t capacity_;
    size_t begin_;

    void resize();
  };

  template< typename T >
  DynamicArray< T >::DynamicArray():
    data_(nullptr),
    size_(0),
    capacity_(100),
    begin_(0)
  {
    data_ = new T[capacity_];
  }

  template< typename T >
  DynamicArray< T >::DynamicArray(size_t size):
    data_(new T[size]),
    size_(size),
    capacity_(size),
    begin_(0)
  {}

  template< typename T >
  DynamicArray< T >::DynamicArray(const DynamicArray& other):
    data_(details::copyData(other.data_, other.size_)),
    size_(other.size_),
    capacity_(other.capacity_),
    begin_(other.begin_)
  {}

  template< typename T >
  DynamicArray< T >::DynamicArray(DynamicArray&& other) noexcept:
    data_(std::exchange(other.data_, nullptr)),
    size_(std::exchange(other.size_, 0)),
    capacity_(std::exchange(other.capacity_, 0)),
    begin_(std::exchange(other.begin_, 0))
  {}

  template< typename T >
  DynamicArray< T >& DynamicArray< T >::operator=(const DynamicArray< T >& rhs)
  {
    if (this != std::addressof(rhs))
    {
      DynamicArray< T > temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  DynamicArray< T >& DynamicArray< T >::operator=(DynamicArray< T >&& rhs)
  {
    if (this != std::addressof(rhs))
    {
      DynamicArray< T > temp(std::move(rhs));
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  T& DynamicArray< T >::operator[](size_t pos) noexcept
  {
    return data_[pos + begin_];
  }

  template< typename T >
  const T& DynamicArray< T >::operator[](size_t pos) const noexcept
  {
    return data_[pos + begin_];
  }

  template< typename T >
  DynamicArray< T >::~DynamicArray()
  {
    delete[] data_;
  }

  template< typename T >
  void DynamicArray< T >::push(const T& value)
  {
    if (size_ == capacity_)
    {
      resize();
    }
    data_[size_++] = value;
  }

  template< typename T >
  void DynamicArray< T >::pop_back()
  {
    if (empty())
    {
      throw std::logic_error("empty");
    }
    size_--;
  }

  template< typename T >
  void DynamicArray< T >::pop_front()
  {
    if (empty())
    {
      throw std::logic_error("empty");
    }
    begin_++;
    size_--;
  }

  template< typename T >
  const T& DynamicArray< T >::front() const
  {
    if (empty())
    {
      throw std::logic_error("empty");
    }
    return data_[begin_];
  }

  template< typename T >
  T& DynamicArray< T >::front()
  {
    if (empty())
    {
      throw std::logic_error("empty");
    }
    return data_[begin_];
  }

  template< typename T >
  const T& DynamicArray< T >::back() const
  {
    if (empty())
    {
      throw std::logic_error("empty");
    }
    return data_[begin_ + size_ - 1];
  }

  template< typename T >
  T& DynamicArray< T >::back()
  {
    if (empty())
    {
      throw std::logic_error("empty");
    }
    return data_[begin_ + size_ - 1];
  }

  template< typename T >
  typename DynamicArray< T >::Iter DynamicArray< T >::begin() const noexcept
  {
    return Iter(data_ + begin_);
  }

  template< typename T >
  typename DynamicArray< T >::Iter DynamicArray< T >::end() const noexcept
  {
    return Iter(data_ + begin_ + size_);
  }

  template< typename T >
  void DynamicArray< T >::swap(DynamicArray< T >& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(begin_, other.begin_);
  }

  template< typename T >
  bool DynamicArray< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t DynamicArray< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  void DynamicArray< T >::clear()
  {
    while (!empty())
    {
      pop_back();
    }
  }

  template< typename T >
  void DynamicArray< T >::resize()
  {
    capacity_ *= 2;
    T* newData = details::copyData(data_, capacity_);
    delete[] data_;
    data_ = newData;
  }
}


#endif
