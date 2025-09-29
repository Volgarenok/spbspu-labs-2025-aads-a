#ifndef CONSTITERATOR_HPP
#define CONSTITERATOR_HPP
#include <memory>
#include "node.hpp"
#include "iterator.hpp"

namespace lebedev
{
  template< class T >
  struct List;

  template< class T >
  struct constIterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
  public:
    using this_t = constIterator< T >;
    constIterator();
    explicit constIterator(const Node< T >* node);
    ~constIterator() = default;
    const T& operator*() const;
    const T* operator->() const;
    this_t& operator++();
    this_t operator++(int);
    this_t& operator--();
    this_t operator--(int);
    bool operator==(const this_t& it) const;
    bool operator!=(const this_t& it) const;
  private:
    const Node< T >* node_;
  };

  template< class T >
  constIterator< T >::constIterator():
    node_(nullptr)
  {}

  template< class T >
  constIterator< T >::constIterator(const Node< T >* node):
    node_(node)
  {}

  template< class T >
  const T& constIterator< T >::operator*() const
  {
    return node_->data;
  }

  template< class T >
  const T* constIterator< T >::operator->() const
  {
    return std::addressof(node_->data);
  }

  template< class T >
  constIterator< T >& constIterator< T >::operator++()
  {
    node_ = node_->next;
    return *this;
  }

  template< class T >
  constIterator< T > constIterator< T >::operator++(int)
  {
    constIterator< T > it = *this;
    ++(*this);
    return it;
  }

  template< class T >
  constIterator< T >& constIterator< T >::operator--()
  {
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  constIterator< T > constIterator< T >::operator--(int)
  {
    constIterator< T > it = *this;
    --(*this);
    return it;
  }

  template< class T >
  bool constIterator< T >::operator==(const this_t& it) const
  {
    return node_ == it.node_;
  }

  template< class T >
  bool constIterator< T >::operator!=(const this_t& it) const
  {
    return !(*this == it);
  }
}
#endif
