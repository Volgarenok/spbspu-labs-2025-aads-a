#ifndef STACK_HPP
#define STACK_HPP
#include "dynamic_array.hpp"
namespace brevnov
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
    Dynamic_array< T > array_;
  };

  template< typename T >
  T& Stack< T >::top()
  {
    return array_.back();
  }

  template< typename T >
  const T& Stack< T >::top() const
  {
    return array_.back();
  }

  template< typename T >
  void Stack< T >::pop() noexcept
  {
    array_.popBack();
  }

  template< typename T >
  void Stack< T >::push(const T& data) noexcept
  {
    array_.push(data);
  }

  template< typename T >
  void Stack< T >::push(T&& data) noexcept
  {
    array_.push(std::move(data));
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return array_.size();
  }

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return array_.empty();
  }

  template< typename T >
  void Stack< T >::clear() noexcept
  {
    array_.clear();
  }
}
#endif
