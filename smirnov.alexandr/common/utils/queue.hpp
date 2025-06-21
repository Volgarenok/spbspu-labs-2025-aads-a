#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <stdexcept>
#include <list/list.hpp>
#include <list/list_utils.hpp>

namespace smirnov
{
  template< typename T >
  class Queue
  {
  public:
    void push(T rhs);
    T drop();
    T front() const;
    bool empty() const;
  private:
    List< T > data_;
  };

  template< typename T >
  void Queue< T >::push(T rhs)
  {
    pushBack(data_, rhs);
  }

  template< typename T >
  T Queue< T >::drop()
  {
    if (empty())
    {
        throw std::runtime_error("Queue is empty");
    }
    T val = data_.front();
    data_.pop_front();
    return val;
  }

  template< typename T >
  T Queue< T >::front() const
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    return data_.front();
  }

  template< typename T >
  bool Queue< T >::empty() const
  {
    return data_.empty();
  }
}
#endif
