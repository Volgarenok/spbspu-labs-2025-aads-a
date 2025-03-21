#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <cassert>
#include <iterator>
#include "node.hpp"
#include "list.hpp"

namespace duhanina
{
  template< typename T >
  class List;

  template < typename T >
  struct Iterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
  public:
    Iterator() noexcept;
    explicit Iterator(Node< T >* node) noexcept;
    ~Iterator() = default;

    Iterator< T >& operator++() noexcept;
    Iterator< T > operator++(int) noexcept;

    T& operator*() const noexcept;
    T* operator->() const noexcept;

    bool operator==(const Iterator< T >&) const noexcept;
    bool operator!=(const Iterator< T >&) const noexcept;

  private:
    friend List< T >;
    Node< T >* node_;
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
  T& Iterator< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  T* Iterator< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool Iterator< T >::operator==(const Iterator< T >& it) const noexcept
  {
    return node_ == it.node_;
  }

  template< typename T >
  bool Iterator< T >::operator!=(const Iterator< T >& it) const noexcept
  {
    return !(it == *this);
  }
}

#endif
