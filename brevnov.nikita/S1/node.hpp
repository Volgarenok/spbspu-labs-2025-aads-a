#ifndef NODE_HPP
#define NODE_HPP
namespace brevnov
{
  template< typename T>
  struct Node
  {
    T data;
    Node< T > * next;
    Node< T > * prev;
  };
}
#endif
