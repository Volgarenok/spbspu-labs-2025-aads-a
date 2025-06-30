#ifndef STACK_HPP
#define STACK_HPP
#include "array.hpp"

namespace averenkov
{
  template< class T >
  class Stack
  {

  public:
    bool empty() const noexcept;
    size_t size() const noexcept;

    const T& top() const noexcept;
    T& top() noexcept;

    void push(const T& rhs);
    void pop();

  private:
    Array< T > arr_;

  };

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return arr_.empty();
  }

  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return arr_.size();
  }

  template< class T >
  const T& Stack< T >::top() const noexcept
  {
    return arr_.back();
  }

  template< class T >
  T& Stack< T >::top() noexcept
  {
    return arr_.back();
  }

  template< class T >
  void Stack< T >::push(const T& rhs)
  {
    arr_.push_back(rhs);
  }

  template< class T >
  void Stack< T >::pop()
  {
    arr_.pop_back();
  }
};

#endif
