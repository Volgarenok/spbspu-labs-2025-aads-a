#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "deque.hpp"

namespace asafov
{
  template< class T >
  class Queue : public Deque< T >
  {
  public:
    void push(const T& value)
    {
      this->push_back(value);
    }

    void pop()
    {
      this->pop_front();
    }

    T& front()
    {
      return Deque<T>::front();
    }

    const T& front() const
    {
      return Deque<T>::front();
    }
  };
}
#endif
