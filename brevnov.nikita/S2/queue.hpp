#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "dynamic_array.hpp"
namespace brevnov
{
  template< typename T >
  class Queue
  {
  public:

    T& front();
    const T& front() const;

    void pop() noexcept;
    void push(const T&);
    void push(T&&);

    size_t size() const noexcept;
    bool empty() const noexcept;
    void clear() noexcept;

  private:
    Dynamic_array< T > array_;
  };

  template< typename T >
  T& Queue< T >::front()
  {
    return array_.front();
  }

  template< typename T >
  const T& Queue< T >::front() const
  {
    return array_.front();
  }

  template< typename T >
  void Queue< T >::pop() noexcept
  {
    array_.popFront();
  }

  template< typename T >
  void Queue< T >::push(const T& data)
  {
    array_.push(data);
  }

  template< typename T >
  void Queue< T >::push(T&& data)
  {
    array_.push(std::move(data));
  }

  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return array_.size();
  }

  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return array_.empty();
  }

  template< typename T >
  void Queue< T >::clear() noexcept
  {
    array_.clear();
  }
}
#endif
