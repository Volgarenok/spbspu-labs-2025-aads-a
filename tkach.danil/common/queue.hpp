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
    void pop();
    void push(const T& data);
    void push(T&& data);
    size_t size() const noexcept;
    bool empty() const noexcept;
    T& front();
    const T& front() const;
    void swap(Queue< T >& other);
  private:
    DynArray< T > array_;
  };

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
