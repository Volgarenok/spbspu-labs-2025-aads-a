#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>

namespace petrov
{
  template< typename T >
  struct Queue
  {
  public:
    using this_t = Queue< T >;
    Queue():
      massive_(nullptr),
      capacity_(0),
      size_of_queue_(0)
    {}
    Queue(const this_t & rhs);
    Queue(this_t && rhs);
    ~Queue();
    this_t & operator=(const this_t & rhs);
    this_t & operator=(this_t && rhs);
    void push(const T & element);
    void pop();
    T & front();
    const T & front() const;
    T & back();
    const T & back() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void swap(this_t & rhs) noexcept;
  private:
    T * massive_;
    size_t capacity_;
    size_t size_of_queue_;
  };

  template< typename T >
  Queue< T >::Queue(const this_t & rhs):
    massive_(nullptr),
    capacity_(rhs.capacity_),
    size_of_queue_(rhs.size_of_queue_)
  {
    massive_ = new T[rhs.size_of_queue_];
    for (size_t i = 0; i < rhs.size_of_queue_; i++)
    {
      massive_[i] = rhs.massive_[i];
    }
  }

  template< typename T >
  Queue< T >::Queue(this_t && rhs):
    massive_(nullptr),
    capacity_(rhs.capacity_),
    size_of_queue_(rhs.size_of_queue_)
  {
    massive_ = new T[rhs.size_of_queue_];
    for (size_t i = 0; i < rhs.size_of_queue_; i++)
    {
      massive_[i] = rhs.massive_[i];
    }
  }

  template< typename T >
  Queue< T >::~Queue()
  {
    delete[] massive_;
  }

  template< typename T >
  typename Queue< T >::this_t & Queue< T >::operator=(const this_t & rhs)
  {
    Queue< T > cpy(rhs);
    this->swap(cpy);
    return *this;
  }

  template< typename T >
  typename Queue< T >::this_t & Queue< T >::operator=(this_t && rhs)
  {
    this->swap(rhs);
    return *this;
  }

  template< typename T >
  void Queue< T >::push(const T & element)
  {
    if (this->empty())
    {
      delete[] massive_;
      capacity_ = 5;
      massive_ = new T[capacity_];
      massive_[size_of_queue_++] = element;
    }
    else if (size_of_queue_ == capacity_)
    {
      T * temp = new T[capacity_ *= 2];
      size_t i = 0;
      while (i < size_of_queue_)
      {
        temp[i] = massive_[i];
        ++i;
      }
      delete[] massive_;
      massive_ = temp;
      massive_[i] = element;
      size_of_queue_++;
    }
    else
    {
      massive_[size_of_queue_++] = element;
    }
  }

  template< typename T >
  void Queue< T >::pop()
  {
    size_of_queue_--;
  }

  template< typename T >
  T & Queue< T >::front()
  {
    return massive_[size_of_queue_ - 1];
  }

  template< typename T >
  const T & Queue< T >::front() const
  {

  }

  template< typename T >
  T & Queue< T >::back()
  {

  }

  template< typename T >
  const T & Queue< T >::back() const
  {

  }

  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return !size_of_queue_;
  }

  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return size_of_queue_;
  }

  template< typename T >
  void Queue< T >::swap(this_t & rhs) noexcept
  {
    std::swap(massive_, rhs.massive_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(size_of_queue_, rhs.size_of_queue_);
  }
}

#endif
