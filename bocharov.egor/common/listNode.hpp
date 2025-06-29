#ifndef LISTNODE_HPP
#define LISTNODE_HPP
#include <utility>

namespace bocharov
{
  template< typename T >
  struct Node
  {
    T data_;
    Node< T > * next_;

    explicit Node(const T & data) : data_(data), next_(nullptr) {}
    explicit Node(T && data) : data_(std::move(data)), next_(nullptr) {}
  };
}
#endif
