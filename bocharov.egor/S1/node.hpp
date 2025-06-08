#ifndef NODE_HPP
#define NODE_HPP

namespace bocharov
{
  template< typename T >
  struct Node
  {
    T data_;
    Node< T > * next_;
  };
}

#endif
