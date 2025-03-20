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
    explicit ListIterator(ListNode< T > * ptr = nullptr);
    ~ListIterator() = default;
    ListIterator(const this_t &) = default;
    this_t & operator=(const this_t &) = default;
    this_t & operator++() noexcept;
    this_t operator++(int) noexcept;
    T & operator*() noexcept;
    const T & operator*() const noexcept;
    T * operator->() noexcept;
    const T * operator->() const noexcept;
    bool operator!=(const this_t & rhs) const noexcept;
    bool operator==(const this_t & rhs) const noexcept;
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
  ListIterator< T > & ListIterator< T >::operator++() noexcept
  {
    assert (node != nullptr);
    node = node->next;
    return *this;
  }

  template< typename T >
  ListIterator< T > ListIterator< T >::operator++(int) noexcept
  {
    assert(node != nullptr);
    this_t tempCopy(*this);
    ++(*this);
    return tempCopy;
  }

  template< typename T >
  T & ListIterator< T >::operator*() noexcept
  {
    assert(node != nullptr);
    return node->data;
  }
  template< typename T >
  const T & ListIterator< T >::operator*() const noexcept
  {
    assert(node != nullptr);
    return node->data;
  }
  template< typename T >
  T * ListIterator< T >::operator->() noexcept
  {
    assert(node != nullptr);
    return std::addressof(node->data);
  }
  template< typename T >
  const T * ListIterator< T >::operator->() const noexcept
  {
    assert(node != nullptr);
    return std::addressof(node->data);
  }

  template< typename T >
  bool ListIterator< T >::operator!=(const this_t & rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< typename T >
  bool ListIterator< T >::operator==(const this_t & rhs) const noexcept
  {
    return node == rhs.node;
  }

  template< typename T >
  struct ConstListIterator: public std::iterator< std::forward_iterator_tag, T >
  {
    const ListNode< T > * node;
    using this_t = ConstListIterator< T >;
  
    explicit ConstListIterator(ListNode< T > * ptr = nullptr);
    ConstListIterator();
    ~ConstListIterator() = default;
    ConstListIterator(const this_t &) = default;
    this_t & operator=(const this_t &) = default;
    this_t & operator++() noexcept;
    this_t operator++(int) noexcept;
    const T & operator*() const noexcept;
    const T * operator->() const noexcept;
    bool operator!=(const this_t & rhs) const noexcept;
    bool operator==(const this_t & rhs) const noexcept;
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
  ConstListIterator< T > & ConstListIterator< T >::operator++() noexcept
  {
    assert(node != nullptr);
    node = node->next;
    return *this;
  }

  template< typename T >
  ConstListIterator< T > ConstListIterator< T >::operator++(int) noexcept
  {
    assert(node != nullptr);
    this_t tempCopy(*this);
    ++(*this);
    return tempCopy;
  }

  template< typename T >
  const T & ConstListIterator< T >::operator*() const noexcept
  {
    assert(node != nullptr);
    return node->data;
  }

  template< typename T >
  const T * ConstListIterator< T >::operator->() const noexcept
  {
    assert(node != nullptr);
    return std::addressof(node->data);
  }

  template< typename T >
  bool ConstListIterator< T >::operator!=(const this_t & rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< typename T >
  bool ConstListIterator< T >::operator==(const this_t & rhs) const noexcept
  {
    return node == rhs.node;
  }
}
#endif
