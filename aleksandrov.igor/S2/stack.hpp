#ifndef STACK_HPP
#define STACK_HPP

#include <list>
#include <cstddef>

namespace aleksandrov
{
  template< typename T >
  class Stack
  {
  public:
    void push(const T&);
    T drop();
    bool empty();
    size_t size();
    T& top();
  private:
    std::list< T > list_;
  };

  template< typename T >
  void Stack< T >::push(const T& rhs)
  {
    list_.push_back(rhs);
  }

  template< typename T >
  T Stack< T >::drop()
  {
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
    return list_.back();
  }
}

#endif

