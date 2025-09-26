#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "vector.hpp"

namespace kushekbaev
{
  template< typename T, typename Sequence = kushekbaev::Vector< T > >
  class Queue
  {
    public:
      Queue() = default;
      Queue(const Queue& other);
      Queue(Queue&& other) noexcept;
      ~Queue() = default;

      Queue& operator=(const Queue& other);
      Queue& operator=(Queue&& other) noexcept;

      T& front() noexcept;
      T& back() noexcept;
      const T& front() const noexcept;
      const T& back() const noexcept;

      bool empty() const noexcept;
      size_t size() const noexcept;

      void push(const T& value);
      void pop();
      void clear();

    private:
      Sequence sequence_;
  };

  template < typename T, typename Sequence >
  Queue< T, Sequence >::Queue(const Queue& other):
    sequence_(other.sequence_)
  {}

  template < typename T, typename Sequence >
  Queue< T, Sequence >::Queue(Queue&& other) noexcept:
    sequence_(std::move(other.sequence_))
  {}

  template < typename T, typename Sequence >
  Queue< T, Sequence >& Queue< T, Sequence >::operator=(const Queue& other)
  {
    if (this != std::addressof(other))
    {
      sequence_ = other.sequence_;
    }
    return *this;
  }

  template< typename T, typename Sequence >
  Queue< T, Sequence >& Queue< T, Sequence >::operator=(Queue&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      sequence_ = std::move(other.sequence_);
    }
    return *this;
  }

  template < typename T, typename Sequence >
  T& Queue< T, Sequence >::front() noexcept
  {
    return sequence_.front();
  }

  template < typename T, typename Sequence >
  T& Queue< T, Sequence >::back() noexcept
  {
    return sequence_.back();
  }

  template < typename T, typename Sequence >
  const T& Queue< T, Sequence >::front() const noexcept
  {
    return sequence_.front();
  }

  template < typename T, typename Sequence >
  const T& Queue< T, Sequence >::back() const noexcept
  {
    return sequence_.back();
  }

  template < typename T, typename Sequence >
  bool Queue< T, Sequence >::empty() const noexcept
  {
    return sequence_.empty();
  }

  template < typename T, typename Sequence >
  size_t Queue< T, Sequence >::size() const noexcept
  {
    return sequence_.size();
  }

  template < typename T, typename Sequence >
  void Queue< T, Sequence >::push(const T& value)
  {
    sequence_.pushBack(value);
  }

  template < typename T, typename Sequence >
  void Queue< T, Sequence >::pop()
  {
    if (sequence_.empty())
    {
      throw std::out_of_range("The vector in the queue is empty!");
    }
    sequence_.popFront();
  }

  template < typename T, typename Sequence >
  void Queue< T, Sequence >::clear()
  {
    while (!empty())
    {
      pop();
    }
  }
}

#endif
