#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <list>
#include <cstddef>
#include <cassert>

namespace aleksandrov
{
  template< typename T >
  class Queue
  {
  public:
    Queue() = default;
    Queue(const Queue< T >&);
    Queue(Queue< T >&&) noexcept;

    Queue< T >& operator=(const Queue< T >&);
    Queue< T >& operator=(Queue< T >&&) noexcept;

    void push(const T&);
    void push(T&&);
    T drop();
    bool empty();
    size_t size();
    T& front();
  private:
    std::list< T > list_;
  };

  template< typename T >
  Queue< T >::Queue(const Queue< T >& rhs):
    list_(rhs.list_)
  {}

  template< typename T >
  Queue< T >::Queue(Queue< T >&& rhs) noexcept:
    list_(std::move(rhs.list_))
  {}

  template< typename T >
  Queue< T >& Queue< T >::operator=(const Queue< T >& rhs)
  {
    list_ = rhs.list_;
    return *this;
  }

  template< typename T >
  Queue< T >& Queue< T >::operator=(Queue< T >&& rhs) noexcept
  {
    list_ = std::move(rhs.list_);
    return *this;
  }

  template< typename T >
  void Queue< T >::push(const T& rhs)
  {
    list_.push_back(rhs);
  }

  template< typename T >
  void Queue< T >::push(T&& rhs)
  {
    list_.push_back(std::move(rhs));
  }

  template< typename T >
  T Queue< T >::drop()
  {
    assert(!empty());
    T first = list_.front();
    list_.pop_front();
    return first;
  }

  template< typename T >
  bool Queue< T >::empty()
  {
    return list_.empty();
  }

  template< typename T >
  size_t Queue< T >::size()
  {
    return list_.size();
  }

  template< typename T >
  T& Queue< T >::front()
  {
    assert(!empty());
    return list_.front();
  }
}

#endif

