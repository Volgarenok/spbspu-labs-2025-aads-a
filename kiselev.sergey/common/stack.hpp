#ifndef STACK_HPP
#define STACK_HPP
#include "dynamicArr.hpp"
namespace kiselev
{
  template< typename T >
  class Stack
  {
  public:

    T& top();
    const T& top() const;
    void pop() noexcept;
    void push(const T&) noexcept;
    void push(T&&) noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;
    void clear() noexcept;

  private:
    DynamicArr< T > arr_;
  };

  template< typename T >
  T& Stack< T >::top()
  {
    return arr_.back();
  }

  template< typename T >
  const T& Stack< T >::top() const
  {
    return arr_.back();
  }

  template< typename T >
  void Stack< T >::pop() noexcept
  {
    arr_.popBack();
  }

  template< typename T >
  void Stack< T >::push(const T& data) noexcept
  {
    arr_.push(data);
  }

  template< typename T >
  void Stack< T >::push(T&& data) noexcept
  {
    arr_.push(std::move(data));
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
