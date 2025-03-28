#ifndef STACK_HPP
#define STACK_HPP

#include <sys/types.h>
#include "array.hpp"

namespace bocharov
{
  template< typename T >
  class Stack
  {
  public:

    Stack() = default;
    Stack(const Stack< T > &);
    Stack(Stack< T > &&) noexcept;
    ~Stack() = default;

    Stack< T > & operator=(const Stack< T > &);
    Stack< T > & operator=(Stack< T > &&) noexcept;

    T & back();
    const T & back() const;
    void pop() noexcept;
    void push(T &) noexcept;
    void push(T &&) noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;
    void clear() noexcept;

  private:

    Array< T > arr_;
  };

  template< typename T >
  Stack< T >::Stack(const Stack< T > & stack):
    arr_(stack.arr_)
  {}

  template< typename T >
  Stack< T >::Stack(Stack< T > && stack) noexcept:
    arr_(std::move(stack.arr_))
  {}

  template< typename T >
  Stack< T > & Stack< T >::operator=(const Stack< T > & stack)
  {
    arr_ = stack.arr_;
    return *this;
  }

  template< typename T >
  Stack< T > & Stack< T >::operator=(Stack< T > && stack) noexcept
  {
    arr_ = std::move(stack.arr_);
    return *this;
  }

  template< typename T >
  T & Stack< T >::back()
  {
    return arr_.back();
  }

  template< typename T >
  const T & Stack< T >::back() const
  {
    return arr_.back();
  }

  template< typename T >
  void Stack< T >::pop() noexcept
  {
    return arr_.popBack();
  }

  template< typename T >
  void Stack< T >::push(T & data) noexcept
  {
    arr_.push(data);
  }

  template< typename T >
  void Stack< T >::push(T && data) noexcept
  {
    return arr_.push(data);
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return arr_.size();
  }

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return arr_.empty();
  }

  template< typename T >
  void Stack< T >::clear() noexcept
  {
    arr_.clear();
  }
}

#endif
