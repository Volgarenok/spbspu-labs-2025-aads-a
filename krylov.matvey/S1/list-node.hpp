#ifndef LISTNODE_HPP
#define LISTNODE_HPP

namespace krylov
{
  template < typename T >
  class Node
  {
  public:
    T data_;
    Node< T >* prev_;
    Node< T >* next_;
    Node(const T& value);
  };

  template < typename T >
  Node< T >::Node(const T& value):
    data_(value),
    prev_(nullptr),
    next_(nullptr)
  {}
}

#endif
