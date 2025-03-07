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

      ListNode();
      ListNode(const T &data);
      ListNode(T data, ListNode< T > *prev, ListNode< T > *next);
  };

  template< class T >
  ListNode< T >::ListNode():
    data_(),
    prev_(nullptr),
    next_(nullptr)
  {};

  template< class T >
  ListNode< T >::ListNode(const T &data):
    data_(data),
    prev_(nullptr),
    next_(nullptr)
  {};

  template< class T >
  ListNode< T >::ListNode(T data, ListNode< T > *prev, ListNode< T > *next):
    data_(data),
    prev_(prev),
    next_(next)
  {
  }
}
#endif
