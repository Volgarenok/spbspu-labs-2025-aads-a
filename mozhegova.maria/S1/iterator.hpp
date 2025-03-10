#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include <cassert>
#include "node.hpp"

namespace mozhegova
{
  template< typename T >
  class List;

  template< typename T >
  struct Iterator:
    public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend class List< T >;
  public:
    Iterator();
    Iterator(const Iterator< T > &) = default;
    ~Iterator() = default;
    Iterator< T > & operator=(const Iterator< T > &) = default;

    Iterator< T > & operator++();
    Iterator< T > operator++(int);

    Iterator< T > & operator--();
    Iterator< T > operator--(int);

    T & operator*();
    T * operator->();
    const T & operator*() const;
    const T * operator->() const;

    bool operator==(const Iterator< T > &) const;
    bool operator!=(const Iterator< T > &) const;
  private:
    detail::Node< T > * node_;

    Iterator(detail::Node< T > * node);
  };

  template< typename T >
  Iterator< T >::Iterator():
    node_(nullptr)
  {};

  template< typename T >
  Iterator< T >::Iterator(detail::Node< T > * node):
    node_(node)
  {}

  template< typename T >
  Iterator< T > & Iterator< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  Iterator< T > Iterator< T >::operator++(int)
  {
    assert(node_ != nullptr);
    Iterator< T > result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  Iterator< T > & Iterator< T >::operator--()
  {
    assert(node_ != nullptr);
    node_ = node_->prev_;
    return *this;
  }

  template< typename T >
  Iterator< T > Iterator< T >::operator--(int)
  {
    assert(node_ != nullptr);
    Iterator< T > result(*this);
    --(*this);
    return result;
  }

  template< typename T >
  T & Iterator< T >::operator*()
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  T * Iterator< T >::operator->()
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  const T & Iterator< T >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  const T * Iterator< T >::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool Iterator< T >::operator==(const Iterator< T > & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool Iterator< T >::operator!=(const Iterator< T > & rhs) const
  {
    return !(rhs == *this);
  }
}

#endif
