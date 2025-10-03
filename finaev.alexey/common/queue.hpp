#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include <utility>
#include "dynamicArr.hpp"

namespace finaev
{
  template< class T >
  class Queue
  {
  public:
    size_t size() const noexcept;
    bool isEmpty() const noexcept;

    void push(const T&);
    void pop();
    const T& top() const;
    T& top();
  private:
    DynamicArr< T > arr_;
  };

  template< class T >
  bool Queue< T >::isEmpty() const noexcept
  {
    return arr_.empty();
  }

  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return arr_.size();
  }

  template< class T >
  void Queue< T >::push(const T& rhs)
  {
    arr_.push(rhs);
  }

  template< class T >
  void Queue< T >::pop()
  {
    arr_.pop_front();
  }

  template< class T >
  T& Queue< T >::top()
  {
    return arr_.front();
  }

  template< class T >
  const T& Queue< T >::top() const
  {
    return arr_.front();
  }
}

#endif
