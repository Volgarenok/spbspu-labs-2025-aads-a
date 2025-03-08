#ifndef LIST_NODE_HPP
#define LIST_NODE_HPP

namespace savintsev
{
  template< typename T >
  struct ListNode
  {
    T * data;
    ListNode< T > * next;
    ListNode< T > * prev;
    ListNode(const T & value, ListNode * nextNode = nullptr, ListNode * prevNode = nullptr):
      data(new T(value)),
      next(nextNode),
      prev(prevNode)
    {}
    ListNode():
      data(nullptr),
      next(this),
      prev(this)
    {}
    ~ListNode()
    {
      delete data;
    }
  };
}

#endif
