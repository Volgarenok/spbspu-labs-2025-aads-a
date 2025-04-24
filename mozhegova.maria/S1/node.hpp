#ifndef NODE_HPP
#define NODE_HPP

namespace mozhegova::detail
{
  template< typename T >
  struct Node
  {
    T data_;
    Node< T > * prev_;
    Node< T > * next_;

    explicit Node(const T & data):
      data_(data),
      prev_(nullptr),
      next_(nullptr)
    {}

    Node():
      data_(T()),
      prev_(nullptr),
      next_(nullptr)
    {}
  };
}

#endif
