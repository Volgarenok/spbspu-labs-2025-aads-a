#ifndef FWD_LIST_NODE_HPP
#define FWD_LIST_NODE_HPP

namespace zakirov
{
  template< typename T >
  struct FwdListNode
  {
    T data_;
    FwdListNode< T > * next_;

    FwdListNode(const T & data);
    FwdListNode(const T & data, FwdListNode< T > * next);
  };

  template< typename T >
  FwdListNode< T >::FwdListNode(const T & data):
    data_(data),
    next_(nullptr)
  {}

  template< typename T >
  FwdListNode< T >::FwdListNode(const T & data, FwdListNode< T > * next):
    data_(data),
    next_(next)
  {}
}

#endif
