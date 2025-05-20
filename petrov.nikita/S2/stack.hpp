#ifndef STACK_HPP
#define STACK_HPP

#include "dynamic_array.hpp"
#include <cstddef>
#include <memory>
#include <stdexcept>

namespace petrov
{
  template< typename T >
  struct Stack
  {
  public:
    using this_t = Stack< T >;
    Stack()
    {}
    template< typename U >
    Stack(this_t && rhs);
    ~Stack() = default;
    template< typename U >
    this_t & operator=(this_t && rhs);
    template< typename U >
    void push(U && element);
    void pop();
    T & top();
    const T & top() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
  private:
    DynamicArray< T > dynamic_array_;
  };

  template< typename T >
  template< typename U >
  Stack< T >::Stack(this_t && rhs)
  {
    dynamic_array_(rhs.dynamic_array_);
  }

  template< typename T >
  template< typename U >
  typename Stack< T >::this_t & Stack< T >::operator=(this_t && rhs)
  {
    dynamic_array_ = rhs.dynamic_array_;
    return *this;
  }

  template< typename T >
  template< typename U >
  void Stack< T >::push(U && element)
  {
    dynamic_array_.push_back(element);
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
