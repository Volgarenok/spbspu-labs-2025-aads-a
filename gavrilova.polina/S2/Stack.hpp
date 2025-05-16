#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include "ArrayBuffer.hpp"

namespace gavrilova {
  template < class T >
  class Stack {
  public:
    Stack() = default;

    void push(const T& value);
    void push(T&& value);

    T& top();
    const T& top() const;
    void pop();
    bool empty() const;
    size_t size() const;
    void swap(Stack& other ) noexcept;

  private:
    ArrayBuffer< T > buffer_;
  };

  template < class T >
  void Stack< T >::push(const T& value)
  {
    buffer_.push_back(value);
  }

  template < class T >
  void Stack< T >::push(T&& value)
  {
    buffer_.push_back(std::move(value));
  }

  template < class T >
  T& Stack< T >::top()
  {
    return buffer_.back();
  }

  template < class T >
  const T& Stack< T >::top() const
  {
    return buffer_.back();
  }

  template < class T >
  void Stack< T >::pop()
  {
    if (empty()) {
      throw std::out_of_range("Stack is empty");
    }
    buffer_.pop_back();
  }

  template < class T >
  bool Stack< T >::empty() const
  {
    return buffer_.empty();
  }

  template < class T >
  size_t Stack< T >::size() const
  {
    return buffer_.size();
  }

  template < class T >
  void Stack< T >::swap(Stack& other) noexcept
  {
    buffer_.swap(other.buffer_);
  }

}

#endif
