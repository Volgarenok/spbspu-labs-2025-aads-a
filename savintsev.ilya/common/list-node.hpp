#ifndef LIST_NODE_HPP
#define LIST_NODE_HPP
#include <utility>

namespace savintsev
{
  template< typename T >
  struct ListNode
  {
    T data;
    ListNode< T > * next;
    ListNode< T > * prev;
  };
}

#endif
