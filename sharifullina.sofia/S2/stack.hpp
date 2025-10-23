#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include <list/list.hpp>

namespace sharifullina
{
  template< typename T >
  class Stack
  {
  public:
    Stack();
    Stack(const Stack< T > & other);
    Stack(Stack< T > && other) noexcept;
    ~Stack();

    Stack< T > & operator=(const Stack< T > & other);
    Stack< T > & operator=(Stack< T > && other) noexcept;

    void push(const T & value);
    void pop();
    T & top();
    const T & top() const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void swap(Stack< T > & other) noexcept;

  private:
    List< T > list_;
  };

  template< typename T >
  Stack< T >::Stack():
    list_()
  {}

  template< typename T >
  Stack< T >::Stack(const Stack< T > & other):
    list_(other.list_)
  {}

  template< typename T >
  Stack< T >::Stack(Stack< T > && other) noexcept:
    list_(std::move(other.list_))
  {}

  template< typename T >
  Stack< T >::~Stack()
  {}

  template< typename T >
  Stack< T > & Stack< T >::operator=(const Stack< T > & other)
  {
    if (this != &other)
    {
      Stack< T > temp(other);
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  Stack< T > & Stack< T >::operator=(Stack< T > && other) noexcept
  {
    if (this != &other)
    {
      Stack< T > temp(std::move(other));
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  void Stack< T >::push(const T & value)
  {
    list_.pushBack(value);
  }

  template< typename T >
  void Stack< T >::pop()
  {
    if (empty())
    {
      throw std::logic_error("empty stack");
    }
    list_.popBack();
  }

  template< typename T >
  T & Stack< T >::top()
  {
    if (empty())
    {
      throw std::logic_error("empty stack");
    }
    return list_.back();
  }

  template< typename T >
  const T & Stack< T >::top() const
  {
    if (empty())
    {
      throw std::logic_error("empty stack");
    }
    return list_.back();
  }

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return list_.empty();
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return list_.size();
  }

  template< typename T >
  void Stack< T >::swap(Stack< T > & other) noexcept
  {
    list_.swap(other.list_);
  }
}

#endif
