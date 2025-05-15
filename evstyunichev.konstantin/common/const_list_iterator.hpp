#ifndef CONST_ITERATOR_HPP
#define CONST_ITERATOR_HPP
#include <memory>
#include <cassert>
#include <iterator>
#include "list_node.hpp"

namespace evstyunichev
{
  template< class T >
  class List;
  
  template< class T >
  class ConstListIterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend class List< T >;
    public:
      ConstListIterator();
      ConstListIterator(const ConstListIterator< T > &) = default;
      ~ConstListIterator() = default;
      ConstListIterator< T > & operator=(const ConstListIterator< T > &) = default;

      ConstListIterator< T > & operator++() noexcept;
      ConstListIterator< T > operator++(int);

      ConstListIterator< T > & operator--() noexcept;
      ConstListIterator< T > operator--(int);

      const T & operator*() const;
      const T * operator->() const;

      bool operator!=(const ConstListIterator< T > &) const;
      bool operator==(const ConstListIterator< T > &) const;

      ListNode< T > * getNode() const;

    private:
      const ListNode< T > *node_;
      ConstListIterator(const ListNode< T > *node);
  };

  template< class T >
  ConstListIterator< T >::ConstListIterator():
    node_(nullptr)
  {
  }

  template< class T >
  ConstListIterator< T >::ConstListIterator(const ListNode< T > *node):
    node_(node)
  {
  }

  template< class T >
  ConstListIterator< T > & ConstListIterator< T >::operator++() noexcept
  {
    node_ = node_->next_;
    return *this;
  }

  template< class T >
  ConstListIterator< T > ConstListIterator< T >::operator++(int)
  {
    ConstListIterator< T > res(node_);
    ++(*this);
    return res;
  }

  template< class T >
  ConstListIterator< T > & ConstListIterator< T >::operator--() noexcept
  {
    assert(node_ == nullptr);
    node_ = node_->prev_;
    return *this;
  }

  template< class T >
  ConstListIterator< T > ConstListIterator< T >::operator--(int)
  {
    assert(node_ == nullptr);
    ConstListIterator< T > res(node_);
    --(*this);
    return res;
  }

  template< class T >
  const T & ConstListIterator< T >::operator*() const
  {
    return node_->data_;
  }

  template< class T >
  const T * ConstListIterator< T >::operator->() const
  {
    return std::addressof(node_->data_);
  }

  template< class T >
  bool ConstListIterator< T >::operator==(const ConstListIterator< T > &it) const
  {
    return (node_ == it.node_);
  }

  template< class T >
  bool ConstListIterator< T >::operator!=(const ConstListIterator< T > &it) const
  {
    return !(*this == it);
  }

  template< class T >
  ListNode< T > * ConstListIterator< T >::getNode() const
  {
    return node_;
  }

}

#endif
