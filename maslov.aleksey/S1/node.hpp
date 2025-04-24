#ifndef NODE_HPP
#define NODE_HPP

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
