#ifndef NODE_HPP
#define NODE_HPP

namespace kiselev
{
  template < typename T >
  struct Node
  {
    T data;
    Node< T >* next;
    Node< T >* prev;

    //explicit Node(const T& data): data_(data), next_(nullptr), prev_(nullptr) {}
    //Node(): data_(T()), next_(nullptr), prev_(nullptr) {}
  };
}
#endif
