#ifndef STACK_HPP
#define STACK_HPP
#include <utility>
#include <cstddef>
#include <stdexcept>
#include "data_utils.hpp"
#include "dynamic_array.hpp"

namespace demehin
{
  template< typename T >
  class Stack
  {
  public:
    Stack() = default;
    Stack(const Stack&) = default;
    Stack(Stack&&) = default;

    Stack< T >& operator=(const Stack< T >&) = default;
    Stack< T >& operator=(Stack< T >&&) = default;

    ~Stack() = default;

    void push(const T&);
    const T& top() const;
    T& top();
    void pop();

    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    DynamicArray< T > data_;
  };

  template< typename T >
  void Stack< T >::push(const T& rhs)
  {
    data_.push(rhs);
  }

  template< typename T >
  const T& Stack< T >::top() const
  {
    return data_.back();
  }

  template< typename T >
  T& Stack< T >::top()
  {
    return data_.back();
  }

  template< typename T >
  void Stack< T >::pop()
  {
    data_.pop_back();
  }

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return data_.empty();
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return data_.size();
  }
}

#endif
