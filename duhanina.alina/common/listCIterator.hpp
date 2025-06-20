#ifndef LISTCITERATOR_HPP
#define LISTCITERATOR_HPP

#include <cassert>
#include <iterator>
#include "listNode.hpp"
#include "list.hpp"

namespace duhanina
{
  template< typename T >
  class List;

  template < typename T >
  struct ConstIteratorList: public std::iterator< std::bidirectional_iterator_tag, T >
  {
  public:
    ConstIteratorList() noexcept;
    ~ConstIteratorList() = default;

    ConstIteratorList< T >& operator++() noexcept;
    ConstIteratorList< T > operator++(int) noexcept;

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;

    bool operator==(const ConstIteratorList< T >&) const noexcept;
    bool operator!=(const ConstIteratorList< T >&) const noexcept;

  private:
    friend List< T >;
    const NodeList< T >* node_;
    explicit ConstIteratorList(NodeList< T >* node) noexcept;
  };

  template< typename T >
  ConstIteratorList< T >::ConstIteratorList() noexcept:
    node_(nullptr)
  {}

  template< typename T >
  ConstIteratorList< T >::ConstIteratorList(NodeList< T >* node) noexcept:
    node_(node)
  {}

  template< typename T >
  ConstIteratorList< T >& ConstIteratorList< T >::operator++() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  ConstIteratorList< T > ConstIteratorList< T >::operator++(int) noexcept
  {
    assert(node_ != nullptr);
    ConstIteratorList< T > result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  const T& ConstIteratorList< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  const T* ConstIteratorList< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool ConstIteratorList< T >::operator==(const ConstIteratorList< T >& it) const noexcept
  {
    return node_ == it.node_;
  }

  template< typename T >
  bool ConstIteratorList< T >::operator!=(const ConstIteratorList< T >& it) const noexcept
  {
    return !(it == *this);
  }
}

#endif
