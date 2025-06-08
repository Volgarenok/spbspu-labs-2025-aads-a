#ifndef FWD_ITERATOR_HPP
#define FWD_ITERATOR_HPP
#include <iterator>
#include "fwd_list_node.hpp"

namespace zakirov
{
  template< typename T >
  class FwdList;

  template< typename T >
  class FwdIterator: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    explicit FwdIterator() noexcept;
    ~FwdIterator() = default;
    FwdIterator(const FwdIterator< T > &) noexcept = default;
    FwdIterator< T > & operator=(const FwdIterator< T > &) noexcept = default;
    T & operator*() noexcept;
    T * operator->() noexcept;
    const T & operator*() const noexcept;
    const T * operator->() const noexcept;
    FwdIterator< T > & operator++() noexcept;
    FwdIterator< T > & operator++(int) noexcept;
    bool operator!=(const FwdIterator< T > & other) const noexcept;
    bool operator==(const FwdIterator< T > & other) const noexcept;
  private:
    friend class FwdList< T >;
    explicit FwdIterator(FwdListNode< T > * node) noexcept;
    FwdListNode< T > * node_;
  };

  template< typename T >
  FwdIterator< T >::FwdIterator() noexcept:
    node_(nullptr)
  {}

  template< typename T >
  FwdIterator< T >::FwdIterator(FwdListNode< T > * node) noexcept:
    node_(node)
  {}

  template< typename T >
  T & FwdIterator< T >::operator*() noexcept
  {
    return node_->data_;
  }

  template< typename T >
  T * FwdIterator< T >::operator->() noexcept
  {
    return std::addressof(node_->data_);
  }

  template< typename T >
  const T & FwdIterator< T >::operator*() const noexcept
  {
    return node_->data_;
  }

  template< typename T >
  const T * FwdIterator< T >::operator->() const noexcept
  {
    return std::addressof(node_->data_);
  }

  template< typename T >
  FwdIterator< T > & FwdIterator< T >::operator++() noexcept
  {
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  FwdIterator< T > & FwdIterator< T >::operator++(int) noexcept
  {
    FwdIterator< T > * primal_state = *this;
    node_ = node_->next_;
    return primal_state;
  }

  template< typename T >
  bool FwdIterator< T >::operator!=(const FwdIterator< T > & other) const noexcept
  {
    return node_ != other.node_;
  }

  template< typename T >
  bool FwdIterator< T >::operator==(const FwdIterator< T > & other) const noexcept
  {
    return node_ == other.node_;
  }

  template< typename T >
  class CFwdIterator: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    explicit CFwdIterator() noexcept;
    ~CFwdIterator() = default;
    CFwdIterator(const CFwdIterator< T > &) noexcept = default;
    CFwdIterator< T > & operator=(const CFwdIterator< T > &) noexcept = default;
    const T & operator*() const noexcept;
    const T * operator->() const noexcept;
    CFwdIterator< T > & operator++() noexcept;
    CFwdIterator< T > & operator++(int) noexcept;
    bool operator!=(const CFwdIterator< T > & other) const noexcept;
    bool operator==(const CFwdIterator< T > & other) const noexcept;
  private:
    friend class FwdList< T >;
    explicit CFwdIterator(FwdListNode< T > * node) noexcept;
    const FwdListNode< T > * node_;
  };

  template< typename T >
  CFwdIterator< T >::CFwdIterator() noexcept:
    node_(nullptr)
  {}

  template< typename T >
  CFwdIterator< T >::CFwdIterator(FwdListNode< T > * node) noexcept:
    node_(node)
  {}

  template< typename T >
  const T & CFwdIterator< T >::operator*() const noexcept
  {
    return node_->data_;
  }

  template< typename T >
  const T * CFwdIterator< T >::operator->() const noexcept
  {
    return std::addressof(node_->data_);
  }

  template< typename T >
  CFwdIterator< T > & CFwdIterator< T >::operator++() noexcept
  {
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  CFwdIterator< T > & CFwdIterator< T >::operator++(int) noexcept
  {
    FwdIterator< T > * primal_state = *this;
    node_ = node_->next_;
    return primal_state;
  }

  template< typename T >
  bool CFwdIterator< T >::operator!=(const CFwdIterator< T > & other) const noexcept
  {
    return node_ != other.node_;
  }

  template< typename T >
  bool CFwdIterator< T >::operator==(const CFwdIterator< T > & other) const noexcept
  {
    return node_ == other.node_;
  }
}

#endif
