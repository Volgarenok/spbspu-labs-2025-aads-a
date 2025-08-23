#ifndef NODE_HPP
#define NODE_HPP

namespace cherkasov
{
  template<typename T>
  struct Node
  {
    T data;
    Node * next;
    Node * prev;
    Node(T data):
    data(data),
    next(nullptr),
    prev(nullptr)
    {}
  };
}
#endif
