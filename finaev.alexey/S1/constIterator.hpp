#ifndef CONSTITERATOR_HPP
#define CONSTITERATOR_HPP
#include <iterator>
#include <memory>
#include "node.hpp"

namespace finaev
{
  template< class T >
  class List;

  template< class T >
  struct constListIterator: public std::iterator< std::forward_iterator_tag, T >
  {
    friend class List< T >;
  public:
    using this_t = constListIterator< T >;
    constListIterator();
    ~constListIterator() = default;
    const T& operator*() const;
    const T* operator->() const;
    this_t& operator++();
    this_t operator++(int);
    this_t& operator--();
    this_t operator--(int);
    bool operator==(const this_t&) const;
    bool operator!=(const this_t&) const;
  private:
    const Node< T >* node_;
    explicit constListIterator(const Node< T >* node);
  };

  template< class T >
  constListIterator< T >::constListIterator():
    node_(nullptr)
  {}

  template< class T >
  constListIterator< T >::constListIterator(const Node< T >* node):
    node_(node)
  {}

  template< class T >
  const T&  constListIterator< T >::operator*() const
  {
    return node_->data;
  }

  template< class T >
  const T* constListIterator< T >::operator->() const
  {
    return std::addressof(node_->data);
  }

  template< class T >
  constListIterator< T >& constListIterator< T >::operator++()
  {
    node_ = node_->next;
    return *this;
  }

  template< class T >
  constListIterator< T > constListIterator< T >::operator++(int)
  {
    constListIterator< T > res(*this);
    ++(*this);
    return res;
  }

  template< class T >
  constListIterator< T >& constListIterator< T >::operator--()
  {
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  constListIterator< T > constListIterator< T >::operator--(int)
  {
    constListIterator< T > res(*this);
    --(*this);
    return res;
  }

  template< class T >
  bool constListIterator< T >::operator==(const this_t& i) const
  {
    return node_ == i.node_;
  }

  template< class T >
  bool constListIterator< T >::operator!=(const this_t& i) const
  {
    return !(node_ == i.node_);
  }
}

#endif
