#ifndef STACK_HPP
#define STACK_HPP
#include <cstddef>
#include <list>
#include <stdexcept>
#include "../S1/fwd_list.hpp"

namespace zakirov
{
  template < typename T >
  class Stack
  {
  public:
    Stack();
    Stack(const Stack & other);
    Stack(Stack && other) noexcept;
    ~Stack() = default;
    bool empty();
    size_t size();
    T & top();
    const T & top() const;
    void push (const T & value);
    void push (T && value);
    void pop();
  private:
    FwdList< T > data_;
  };

  template < typename T >
  Stack< T >::Stack():
    data_()
  {}

  template < typename T >
  Stack< T >::Stack(const Stack & other):
    data_(other.data_)
  {}

  template < typename T >
  Stack< T >::Stack(Stack && other) noexcept:
    data_(std::move(other.data_))
  {}

  template < typename T >
  bool Stack< T >::empty()
  {
    return data_.empty();
  }

  template < typename T >
  size_t Stack< T >::size()
  {
    return data_.size();
  }

  template < typename T >
  T & Stack< T >::top()
  {
    return *(data_.begin());
  }

  template < typename T >
  const T & Stack< T >::top() const
  {
    return *(data_.cbegin());
  }

  template < typename T >
  void Stack< T >::push(const T & value)
  {
    data_.push_front(value);
  }

  template < typename T >
  void Stack< T >::push(T && value)
  {
    data_.push_front(std::move(value));
  }

  template < typename T >
  void Stack< T >::pop()
  {
    data_.pop_front();
  }
}

#endif
