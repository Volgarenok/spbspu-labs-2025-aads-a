#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>
#include "dynamic_array.hpp"

namespace petrov
{
  template< typename T >
  struct Stack
  {
  public:
    using this_t = Stack< T >;
    template< typename T1 >
    void push(T1 && element);
    void pop();
    T & top();
    const T & top() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
  private:
    DynamicArray< T > dynamic_array_;
  };

  template< typename T >
  template< typename T1 >
  void Stack< T >::push(T1 && element)
  {
    dynamic_array_.push_back(std::forward< T1 >(element));
  }

  template< typename T >
  void Stack< T >::pop()
  {
    dynamic_array_.pop_back();
  }

  template< typename T >
  T & Stack< T >::top()
  {
    return dynamic_array_.back();
  }

  template< typename T >
  const T & Stack< T >::top() const
  {
    return dynamic_array_.back();
  }

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return dynamic_array_.empty();
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return dynamic_array_.size();
  }
}
#endif
