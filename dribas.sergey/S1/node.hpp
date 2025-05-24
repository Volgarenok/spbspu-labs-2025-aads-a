#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace dribas
{
  template < class T >
  class Node
  {
  public:
    T data_;
    Node< T >* next_;
    Node< T >* prev_;

  Node():
    data_(T()),
    next_(nullptr),
    prev_(nullptr)
  {}

  Node(T&& data):
    data_(std::move(data)),
    next_(nullptr),
    prev_(nullptr)
  {}

  Node(const T& data):
    data_(data),
    next_(nullptr),
    prev_(nullptr)
  {}
 };
}

#endif
