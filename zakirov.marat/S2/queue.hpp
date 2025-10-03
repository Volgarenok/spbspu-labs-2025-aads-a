#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace zakirov
{
  template < class T >
  class Queue
  {
  public:
    Queue() noexcept;
    Queue(const Queue< T > & other);
    Queue(Queue< T > && other) noexcept;
    ~Queue();
    Queue< T > & operator=(const Queue< T > & other);
    Queue< T > & operator=(Queue< T > && other) noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    T & front() noexcept;
    const T & front() const noexcept;
    T & back() noexcept;
    const T & back() const noexcept;
    void push(const T & value);
    void push(T && value);
    void pop() noexcept;
    void swap(Queue & other) noexcept;
  private:
    template < class U >
    void uni_push(U && value);
    void add_capacity();
    T * data_;
    size_t first_;
    size_t size_;
    size_t capacity_;
  };

  template < class T >
  Queue< T >::Queue() noexcept:
    data_(nullptr),
    first_(0),
    size_(0),
    capacity_(0)
  {}

  template < class T >
  Queue< T >::Queue(const Queue< T > & other):
    data_(new T[other.capacity_]),
    first_(other.first_),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    try
    {
      for (size_t i = first_; i < first_ + size_; ++i)
      {
        data_[i] = other.data_[i];
      }
    }
    catch (...)
    {
      delete[] data_;
      throw;
    }
  }

  template < class T >
  Queue< T >::Queue(Queue< T > && other) noexcept:
    data_(std::exchange(other.data_, nullptr)),
    first_(std::exchange(other.first_, 0)),
    size_(std::exchange(other.size_, 0)),
    capacity_(std::exchange(other.capacity_, 0))
  {}

  template < class T >
  Queue< T >::~Queue()
  {
    delete[] data_;
  }

  template < class T >
  Queue< T > & Queue< T >::operator=(const Queue< T > & other)
  {
    if (this != std::addressof(other))
    {
      Queue< T > queue_temp(other);
      swap(queue_temp);
    }

    return *this;
  }

  template < class T >
  Queue< T > & Queue< T >::operator=(Queue< T > && other) noexcept
  {
    if (this != std::addressof(other))
    {
      Queue< T > queue_temp(std::move(other));
      swap(queue_temp);
    }

    return *this;
  }

  template < class T >
  bool Queue< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < class T >
  size_t Queue< T >::size() const noexcept
  {
    return size_;
  }

  template < class T >
  T & Queue< T >::front() noexcept
  {
    return data_[first_];
  }

  template < class T >
  const T & Queue< T >::front() const noexcept
  {
    return data_[first_];
  }

  template < class T >
  T & Queue< T >::back() noexcept
  {
    return data_[first_ + size_ - 1];
  }

  template < class T >
  const T & Queue< T >::back() const noexcept
  {
    return data_[first_ + size_ - 1];
  }

  template < class T >
  template < class U >
  void Queue< T >::uni_push(U && value)
  {
    if (first_ + size_ >= capacity_)
    {
      add_capacity();
    }

    data_[first_ + size_] = std::forward< U >(value);
    ++size_;
  }

  template < class T >
  void Queue< T >::push(const T & value)
  {
    uni_push(value);
  }

  template < class T >
  void Queue< T >::push(T && value)
  {
    uni_push(std::move(value));
  }

  template < class T >
  void Queue< T >::pop() noexcept
  {
    ++first_;
    --size_;
  }

  template < class T >
  void Queue< T >::swap(Queue< T > & other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(first_, other.first_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  template < class T >
  void Queue< T >::add_capacity()
  {
    size_t new_capacity = capacity_ * 2 + 1;
    T * new_data = new T[new_capacity];
    try
    {
      for (size_t i = 0, j = first_; j < first_ + size_; ++i, ++j)
      {
        new_data[i] = data_[j];
      }
    }
    catch (...)
    {
      delete[] new_data;
      throw;
    }

    delete[] data_;
    data_ = new_data;
    first_ = 0;
    capacity_ = new_capacity;
  }
}

#endif
