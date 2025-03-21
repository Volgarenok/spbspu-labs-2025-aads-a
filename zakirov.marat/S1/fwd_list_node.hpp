#ifndef FWD_LIST_NODE_HPP
#define FWD_LIST_NODE_HPP

namespace zakirov
{
  template< typename T >
  struct FwdListNode
  {
    T data_;
    FwdListNode< T > * next_;

    FwdListNode();
    FwdListNode(T data);
    FwdListNode(T data, FwdListNode< T > * next);
  };

  template< typename T >
  FwdListNode< T >::FwdListNode():
    data_(new T),
    next_(nullptr)
  {}

  template< typename T >
  FwdListNode< T >::FwdListNode(T data):
    data_(data),
    next_(nullptr)
  {}

  template< typename T >
  FwdListNode< T >::FwdListNode(T data, FwdListNode< T > * next):
    data_(data),
    next_(next)
  {}
}

#endif
