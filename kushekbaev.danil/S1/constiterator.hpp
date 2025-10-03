#ifndef CONSTITERATOR_HPP
#define CONSTITERATOR_HPP

#include <cassert>
#include <iterator>
#include "node.hpp"

namespace kushekbaev
{
  template< typename T >
  struct FwdList;

  template< typename T >
  struct ConstIterator final: public std::iterator< std::forward_iterator_tag, T >
  {
    using this_t = ConstIterator< T >;
    ConstIterator() noexcept;

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;

    bool operator!=(const this_t&) const noexcept;
    bool operator==(const this_t&) const noexcept;

    private:
      explicit ConstIterator(Node< T >* node) noexcept;
      Node< T >* node_;
      friend struct FwdList< T >;
  };

  template< typename T >
  ConstIterator< T >::ConstIterator() noexcept:
    node_(nullptr)
  {}

  template< typename T >
  ConstIterator< T >::ConstIterator(Node< T >* node) noexcept:
    node_(node)
  {}

  template< typename T >
  ConstIterator< T >& ConstIterator< T >::operator++() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  ConstIterator< T > ConstIterator< T >::operator++(int) noexcept
  {
    assert(node_ != nullptr);
    ConstIterator< T > result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  const T& ConstIterator< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  const T* ConstIterator< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool ConstIterator< T >::operator!=(const this_t& other) const noexcept
  {
    return !(other == *this);
  }

  template< typename T >
  bool ConstIterator< T >::operator==(const this_t& other) const noexcept
  {
    return node_ == other.node_;
  }
}

#endif
