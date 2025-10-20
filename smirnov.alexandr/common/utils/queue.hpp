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
    void push(const T & value);
    void push(T && value);
    void pop();
    T & front();
    const T & front() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
  private:
    List< T > data_;
  };

  template< typename T >
  void Queue< T >::push(const T & value)
  {
    pushBack(data_, value);
  }

  template< typename T >
  void Queue< T >::push(T && value)
  {
    pushBack(data_, std::move(value));
  }

  template< typename T >
  void Queue< T >::pop()
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    data_.pop_front();
  }

  template< typename T >
  const T & Queue< T >::front() const
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    return data_.front();
  }

  template< typename T >
  T & Queue< T >::front()
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    return data_.front();
  }

  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return data_.empty();
  }

  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return data_.size();
  }
}
#endif
