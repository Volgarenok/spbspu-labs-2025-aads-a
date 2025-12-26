#ifndef LIST_NODE_HPP
#define LIST_NODE_HPP
#include <cstddef>
namespace evstyunichev
{
  template< class T >
  class ListNode
  {
    public:
      T data_;
      ListNode< T > *prev_;
      ListNode< T > *next_;
      ListNode(const T &data);
      ListNode(T &&data);
      ListNode();
  };

  template< class T >
  ListNode< T >::ListNode(const T &data):
    data_(data),
    prev_(nullptr),
    next_(nullptr)
  {};

  template< class T >
  ListNode< T >::ListNode(T &&data):
    data_(data),
    prev_(nullptr),
    next_(nullptr)
  {};

  template< class T >
  ListNode< T >::ListNode():
    data_(T{}),
    prev_(this),
    next_(this)
  {};

}
#endif
