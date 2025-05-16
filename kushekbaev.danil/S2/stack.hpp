#ifndef STACK_HPP
#define STACK_HPP

#include "array.hpp"

namespace kushekbaev
{
  template< typename T >
  class Stack
  {
    public:
      Stack() = default;
      Stack(const Stack& other);
      Stack(Stack&& other);
      ~Stack() = default;

      Stack& operator=(const Stack& other);

      bool empty() const noexcept;
      size_t size() const noexcept;

      T& top() noexcept;
      const T& top() const noexcept;

      void push(const T& value);
      T drop();
      void pop();

    private:
      Array< T > array_;
  };

  template< typename T >
  Stack< T >::Stack(const Stack& other):
    array_(other.array_)
  {}

  template< typename T >
  Stack< T >::Stack(Stack&& other):
    array_(std::move(other.array_))
  {}

  template< typename T >
  Stack< T >& Stack< T >::operator=(const Stack& other)
  {
    if (this != &other)
    {
      array_ = other.array_;
    }
    return *this;
  }

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return array_.empty();
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return array_.size();
  }

  template< typename T >
  T& Stack< T >::top() noexcept
  {
    return array_.back();
  }

  template< typename T >
  const T& Stack< T >::top() const noexcept
  {
    return array_.back();
  }

  template< typename T >
  void Stack< T >::push(const T& value)
  {
    return array_.push_back(value);
  }

  template< typename T >
  T Stack< T >::drop()
  {
    if (array_.empty())
    {
      throw std::out_of_range("The array in the queue is empty!");
    }
    T tmp = top();
    array_.pop_back();
    return tmp;
  }

  template< typename T >
  void Stack< T >::pop()
  {
    if (array_.empty())
    {
      throw std::out_of_range("The array in the queue is empty!");
    }
    array_.pop_back();
  }
}

#endif
