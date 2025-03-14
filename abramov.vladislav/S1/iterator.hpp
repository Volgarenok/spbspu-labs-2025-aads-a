#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include "list.hpp"

namespace abramov
{
  template< class T >
  struct List;

  template< class T >
  struct Iterator: std::iterator< std::bidirectional_iterator_tag, T >
  {
    Iterator();
    Iterator(Node< T > *node, const List< T > *list);
    Iterator(const Iterator &iter) = default;
    ~Iterator() = default;
    Iterator &operator=(const Iterator &iter) = default;
    Iterator &operator++();
    Iterator operator++(int);
    Iterator &operator--();
    Iterator operator--(int);
    bool operator==(const Iterator &iter) const;
    bool operator!=(const Iterator &iter) const;
    T &operator*();
    T *operator->();
  private:
    Node< T > *node_;
    const List< T > *list_;
    friend struct List< T >;
  };

  template< class T >
  Iterator< T >::Iterator():
    node_(nullptr),
    list_(nullptr)
  {}

  template< class T >
  Iterator< T >::Iterator(Node< T > *node, const List< T > *list):
    node_(node),
    list_(list)
  {}

  template< class T >
  Iterator< T > &Iterator< T >::operator++()
  {
    node_ = node_->next_;
    return *this;
  }

  template< class T >
  Iterator< T > Iterator< T >::operator++(int)
  {
    Iterator< T > iter(*this);
    ++(*this);
    return iter;
  }

  template< class T >
  Iterator< T > &Iterator< T >::operator--()
  {
    if (node_)
    {
      node_ = node_->prev_;
    }
    else
    {
      node_ = list_->tail_;
    }
    return *this;
  }

  template< class T >
  Iterator< T > Iterator< T >::operator--(int)
  {
    Iterator< T > iter(*this);
    --(*this);
    return iter;
  }

  template< class T >
  bool Iterator< T >::operator==(const Iterator< T > &iter) const
  {
    return node_ == iter.node_;
  }

  template< class T >
  bool Iterator< T >::operator!=(const Iterator< T > &iter) const
  {
    return !(iter == *this);
  }

  template< class T >
  T &Iterator< T >::operator*()
  {
    return node_->data_;
  }

  template< class T >
  T *Iterator< T >::operator->()
  {
    return std::addressof(node_->data_);
  }
}
#endif
