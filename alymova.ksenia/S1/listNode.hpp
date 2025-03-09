#ifndef LIST_NODE_HPP
#define LIST_NODE_HPP

namespace alymova
{
  template< typename T >
  struct ListNode
  {
    T data;
    ListNode< T >* next;
    ListNode< T >* prev;
  };

  template< typename T >
  struct EqualNode
  {
    const T& value;
    EqualNode(const T& new_value):
      value(new_value)
    {}
    bool operator()(const T& data)
    {
      return value == data;
    }
  };

  template< typename T >
  bool equal_node(const T& one, const T& two)
  {
    return one == two;
  }

  template< typename T >
  bool less_node(const T& one, const T& two)
  {
    return one < two;
  }
}
#endif
