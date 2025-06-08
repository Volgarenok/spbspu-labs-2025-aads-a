#ifndef STACK_HPP
#define STACK_HPP
#include "dynamic-array.hpp"

namespace savintsev
{
  template< typename T >
  class Stack
  {
  public:
    Stack();

    bool empty() const noexcept;

    void pop() noexcept;
    template< typename U >
    void push(U && rhs);

    size_t size() const noexcept;

    T & top();
    const T & top() const;

    template< typename U >
    friend void swap(Stack< U > & x, Stack< U > & y) noexcept;
  private:
    Array< T > stack_;
  };

  template< typename U >
  void swap(Stack< U > & x, Stack< U > & y) noexcept
  {
    swap(x.stack_, y.stack_);
  }

  template< typename T >
  Stack< T >::Stack():
    stack_{2}
  {}

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return stack_.empty();
  }

  template< typename T >
  void Stack< T >::pop() noexcept
  {
    stack_.pop_back();
  }

  template< typename T >
  template< typename U >
  void Stack< T >::push(U && rhs)
  {
    stack_.push_back(std::forward< U >(rhs));
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return stack_.size();
  }

  template< typename T >
  T & Stack< T >::top()
  {
    return stack_.back();
  }

  template< typename T >
  const T & Stack< T >::top() const
  {
    return stack_.back();
  }
}

#endif
