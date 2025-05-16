#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "dynamic_array.hpp"
#include <cstddef>
#include <memory>
#include <stdexcept>

namespace petrov
{
  template< typename T >
  struct Queue
  {
  public:
    using this_t = Queue< T >;
    Queue()
    {}
    template< typename U >
    Queue(this_t && rhs);
    ~Queue() = default;
    template< typename U >
    this_t & operator=(this_t && rhs);
    template< typename U >
    void push(U && element);
    void pop();
    T & front();
    const T & front() const;
    T & back();
    const T & back() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
  private:
    DynamicArray< T > dynamic_array;
  };

  template< typename T >
  template< typename U >
  Queue< T >::Queue(this_t && rhs)
  {
    dynamic_array(rhs.dynamic_array);
  }

  template< typename T >
  template< typename U >
  typename Queue< T >::this_t & Queue< T >::operator=(this_t && rhs)
  {
    dynamic_array = rhs.dynamic_array;
    return *this;
  }

  template< typename T >
  template< typename U >
  void Queue< T >::push(U && element)
  {
    dynamic_array.push_back(element);
  }

  template< typename T >
  void Queue< T >::pop()
  {
    dynamic_array.pop_front();
  }

  template< typename T >
  T & Queue< T >::front()
  {
    return dynamic_array.front();
  }

  template< typename T >
  const T & Queue< T >::front() const
  {
    return dynamic_array.front();
  }

  template< typename T >
  T & Queue< T >::back()
  {
    return dynamic_array.back();
  }

  template< typename T >
  const T & Queue< T >::back() const
  {
    return dynamic_array.back();
  }

  template< typename T >
  bool Queue< T >::empty() const noexcept
  {
    return dynamic_array.empty();
  }

  template< typename T >
  size_t Queue< T >::size() const noexcept
  {
    return dynamic_array.size();
  }
}

#endif
