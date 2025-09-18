#ifndef STACK_HPP
#define STACK_HPP

#include "array.hpp"

namespace lanovenko
{
  template< typename T >
  class Stack
  {
  public:
    Stack(size_t max_size = 1'000);
    bool empty() const noexcept;
    void pop() noexcept;
    template< typename U >
    void push(U&& value);
    size_t size() const noexcept;
    T& top() noexcept;
    const T& top() const noexcept;
    void swap(Stack& rhs);
  private:
    Array< T > data_;
  };

  template< typename T >
  Stack< T >::Stack(size_t max_size):
    data_(Array< T >(max_size))
  {}

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return data_.empty();
  }

  template< typename T >
  void Stack< T >::pop() noexcept
  {
    data_.pop_back();
  }

  template< typename T >
  template< typename U >
  void Stack< T >::push(U&& value)
  {
    data_.push(std::forward< U >(value));
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return data_.size();
  }

  template< typename T >
  T& Stack< T >::top() noexcept
  {
    return data_.back();
  }

  template< typename T >
  const T& Stack< T >::top() const noexcept
  {
    return data_.back();
  }

  template< typename T >
  void Stack< T >::swap(Stack& rhs)
  {
    data_.swap(rhs.data_);
  }
}

#endif
