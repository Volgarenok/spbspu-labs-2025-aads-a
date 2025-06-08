#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
#include <memory>
#include "node.hpp"

namespace finaev
{
  template< class T >
  class List;

  template< class T >
  struct listIterator: public std::iterator< std::forward_iterator_tag, T >
  {
    friend class List< T >;
  public:
    using this_t = listIterator< T >;
    listIterator();
    ~listIterator() = default;
    T& operator*();
    T* operator->();
    this_t& operator++();
    this_t operator++(int);
    this_t& operator--();
    this_t operator--(int);
    bool operator==(const this_t&) const;
    bool operator!=(const this_t&) const;
  private:
    Node< T >* node_;
    explicit listIterator(Node< T >* node);
  };

  template< class T >
  listIterator< T >::listIterator():
    node_(nullptr)
  {}

  template< class T >
  listIterator< T >::listIterator(Node< T >* node):
    node_(node)
  {}

  template< class T >
  T& listIterator< T >::operator*()
  {
    return node_->data;
  }

  template< class T >
  T* listIterator< T >::operator->()
  {
    return std::addressof(node_->data);
  }

  template< class T >
  listIterator< T >& listIterator< T >::operator++()
  {
    node_ = node_->next;
    return *this;
  }

  template< class T >
  listIterator< T > listIterator< T >::operator++(int)
  {
    this_t res(*this);
    ++(*this);
    return res;
  }

  template< class T >
  listIterator< T >& listIterator< T >::operator--()
  {
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  listIterator< T > listIterator< T >::operator--(int)
  {
    listIterator< T > res(*this);
    --(*this);
    return res;
  }

  template< class T >
  bool listIterator< T >::operator==(const this_t& i) const
  {
    return node_ == i.node_;
  }

  template< class T >
  bool listIterator< T >::operator!=(const this_t& i) const
  {
    return !(*this == i);
  }
}

#endif

