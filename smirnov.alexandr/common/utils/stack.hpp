#ifndef STACK_HPP
#define STACK_HPP
#include <stdexcept>
#include <list/list.hpp>

namespace smirnov
{
  template< typename T >
  class Stack
  {
  public:
    void push(T rhs);
    T drop();
    T top() const;
    bool empty() const;
  private:
    List< T > data_;
  };

  template< typename T >
  void Stack< T >::push(T rhs)
  {
    data_.push_front(rhs);
  }

  template< typename T >
  T Stack< T >::drop()
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    T val = data_.front();
    data_.pop_front();
    return val;
  }

  template< typename T >
  T Stack< T >::top() const
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    return data_.front();
  }

  template< typename T >
  bool Stack< T >::empty() const
  {
    return data_.empty();
  }
}
#endif
