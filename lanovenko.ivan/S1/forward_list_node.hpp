#ifndef FORWARD_LIST_NODE_HPP
#define FORWARD_LIST_NODE_HPP

namespace lanovenko
{
  template< typename T >
  class ForwardListNode
  {
  public:
    T data_;
    ForwardListNode< T >* next_;
    explicit ForwardListNode(T data) noexcept;
  };

  template< typename T >
  ForwardListNode< T >::ForwardListNode(T data) noexcept:
    data_(data),
    next_(nullptr)
  {}
}

#endif
