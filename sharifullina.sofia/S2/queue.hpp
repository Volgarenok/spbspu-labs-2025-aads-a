#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>
#include <list/list.hpp>

namespace sharifullina
{
  template< typename T >
  class Queue
  {
  public:
    Queue();
    Queue(const Queue< T > & other);
    Queue(Queue< T > && other) noexcept;
    ~Queue();

    Queue< T > & operator=(const Queue< T > & other);
    Queue< T > & operator=(Queue< T > && other) noexcept;

    void push(const T & value);
    void pop();
    T & front();
    const T & front() const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void swap(Queue< T > & other) noexcept;

  private:
    List< T > list_;
  };

  template< typename T >
  Queue< T >::Queue():
    list_()
  {}

  template< typename T >
  Queue< T >::Queue(const Queue< T > & other):
    list_(other.list_)
  {}

  template< typename T >
  Queue< T >::Queue(Queue< T > && other) noexcept:
    list_(std::move(other.list_))
  {}

  template< typename T >
  Queue< T >::~Queue()
  {}

  template< typename T >
  Queue< T > & Queue< T >::operator=(const Queue< T > & other)
  {
    if (this != &other)
    {
      Queue< T > temp(other);
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  Queue< T > & Queue< T >::operator=(Queue< T > && other) noexcept
  {
    if (this != &other)
    {
      Queue< T > temp(std::move(other));
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  void Queue< T >::push(const T & value)
  {
    list_.pushBack(value);
  }

  template< typename T >
  void Queue< T >::pop()
  {
    if (empty())
    {
      throw std::logic_error("empty queue");
    }
    list_.popFront();
  }

  template< typename T >
  T & Queue< T >::front()
  {
    if (empty())
    {
      throw std::logic_error("empty queue");
    }
    return list_.front();
  }

  template< typename T >
  const T & Queue< T >::front() const
  {
    if (empty())
    {
      throw std::logic_error("empty queue");
    }
    return list_.front();
  }

  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return list_.empty();
  }

  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return list_.size();
  }

  template< typename T >
  void Queue< T >::swap(Queue< T > & other) noexcept
  {
    list_.swap(other.list_);
  }
}

#endif
