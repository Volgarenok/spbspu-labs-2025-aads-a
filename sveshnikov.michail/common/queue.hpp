#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include <cassert>
#include <algorithm>
#include "array.hpp"

namespace sveshnikov
{
  template< class T >
  class Queue
  {
  public:
    T &front() noexcept;
    const T &front() const noexcept;
    size_t getSize() const noexcept;
    bool empty() const noexcept;
    void push(const T &val);
    void push(T &&val);
    void pop();
    void swap(Queue &other) noexcept;

  private:
    Array< T > data_;
  };

  template< class T >
  T &Queue< T >::front() noexcept
  {
    return data_.front();
  }

  template< class T >
  const T &Queue< T >::front() const noexcept
  {
    return data_.front();
  }

  template< class T >
  size_t Queue< T >::getSize() const noexcept
  {
    return data_.getSize();
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return data_.empty();
  }

  template< class T >
  void Queue< T >::push(const T &val)
  {
    data_.push_back(val);
  }

  template< class T >
  void Queue< T >::push(T &&val)
  {
    data_.push_back(std::forward< T >(val));
  }

  template< class T >
  void Queue< T >::pop()
  {
    data_.pop_front();
  }

  template< class T >
  void Queue< T >::swap(Queue &other) noexcept
  {
    data_.swap(other.data_);
  }
}

#endif
