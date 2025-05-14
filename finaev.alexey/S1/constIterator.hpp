#ifndef CONSTITERATOR_HPP
#define CONSTITERATOR_HPP
#include <memory>
#include "node.hpp"
#include "iterator.hpp"

namespace finaev
{
  template < class T >
  struct constListIterator: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    using this_t = constListIterator< T >;
    constListIterator();
    explicit constListIterator(const Node< T >* node);
    ~constListIterator() = default;
    const T& operator*() const;
    const T* operator->() const;
    this_t& operator++();
    this_t operator++(int);
    this_t& operator--();
    this_t operator--(int);
    bool operator==(const this_t& i) const;
    bool operator!=(const this_t& i) const;
  private:
    const Node< T >* node_;
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
