#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <stdexcept>

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
    T & back();
    const T & back() const;

    bool empty() const noexcept;
    size_t size() const noexcept;
  private:
    size_t size_;
    size_t capacity_;
    T * data_;

    void resize();
  };

  template< typename T >
  Queue< T >::Queue():
    size_(0),
    capacity_(50),
    data_(new T[capacity_])
  {}

  template< typename T >
  Queue< T >::Queue(const Queue< T > & other):
    size_(other.size_),
    capacity_(other.capacity_),
    data_(new T[capacity_])
  {
    for (size_t i = 0; i < size_; i++)
    {
      data_[i] = other.data_[i];
    }
  }

  template< typename T >
  Queue< T >::Queue(Queue< T > && other):
    size_(other.size_),
    capacity_(other.capacity_),
    data_(other.data_)
  {
    other.size_ = 0;
    other.capacity_ = 0;
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
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
      data_ = other.data_;
      other.data_ = nullptr;
    }
    return *this;
  }

  template< typename T >
  void Queue< T >::push(const T & value)
  {
    if (size_ == capacity_)
    {
      resize();
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
    for (size_t i = 1; i < size_; i++)
    {
      data_[i - 1] = data_[i];
    }
    size_--;
  }

  template< typename T >
  T & Queue< T >::front()
  {
    if (empty())
    {
      throw std::logic_error("empty queue");
    }
    return data_[0];
  }

  template< typename T >
  const T & Queue< T >::front() const
  {
    if (empty())
    {
      throw std::logic_error("empty queue");
    }
    return data_[0];
  }

  template< typename T >
  T & Queue< T >::back()
  {
    if (empty())
    {
      throw std::logic_error("empty queue");
    }
    return data_[size_ - 1];
  }

  template< typename T >
  const T & Queue< T >::back() const
  {
    if (empty())
    {
      throw std::logic_error("empty queue");
    }
    return data_[size_ - 1];
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
  void Queue< T >::resize()
  {
    capacity_ *= 2;
    T * temp = new T[capacity_];
    for (size_t i = 0; i < size_; i++)
    {
      temp[i] = data_[i];
    }
    delete[] data_;
    data_ = temp;
  }
}

#endif
