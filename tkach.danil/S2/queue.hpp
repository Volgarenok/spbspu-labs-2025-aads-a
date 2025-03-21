#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include "dynamic_array.hpp"

namespace tkach
{
  template< typename T >
  class Queue
  {
  public:
    Queue() = default;
    Queue(const Queue< T >&);
    Queue(Queue< T >&&);
    ~Queue() = default;
    Queue< T >& operator=(Queue< T >&&) noexcept;
    Queue< T >& operator=(const Queue< T >&) noexcept;
    void pop();
    void push(const T& data);
    void push(T&& data);
    size_t size() const noexcept;
    bool empty() const noexcept;
    T& back();
    const T& back() const;
    T& front();
    const T& front() const;
    void swap(Queue< T >& other);
  private:
    DynArray< T > array_;
  };

  template< typename T >
  Queue< T >::Queue(const Queue< T >& other):
    array_(other.array_)
  {}

  template< typename T >
  Queue< T >::Queue(Queue< T >&& other):
    array_(std::move(other.array_))
  {}

  template< typename T >
  Queue< T >& Queue< T >::operator=(Queue< T >&& other) noexcept
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    Queue< T > temp(std::move(other));
    swap(temp);
    return *this;
  }

  template< typename T >
  Queue< T >& Queue< T >::operator=(const Queue< T >& other) noexcept
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    Queue< T > temp(other);
    swap(temp);
    return *this;
  }

  template< typename T >
  void Queue< T >::pop()
  {
    array_.popFront();
  }

  template< typename T >
  void Queue< T >::push(const T& data)
  {
    array_.pushBack(data);
  }

  template< typename T >
  void Queue< T >::push(T&& data)
  {
    array_.pushBack(std::move(data));
  }

  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return array_.size();
  }

  template< typename T >
  void Queue< T >::swap(Queue< T >& other)
  {
    array_.swap(other.array_);
  }

  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return array_.empty();
  }

  template< typename T >
  T& Queue< T >::back()
  {
    return const_cast< T& >(static_cast< const Queue< T >* >(this)->back());
  }

  template< typename T >
  const T& Queue< T >::back() const
  {
    return array_.back();
  }

  template< typename T >
  T& Queue< T >::front()
  {
    return const_cast< T& >(static_cast< const Queue< T >* >(this)->front());
  }

  template< typename T >
  const T& Queue< T >::front() const
  {
    return array_.front();
  }
}

#endif
