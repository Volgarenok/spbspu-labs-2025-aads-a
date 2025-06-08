#ifndef NODE_HPP
#define NODE_HPP

namespace averenkov
{
  template< class T>
  struct Node
  {
    T data;
    Node* next;
  };
}

#endif
