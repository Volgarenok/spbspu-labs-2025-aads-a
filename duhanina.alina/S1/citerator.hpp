#ifndef CITERATOR_HPP
#define CITERATOR_HPP

#include <cassert>
#include <iterator>
#include "node.hpp"
#include "list.hpp"

namespace duhanina
{
  template< typename T >
  class List;

  template < typename T >
  struct ConstIterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
  public:
    ConstIterator() noexcept;
    ~ConstIterator() = default;

    ConstIterator< T >& operator++() noexcept;
    ConstIterator< T > operator++(int) noexcept;

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;

    bool operator==(const ConstIterator< T >&) const noexcept;
    bool operator!=(const ConstIterator< T >&) const noexcept;

  private:
    friend List< T >;
    const Node< T >* node_;
    explicit ConstIterator(Node< T >* node) noexcept;
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
  bool ConstIterator< T >::operator==(const ConstIterator< T >& it) const noexcept
  {
    return node_ == it.node_;
  }

  template< typename T >
  bool ConstIterator< T >::operator!=(const ConstIterator< T >& it) const noexcept
  {
    return !(it == *this);
  }
}

#endif
