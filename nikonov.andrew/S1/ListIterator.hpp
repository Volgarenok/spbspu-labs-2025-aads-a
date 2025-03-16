#ifndef LISTITERATOR_HPP
#define LISTITERATOR_HPP
#include "ListNode.hpp"
#include <iterator>
#include <memory>
#include <cassert>
namespace nikonov
{
  template< typename T >
  struct ListIterator: public std::iterator< std::forward_iterator_tag, T >
  {
    ListNode< T > * node;
    using this_t = ListIterator< T >;

    ListIterator();
    ListIterator(ListNode< T > * ptr = nullptr);
    ~ListIterator() = default;
    ListIterator(const this_t &) = default;
    this_t & operator=(const this_t &) = default;
    this_t & operator++();
    this_t operator++(int);
    T & operator*();
    const T & operator*() const;
    T * operator->();
    const T * operator->() const;
    bool operator!=(const this_t & rhs) const;
    bool operator==(const this_t & rhs) const;
  };

  template< typename T >
  ListIterator< T >::ListIterator():
    node(nullptr)
  {}

  template< typename T >
  ListIterator< T >::ListIterator(ListNode< T > * ptr):
    node(ptr)
  {}

  template< typename T >
  ListIterator< T > & ListIterator< T >::operator++()
  {
    assert (node != nullptr);
    node = node->next;
    return *this;
  }

  template< typename T >
  ListIterator< T > ListIterator< T >::operator++(int)
  {
    assert(node != nullptr);
    this_t tempCopy(*this);
    ++(*this);
    return tempCopy;
  }

  template< typename T >
  T & ListIterator< T >::operator*()
  {
    assert(node != nullptr);
    return node->data;
  }
  template< typename T >
  const T & ListIterator< T >::operator*() const
  {
    assert(node != nullptr);
    return node->data;
  }
  template< typename T >
  T * ListIterator< T >::operator->()
  {
    assert(node != nullptr);
    return std::addressof(node->data);
  }
  template< typename T >
  const T * ListIterator< T >::operator->() const
  {
    assert(node != nullptr);
    return std::addressof(node->data);
  }

  template< typename T >
  bool ListIterator< T >::operator!=(const this_t & rhs) const
  {
    return !(*this == rhs);
  }

  template< typename T >
  bool ListIterator< T >::operator==(const this_t & rhs) const
  {
    return node == rhs.node;
  }

  template< typename T >
  struct ConstListIterator: public std::iterator< std::forward_iterator_tag, T >
  {
    const ListNode< T > * node;
    using this_t = ConstListIterator< T >;
    ConstListIterator(ListNode< T > * ptr = nullptr);
    ConstListIterator();
    ~ConstListIterator() = default;
    ConstListIterator(const this_t &) = default;
    this_t & operator=(const this_t &) = default;
    this_t & operator++();
    this_t operator++(int);
    const T & operator*() const;
    const T * operator->() const;
    bool operator!=(const this_t & rhs) const;
    bool operator==(const this_t & rhs) const;
  };

  template< typename T >
  ConstListIterator< T >::ConstListIterator():
    node(nullptr)
  {}

  template< typename T >
  ConstListIterator< T >::ConstListIterator(ListNode< T > * ptr):
    node(ptr)
  {}

  template< typename T >
  ConstListIterator< T > & ConstListIterator< T >::operator++()
  {
    assert(node != nullptr);
    node = node->next;
    return *this;
  }

  template< typename T >
  ConstListIterator< T > ConstListIterator< T >::operator++(int)
  {
    assert(node != nullptr);
    this_t tempCopy(*this);
    ++(*this);
    return tempCopy;
  }

  template< typename T >
  const T & ConstListIterator< T >::operator*() const
  {
    assert(node != nullptr);
    return node->data;
  }

  template< typename T >
  const T * ConstListIterator< T >::operator->() const
  {
    assert(node != nullptr);
    return std::addressof(node->data);
  }

  template< typename T >
  bool ConstListIterator< T >::operator!=(const this_t & rhs) const
  {
    return !(*this == rhs);
  }

  template< typename T >
  bool ConstListIterator< T >::operator==(const this_t & rhs) const
  {
    return node == rhs.node;
  }
}
#endif
