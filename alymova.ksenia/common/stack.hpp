#ifndef STACK_HPP
#define STACK_HPP
#include <cstddef>
#include "array.hpp"

namespace alymova
{
  template< typename T >
  struct Stack
  {
    bool empty() const noexcept;
    size_t size() const noexcept;
    T& top();
    const T& top() const noexcept;
    void push(const T& value);
    void push(T&& value);
    void pop() noexcept;

  private:
    Array< T > data_;
  };

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return data_.empty();
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return data_.size();
  }

  template< typename T >
  T& Stack< T >::top()
  {
    return data_.back();
  }

  template< typename T >
  const T& Stack< T >::top() const noexcept
  {
    return data_.back();
  }

  template< typename T >
  void Stack< T >::push(const T& value)
  {
    data_.push_back(value);
  }

  template< typename T >
  void Stack< T >::push(T&& value)
  {
    data_.push_back(std::forward< T >(value));
  }

  template< typename T >
  void Stack< T >::pop() noexcept
  {
    data_.pop_back();
  }
}
#endif
