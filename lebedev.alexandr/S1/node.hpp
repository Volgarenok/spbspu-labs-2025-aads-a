#ifndef NODE_HPP
#define NODE_HPP

namespace lebedev
{
  template< class T >
  struct Node
  {
    T data;
    Node* next;
    Node* prev;

    explicit Node(const T& value):
      data(value),
      next(nullptr),
      prev(nullptr)
    {}
  };
}
#endif
