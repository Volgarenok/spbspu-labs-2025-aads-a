#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "array.hpp"

namespace lanovenko
{
  template< typename T >
  class Queue
  {
  public:
    Queue(size_t max_size = 1'000);
    template< typename U >
    void push(U&& data);
    void pop() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    T& front() noexcept;
    const T& front() const noexcept;
    void swap(Queue& rhs) noexcept;
  private:
    Array< T > data_;
  };

  template< typename T >
  Queue< T >::Queue(size_t max_size):
    data_(Array< T >(max_size))
  {}

  template< typename T >
  template< typename U >
  void Queue< T >::push(U&& data)
  {
    data_.push(std::forward< U >(data));
  }

  template< typename T >
  void Queue< T >::pop() noexcept
  {
    data_.pop_front();
  }

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
  T& Queue< T >::front() noexcept
  {
    return data_.front();
  }

  template< typename T >
  const T& Queue< T >::front() const noexcept
  {
    return data_.front();
  }

  template< typename T >
  void Queue< T >::swap(Queue& rhs) noexcept
  {
    data_.swap(rhs.data_);
  }
}

#endif
