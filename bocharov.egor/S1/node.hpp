#ifndef NODE_HPP
#define NODE_HPP

namespace bocharov
{
  template< typename T >
  class Node
  {
  public:
    T data_;
    Node< T > * next_;
  };
}

#endif
