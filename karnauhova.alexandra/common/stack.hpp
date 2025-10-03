#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <stdexcept>
#include "dynamic_array.hpp"

namespace karnauhova
{
  template< typename T >
  class Stack
  {
  public:
    Stack() = default;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void pop();
    void push(const T& val);
    void push(T&& val);
    T& top();
    const T& top() const;

    void swap(Stack< T >& other) noexcept;
  private:
    DynamicArray< T > array_;
  };

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return array_.empty();
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return array_.size();
  }

  template< typename T >
  void Stack< T >::pop()
  {
    if (empty())
    {
      throw std::logic_error("empty stack for pop");
    }
    array_.pop_back();
  }

  template< typename T >
  void Stack< T >::push(const T& val)
  {
    array_.push(val);
  }

  template< typename T >
  void Stack< T >::push(T&& val)
  {
    array_.push(std::move(val));
  }

  template< typename T >
  const T& Stack< T >::top() const
  {
    return array_.back();
  }

  template< typename T >
  T& Stack< T >::top()
  {
    return array_.back();
  }

  template< typename T >
  void Stack< T >::swap(Stack< T >& other) noexcept
  {
    array_.swap(other.array_);
  }
}

#endif
