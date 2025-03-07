#ifndef MY_LIST_NODE_HPP
#define MY_LIST_NODE_HPP

namespace zakirov
{
  template< typename T >
  struct MyListNode
  {
    T data_;
    MyListNode< T >* next_;

    MyListNode() = default;
    MyListNode(T data, MyListNode< T >* next);
  };

  template< typename T >
  MyListNode< T >::MyListNode(T data, MyListNode< T >* next):
    data_(data),
    next_(next)
  {}
}

#endif
