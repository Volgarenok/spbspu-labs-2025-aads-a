#ifndef STACK_HPP
#define STACK_HPP
#include <stdexcept>
const int STACK_SIZE = 100;
namespace hismatova
{
  template< typename T >
  class Stack
  {
  private:
    T data[STACK_SIZE];
    size_t topIndex;
  public:
    Stack();
    Stack(const Stack& other);
    Stack& operator=(const Stack& other);
    Stack(Stack&& other) noexcept;
    Stack& operator=(Stack&& other) noexcept;
    void push(const T& value);
    T pop();
    T top() const;
    bool empty() const;
  };
  template< typename T >
  Stack< T >::Stack() : topIndex(0) {}
  template< typename T >
  Stack< T >::Stack(const Stack& other)
  {
    topIndex = other.topIndex;
    for (size_t i = 0; i < topIndex; ++i)
    {
      data[i] = other.data[i];
    }
  }
  template< typename T >
  Stack< T >& Stack< T >::operator=(const Stack& other)
  {
    if (this != &other)
    {
      topIndex = other.topIndex;
      for (size_t i = 0; i < topIndex; ++i)
      {
        data[i] = other.data[i];
      }
    }
    return *this;
  }
  template< typename T >
  Stack< T >::Stack(Stack&& other) noexcept : topIndex(other.topIndex)
  {
    for (size_t i = 0; i < topIndex; ++i)
    {
      data[i] = std::move(other.data[i]);
    }
    other.topIndex = 0;
  }
  template< typename T >
  Stack< T >& Stack< T >::operator=(Stack&& other) noexcept
  {
    if (this != &other)
    {
      topIndex = other.topIndex;
      for (size_t i = 0; i < topIndex; ++i)
      {
        data[i] = std::move(other.data[i]);
      }
      other.topIndex = 0;
    }
    return *this;
  }
  template< typename T >
  void Stack< T >::push(const T& value)
  {
    if (topIndex >= STACK_SIZE)
    {
      throw std::overflow_error("Stack overflow");
    }
    data[topIndex++] = value;
  }
  template< typename T >
  T Stack< T >::pop()
  {
    if (topIndex == 0)
    {
      throw std::underflow_error("Stack underflow");
    }
    return data[--topIndex];
  }
  template< typename T >
  T Stack< T >::top() const
  {
    if (topIndex == 0)
    {
      throw std::underflow_error("Stack is empty");
    }
    return data[topIndex - 1];
  }
  template< typename T >
  bool Stack< T >::empty() const
  {
    return topIndex == 0;
  }
}
#endif
