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
    Node();
    template< class... Args >
    Node(Node< T >* next, Args&&... args);
    ~Node() = default;
  };

  template< typename T >
  template< class... Args >
  Node< T >::Node(Node< T >* next, Args&&... args):
    data_{T(std::forward< Args >(args)...)},
    next_(next)
  {}

  template< typename T >
  Node< T >::Node():
    data_(),
    next_(nullptr)
  {}
}

#endif
