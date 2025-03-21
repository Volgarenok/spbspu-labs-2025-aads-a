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
    bool operator!=(const FwdIterator< T > & other) const;
    bool operator==(const FwdIterator< T > & other) const;
    FwdListNode< T > * node_;
  };

  template< typename T >
  FwdIterator< T >::FwdIterator():
    node_(nullptr)
  {}

  template< typename T >
  FwdIterator< T >::FwdIterator(FwdListNode< T > * node):
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
  bool FwdIterator< T >::operator!=(const FwdIterator& other) const
  {
    return node_ != other.node_;
  }

  template< typename T >
  bool FwdIterator< T >::operator==(const FwdIterator& other) const
  {
    return node_ == other.node_;
  }

  template< typename T >
  struct CFwdIterator: public std::iterator< std::forward_iterator_tag, T >
  {
    CFwdIterator();
    CFwdIterator(CFwdIterator< T > * node);
    ~CFwdIterator() = default;
    CFwdIterator(const CFwdIterator< T > &) = default;
    CFwdIterator< T > & operator=(const CFwdIterator< T > &) = default;
    T & operator*();
    T * operator->();
    CFwdIterator< T > & operator++();
    bool operator!=(const CFwdIterator< T > & other) const;
    bool operator==(const CFwdIterator< T > & other) const;
  private:
    CFwdIterator< T > * node_;
  };

  template< typename T >
  CFwdIterator< T >::CFwdIterator():
    node_(nullptr)
  {}

  template< typename T >
  CFwdIterator< T >::CFwdIterator(CFwdIterator< T > * node):
    node_(node)
  {}

  template< typename T >
  T & CFwdIterator< T >::operator*()
  {
    return node_->data_;
  }

  template< typename T >
  T * CFwdIterator< T >::operator->()
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
  bool CFwdIterator< T >::operator!=(const CFwdIterator& other) const
  {
    return node_ != other.node_;
  }

  template< typename T >
  bool CFwdIterator< T >::operator==(const CFwdIterator& other) const
  {
    return node_ == other.node_;
  }
}




#endif
