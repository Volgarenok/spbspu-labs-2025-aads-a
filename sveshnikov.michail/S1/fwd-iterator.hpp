#ifndef FWD_ITERATOR_HPP
#define FWD_ITERATOR_HPP
#include <memory>
#include <cassert>
#include "fwdlist-node.hpp"

namespace sveshnikov
{
  template< typename T >
  class FwdList;

  template< typename T >
  class Iterator: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    Iterator();
    Iterator(const Iterator< T > &) = default;
    ~Iterator() = default;
    Iterator< T > &operator=(const Iterator< T > &) = default;
    Iterator< T > &operator++() noexcept;
    Iterator< T > operator++(int) noexcept;
    T &operator*() noexcept;
    const T &operator*() const noexcept;
    T *operator->() noexcept;
    const T *operator->() const noexcept;
    bool operator!=(const Iterator< T > &) const noexcept;
    bool operator==(const Iterator< T > &) const noexcept;

  private:
    node_t< T > *node_;
    explicit Iterator(node_t< T > *node);
    friend class FwdList< T >;
  };

  template< typename T >
  Iterator< T >::Iterator():
    node_(nullptr)
  {}

  template< typename T >
  Iterator< T >::Iterator(node_t< T > *node):
    node_(node)
  {}

  template< typename T >
  Iterator< T > &Iterator< T >::operator++() noexcept
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
  bool Iterator< T >::operator==(const Iterator< T > &rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool Iterator< T >::operator!=(const Iterator< T > &rhs) const noexcept
  {
    return !(rhs == *this);
  }

  template< typename T >
  T &Iterator< T >::operator*() noexcept
  {
    return const_cast< T & >(static_cast< const Iterator< T > & >(*this).operator*());
  }

  template< typename T >
  const T &Iterator< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  T *Iterator< T >::operator->() noexcept
  {
    return const_cast< T * >(static_cast< const Iterator< T > * >(this)->operator->());
  }

  template< typename T >
  const T *Iterator< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }
}

#endif
