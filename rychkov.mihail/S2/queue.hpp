#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "queue_base.hpp"

namespace rychkov
{
  template< class T >
  class Queue: public details::QueueBase< T, false >
  {
  public:
    using value_type = typename details::QueueBase< T, false >::value_type;
    using size_type = typename details::QueueBase< T, false >::size_type;
    using reference = typename details::QueueBase< T, false >::reference;
    using const_reference = typename details::QueueBase< T, false >::const_reference;

    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
  };
}

template< class T >
typename rychkov::Queue< T >::reference rychkov::Queue< T >::front()
{
  return (*this)[0];
}
template< class T >
typename rychkov::Queue< T >::const_reference rychkov::Queue< T >::front() const
{
  return (*this)[0];
}
template< class T >
typename rychkov::Queue< T >::reference rychkov::Queue< T >::back()
{
  return (*this)[this->size() - 1];
}
template< class T >
typename rychkov::Queue< T >::const_reference rychkov::Queue< T >::back() const
{
  return (*this)[this->size() - 1];
}

#endif
