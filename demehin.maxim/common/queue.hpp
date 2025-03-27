#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <utility>
#include <cstddef>
#include <stdexcept>
#include "data_utils.hpp"

namespace demehin
{
  template< typename T >
  class Queue
  {
  public:

    Queue();
    Queue(const Queue&);
    Queue(Queue&&) noexcept;

    Queue< T >& operator=(const Queue< T >&);

    ~Queue();

    void push(const T& rhs);
    void pop();
    const T& front() const;
    T& front();
    const T& back() const;
    T& back();

    bool empty() const noexcept;
    size_t size() const noexcept;

    void clear();

  private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void resize();
  };

  template< typename T >
  Queue< T >::Queue():
    data_(nullptr),
    size_(0),
    capacity_(100)
  {
    data_ = new T[capacity_];
  }

  template< typename T >
  Queue< T >::Queue(const Queue& rhs):
  data_(nullptr),
  size_(rhs.size_),
  capacity_(rhs.capacity_)
  {
    data_ = details::copyData(rhs.data_, rhs.size_);
  }

  template< typename T >
  Queue< T >::Queue(Queue&& rhs) noexcept:
    data_(std::exchange(rhs.data_, nullptr)),
    size_(std::exchange(rhs.size_, 0)),
    capacity_(std::exchange(rhs.capacity_, 0))
  {}

  template< typename T >
  Queue< T >& Queue< T >::operator=(const Queue< T >& rhs)
  {
    if (this != &rhs)
    {
      Queue< T > temp(rhs);
      std::swap(data_, temp.data_);
      std::swap(size_, temp.size_);
      std::swap(capacity_, temp.capacity_);
    }
    return *this;
  }

  template< typename T >
  Queue< T >::~Queue()
  {
    delete[] data_;
  }

  template< typename T >
  void Queue< T >::push(const T& rhs)
  {
    if (size_ == capacity_)
    {
      resize();
    }

    data_[size_++] = rhs;
  }

  template< typename T >
  void Queue< T >::pop()
  {
    if (empty())
    {
      throw std::logic_error("empty");
    }

    for (size_t i = 1; i < size_; i++)
    {
      data_[i - 1] = data_[i];
    }
    size_--;
  }

  template< typename T >
  const T& Queue< T >::back() const
  {
    if (empty())
    {
      throw std::logic_error("empty");
    }
    return data_[size_ - 1];
  }

  template< typename T >
  T& Queue< T >::back()
  {
    if (empty())
    {
      throw std::logic_error("empty");
    }
    return data_[size_ - 1];
  }

  template< typename T >
  const T& Queue< T >::front() const
  {
    if (empty())
    {
      throw std::logic_error("empty");
    }
    return data_[0];
  }

  template< typename T >
  T& Queue< T >::front()
  {
    if (empty())
    {
      throw std::logic_error("empty");
    }
    return data_[0];
  }

  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  void Queue< T >::resize()
  {
    capacity_ *= 2;
    T* newData = details::copyData(data_, capacity_);
    delete[] data_;
    data_ = newData;
  }

  template< typename T >
  void Queue< T >::clear()
  {
    while (!empty())
    {
      pop();
    }
  }
}

#endif
