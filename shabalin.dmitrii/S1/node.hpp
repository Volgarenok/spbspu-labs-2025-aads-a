#ifndef NODE_HPP
#define NODE_HPP

namespace shabalin
{
  template < typename T >
  struct Node
  {
    T data_;
    Node< T > * next_;

    explicit Node(const T & data):
      data_(data),
      next_(nullptr)
    {}
  };
}

#endif