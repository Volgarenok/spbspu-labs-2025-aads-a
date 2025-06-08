#ifndef STACK_HPP
#define STACK_HPP

#include "array.hpp"

namespace duhanina
{
  template < typename T >
  class Stack
  {
  public:
    bool empty() const noexcept;
    void push(const T& value);
    void pop();
    const T& top() const;
    T& top();
    size_t size() const noexcept;

  private:
    DynamicArray< T > array_;
  };

  template < typename T >
  bool Stack< T >::empty() const noexcept
  {
    return array_.empty();
  }

  template < typename T >
  void Stack< T >::push(const T& value)
  {
    array_.push_back(value);
  }

  template < typename T >
  void Stack< T >::pop()
  {
    array_.pop_back();
  }

  template < typename T >
  const T& Stack< T >::top() const
  {
    return array_.back();
  }

  template < typename T >
  T& Stack< T >::top()
  {
    return array_.back();
  }

  template < typename T >
  size_t Stack< T >::size() const noexcept
  {
    return array_.size();
  }
}

#endif
