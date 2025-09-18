#ifndef STACK_HPP
#define STACK_HPP
#include <cstddef>
#include <cassert>
#include <algorithm>
#include "array.hpp"

namespace sveshnikov
{
  template< class T >
  class Stack
  {
  public:
    T &top() noexcept;
    const T &top() const noexcept;
    size_t getSize() const noexcept;
    bool empty() const noexcept;
    void push(const T &val);
    void push(T &&val);
    void pop();
    void swap(Stack &other) noexcept;

  private:
    Array< T > data_;
  };

  template< class T >
  T &Stack< T >::top() noexcept
  {
    return data_.back();
  }

  template< class T >
  const T &Stack< T >::top() const noexcept
  {
    return data_.back();
  }

  template< class T >
  size_t Stack< T >::getSize() const noexcept
  {
    return data_.getSize();
  }

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return data_.empty();
  }

  template< class T >
  void Stack< T >::push(const T &val)
  {
    data_.push_back(val);
  }

  template< class T >
  void Stack< T >::push(T &&val)
  {
    data_.push_back(std::forward< T >(val));
  }

  template< class T >
  void Stack< T >::pop()
  {
    data_.pop_back();
  }

  template< class T >
  void Stack< T >::swap(Stack &other) noexcept
  {
    data_.swap(other.data_);
  }
}

#endif
