#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <cassert>
#include <memory>
#include "node.hpp"

namespace smirnov
{
  template< typename T >
  class List;
  template < typename T >
  class Iterator: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    using this_t = Iterator< T >;
    Iterator() noexcept;
    ~Iterator() = default;
    T & operator*() noexcept;
    T * operator->() noexcept;
    this_t & operator++() noexcept;
    this_t operator++(int) noexcept;
    bool operator==(const this_t &) const noexcept;
    bool operator!=(const this_t &) const noexcept;
  private:
    friend class List< T >;
    Node< T > * node_;
    explicit Iterator(Node< T > * node) noexcept;
  };

  template < typename T >
  Iterator< T >::Iterator() noexcept:
    node_(nullptr)
  {}

  template < typename T >
  Iterator< T >::Iterator(Node< T > * node) noexcept:
    node_(node)
  {}

  template < typename T >
  T & Iterator< T >::operator*() noexcept
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template < typename T >
  T * Iterator< T >::operator->() noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template < typename T >
  Iterator< T > & Iterator< T >::operator++() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }

  template < typename T >
  Iterator< T > Iterator< T >::operator++(int) noexcept
  {
    assert(node_ != nullptr);
    this_t result(*this);
    ++(*this);
    return result;
  }

  template < typename T >
  bool Iterator< T >::operator==(const this_t & rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template < typename T >
  bool Iterator< T >::operator!=(const this_t & rhs) const noexcept
  {
    return !(rhs == *this);
  }
}
#endif
