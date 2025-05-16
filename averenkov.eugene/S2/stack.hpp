#ifndef STACK_HPP
#define STACK_HPP
#include "array.hpp"

namespace averenkov
{
  template< class T >
  class Stack
  {

  public:
    Stack() = default;
    ~Stack() = default;
    Stack(const Stack &rhs);
    Stack(Stack &&rhs) noexcept;
    Stack &operator=(const Stack &rhs);
    bool empty() const noexcept;
    size_t size() const noexcept;

    const T& top() const noexcept;
    T& top() noexcept;

    void push(T rhs);
    T drop();

  private:
    Array< T > arr_;

  };

  template< class T >
  Stack< T >::Stack(const Stack& rhs):
    arr_(rhs.arr_)
  {}

  template< class T >
  Stack< T >::Stack(Stack&& rhs) noexcept:
    arr_(std::move(rhs.arr_))
  {}

  template< class T >
  Stack< T >& Stack< T >::operator=(const Stack& rhs)
  {
    if (this != std::addressof(rhs))
    {
      arr_ = rhs.arr_;
    }
    return *this;
  }

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return arr_.empty();
  }

  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return arr_.size();
  }

  template< class T >
  const T& Stack< T >::top() const noexcept
  {
    return arr_.back();
  }

  template< class T >
  T& Stack< T >::top() noexcept
  {
    return arr_.back();
  }

  template< class T >
  void Stack< T >::push(T rhs)
  {
    arr_.push_back(rhs);
  }

  template< class T >
  T Stack< T >::drop()
  {
    if (arr_.empty())
    {
      throw std::underflow_error("empty");
    }
    T temp = top();
    arr_.pop_back();
    return temp;
  }
};

#endif
