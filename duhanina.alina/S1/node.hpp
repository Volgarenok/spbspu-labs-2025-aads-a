#ifndef NODE_HPP
#define NODE_HPP

namespace duhanina
{
  template < typename T >
  struct Node
  {
    T data_;
    Node* next_;
    Node();
    Node(const T& value);
  };

  template< typename T >
  Node< T >::Node():
    data_(T()),
    next_(nullptr)
  {}

  template< typename T >
  Node< T >::Node(const T& data):
    data_(data),
    next_(nullptr)
  {}
}

#endif
