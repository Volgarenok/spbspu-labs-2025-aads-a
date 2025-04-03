#ifndef ITERATORS_HPP
#define ITERATORS_HPP

#include <iterator>
#include "node.hpp"

namespace shramko
{
  template< typename T >
  class FwdListIterator final: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    using this_t = FwdListIterator< T >;

    FwdListIterator();
    FwdListIterator(const this_t&) = default;
    FwdListIterator(FwdListNode< T >*);
    ~FwdListIterator() = default;

    this_t& operator++();
    this_t& operator++(int);

    T& operator*();
    T* operator->();

    bool operator==(const this_t&) const;
    bool operator!=(const this_t&) const;
  private:
    FwdListNode< T >* node_;
  };

  template< typename T >
  FwdListIterator< T >::FwdListIterator():
    node_(rhs.node)
  {}

  template< typename T >
  FwdListIterator< T >::FwdListIterator(FwdListNode< T >* node):
    node_(node)
  {}

  template< typename T >
  typename FwdListIterator< T >::this_t& FwdListIterator< T >::operator++() //done
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  typename FwdListIterator< T >::this_t& FwdListIterator< T >::operator++(int) //done
  {
    assert(node_ != nullptr);
    Fthis_t res = *this;
    ++(*this);
    return res;
  }

  template< typename T >
  T& FwdListIterator< T >::operator*() //done
  {
    assert(node_ != nullptr);
    return node_ -> data;
  }

  template< typename T >
  T* FwdListIterator< T >::operator->() //done
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool FwdListIterator< T >::operator==(const this_t& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool FwdListIterator< T >::operator!=(const this_t& rhs) const
  {
    return node_ != rhs.node_;
  }

  template< typename T >
  class FwdListConstIterator final: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    using this_t = FwdListConstIterator< T >;

    FwdListConstIterator();
    FwdListConstIterator(const this_t&) = default;
    FwdListConstIterator(FwdListNode< T >*);
    ~FwdListConstIterator() = default;

    this_t& operator++();
    this_t operator++(int);

    const T& operator*();
    const T* operator->();

    bool operator==(const this_t&) const;
    bool operator!=(const this_t&) const;
  private:
    FwdListNode< T >* node_;
  };

  template< typename T >
  FwdListConstIterator< T >::FwdListConstIterator():
    node_(rhs.node)
  {}

  template< typename T >
  FwdListConstIterator< T >::FwdListConstIterator(FwdListNode< T >* node):
    node_(node)
  {}

  template< typename T >
  typename FwdListConstIterator< T >::this_t& FwdListConstIterator< T >::operator++() //done
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  typename FwdListConstIterator< T >::this_t FwdListConstIterator< T >::operator++(int)
  {
    assert(node_ != nullptr);
    Fthis_t res = *this;
    ++(*this);
    return res;
  }

  template< typename T >
  const T& FwdListConstIterator< T >::operator*() //done
  {
    assert(node_ != nullptr);
    return node_ -> data;
  }

  template< typename T >
  const T* FwdListConstIterator< T >::operator->()
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool FwdListConstIterator< T >::operator==(const this_t& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool FwdListConstIterator< T >::operator!=(const this_t& rhs) const
  {
    return node_ != rhs.node_;
  }
}

#endif