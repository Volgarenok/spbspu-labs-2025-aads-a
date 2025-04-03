#ifndef NODE_HPP
#define NODE_HPP

namespace duhanina
{
  template < typename T >
  struct Node
  {
    T data_;
    Node* next_;
  };
}

#endif
