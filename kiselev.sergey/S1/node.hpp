#ifndef NODE_HPP
#define NODE_HPP

namespace kiselev
{
  template < typename T >
  struct Node
  {
    T data_;
    Node< T >* next_;
    Node< T >* prev_;

    Node(const T& data):
      data_(data),
      next_(nullptr),
      prev_(nullptr)
    {}

    Node():
      data_(T()),
      next_(nullptr),
      prev_(nullptr)
    {}

  };
}
#endif
