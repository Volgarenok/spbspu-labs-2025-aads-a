#ifndef EXPRESSION_QUEUE_HPP
#define EXPRESSION_QUEUE_HPP

#include "dynamic-array-utils.hpp"

namespace guseynov
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
    size_t currentSize_;
    size_t capacity_;
    size_t frontIndex_;
    T * data_;
  };

  template< typename T >
  Queue< T >::Queue():
    currentSize_(0),
    capacity_(1),
    frontIndex_(0),
    data_(new T[capacity_])
  {}

  template< typename T >
  Queue< T >::Queue(const Queue< T > & other):
    currentSize_(other.currentSize_),
    capacity_(other.capacity_),
    frontIndex_(other.frontIndex_),
    data_(new T[capacity_])
  {
    try
    {
      for (size_t i = 0; i < currentSize_; i++)
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
    currentSize_(other.currentSize_),
    capacity_(other.capacity_),
    frontIndex_(other.frontIndex_),
    data_(other.data_)
  {
    other.currentSize_ = 0;
    other.capacity_ = 0;
    other.frontIndex_ = 0;
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
    if (currentSize_ == capacity_)
    {
      data_ = resizeArray(data_, capacity_);
    }
    data_[currentSize_++] = value;
  }

  template< typename T >
  void Queue< T >::pop()
  {
    if (empty())
    {
      throw std::logic_error("Queue is empty");
    }
    frontIndex_++;
    currentSize_--;
  }

  template< typename T >
  T & Queue< T >::front()
  {
    if (empty())
    {
      throw std::logic_error("Queue is empty");
    }
    return data_[frontIndex_];
  }

  template< typename T >
  const T & Queue< T >::front() const
  {
    if (empty())
    {
      throw std::logic_error("Queue is empty");
    }
    return data_[frontIndex_];
  }

  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return currentSize_ == 0;
  }

  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return currentSize_;
  }

  template< typename T >
  void Queue< T >::swap(Queue< T > & other) noexcept
  {
    std::swap(currentSize_, other.currentSize_);
    std::swap(capacity_, other.capacity_);
    std::swap(frontIndex_, other.frontIndex_);
    std::swap(data_, other.data_);
  }
}

#endif
