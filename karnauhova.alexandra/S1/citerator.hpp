#ifndef CITERATOR_HPP
#define CITERATOR_HPP

#include <memory>
#include "nodelist.hpp"
namespace karnauhova
{
  template < typename T >
  class FwdList;

  template < typename T >
  struct ListIterator;

  template< typename T >
  struct ConstListIterator: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    using this_t = ConstListIterator< T >;
    ConstListIterator();
    ~ConstListIterator() = default;
    ConstListIterator(const this_t&) = default;
    ConstListIterator(ListIterator< T >& other);
    this_t& operator=(const this_t&) = default;

    this_t& operator++();
    this_t operator++(int);

    const T& operator*() const;
    const T* operator->() const;
    bool operator!=(const this_t&) const;
    bool operator==(const this_t&) const;
  private:
    NodeList< T >* node;
    friend class FwdList< T >;
    friend class ListIterator< T >;
    explicit ConstListIterator(NodeList< T >* element);
  };

  template< typename T >
  ConstListIterator< T >::ConstListIterator():
    node(nullptr)
  {}

  template < class T >
  ConstListIterator< T >::ConstListIterator(ListIterator< T >& other):
  node(other.node)
  {}

  template< typename T >
  ConstListIterator< T >::ConstListIterator(NodeList< T >* element):
    node(element)
  {}
  template< typename T >
  ConstListIterator< T >& ConstListIterator< T >::operator++()
  {
    node = node->next;
    return *this;
  }

  template< typename T >
  ConstListIterator< T > ConstListIterator< T >::operator++(int)
  {
    ConstListIterator< T > result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  bool ConstListIterator< T >::operator==(const this_t& rhs) const
  {
    return node == rhs.node;
  }

  template< typename T >
  bool ConstListIterator< T >::operator!=(const this_t& rhs) const
  {
    return !(rhs == *this);
  }

  template< typename T >
  const T& ConstListIterator< T >::operator*() const
  {
    return node->data;
  }

  template< typename T >
  const T* ConstListIterator< T >::operator->() const
  {
    return std::addressof(node->data);
  }
}
#endif
