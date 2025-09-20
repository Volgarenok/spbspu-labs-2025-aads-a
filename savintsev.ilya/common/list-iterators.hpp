#ifndef LIST_ITERATORS_HPP
#define LIST_ITERATORS_HPP
#include <iterator>
#include "list-node.hpp"

namespace savintsev
{
  template< typename T >
  class List;

  template< typename T >
  class ConstIterator;

  template< typename T >
  class Iterator:
    public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend class List< T >;
    friend class ConstIterator< T >;
  public:
    Iterator();
    T & operator*();
    T * operator->();
    const T & operator*() const;
    const T * operator->() const;
    Iterator & operator++();
    Iterator operator++(int);
    Iterator & operator--();
    Iterator operator--(int);
    bool operator!=(const Iterator & rhs) const;
    bool operator==(const Iterator & rhs) const;
  private:
    ListNode< T > * node;
    Iterator(ListNode< T > * rhs);
  };

  template< typename T >
  class ConstIterator:
    public std:: iterator< std::bidirectional_iterator_tag, T >
  {
    template< typename U >
    friend class List;
  public:
    ConstIterator();
    ConstIterator(Iterator< T > rhs);
    const T & operator*() const;
    const T * operator->() const;
    ConstIterator & operator++();
    ConstIterator operator++(int);
    ConstIterator & operator--();
    ConstIterator operator--(int);
    bool operator!=(const ConstIterator & rhs) const;
    bool operator==(const ConstIterator & rhs) const;
  private:
    ListNode< T > * node;
    ConstIterator(ListNode< T > * rhs);
  };
}

template< typename T >
savintsev::Iterator< T >::Iterator():
  node(nullptr)
{}

template< typename T >
savintsev::Iterator< T >::Iterator(ListNode< T > * rhs):
  node(rhs)
{}

template< typename T >
T & savintsev::Iterator< T >::operator*()
{
  return node->data;
}

template< typename T >
T * savintsev::Iterator< T >::operator->()
{
  return std::addressof(node->data);
}

template< typename T >
const T & savintsev::Iterator< T >::operator*() const
{
  return node->data;
}

template< typename T >
const T * savintsev::Iterator< T >::operator->() const
{
  return std::addressof(node->data);
}

template< typename T >
savintsev::Iterator< T > & savintsev::Iterator< T >::operator++()
{
  node = node->next;
  return *this;
}

template< typename T >
savintsev::Iterator< T > savintsev::Iterator< T >::operator++(int)
{
  Iterator result(*this);
  ++(*this);
  return result;
}

template< typename T >
savintsev::Iterator< T > & savintsev::Iterator< T >::operator--()
{
  node = node->prev;
  return *this;
}

template< typename T >
savintsev::Iterator< T > savintsev::Iterator< T >::operator--(int)
{
  Iterator result(*this);
  --(*this);
  return result;
}

template< typename T >
bool savintsev::Iterator< T >::operator!=(const Iterator & rhs) const
{
  return node != rhs.node;
}

template< typename T >
bool savintsev::Iterator< T >::operator==(const Iterator & rhs) const
{
  return node == rhs.node;
}

template< typename T >
savintsev::ConstIterator< T >::ConstIterator():
  node(nullptr)
{}

template< typename T >
savintsev::ConstIterator< T >::ConstIterator(ListNode< T > * rhs):
  node(rhs)
{}

template< typename T >
savintsev::ConstIterator< T >::ConstIterator(Iterator< T > rhs):
  node(rhs.node)
{}

template< typename T >
const T & savintsev::ConstIterator< T >::operator*() const
{
  return node->data;
}

template< typename T >
const T * savintsev::ConstIterator< T >::operator->() const
{
  return std::addressof(node->data);
}

template< typename T >
savintsev::ConstIterator< T > & savintsev::ConstIterator< T >::operator++()
{
  node = node->next;
  return *this;
}

template< typename T >
savintsev::ConstIterator< T > savintsev::ConstIterator< T >::operator++(int)
{
  ConstIterator result(*this);
  ++(*this);
  return result;
}

template< typename T >
savintsev::ConstIterator< T > & savintsev::ConstIterator< T >::operator--()
{
  node = node->prev;
  return *this;
}

template< typename T >
savintsev::ConstIterator< T > savintsev::ConstIterator< T >::operator--(int)
{
  ConstIterator result(*this);
  --(*this);
  return result;
}

template< typename T >
bool savintsev::ConstIterator< T >::operator!=(const ConstIterator & rhs) const
{
  return node != rhs.node;
}

template< typename T >
bool savintsev::ConstIterator< T >::operator==(const ConstIterator & rhs) const
{
  return node == rhs.node;
}

#endif
