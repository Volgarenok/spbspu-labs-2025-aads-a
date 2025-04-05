#ifndef STACK_HPP
#define STACK_HPP

#include "array.hpp"

namespace duhanina
{
  template < typename T >
  class Stack
  {
  public:
    Stack() = default;
    ~Stack() = default;

    Stack(const Stack& other);
    Stack(Stack&& other) noexcept;
    Stack& operator=(const Stack& other);

    bool empty() const noexcept;

    void push(const T& value);
    void pop();

    const T& top() const noexcept;
    T& top() noexcept;

    size_t size() const noexcept;

  private:
    DynamicArray< T > array_;
  };

  template < typename T >
  Stack< T >::Stack(const Stack& other):
    array_(other.array_)
  {}

  template < typename T >
  Stack< T >::Stack(Stack&& other) noexcept:
    array_(std::move(other.array_))
  {}

  template < typename T >
  Stack< T >& Stack< T >::operator=(const Stack& other)
  {
    if (this != &other)
    {
      array_ = other.array_;
    }
    return *this;
  }

  template < typename T >
  bool Stack< T >::empty() const noexcept
  {
    return array_.empty();
  }

  template < typename T >
  void Stack< T >::push(const T& value)
  {
    array_.push_back(value);
  }

  template < typename T >
  void Stack< T >::pop()
  {
    array_.pop_back();
  }

  template < typename T >
  const T& Stack< T >::top() const noexcept
  {
    return array_.back();
  }

  template < typename T >
  T& Stack< T >::top() noexcept
  {
    return array_.back();
  }

  template < typename T >
  size_t Stack< T >::size() const noexcept
  {
    return array_.size();
  }
}

#endif
