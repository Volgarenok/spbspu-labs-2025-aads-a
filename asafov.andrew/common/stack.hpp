#ifndef STACK_HPP
#define STACK_HPP
#include <deque.hpp>

namespace asafov
{
  template< class T >
  class Stack : public Deque<T>
  {
  public:
    void push(const T& value)
    {
      this->push_back(value);
    }

    void pop()
    {
      this->pop_back();
    }

    T& top()
    {
      return this->back();
    }

    const T& top() const
    {
      return this->back();
    }
  };
}
#endif
