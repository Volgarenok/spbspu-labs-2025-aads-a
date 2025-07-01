#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "array.hpp"

namespace averenkov
{
  template< class T >
  class Queue
  {
  public:
    Queue() = default;
    ~Queue() = default;
    Queue(const Queue &rhs);
    Queue(Queue &&rhs) noexcept;
    Queue &operator=(const Queue &rhs);
    bool empty() const noexcept;
    size_t size() const noexcept;

    const T& front() const noexcept;
    T& front() noexcept;

    void push(const T& rhs) noexcept;
    T drop();

  private:
    Array< T > arr_;
  };

  template< class T >
  Queue< T >::Queue(const Queue &rhs):
    arr_(rhs.arr_)
  {}

  template< class T >
  Queue< T >::Queue(Queue &&rhs) noexcept:
    arr_(std::move(rhs.arr_))
  {}

  template< class T >
  Queue< T > &Queue< T >::operator=(const Queue &rhs)
  {
    if (this != std::addressof(rhs))
    {
      arr_ = rhs.arr_;
    }
    return *this;
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return arr_.empty();
  }

  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return arr_.size();
  }

  template< class T >
  const T& Queue< T >::front() const noexcept
  {
    return arr_.front();
  }

  template< class T >
  T& Queue< T >::front() noexcept
  {
    return arr_.front();
  }

  template< class T >
  void Queue< T >::push(const T& rhs) noexcept
  {
    arr_.push_back(rhs);
  }

  template< class T >
  T Queue< T >::drop()
  {
    if (arr_.empty())
    {
      throw std::underflow_error("error");
    }
    T temp = front();
    arr_.pop_front();
    return temp;
  }

}

#endif
