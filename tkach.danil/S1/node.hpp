#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace tkach
{
  template< typename T >
  struct Node
  {
    T data_;
    Node< T >* next_;
    Node() = default;
    template< typename Q >
    Node(Q&& data);
    template< typename Q >
    Node(Q&& data, Node< T >* next);
    ~Node() =  default;
  };

  template< typename T >
  template< typename Q >
  Node< T >::Node(Q&& data):
    data_(std::forward< Q >(data)),
    next_(nullptr)
  {}

  template< typename T >
  template< typename Q >
  Node< T >::Node(Q&& data, Node< T > * next):
    data_(std::forward< Q >(data)),
    next_(next)
  {}
}

#endif
