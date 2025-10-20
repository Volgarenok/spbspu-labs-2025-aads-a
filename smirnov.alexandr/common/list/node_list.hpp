#ifndef NODE_LIST_HPP
#define NODE_LIST_HPP

namespace smirnov
{
  template< typename T >
  struct ListNode
  {
    ListNode< T > * next;
    T data;
    ListNode():
      next(nullptr),
      data()
    {}
    ListNode(const T & value):
      next(nullptr),
      data(value)
    {}
  };
}
#endif
