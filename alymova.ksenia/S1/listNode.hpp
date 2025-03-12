#ifndef LIST_NODE_HPP
#define LIST_NODE_HPP

namespace alymova
{
  namespace detail
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
  }
}
#endif
