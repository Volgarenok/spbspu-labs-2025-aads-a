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
    template< typename T1 >
    void push(T1 && element);
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
  template< typename T1 >
  void Queue< T >::push(T1 && element)
  {
    dynamic_array.push_back(std::forward< T1 >(element));
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
