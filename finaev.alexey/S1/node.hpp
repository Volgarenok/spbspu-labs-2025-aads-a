#ifndef NODE_HPP
#define NODE_HPP

namespace finaev
{
  template< class T >
  struct Node
  {
    Node(const T& data);
    T data;
    Node< T >* next;
    Node< T >* prev;
  };
}

template< class T >
finaev::Node< T >::Node(const T& data):
  data(data),
  next(nullptr),
  prev(nullptr)
{}

#endif
