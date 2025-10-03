#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <memory>
#include "node.hpp"

namespace lebedev
{
  template< class T >
  struct List;

  template< class T >
  struct Iterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
  public:
    using this_t = Iterator< T >;
    Iterator();
    ~Iterator() = default;
    T& operator*();
    T* operator->();
    this_t& operator++();
    this_t operator++(int);
    this_t& operator--();
    this_t operator--(int);
    bool operator==(const this_t& it) const;
    bool operator!=(const this_t& it) const;
  private:
    Node< T >* node_;
    explicit Iterator(Node< T >* node);
    friend class List< T >;
  };

  template< class T >
  Iterator< T >::Iterator():
    node_(nullptr)
  {}

  template< class T >
  Iterator< T >::Iterator(Node< T >* node):
    node_(node)
  {}

  template< class T >
  T& Iterator< T >::operator*()
  {
    return node_->data;
  }

  template< class T >
  T* Iterator< T >::operator->()
  {
    return std::addressof(this->node_);
  }

  template< class T >
  Iterator< T >& Iterator< T >::operator++()
  {
    node_ = node_->next;
    return *this;
  }

  template< class T >
  Iterator< T > Iterator< T >::operator++(int)
  {
    Iterator< T > it = *this;
    ++(*this);
    return it;
  }

  template< class T >
  Iterator< T >& Iterator< T >::operator--()
  {
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  Iterator< T > Iterator< T >::operator--(int)
  {
    Iterator< T > it = *this;
    --(*this);
    return it;
  }

  template< class T >
  bool Iterator< T >::operator==(const this_t& it) const
  {
    return node_ == it.node_;
  }

  template< class T >
  bool Iterator< T >::operator!=(const this_t& it) const
  {
    return !(*this == it);
  }
}
#endif
