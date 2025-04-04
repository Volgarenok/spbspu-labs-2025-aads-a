#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include <cassert>
#include "list.hpp"

namespace aleksandrov
{
  template< typename T >
  class List;

  template< typename T >
  class Iterator: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    Iterator():
      node_(nullptr)
    {}
    Iterator(detail::Node< T >* node):
      node_(node)
    {}

    Iterator< T >& operator++();
    Iterator< T > operator++(int);

    T& operator*() const;
    T* operator->() const;

    bool operator!=(const Iterator< T >&) const noexcept;
    bool operator==(const Iterator< T >&) const noexcept;
  private:
    friend List< T >;
    detail::Node< T >* node_;
  };

  template< typename T >
  Iterator< T >& Iterator< T >::operator++()
  {
    assert(node_);
    node_ = node_->next;
    return *this;
  }

  template< typename T >
  Iterator< T > Iterator< T >::operator++(int)
  {
    assert(node_);
    Iterator< T > result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  T& Iterator< T >::operator*() const
  {
    assert(node_);
    return node_->data;
  }

  template< typename T >
  T* Iterator< T >::operator->() const
  {
    assert(node_);
    return std::addressof(node_->data);
  }

  template< typename T >
  bool Iterator< T >::operator==(const Iterator< T >& rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool Iterator< T >::operator!=(const Iterator< T >& rhs) const noexcept
  {
    return !(rhs == *this);
  }
}

#endif

