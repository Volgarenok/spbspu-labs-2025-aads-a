#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <stdexcept>
#include "dynamic_array.hpp"

namespace karnauhova
{
  template< typename T >
  class Queue
  {
  public:
    Queue();

    bool empty() const noexcept;
    size_t size() const noexcept;

    void pop();
    void push(const T& val);
    void push(T&& val);
    T& front();
    const T& front() const;

    void swap(Queue& other) noexcept;
  private:
    size_t head_data_;
    DynamicArray< T > array_;
  };

  template< typename T >
  Queue< T >::Queue():
    head_data_(0),
    array_()
  {}

  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return head_data_ >= array_.size();
  }

  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return array_.size() - head_data_;
  }

  template< typename T >
  void Queue< T >::pop()
  {
    if (empty())
    {
      throw std::logic_error("empty queue for pop");
    }
    ++head_data_;
    if (head_data_ >= array_.size())
    {
      head_data_ = 0;
      array_.clear();
    }
  }

  template< typename T >
  void Queue< T >::push(const T& val)
  {
    array_.push(val);
  }

  template < class T >
  void Queue< T >::push(T&& val)
  {
    array_.push(std::move(val));
  }

  template< typename T >
  T& Queue< T >::front()
  {
    return array_[head_data_];
  }

  template< typename T >
  const T& Queue< T >::front() const
  {
    return array_[head_data_];
  }

  template< typename T >
  void Queue< T >::swap(Queue< T >& other) noexcept
  {
    std::swap(head_data_, other.head_data_);
    array_.swap(other.array_);
  }
}

#endif
