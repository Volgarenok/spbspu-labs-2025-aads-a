#ifndef STACK_HPP
#define STACK_HPP

#include "queue_base.hpp"

namespace rychkov
{
  template< class T >
  class Stack: public details::QueueBase< T, true >
  {
  public:
    using value_type = typename details::QueueBase< T, true >::value_type;
    using size_type = typename details::QueueBase< T, true >::size_type;
    using reference = typename details::QueueBase< T, true >::reference;
    using const_reference = typename details::QueueBase< T, true >::const_reference;

    reference top();
    const_reference top() const;
  };
}

template< class T >
typename rychkov::Stack< T >::reference rychkov::Stack< T >::top()
{
  return (*this)[this->size() - 1];
}
template< class T >
typename rychkov::Stack< T >::const_reference rychkov::Stack< T >::top() const
{
  return (*this)[this->size() - 1];
}

#endif
