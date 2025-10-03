#ifndef NODE_HPP
#define NODE_HPP

namespace sharifullina
{
  template< typename T >
  struct Node
  {
    T data_;
    Node< T > * prev_;
    Node< T > * next_;

    Node(const T & data, Node< T > * prev = nullptr, Node< T > * next = nullptr);
    Node();
  };

  template< typename T >
  Node< T >::Node(const T & data, Node< T > * prev, Node< T > * next):
    data_(data),
    prev_(prev),
    next_(next)
  {}

  template< typename T >
  Node< T >::Node():
    data_(T()),
    prev_(nullptr),
    next_(nullptr)
  {}
}

#endif
