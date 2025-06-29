#ifndef LIST_NODE_HPP
#define LIST_NODE_HPP

namespace maslov
{
  template< typename T >
  struct FwdListNode
  {
    T data;
    FwdListNode< T > * next;
  };
}

#endif
