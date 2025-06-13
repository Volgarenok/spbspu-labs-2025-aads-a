#ifndef FORWARD_LIST_NODE_HPP
#define FORWARD_LIST_NODE_HPP

namespace lanovenko
{
  template< typename T>
  class ForwardListNode
  {
  public:
    T data_;
    ForwardListNode<T>* next_;
    ForwardListNode(T data);
  };
}

template< typename T>
lanovenko::ForwardListNode<T>::ForwardListNode(T data):
  data_(data),
  next_(nullptr)
{}

#endif
