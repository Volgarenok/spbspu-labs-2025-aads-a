#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "array.hpp"

namespace duhanina
{
  template < typename T >
  class Queue
  {
  public:
    bool empty() const noexcept;
    void push(const T& value);
    void pop();
    const T& front() const;
    T& front();
    size_t size() const noexcept;

  private:
    DynamicArray< T > array_;
  };

  template < typename T >
  bool Queue< T >::empty() const noexcept
  {
    return array_.empty();
  }

  template < typename T >
  void Queue< T >::push(const T& value)
  {
    array_.push_back(value);
  }

  template < typename T >
  void Queue< T >::pop()
  {
    array_.pop_front();
  }

  template < typename T >
  const T& Queue< T >::front() const
  {
    return array_.front();
  }

  template < typename T >
  T& Queue< T >::front()
  {
    return array_.front();
  }

  template < typename T >
  size_t Queue< T >::size() const noexcept
  {
    return array_.size();
  }
}

#endif
