#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <stdexcept>

namespace maslov
{
  template< typename T >
  struct Queue
  {
    Queue();
    Queue(const Queue< T > & rhs);
    Queue(Queue< T > && rhs) noexcept;
    ~Queue();
    Queue< T > & operator=(const Queue< T > & rhs);
    Queue< T > & operator=(Queue< T > && rhs) noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void push(const T & data);
    void pop();
    T & front();
    const T & front() const;
    void swap(Queue< T > & rhs) noexcept;
   private:
    size_t capacity_;
    size_t size_;
    size_t head_;
    T * data_;
  };

  template< typename T >
  Queue< T >::Queue():
    capacity_(0),
    size_(0),
    head_(0),
    data_(new T[capacity_])
  {}

  template< typename T >
  Queue< T >::Queue(const Queue< T > & rhs):
    capacity_(rhs.capacity_),
    size_(rhs.size_),
    head_(rhs.head_),
    data_(new T[rhs.capacity_])
  {
    try
    {
      for (size_t i = 0; i < size_; ++i)
      {
        data_[i] = rhs.data_[i];
      }
    }
    catch (const std::exception &)
    {
      delete[] data_;
      throw;
    }
  }

  template< typename T >
  Queue< T >::Queue(Queue< T > && rhs) noexcept:
    capacity_(rhs.capacity_),
    size_(rhs.size_),
    head_(rhs.head_),
    data_(rhs.data_)
  {
    rhs.capacity_ = 0;
    rhs.size_ = 0;
    rhs.head_ = 0;
    rhs.data_ = nullptr;
  }

  template< typename T >
  Queue< T >::~Queue()
  {
    delete[] data_;
  }

  template< typename T >
  Queue< T > & Queue< T >::operator=(const Queue< T > & rhs)
  {
    if (this != std::addressof(rhs))
    {
      Queue< T > temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  Queue< T > & Queue< T >::operator=(Queue< T > && rhs) noexcept
  {
    if (this != std::addressof(rhs))
    {
      delete[] data_;
      capacity_ = rhs.capacity_;
      size_ = rhs.size_;
      head_ = rhs.head_;
      data_ = rhs.data_;
      rhs.capacity_ = 0;
      rhs.size_ = 0;
      rhs.head_ = 0;
      rhs.data_ = nullptr;
    }
    return *this;
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
  void Queue< T >::push(const T & data)
  {
    if (size_ == capacity_)
    {
      T * newData = nullptr;
      try
      {
        size_t newCapacity = 1;
        if (capacity_ != 0)
        {
          newCapacity = capacity_ * 2;
        }
        newData = new T[newCapacity];
        for (size_t i = 0; i < size_; ++i)
        {
          newData[i] = data_[i];
        }
        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
      }
      catch (const std::exception &)
      {
        delete[] newData;
        throw;
      }
    }
    data_[size_++] = data;
  }

  template< typename T >
  void Queue< T >::pop()
  {
    if (empty())
    {
      throw std::runtime_error("ERROR: empty queue, cannot pop");
    }
    head_ = head_ + 1;
    --size_;
  }

  template< typename T >
  T & Queue< T >::front()
  {
    return const_cast< T & >(static_cast< const Queue< T > & >(*this).front());
  }

  template< typename T >
  const T & Queue< T >::front() const
  {
    if (empty())
    {
      throw std::runtime_error("ERROR: empty queue, cannot front");
    }
    return data_[head_];
  }

  template< typename T >
  void Queue< T >::swap(Queue< T > & rhs) noexcept
  {
    std::swap(capacity_, rhs.capacity_);
    std::swap(size_, rhs.size_);
    std::swap(head_, rhs.head_);
    std::swap(data_, rhs.data_);
  }
}

#endif
