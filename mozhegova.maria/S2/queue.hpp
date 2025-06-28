#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "resize.hpp"

namespace mozhegova
{
  template< typename T >
  class Queue
  {
  public:
    Queue();
    Queue(const Queue< T > &);
    Queue(Queue< T > &&) noexcept;
    ~Queue();

    Queue< T > & operator=(const Queue< T > &);
    Queue< T > & operator=(Queue< T > &&) noexcept;

    void push(const T & value);
    void pop();
    T & front();
    const T & front() const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void swap(Queue< T > &) noexcept;
  private:
    size_t size_;
    size_t capacity_;
    size_t first_;
    T * data_;
  };

  template< typename T >
  Queue< T >::Queue():
    size_(0),
    capacity_(1),
    first_(0),
    data_(new T[capacity_])
  {}

  template< typename T >
  Queue< T >::Queue(const Queue< T > & other):
    size_(other.size_),
    capacity_(other.capacity_),
    first_(other.first_),
    data_(new T[capacity_])
  {
    try
    {
      for (size_t i = 0; i < size_; i++)
      {
        data_[i] = other.data_[i];
      }
    }
    catch (const std::exception &)
    {
      delete[] data_;
      throw;
    }
  }

  template< typename T >
  Queue< T >::Queue(Queue< T > && other) noexcept:
    size_(other.size_),
    capacity_(other.capacity_),
    first_(other.first_),
    data_(other.data_)
  {
    other.size_ = 0;
    other.capacity_ = 0;
    other.first_ = 0;
    other.data_ = nullptr;
  }

  template< typename T >
  Queue< T >::~Queue()
  {
    delete[] data_;
  }

  template< typename T >
  Queue< T > & Queue< T >::operator=(const Queue< T > & other)
  {
    if (this != std::addressof(other))
    {
      Queue< T > copy(other);
      swap(copy);
    }
    return *this;
  }

  template< typename T >
  Queue< T > & Queue< T >::operator=(Queue< T > && other) noexcept
  {
    if (this != std::addressof(other))
    {
      Queue< T > copy(std::move(other));
      swap(copy);
    }
    return *this;
  }

  template< typename T >
  void Queue< T >::push(const T & value)
  {
    if (size_ == capacity_)
    {
      data_ = resize(data_, capacity_);
    }
    data_[size_++] = value;
  }

  template< typename T >
  void Queue< T >::pop()
  {
    if (empty())
    {
      throw std::logic_error("empty queue");
    }
    first_++;
    size_--;
  }

  template< typename T >
  T & Queue< T >::front()
  {
    if (empty())
    {
      throw std::logic_error("empty queue");
    }
    return data_[first_];
  }

  template< typename T >
  const T & Queue< T >::front() const
  {
    if (empty())
    {
      throw std::logic_error("empty queue");
    }
    return data_[first_];
  }

  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  void Queue< T >::swap(Queue< T > & other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(first_, other.first_);
    std::swap(data_, other.data_);
  }
}

#endif
