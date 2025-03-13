#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <memory>
#include "node.hpp"

namespace finaev
{
  template < class T >
  struct listIterator
  {
  public:
    using this_t = listIterator< T >;
    listIterator();
    listIterator(Node<T>* node);
    ~listIterator() = default;
    T& operator*() const;
    Node< T > operator->();
    this_t& operator++();
    this_t operator++(int);
    this_t& operator--();
    this_t operator--(int);
    bool operator==(const listIterator& i);
    bool operator!=(const listIterator& i);
  private:
    Node< T >* node_;
  };

  template < class T >
  listIterator < T >::listIterator():
    node_(nullptr)
  {}

  template < class T >
  listIterator < T >::listIterator(Node<T>* node):
    node_(node)
  {}

  template < class T >
  T& listIterator< T >::operator*() const
  {
    return node_->data;
  }

  template < class T >
  Node< T > listIterator< T >::operator->()
  {
    return std::addressof(this->node_);
  }

  template < class T >
  listIterator< T >& listIterator< T >::operator++()
  {
    node_ = node_->next;
    return *this;
  }

  template < class T >
  listIterator< T > listIterator< T >::operator++(int)
  {
    this_t res(*this);
    ++(*this);
    return res;
  }

  template < class T >
  listIterator< T >& listIterator< T >::operator--()
  {
    node_ = node_->prev;
    return *this;
  }

  template < class T >
  listIterator< T > listIterator< T >::operator--(int)
  {
    listIterator< T > res(*this);
    --(*this);
    return res;
  }

  template < class T >
  bool listIterator< T > ::operator==(const listIterator& i)
  {
    return node_ == i.node_;
  }

  template < class T >
  bool listIterator< T > ::operator!=(const listIterator& i)
  {
    return !(*this == i);
  }
}

#endif
