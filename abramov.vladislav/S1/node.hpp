#ifndef NODE_HPP
#define NODE_HPP

namespace abramov
{
  template< class T >
  struct Node
  {
    Node(const T &data);
    T data_;
    Node *prev_;
    Node *next_;
  };

  template< class T >
  Node< T >::Node(const T &data):
    data_(data),
    prev_(nullptr),
    next_(nullptr)
  {}
}
#endif
