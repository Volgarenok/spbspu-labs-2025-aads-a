#ifndef FWD_ITERATOR_HPP
#define FWD_ITERATOR_HPP
#include <iterator>
#include "fwd_list_node.hpp"

namespace zakirov
{
  template< typename T >
  struct FwdIterator: public std::iterator< std::forward_iterator_tag, T >
  {
    FwdIterator();
    FwdIterator(FwdListNode< T > * node);
    ~FwdIterator() = default;
    FwdIterator(const FwdIterator< T > &) = default;
    FwdIterator< T > & operator=(const FwdIterator< T > &) = default;
    T & operator*();
    T * operator->();
    FwdIterator< T > & operator++();
    FwdIterator< T > & operator++(int);
    bool operator!=(const FwdIterator< T > & other) const noexcept;
    bool operator==(const FwdIterator< T > & other) const noexcept;
    zakirov::FwdListNode< T > * node_;
  };

  template< typename T >
  FwdIterator< T >::FwdIterator():
    node_(nullptr)
  {}

  template< typename T >
  FwdIterator< T >::FwdIterator(zakirov::FwdListNode< T > * node):
    node_(node)
  {}

  template< typename T >
  T & FwdIterator< T >::operator*()
  {
    return node_->data_;
  }

  template< typename T >
  T * FwdIterator< T >::operator->()
  {
    return std::addressof(node_->data_);
  }

  template< typename T >
  FwdIterator< T > & FwdIterator< T >::operator++()
  {
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  FwdIterator< T > & FwdIterator< T >::operator++(int)
  {
    FwdIterator< T > * primal_state = *this;
    node_ = node_->next_;
    return primal_state;
  }

  template< typename T >
  bool FwdIterator< T >::operator!=(const FwdIterator< T > & other) const noexcept
  {
    return node_ != other.node_;
  }

  template< typename T >
  bool FwdIterator< T >::operator==(const FwdIterator< T > & other) const noexcept
  {
    return node_ == other.node_;
  }

  template< typename T >
  struct CFwdIterator: public std::iterator< std::forward_iterator_tag, T >
  {
    CFwdIterator();
    CFwdIterator(FwdListNode< T > * node);
    ~CFwdIterator() = default;
    CFwdIterator(const CFwdIterator< T > &) = default;
    CFwdIterator< T > & operator=(const CFwdIterator< T > &) = default;
    const T & operator*() const noexcept;
    const T * operator->() const noexcept;
    CFwdIterator< T > & operator++();
    CFwdIterator< T > & operator++(int);
    bool operator!=(const CFwdIterator< T > & other) const;
    bool operator==(const CFwdIterator< T > & other) const;
  private:
    const zakirov::FwdListNode< T > * node_;
  };

  template< typename T >
  CFwdIterator< T >::CFwdIterator():
    node_(nullptr)
  {}

  template< typename T >
  CFwdIterator< T >::CFwdIterator(FwdListNode< T > * node):
    node_(node)
  {}

  template< typename T >
  const T & CFwdIterator< T >::operator*() const noexcept
  {
    return node_->data_;
  }

  template< typename T >
  const T * CFwdIterator< T >::operator->() const noexcept
  {
    return std::addressof(node_->data_);
  }

  template< typename T >
  CFwdIterator< T > & CFwdIterator< T >::operator++()
  {
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  CFwdIterator< T > & CFwdIterator< T >::operator++(int)
  {
    FwdIterator< T > * primal_state = *this;
    node_ = node_->next_;
    return primal_state;
  }

  template< typename T >
  bool CFwdIterator< T >::operator!=(const CFwdIterator< T > & other) const
  {
    return node_ != other.node_;
  }

  template< typename T >
  bool CFwdIterator< T >::operator==(const CFwdIterator< T > & other) const
  {
    return node_ == other.node_;
  }
}

#endif
