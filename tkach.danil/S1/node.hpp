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
    Node(const T& data);
    Node(const T& data, Node< T >* next);
    Node(T&& data);
    Node(T&& data, Node< T >* next);
    ~Node() = default;
  };

  template< typename T >
  Node< T >::Node(T&& data):
    data_(std::move(data)),
    next_(nullptr)
  {}

  template< typename T >
  Node< T >::Node(T&& data, Node< T > * next):
    data_(std::move(data)),
    next_(next)
  {}

  template< typename T >
  Node< T >::Node(const T& data, Node< T > * next):
    data_(data),
    next_(next)
  {}

  template< typename T >
  Node< T >::Node(const T& data):
    data_(data),
    next_(nullptr)
  {}

  template< typename T >
  Node< T >::Node():
    data_(),
    next_(nullptr)
  {}
}

#endif
