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

    template< typename U >
    ListNode(U&& value, ListNode * nextNode = nullptr, ListNode * prevNode = nullptr):
      data(std::forward< U >(value)),
      next(nextNode),
      prev(prevNode)
    {}
    ListNode():
      data(),
      next(this),
      prev(this)
    {}
  };
}

#endif
