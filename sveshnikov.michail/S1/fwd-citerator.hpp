#ifndef FWD_CITERATOR_HPP
#define FWD_CITERATOR_HPP
#include <memory>
#include <cassert>
#include "fwdlist-node.hpp"

namespace sveshnikov
{
  template< typename T >
  class FwdList;

  template< typename T >
  class ConstIterator: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    ConstIterator();
    ConstIterator(const ConstIterator< T > &) = default;
    ~ConstIterator() = default;
    ConstIterator< T > &operator=(const ConstIterator< T > &) = default;
    ConstIterator< T > &operator++() noexcept;
    ConstIterator< T > operator++(int) noexcept;
    const T &operator*() const noexcept;
    const T *operator->() const noexcept;
    bool operator!=(const ConstIterator< T > &) const noexcept;
    bool operator==(const ConstIterator< T > &) const noexcept;

  private:
    node_t< T > *node_;
    explicit ConstIterator(node_t< T > *node);
    friend class FwdList< T >;
  };

  template< typename T >
  ConstIterator< T >::ConstIterator():
    node_(nullptr)
  {}

  template< typename T >
  ConstIterator< T >::ConstIterator(node_t< T > *node):
    node_(node)
  {}

  template< typename T >
  ConstIterator< T > &ConstIterator< T >::operator++() noexcept
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
  bool ConstIterator< T >::operator==(const ConstIterator< T > &rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool ConstIterator< T >::operator!=(const ConstIterator< T > &rhs) const noexcept
  {
    return !(rhs == *this);
  }

  template< typename T >
  const T &ConstIterator< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  const T *ConstIterator< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }
}

#endif
