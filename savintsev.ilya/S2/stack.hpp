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
    Stack(const Stack & rhs);
    Stack(Stack && rhs) noexcept;

    bool empty() const noexcept;

    void pop();
    void push(const T & rhs);
    void push(T && rhs);

    size_t size() const noexcept;

    T & top() noexcept;
    const T & top() const noexcept;
  private:
    Array< T > stack_;
  };

  template< typename T >
  Stack< T >::Stack():
    stack_{2}
  {}

  template< typename T >
  Stack< T >::Stack(const Stack & rhs):
    stack_(rhs.stack_)
  {}

  template< typename T >
  Stack< T >::Stack(Stack && rhs) noexcept:
    stack_(std::move(rhs.stack_))
  {}

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return stack_.empty();
  }

  template< typename T >
  void Stack< T >::pop()
  {
    stack_.pop_back();
  }

  template< typename T >
  void Stack< T >::push(const T & rhs)
  {
    stack_.push_back(rhs);
  }

  template< typename T >
  void Stack< T >::push(T && rhs)
  {
    stack_.push_back(rhs);
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return stack_.size();
  }

  template< typename T >
  T & Stack< T >::top() noexcept
  {
    return stack_.back();
  }

  template< typename T >
  const T & Stack< T >::top() const noexcept
  {
    return stack_.back();
  }
}

#endif
