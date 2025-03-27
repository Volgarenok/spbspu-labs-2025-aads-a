#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <list>
#include <cstddef>

namespace aleksandrov
{
  template< typename T >
  class Queue
  {
  public:
    void push(const T&);
    T drop();
    bool empty();
    size_t size();
    T& front();
  private:
    std::list< T > list_;
  };

  template< typename T >
  void Queue< T >::push(const T& rhs)
  {
    list_.push_back(rhs);
  }

  template< typename T >
  T Queue< T >::drop()
  {
    T first = list_.front();
    list_.pop_front();
    return first;
  }

  template< typename T >
  bool Queue< T >::empty()
  {
    return list_.empty();
  }

  template< typename T >
  size_t Queue< T >::size()
  {
    return list_.size();
  }

  template< typename T >
  T& Queue< T >::front()
  {
    return list_.front();
  }
}

#endif

