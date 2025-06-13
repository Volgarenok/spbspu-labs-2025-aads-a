#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "array.hpp"

namespace bocharov
{
  template< typename T >
  class Queue
  {
  public:
    T & front();
    const T & front() const;

    void pop() noexcept;
    void push(const T &);
    void push(T &&);

    size_t size() const noexcept;
    bool empty() const noexcept;
    void clear() noexcept;

  private:
    Array< T > arr_;
  };

  template< typename T >
  T & Queue< T >::front()
  {
    return arr_.front();
  }

  template< typename T >
  const T & Queue< T >::front() const
  {
    return arr_.front();
  }

  template< typename T >
  void Queue< T >::pop() noexcept
  {
    arr_.popFront();
  }

  template< typename T >
  void Queue< T >::push(const T & data)
  {
    arr_.push(data);
  }

  template< typename T >
  void Queue< T >::push(T && data)
  {
    arr_.push(std::move(data));
  }

  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return arr_.size();
  }

  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return arr_.empty();
  }

  template< typename T >
  void Queue< T >::clear() noexcept
  {
    arr_.clear();
  }
}

#endif
