#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "dynamic-array.hpp"

namespace savintsev
{
  template< typename T >
  class Queue
  {
  public:
    Queue();

    bool empty() const noexcept;

    void pop() noexcept;
    template< typename U >
    void push(U && rhs);

    size_t size() const noexcept;

    T & back();
    const T & back() const;
    T & front();
    const T & front() const;

    template< typename U >
    friend void swap(Queue< U > & x, Queue< U > & y) noexcept;
  private:
    Array< T > queue_;
  };

  template< typename U >
  void swap(Queue< U > & x, Queue< U > & y) noexcept
  {
    swap(x.queue_, y.queue_);
  }

  template< typename T >
  Queue< T >::Queue():
    queue_{2}
  {}

  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return queue_.empty();
  }

  template< typename T >
  void Queue< T >::pop() noexcept
  {
    queue_.pop_front();
  }

  template< typename T >
  template< typename U >
  void Queue< T >::push(U && rhs)
  {
    queue_.push_back(std::forward< U >(rhs));
  }

  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return queue_.size();
  }

  template< typename T >
  T & Queue< T >::back()
  {
    return queue_.back();
  }

  template< typename T >
  const T & Queue< T >::back() const
  {
    return queue_.back();
  }

  template< typename T >
  T & Queue< T >::front()
  {
    return queue_.front();
  }

  template< typename T >
  const T & Queue< T >::front() const
  {
    return queue_.front();
  }
}

#endif
