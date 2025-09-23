#ifndef NODE_HPP
#define NODE_HPP

namespace abramov
{
  template< class T >
  struct Node
  {
    T data_;
    Node *prev_;
    Node *next_;

    Node(const T &data);
  };

  template< class T >
  Node< T >::Node(const T &data):
    data_(data),
    prev_(nullptr),
    next_(nullptr)
  {}
}
#endif
