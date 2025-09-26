#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include "vector.hpp"

namespace kushekbaev
{
  template< typename T, typename Sequence = kushekbaev::Vector< T > >
  class Stack
  {
    public:
      Stack() = default;
      Stack(const Stack& other);
      Stack(Stack&& other) noexcept;
      ~Stack() = default;

      Stack& operator=(const Stack& other);
      Stack& operator=(Stack&& other) noexcept;

      bool empty() const noexcept;
      size_t size() const noexcept;

      T& top() noexcept;
      const T& top() const noexcept;

      void push(const T& value);
      void pop();

    private:
      Sequence sequence_;
  };

  template< typename T, typename Sequence >
  Stack< T, Sequence >::Stack(const Stack& other):
    sequence_(other.sequence_)
  {}

  template< typename T, typename Sequence >
  Stack< T, Sequence >::Stack(Stack&& other) noexcept:
    sequence_(std::move(other.sequence_))
  {}

  template< typename T, typename Sequence >
  Stack< T, Sequence >& Stack< T, Sequence >::operator=(const Stack& other)
  {
    if (this != std::addressof(other))
    {
      sequence_ = other.sequence_;
    }
    return *this;
  }

  template< typename T, typename Sequence >
  Stack< T, Sequence >& Stack< T, Sequence >::operator=(Stack&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      sequence_ = std::move(other.sequence_);
    }
    return *this;
  }

  template< typename T, typename Sequence >
  bool Stack< T, Sequence >::empty() const noexcept
  {
    return sequence_.empty();
  }

  template< typename T, typename Sequence >
  size_t Stack< T, Sequence >::size() const noexcept
  {
    return sequence_.size();
  }

  template< typename T, typename Sequence >
  T& Stack< T, Sequence >::top() noexcept
  {
    return sequence_.back();
  }

  template< typename T, typename Sequence >
  const T& Stack< T, Sequence >::top() const noexcept
  {
    return sequence_.back();
  }

  template< typename T, typename Sequence >
  void Stack< T, Sequence >::push(const T& value)
  {
    return sequence_.pushBack(value);
  }

  template< typename T, typename Sequence >
  void Stack< T, Sequence >::pop()
  {
    if (sequence_.empty())
    {
      throw std::out_of_range("The Vector in the queue is empty!");
    }
    sequence_.popBack();
  }
}

#endif
