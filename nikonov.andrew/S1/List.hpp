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
  List():
    fake(new ListNode< T >{ T(), nullptr })
  {}
  List(size_t k, const T & value):
    fake(new ListNode< T >{ T(), nullptr })
  {
    for (size_t i = 0; i < k; ++i)
    {
      ListNode< T > * newNode = new ListNode< T >{ value, fake };
      ListNode< T > * fakeIter = fake->next;
      if (fake->next == nullptr)
      {
        fake->next = newNode;
      }
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
  ListNode< T > * node;
  using this_t = ListIterator< T >;

  ListIterator():
    node(nullptr)
  {}
  ~ListIterator() = default;
  ListIterator(const this_t &) = default;
  this_t & operator=(const this_t &) = default;

  this_t & operator++()
  {
    node = node->next;
    return *this;
  }
  this_t operator++(int)
  {
    this_t tempCopy = *this;
    node = node->next;
    return tempCopy;
  }

  T & operator*()
  {
    return *node;
  }
  const T & operator*()
  {
    return *node;
  }

  T * operator->()
  {
    return node;
  }
  const T * operator->()
  {
    return node;
  }

  bool operator!=(const this_t &) const;
  bool operator==(const this_t &) const;
};
#endif
