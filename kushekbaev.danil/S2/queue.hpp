#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "array.hpp"
#include <stdexcept>

namespace kushekbaev
{
  template< typename T >
  class Queue
  {
    public:
      Queue() = default;
      Queue(const Queue& other);
      Queue(Queue&& other) noexcept;
      ~Queue() = default;

      Queue& operator=(const Queue& other);

      T& front() noexcept;
      T& back() noexcept;
      const T& front() const noexcept;
      const T& back() const noexcept;

      bool empty() const noexcept;
      size_t size() const noexcept;

      void push(const T& value);
      T drop();
      void pop();

    private:
      Array< T > array_;
  };

  template< typename T >
  Queue< T >::Queue(const Queue& other):
    array_(other.array_)
  {}

  template< typename T >
  Queue< T >::Queue(Queue&& other) noexcept:
    array_(std::move(other.array_))
  {}

  template< typename T >
  Queue< T >& Queue< T >::operator=(const Queue& other)
  {
    if (this != &other)
    {
      array_ = other.array_;
    }
    return *this;
  }

  template< typename T >
  T& Queue< T >::front() noexcept
  {
    return array_.front();
  }

  template< typename T >
  T& Queue< T >::back() noexcept
  {
    return array_.back();
  }

  template< typename T >
  const T& Queue< T >::front() const noexcept
  {
    return array_.front();
  }

  template< typename T >
  const T& Queue< T >::back() const noexcept
  {
    return array_.back();
  }

  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return array_.empty();
  }

  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return array_.size();
  }

  template< typename T >
  void Queue< T >::push(const T& value)
  {
    array_.push_back(value);
  }

  template< typename T >
  T Queue< T >::drop()
  {
    if (array_.empty())
    {
      throw std::out_of_range("The array in the queue is empty!");
    }
    T tmp = array_.front();
    array_.pop_front();
    return tmp;
  }

  template< typename T >
  void Queue< T >::pop()
  {
    if (array_.empty())
    {
      throw std::out_of_range("The array in the queue is empty!");
    }
    array_.pop_front();
  }
}

#endif
