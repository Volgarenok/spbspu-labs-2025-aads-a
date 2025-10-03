#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include "array.hpp"

namespace alymova
{
  template< typename T >
  struct Queue
  {
    bool empty() const noexcept;
    size_t size() const noexcept;
    T& front();
    const T& front() const noexcept;
    void push(const T& value);
    void push(T&& value);
    void pop() noexcept;

  private:
    Array< T > data_;
  };

  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return data_.empty();
  }

  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return data_.size();
  }

  template< typename T >
  T& Queue< T >::front()
  {
    return data_.front();
  }

  template< typename T >
  const T& Queue< T >::front() const noexcept
  {
    return data_.front();
  }

  template< typename T >
  void Queue< T >::push(const T& value)
  {
    data_.push_back(value);
  }

  template< typename T >
  void Queue< T >::push(T&& value)
  {
    data_.push_back(std::forward< T >(value));
  }

  template< typename T >
  void Queue< T >::pop() noexcept
  {
    data_.pop_front();
  }
}
#endif
