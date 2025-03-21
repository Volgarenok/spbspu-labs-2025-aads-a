#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include "dynamic_array.hpp"

namespace tkach
{
  template< typename T >
  class Stack
  {
  public:
    Stack() = default;
    Stack(const Stack< T >&);
    Stack(Stack< T >&&);
    ~Stack() = default;
    Stack< T >& operator=(Stack< T >&&) noexcept;
    Stack< T >& operator=(const Stack< T >&) noexcept;
    void pop();
    void push(const T& data);
    void push(T&& data);
    size_t size() const noexcept;
    bool empty() const noexcept;
    T& top();
    const T& top() const;
    void swap(Stack< T >& other);
  private:
    DynArray< T > array_;
  };

   template< typename T >
  Stack< T >::Stack(const Stack< T >& other):
    array_(other.array_)
  {}

  template< typename T >
  Stack< T >::Stack(Stack< T >&& other):
    array_(std::move(other.array_))
  {}

  template< typename T >
  Stack< T >& Stack< T >::operator=(Stack< T >&& other) noexcept
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    Stack< T > temp(std::move(other));
    swap(temp);
    return *this;
  }

  template< typename T >
  Stack< T >& Stack< T >::operator=(const Stack< T >& other) noexcept
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    Stack< T > temp(other);
    swap(temp);
    return *this;
  }

  template< typename T >
  void Stack< T >::pop()
  {
    array_.popBack();
  }

  template< typename T >
  void Stack< T >::swap(Stack< T >& other)
  {
    array_.swap(other.array_);
  }

  template< typename T >
  void Stack< T >::push(const T& data)
  {
    array_.pushBack(data);
  }

  template< typename T >
  void Stack< T >::push(T&& data)
  {
    array_.pushBack(std::move(data));
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
  T& Stack< T >::top()
  {
    return const_cast< T& >(static_cast< const Stack< T >* >(this)->top());
  }

  template< typename T >
  const T& Stack< T >::top() const
  {
    return array_.back();
  }
}

#endif
