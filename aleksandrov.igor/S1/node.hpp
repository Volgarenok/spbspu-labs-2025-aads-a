#ifndef NODE_HPP
#define NODE_HPP

namespace aleksandrov
{
  template< typename T>
  struct Node
  {
    T data_;
    Node< T >* next_;

    Node():
      data_(T()),
      next_(nullptr)
    {}

    Node(const T& data):
      data_(data),
      next_(nullptr)
    {}
  };
}

#endif

