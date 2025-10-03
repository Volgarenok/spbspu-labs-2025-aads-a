#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <cassert>
#include <iterator>
#include "node.hpp"

namespace kushekbaev
{
  template < typename T >
  struct FwdList;

  template< typename T >
  struct Iterator final: public std::iterator< std::forward_iterator_tag, T >
  {
    using this_t = Iterator< T >;
    Iterator() noexcept;

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;

    T& operator*() noexcept;
    T* operator->() noexcept;

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;

    bool operator!=(const this_t&) const noexcept;
    bool operator==(const this_t&) const noexcept;

    private:
      explicit Iterator(Node< T >*) noexcept;
      Node< T >* node_;
      friend struct FwdList< T >;
  };

  template< typename T >
  Iterator< T >::Iterator() noexcept:
    node_(nullptr)
  {}

  template< typename T >
  Iterator< T >::Iterator(Node< T >* node) noexcept:
    node_(node)
  {}

  template< typename T >
  Iterator< T >& Iterator< T >::operator++() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  Iterator< T > Iterator< T >::operator++(int) noexcept
  {
    assert(node_ != nullptr);
    Iterator< T > result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  T& Iterator< T >::operator*() noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  T* Iterator< T >::operator->() noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  const T& Iterator< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  const T* Iterator< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool Iterator< T >::operator!=(const this_t& other) const noexcept
  {
    return !(other == *this);
  }

  template< typename T >
  bool Iterator< T >::operator==(const this_t& other) const noexcept
  {
    return node_ == other.node_;
  }
}

#endif
