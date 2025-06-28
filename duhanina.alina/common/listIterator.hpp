#ifndef LISTITERATOR_HPP
#define LISTITERATOR_HPP

#include <cassert>
#include <iterator>
#include "listNode.hpp"
#include "list.hpp"

namespace duhanina
{
  template< typename T >
  class List;

  template < typename T >
  struct IteratorList: public std::iterator< std::bidirectional_iterator_tag, T >
  {
  public:
    IteratorList() noexcept;
    explicit IteratorList(NodeList< T >* node) noexcept;
    ~IteratorList() = default;

    IteratorList< T >& operator++() noexcept;
    IteratorList< T > operator++(int) noexcept;

    T& operator*() const noexcept;
    T* operator->() const noexcept;

    bool operator==(const IteratorList< T >&) const noexcept;
    bool operator!=(const IteratorList< T >&) const noexcept;

  private:
    friend List< T >;
    NodeList< T >* node_;
  };

  template< typename T >
  IteratorList< T >::IteratorList() noexcept:
    node_(nullptr)
  {}

  template< typename T >
  IteratorList< T >::IteratorList(NodeList< T >* node) noexcept:
    node_(node)
  {}

  template< typename T >
  IteratorList< T >& IteratorList< T >::operator++() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  IteratorList< T > IteratorList< T >::operator++(int) noexcept
  {
    assert(node_ != nullptr);
    IteratorList< T > result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  T& IteratorList< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  T* IteratorList< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool IteratorList< T >::operator==(const IteratorList< T >& it) const noexcept
  {
    return node_ == it.node_;
  }

  template< typename T >
  bool IteratorList< T >::operator!=(const IteratorList< T >& it) const noexcept
  {
    return !(it == *this);
  }
}

#endif
