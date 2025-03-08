#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
#include "node.hpp"

namespace hismatova
{
  template< typename T >
  struct Iterator: public std::iterator< std::forward_iterator_tag, T >
  {
    Node< T >* node;
    ~Iterator() = default;
    Iterator(): node(nullptr) {}
    Iterator(Node< T >* num): node(num) {}
    Iterator(const Iterator< T >&) = default;
    Iterator< T >& operator=(const Iterator< T >&) = default;
    bool operator==(const Iterator< T >& right) const;
    bool operator!=(const Iterator< T >& right) const;
    Iterator< T >& operator++();
    Iterator< T > operator++(int);
    T* operator->();
    T& operator*();
  };
  template< typename T >
  bool Iterator< T >::operator==(const Iterator< T >& right) const
  {
    return node == right.node;
  }
  template< typename T >
  bool Iterator< T >::operator!=(const Iterator< T >& right) const
  {
    return !(node == right.node);
  }
  template< typename T >
  Iterator< T >& Iterator< T >::operator++()
  {
    node = node->next;
    return *this;
  }
  template< typename T >
  Iterator< T > Iterator< T >::operator++(int)
  {
    Iterator< T > result(*this);
    ++(*this);
    return result;
  }
  template< typename T >
  T* Iterator< T >::operator->()
  {
    return std::addressof(node->data);
  }
  template< typename T >
  T& Iterator< T >::operator*()
  {
    return node->data;
  }
}
#endif
