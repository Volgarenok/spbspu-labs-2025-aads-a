#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <cassert>
#include <iterator>
#include "node.hpp"

namespace maslov
{
  template< typename T >
  struct FwdList;

  template< typename T >
  struct FwdListIterator final: public std::iterator< std::forward_iterator_tag, T >
  {
    friend struct FwdList< T >;
   public:
    using thisT = FwdListIterator< T >;

    FwdListIterator();
    FwdListIterator(const thisT &) = default;
    ~FwdListIterator() = default;

    thisT & operator=(const thisT &) = default;
    T & operator*();
    T * operator->();
    thisT & operator++();
    thisT operator++(int);
    bool operator!=(const thisT &) const;
    bool operator==(const thisT &) const;
   private:
    FwdListNode< T > * node_;
    explicit FwdListIterator(FwdListNode< T > * node);
  };

  template< typename T >
  FwdListIterator< T >::FwdListIterator():
    node_(nullptr)
  {}

  template< typename T >
  FwdListIterator< T >::FwdListIterator(FwdListNode< T > * node):
    node_(node)
  {}

  template< typename T >
  T & FwdListIterator< T >::operator*()
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< typename T >
  T * FwdListIterator< T >::operator->()
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename T >
  typename FwdListIterator< T >::thisT & FwdListIterator< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }

  template< typename T >
  typename FwdListIterator< T >::thisT FwdListIterator< T >::operator++(int)
  {
    assert(node_ != nullptr);
    thisT temp = *this;
    ++(*this);
    return temp;
  }

  template< typename T >
  bool FwdListIterator< T >::operator==(const thisT & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool FwdListIterator< T >::operator!=(const thisT & rhs) const
  {
    return node_ != rhs.node_;
  }
  template < typename T >
  struct FwdListConstIterator final: public std::iterator< std::forward_iterator_tag, T >
  {
    friend struct FwdList< T >;
   public:
    using thisT = FwdListConstIterator< T >;

    FwdListConstIterator();
    FwdListConstIterator(const thisT &) = default;
    ~FwdListConstIterator() = default;

    thisT & operator=(const thisT &) = default;
    thisT & operator++();
    thisT operator++(int);
    const T & operator*() const;
    const T * operator->() const;
    bool operator==(const thisT &) const;
    bool operator!=(const thisT &) const;
   private:
    FwdListNode< T > * node_;
    explicit FwdListConstIterator(FwdListNode< T > * node);
  };

  template< typename T >
  FwdListConstIterator< T >::FwdListConstIterator():
    node_(nullptr)
  {}

  template< typename T >
  FwdListConstIterator< T >::FwdListConstIterator(FwdListNode< T > * node):
    node_(node)
  {}

  template< typename T >
  const T & FwdListConstIterator< T >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< typename T >
  const T * FwdListConstIterator< T >::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename T >
  typename FwdListConstIterator< T >::thisT & FwdListConstIterator< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }

  template< typename T >
  typename FwdListConstIterator< T >::thisT FwdListConstIterator< T >::operator++(int)
  {
    assert(node_ != nullptr);
    thisT temp = *this;
    ++(*this);
    return temp;
  }

  template< typename T >
  bool FwdListConstIterator< T >::operator==(const thisT & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool FwdListConstIterator< T >::operator!=(const thisT & rhs) const
  {
    return node_ != rhs.node_;
  }
}
#endif
