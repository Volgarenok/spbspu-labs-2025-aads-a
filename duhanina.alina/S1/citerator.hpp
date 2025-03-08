#ifndef CITERATOR_HPP
#define CITERATOR_HPP

#include <cassert>
#include <iterator>
#include "node.hpp"

namespace duhanina
{
  template < typename T >
  struct constIterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
  public:
    constIterator() noexcept;
    explicit constIterator(const Node< T >* node) noexcept;
    ~constIterator() = default;

    constIterator< T >& operator++() noexcept;
    constIterator< T > operator++(int) noexcept;

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;

    bool operator==(const constIterator< T >&) const noexcept;
    bool operator!=(const constIterator< T >&) const noexcept;

  private:
    const Node< T >* node_;
  };

  template< typename T >
  constIterator< T >::constIterator() noexcept:
    node_(nullptr)
  {}

  template< typename T >
  constIterator< T >::constIterator(const Node< T >* node) noexcept:
    node_(node)
  {}

  template< typename T >
  constIterator< T >& constIterator< T >::operator++() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  constIterator< T > constIterator< T >::operator++(int) noexcept
  {
    assert(node_ != nullptr);
    constIterator< T > result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  const T& constIterator< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  const T* constIterator< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool constIterator< T >::operator==(const constIterator< T >& it) const noexcept
  {
    return node_ == it.node_;
  }

  template< typename T >
  bool constIterator< T >::operator!=(const constIterator< T >& it) const noexcept
  {
    return !(it == *this);
  }
}

#endif
