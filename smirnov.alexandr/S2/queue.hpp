#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <list>
#include <stdexcept>

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
    std::list< T > data_;
  };

  template< typename T >
  void Queue< T >::push(T rhs)
  {
    data_.push_back(rhs);
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
