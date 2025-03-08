#ifndef CONSTITERATOR_HPP
#define CONSTITERATOR_HPP

#include <iterator>
#include <cassert>
#include "node.hpp"

namespace mozhegova
{
  template< typename T >
  struct ConstIterator:
    public std::iterator< std::bidirectional_iterator_tag, T >
  {
  public:
    ConstIterator(): node_(nullptr) {};
    ConstIterator(const ConstIterator< T > &) = default;
    ConstIterator(Node< T > * node): node_(node) {}
    ~ConstIterator() = default;
    ConstIterator< T > & operator=(const ConstIterator< T > &) = default;

    ConstIterator< T > & operator++();
    ConstIterator< T > operator++(int);

    ConstIterator< T > & operator--();
    ConstIterator< T > operator--(int);

    T & operator*() const;
    T * operator->() const;

    bool operator==(const ConstIterator< T > &) const;
    bool operator!=(const ConstIterator< T > &) const;

    Node< T > * getNode() const;
  private:
    Node< T > * node_;
  };

  template< typename T >
  ConstIterator< T > & ConstIterator< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  ConstIterator< T > ConstIterator< T >::operator++(int)
  {
    assert(node_ != nullptr);
    ConstIterator< T > result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  ConstIterator< T > & ConstIterator< T >::operator--()
  {
    assert(node_ != nullptr);
    node_ = node_->prev_;
    return *this;
  }

  template< typename T >
  ConstIterator< T > ConstIterator< T >::operator--(int)
  {
    assert(node_ != nullptr);
    ConstIterator< T > result(*this);
    --(*this);
    return result;
  }

  template< typename T >
  T & ConstIterator< T >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  T * ConstIterator< T >::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool ConstIterator< T >::operator==(const ConstIterator< T > & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool ConstIterator< T >::operator!=(const ConstIterator< T > & rhs) const
  {
    return !(rhs == *this);
  }

  template< typename T >
  Node< T > * ConstIterator< T >::getNode() const
  {
    return node_;
  }
}


#endif
