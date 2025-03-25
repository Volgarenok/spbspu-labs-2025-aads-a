#ifndef NODE_HPP
#define NODE_HPP

namespace ivanova
{
  template < typename T >
  struct Node
  {
    T data_;
    Node* next_;
  };
}

#endif
