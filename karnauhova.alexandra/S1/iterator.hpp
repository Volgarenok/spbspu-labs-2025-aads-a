#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <memory>
#include "nodelist.hpp"
namespace karnauhova
{
  template< typename T >
  struct ListIterator: public std::iterator< std::forward_iterator_tag, T >
  {
    NodeList< T >* node;
    using this_t = ListIterator< T >;

    ListIterator(): node(nullptr) {};
    ListIterator(NodeList< T >* element);
    ~ListIterator() = default;
    ListIterator(const this_t&) = default;
    this_t& operator=(const this_t&) = default;

    this_t& operator++();
    this_t operator++(int);

    T& operator*();
    T* operator->();
    bool operator!=(const this_t&) const;
    bool operator==(const this_t&) const;
  };

  template< typename T >
  ListIterator< T >::ListIterator(NodeList< T >* element):
    node(element)
  {}
  template< typename T >
  ListIterator< T >& ListIterator< T >::operator++()
  {
    node = node->next;
    return *this;
  }

  template< typename T >
  ListIterator< T > ListIterator<T>::operator++(int)
  {
    ListIterator< T > result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  bool ListIterator< T >::operator==(const this_t& rhs) const
  {
    return node == rhs.node;
  }

  template< typename T >
  bool ListIterator< T >::operator!=(const this_t& rhs) const
  {
    return !(rhs == *this);
  }

  template< typename T >
  T& ListIterator< T >::operator*()
  {
    return node->data;
  }

  template< typename T >
  T* ListIterator< T >::operator->()
  {
    return std::addressof(node->data);
  }
}
#endif
