#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include <exception>

namespace krylov
{
  template< typename T >
  class Queue
  {
  public:
    Queue();
    Queue(const Queue< T >& other);
    Queue(Queue< T >&& other);
    ~Queue();
    Queue< T >& operator=(const Queue< T >& other);
    Queue< T >& operator=(Queue< T >&& other);
    bool empty() const noexcept;
    size_t size() const noexcept;
    const T& front() const noexcept;
    const T& back() const noexcept;
    T& front() noexcept;
    T& back() noexcept;
    void push(const T& value);
    void pop();
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
    capacity_(0)
  {}

  template< typename T >
  Queue< T >::~Queue()
  {
    delete[] data_;
  }

  template< typename T >
  Queue< T >::Queue(const Queue< T >& other):
    data_(nullptr),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    T* data = new T[other.capacity_];
    try
    {
      for (size_t i = 0; i < other.size(); ++i)
      {
        data[i] = other.data_[i];
      }
    }
    catch (const std::exception& e)
    {
      delete[] data;
    }
  }

  template< typename T >
  Queue< T >& Queue< T >::operator=(const Queue< T >& other)
  {
    if (this == &other)
    {
      return *this;
    }
    Queue< T > newQueue(other);
    data_ = newQueue.data_;
    size_ = newQueue.size_;
    capacity_ = newQueue.capacity_;
    return *this;
  }

  template< typename T >
  Queue< T >::Queue(Queue< T >&& other):
    data_(other.data_),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    other.data_ = nullptr;
  }

  template< typename T >
  Queue< T >& Queue< T >::operator=(Queue< T >&& other)
  {
    delete[] data_;
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
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
  T& Queue< T >::back() noexcept
  {
    return data_[size_-1];
  }

  template< typename T >
  const T& Queue< T >::back() const noexcept
  {
    return data_[size_-1];
  }

  template< typename T >
  T& Queue< T >::front() noexcept
  {
    return data_[0];
  }

  template< typename T >
  const T& Queue< T >::front() const noexcept
  {
    return data_[0];
  }

  template< typename T >
  void Queue< T >::push(const T& value)
  {
    if (size_ != capacity_)
    {
      data_[size_] = value;
    }
    else
    {
      resize();
      data_[size_] = value;
    }
    ++size_;
  }

  template< typename T >
  void Queue< T >::pop()
  {
    if (!empty())
    {
      for (size_t i = 0; i < size_; ++i)
      {
        data_[i] = data_[i + 1];
      }
      --size_;
    }
  }

  template< typename T >
  void Queue< T >::resize()
  {
    constexpr size_t k = 161;
    T* array = new T[capacity_ + k];
    for (size_t i = 0; i < size_; ++i)
    {
      array[i] = data_[i];
    }
    delete[] data_;
    data_ = array;
    capacity_ += k;
  }

}

#endif
