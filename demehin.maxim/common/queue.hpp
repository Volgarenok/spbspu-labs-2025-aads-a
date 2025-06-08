#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <utility>
#include <cstddef>
#include <stdexcept>
#include "data_utils.hpp"
#include "dynamic_array.hpp"

namespace demehin
{
  template< typename T >
  class Queue
  {
  public:

    Queue() = default;
    Queue(const Queue&) = default;
    Queue(Queue&&) = default;

    Queue< T >& operator=(const Queue< T >&) = default;
    Queue< T >& operator=(Queue< T >&&) = default;

    ~Queue() = default;

    void push(const T& rhs);
    void pop();
    const T& front() const;
    T& front();
    const T& back() const;
    T& back();

    bool empty() const noexcept;
    size_t size() const noexcept;

    void clear();

  private:
    DynamicArray< T > data_;
  };

  template< typename T >
  void Queue< T >::push(const T& rhs)
  {
    data_.push(rhs);
  }

  template< typename T >
  void Queue< T >::pop()
  {
    data_.pop_front();
  }

  template< typename T >
  const T& Queue< T >::back() const
  {
    return data_.back();
  }

  template< typename T >
  T& Queue< T >::back()
  {
    return data_.back();
  }

  template< typename T >
  const T& Queue< T >::front() const
  {
    return data_.front();
  }

  template< typename T >
  T& Queue< T >::front()
  {
    return data_.front();
  }

  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return data_.size();
  }

  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return data_.empty();
  }

  template< typename T >
  void Queue< T >::clear()
  {
    data_.clear();
  }
}

#endif
