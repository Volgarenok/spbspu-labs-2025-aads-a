#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include "ArrayBuffer.hpp"

namespace gavrilova {
  template < class T >
  class Queue {
  public:
    Queue() = default;

    void push(const T& value);
    void push(T&& value);
    void pop();

    T& front();
    const T& front() const;

    bool empty() const;
    size_t size() const;
    void swap(Queue& other) noexcept;
    void clear();

  private:
    ArrayBuffer< T > buffer_;
    size_t head_ = 0;
  };


  template < class T >
  void Queue< T >::push(const T& value)
  {
    buffer_.push_back(value);
  }

  template < class T >
  void Queue< T >::push(T&& value)
  {
    buffer_.push_back(std::move(value));
  }

  template < class T >
  T& Queue< T >::front()
  {
    return buffer_[head_];
  }

  template < class T >
  const T& Queue< T >::front() const
  {
    return buffer_[head_];
  }

  template < class T >
  void Queue< T >::pop()
  {
    if (empty()) {
      throw std::out_of_range("Stack is empty");
    }
    if (++head_ >= buffer_.size()) {
      clear();
    }
  }

  template < class T >
  bool Queue< T >::empty() const
  {
    return head_ >= buffer_.size();
  }

  template < class T >
  size_t Queue< T >::size() const
  {
    return buffer_.size() - head_;
  }

  template < class T >
  void Queue< T >::swap(Queue& other) noexcept
  {
    buffer_.swap(other.buffer_);
    std::swap(head_, other.head_);
  }

  template <class T>
  void Queue< T >::clear()
  {
    buffer_.clear();
    head_ = 0;
  }
}

#endif
