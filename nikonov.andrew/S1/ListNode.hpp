namespace nikonov
{
  template< typename T >
  struct ListNode
  {
    T data;
    ListNode< T > * next;
  };
}
