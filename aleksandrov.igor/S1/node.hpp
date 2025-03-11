#ifndef NODE_HPP
#define NODE_HPP

#include <cstdlib>

namespace aleksandrov
{
  template< typename T>
  struct Node
  {
    T data;
    Node< T >* next;

    Node():
      data(),
      next(nullptr)
    {}

    Node(const T& data):
      data(data),
      next(nullptr)
    {}
  };
}

#endif

