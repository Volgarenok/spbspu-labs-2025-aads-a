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
    void push(const T & value);
    void push(T && value);
    void pop();
    T & top();
    const T & top() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
  private:
    List< T > data_;
  };

  template< typename T >
  void Stack< T >::push(const T & value)
  {
    data_.push_front(value);
  }

  template< typename T >
  void Stack< T >::push(T && value)
  {
    data_.push_front(std::move(value));
  }

  template< typename T >
  void Stack< T >::pop()
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    data_.pop_front();
  }

  template< typename T >
  T & Stack< T >::top()
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    return data_.front();
  }

  template< typename T >
  const T & Stack< T >::top() const
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    return data_.front();
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
