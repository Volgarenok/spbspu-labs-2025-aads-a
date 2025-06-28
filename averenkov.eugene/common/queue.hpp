#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "array.hpp"

namespace averenkov
{
  template< class T >
  class Queue
  {
  public:
    bool empty() const noexcept;
    size_t size() const noexcept;

    const T& front() const noexcept;
    T& front() noexcept;

    void push(const T& rhs) noexcept;
    void pop();
  private:
    Array< T > arr_;
  };

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
  void Queue< T >::pop()
  {
    arr_.pop_front();
  }

}

#endif
