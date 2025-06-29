#ifndef LISTNODE_HPP
#define LISTNODE_HPP
namespace nikonov::detail
{
  template< typename T >
  struct ListNode
  {
    T data;
    ListNode< T > * next;
  };
  template< typename T >
  struct FakeListNode
  { 
    ListNode< T > * next;
  };
}
#endif
