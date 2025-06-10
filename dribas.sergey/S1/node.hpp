#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace dribas
{
  template < class T >
  struct Node
  {
    T data_;
    Node< T >* next_;
    Node< T >* prev_;
 };
}

#endif
