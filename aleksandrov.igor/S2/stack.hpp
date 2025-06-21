#ifndef STACK_HPP
#define STACK_HPP

#include <list>
#include <cstddef>
#include <cassert>

namespace aleksandrov
{
  template< typename T >
  class Stack
  {
  public:
    Stack() = default;
    Stack(const Stack< T >&);
    Stack(Stack< T >&&) noexcept;

    Stack< T >& operator=(const Stack< T >&);
    Stack< T >& operator=(Stack< T >&&) noexcept;

    void push(const T&);
    void push(T&&);
    T drop();
    bool empty();
    size_t size();
    T& top();
  private:
    std::list< T > list_;
  };

  template< typename T >
  Stack< T >::Stack(const Stack< T >& rhs):
    list_(rhs.list_)
  {}

  template< typename T >
  Stack< T >::Stack(Stack< T >&& rhs) noexcept:
    list_(std::move(rhs.list_))
  {}

  template< typename T >
  Stack< T >& Stack< T >::operator=(const Stack< T >& rhs)
  {
    list_ = rhs.list_;
    return *this;
  }

  template< typename T >
  Stack< T >& Stack< T >::operator=(Stack< T >&& rhs) noexcept
  {
    list_ = std::move(rhs.list_);
    return *this;
  }

  template< typename T >
  void Stack< T >::push(const T& rhs)
  {
    list_.push_back(rhs);
  }

  template< typename T >
  void Stack< T >::push(T&& rhs)
  {
    list_.push_back(std::move(rhs));
  }

  template< typename T >
  T Stack< T >::drop()
  {
    assert(!empty());
    T last = list_.back();
    list_.pop_back();
    return last;
  }

  template< typename T >
  bool Stack< T >::empty()
  {
    return list_.empty();
  }

  template< typename T >
  size_t Stack< T >::size()
  {
    return list_.size();
  }

  template< typename T >
  T& Stack< T >::top()
  {
    assert(!empty());
    return list_.back();
  }
}

#endif

