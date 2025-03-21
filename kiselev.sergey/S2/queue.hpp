#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include <exception>
#include "dynamicArr.hpp"
namespace kiselev
{
  template< typename T >
  class Queue
  {
  public:
    Queue() = default;
    Queue(const Queue< T >&);
    Queue(Queue< T >&&) noexcept;
    ~Queue() = default;

    Queue< T >& operator=(const Queue< T >&);
    Queue< T >& operator=(Queue< T >&&) noexcept;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    void pop() noexcept;
    void push(T&);
    void push(T&&);

    size_t size() const noexcept;
    bool empty() const noexcept;
    void clear() noexcept;

  private:

    DynamicArr< T > arr_;

  };

  template< typename T >
  Queue< T >::Queue(const Queue< T >& queue):
    arr_(queue.arr_)
  {}

  template< typename T >
  Queue< T >::Queue(Queue< T >&& queue) noexcept:
    arr_(std::move(queue.arr_))
  {}

  template< typename T >
  Queue< T >& Queue< T >::operator=(const Queue< T >& queue)
  {
    arr_ = queue.arr_;
    return *this;
  }

  template< typename T >
  Queue< T >& Queue< T >::operator=(Queue< T >&& queue) noexcept
  {
    arr_ = std::move(queue.arr_);
    return *this;
  }

  template< typename T >
  T& Queue< T >::front()
  {
    return arr_.front();
  }

  template< typename T >
  const T& Queue< T >::front() const
  {
    return arr_.front();
  }

  template< typename T >
  T& Queue< T >::back()
  {
    return arr_.back();
  }

  template< typename T >
  const T& Queue< T >::back() const
  {
    return arr_.back();
  }

  template< typename T >
  void Queue< T >::pop() noexcept
  {
    arr_.popFront();
  }

  template< typename T >
  void Queue< T >::push(T& data)
  {
    arr_.push(data);
  }

  template< typename T >
  void Queue< T >::push(T&& data)
  {
    arr_.push(data);
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
