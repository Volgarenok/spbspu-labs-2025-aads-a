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
    Queue(Queue< T > &&);
    ~Queue();

    Queue< T > & operator=(const Queue< T > &);
    Queue< T > & operator=(Queue< T > &&);

    void push(const T & value);
    void pop();
    T & front();
    const T & front() const;

    bool empty() const noexcept;
    size_t size() const noexcept;
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
    catch(const std::exception &)
    {
      delete[] data_;
      throw;
    }
  }

  template< typename T >
  Queue< T >::Queue(Queue< T > && other):
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
    if (this != &other)
    {
      Queue< T > copy(other);
      std::swap(size_, copy.size_);
      std::swap(capacity_, copy.capacity_);
      std::swap(first_, copy.first_);
      std::swap(data_, copy.data_);
    }
    return *this;
  }

  template< typename T >
  Queue< T > & Queue< T >::operator=(Queue< T > && other)
  {
    if (this != &other)
    {
      delete[] data_;
      capacity_ = other.capacity_;
      size_ = other.size_;
      first_ = other.first_;
      data_ = other.data_;
      other.capacity_ = 0;
      other.size_ = 0;
      other.first_ = 0;
      other.data_ = nullptr;
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
}

#endif
