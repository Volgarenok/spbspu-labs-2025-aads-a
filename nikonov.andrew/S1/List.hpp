#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
template< typename T >
struct ListNode
{
  T data;
  ListNode< T > * next;
};
template< typename T >
class List
{
  ListNode< T > * fake;
public:
  List(size_t k, const T & value):
    fake(new ListNode< T >{ T(), nullptr })
  {
    for (size_t i = 0; i < k; ++i)
    {
      ListNode< T > * newNode = new ListNode< T >{ value, fake };
      ListNode< T > * fakeIter = fake->next;
      while (fakeIter->next != fake)
      {
        fakeIter = fakeIter->next;
      }
      fakeIter->next = newNode;
    }
  }
};
template< typename T >
struct ListIterator
{
  List< T > * node;
  using this_t = ListIterator< T >;

  ListIterator(): node(nullptr) {}
  ~ListIterator() = default;
  ListIterator(const this_t &) = default;
  this_t & operator=(const this_t &) = default;
  
  this_t & operator++();
  this_t operator++(int);

  T & operator*(); //+ const-версия
  T * operator->(); //+ const-версия

  bool operator!=(const this_t &) const;
  bool operator==(const this_t &) const;
};
#endif
