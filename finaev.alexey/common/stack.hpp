#ifndef STACK_HPP
#define STACK_HPP
#include <cstddef>
#include <utility>
#include "dynamicArr.hpp"

namespace finaev
{
  template< class T >
  class Stack
  {
  public:
    bool isEmpty() const noexcept;
    size_t size() const noexcept;

    void push(const T& rhs);
    void pop();
    T& top();
    const T& top() const;
  private:
    DynamicArr< T > arr_;
  };

  template< class T >
  bool Stack< T >::isEmpty() const noexcept
  {
    return arr_.empty();
  }

  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return arr_.size();
  }

  template< class T >
  void Stack< T >::push(const T& rhs)
  {
    arr_.push(rhs);
  }

  template< class T >
  void Stack< T >::pop()
  {
    arr_.pop_back();
  }

  template< class T >
  T& Stack< T >::top()
  {
    return arr_.back();
  }

  template< class T >
  const T& Stack< T >::top() const
  {
    return arr_.back();
  }
}

#endif
