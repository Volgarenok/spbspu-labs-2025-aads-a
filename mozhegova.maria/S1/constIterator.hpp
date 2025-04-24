#ifndef CONSTITERATOR_HPP
#define CONSTITERATOR_HPP

#include <iterator>
#include <cassert>
#include "node.hpp"

namespace mozhegova
{
  template< typename T >
  class List;

  template< typename T >
  struct ConstIterator:
    public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend class List< T >;
  public:
    ConstIterator();
    ConstIterator(const ConstIterator< T > &) = default;
    ~ConstIterator() = default;
    ConstIterator< T > & operator=(const ConstIterator< T > &) = default;

    ConstIterator< T > & operator++() noexcept;
    ConstIterator< T > operator++(int) noexcept;

    ConstIterator< T > & operator--() noexcept;
    ConstIterator< T > operator--(int) noexcept;

    const T & operator*() const noexcept;
    const T * operator->() const noexcept;

    bool operator==(const ConstIterator< T > &) const noexcept;
    bool operator!=(const ConstIterator< T > &) const noexcept;
  private:
    detail::Node< T > * node_;

    explicit ConstIterator(detail::Node< T > * node);
  };

  template< typename T >
  ConstIterator< T >::ConstIterator():
    node_(nullptr)
  {}

  template< typename T >
  ConstIterator< T >::ConstIterator(detail::Node< T > * node):
    node_(node)
  {}

  template< typename T >
  ConstIterator< T > & ConstIterator< T >::operator++() noexcept
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
  ConstIterator< T > & ConstIterator< T >::operator--() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->prev_;
    return *this;
  }

  template< typename T >
  ConstIterator< T > ConstIterator< T >::operator--(int) noexcept
  {
    assert(node_ != nullptr);
    ConstIterator< T > result(*this);
    --(*this);
    return result;
  }

  template< typename T >
  const T & ConstIterator< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  const T * ConstIterator< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool ConstIterator< T >::operator==(const ConstIterator< T > & rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool ConstIterator< T >::operator!=(const ConstIterator< T > & rhs) const noexcept
  {
    return !(rhs == *this);
  }
}


#endif
