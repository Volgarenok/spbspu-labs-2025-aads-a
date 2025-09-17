#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <cassert>
#include <memory>
#include "list_node.hpp"

namespace evstyunichev
{
  template< typename T >
  class ListIterator
  {
    public:
      ListIterator();
      ListIterator(const ListIterator< T > &) = default;
      ListIterator(ListNode< T > *node);
      ~ListIterator() = default;
      ListIterator< T > & operator=(const ListIterator< T > &) = default;

      ListIterator< T > & operator++() noexcept;
      ListIterator< T > operator++(int);

      ListIterator< T > & operator--() noexcept;
      ListIterator< T > operator--(int);

      T & operator*();
      T * operator->();

      bool operator!=(const ListIterator< T > &) const;
      bool operator==(const ListIterator< T > &) const;

      ListNode< T > * getNode() const;

  private:
    ListNode< T > *node_;
  };

  template< class T >
  ListIterator< T >::ListIterator():
    node_(nullptr)
  {
  }

  template< class T >
  ListIterator< T >::ListIterator(ListNode< T > *node):
    node_(node)
  {
  }

  template< class T >
  ListIterator< T > & ListIterator< T >::operator++() noexcept
  {
    node_ = node_->next_;
    return *this;
  }

  template< class T >
  ListIterator< T > ListIterator< T >::operator++(int)
  {
    ListIterator< T > res(node_);
    ++(*this);
    return res;
  }

  template< class T >
  ListIterator< T > & ListIterator< T >::operator--() noexcept
  {
    assert(node_ == nullptr);
    node_ = node_->prev_;
    return *this;
  }

  template< class T >
  ListIterator< T > ListIterator< T >::operator--(int)
  {
    assert(node_ == nullptr);
    ListIterator< T > res(node_);
    --(*this);
    return res;
  }

  template< class T >
  T & ListIterator< T >::operator*()
  {
    return node_->data_;
  }

  template< class T >
  T * ListIterator< T >::operator->()
  {
    return std::addressof(node_->data_);
  }

  template< class T >
  bool ListIterator< T >::operator==(const ListIterator< T > &it) const
  {
    return (node_ == it.node_);
  }

  template< class T >
  bool ListIterator< T >::operator!=(const ListIterator< T > &it) const
  {
    return !(*this == it);
  }

  template< class T >
  ListNode< T > * ListIterator< T >::getNode() const
  {
    return node_;
  }

}

#endif
