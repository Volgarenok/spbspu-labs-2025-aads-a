#ifndef NODE_HPP
#define NODE_HPP

namespace kushekbaev
{
  template< typename T >
  struct Node
  {
    T data_;
    Node< T >* next_;
    Node(const T& data, Node< T >* next);
    Node(const T& data);
  };

  template< typename T >
  Node< T >::Node(const T& data, Node< T >* next):
    data_(data),
    next_(next)
  {}

  template< typename T >
  Node< T >::Node(const T& data):
    data_(data),
    next_(nullptr)
  {}
}

#endif
