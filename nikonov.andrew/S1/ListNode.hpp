#ifndef LISTNODE_HPP
#define LISTNODE_HPP
namespace nikonov
{
  template< typename T >
  struct ListNode
  {
    T data;
    ListNode< T > * next;
  };
}
#endif
