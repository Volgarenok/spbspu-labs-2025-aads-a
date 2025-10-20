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

    Iterator< T > & operator++() noexcept;
    Iterator< T > operator++(int) noexcept;

    Iterator< T > & operator--() noexcept;
    Iterator< T > operator--(int) noexcept;

    T & operator*() noexcept;
    T * operator->() noexcept;
    const T & operator*() const noexcept;
    const T * operator->() const noexcept;

    bool operator==(const Iterator< T > &) const noexcept;
    bool operator!=(const Iterator< T > &) const noexcept;
  private:
    detail::Node< T > * node_;

    explicit Iterator(detail::Node< T > * node);
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
  Iterator< T > & Iterator< T >::operator++() noexcept
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
  Iterator< T > & Iterator< T >::operator--() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->prev_;
    return *this;
  }

  template< typename T >
  Iterator< T > Iterator< T >::operator--(int) noexcept
  {
    assert(node_ != nullptr);
    Iterator< T > result(*this);
    --(*this);
    return result;
  }

  template< typename T >
  T & Iterator< T >::operator*() noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  T * Iterator< T >::operator->() noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  const T & Iterator< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  const T * Iterator< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool Iterator< T >::operator==(const Iterator< T > & rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool Iterator< T >::operator!=(const Iterator< T > & rhs) const noexcept
  {
    return !(rhs == *this);
  }
}

#endif
