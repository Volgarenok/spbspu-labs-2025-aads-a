#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <cassert>
#include <iterator>
#include <new>
#include "node.hpp"
namespace kiselev
{
  template< typename T >
  class List;

  template< typename T, bool IsConst >
  class Iterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend class List< T >;
    friend class Iterator< T, !IsConst >;
  public:
    using reference = std::conditional_t< IsConst, const T&, T& >;
    using pointer = std::conditional_t< IsConst, const T*, T* >;

    Iterator(): node_(nullptr) {}
    Iterator(Node< T >* node): node_(node) {}
    template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
    Iterator(const Iterator< T, OtherIsConst >& other): node_(other.node_) {}
    template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
    Iterator< T, IsConst >& operator=(const Iterator< T, OtherIsConst >& other);

    Iterator< T, IsConst >& operator++() noexcept;
    Iterator< T, IsConst > operator++(int) noexcept;

    Iterator< T, IsConst >& operator--() noexcept;
    Iterator< T, IsConst > operator--(int) noexcept;

    reference operator*() const noexcept;
    pointer operator->() const noexcept;

    bool operator==(const Iterator< T, IsConst >&) const noexcept;
    bool operator!=(const Iterator< T, IsConst >&) const noexcept;

  private:
    Node< T >* node_;
  };

  template< typename T, bool IsConst >
  template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > >
  Iterator< T, IsConst >& Iterator< T, IsConst >::operator=(const Iterator< T, OtherIsConst >& other)
  {
    node_ = other.node_;
    return *this;
  }

  template< typename T, bool IsConst >
  Iterator< T, IsConst >& Iterator< T, IsConst >::operator++() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
    return *this;
  }

  template< typename T, bool IsConst >
  Iterator< T, IsConst > Iterator< T, IsConst >::operator++(int) noexcept
  {
    assert(node_ != nullptr);
    Iterator< T, IsConst > result(*this);
    ++(*this);
    return result;
  }

  template< typename T, bool IsConst >
  Iterator< T, IsConst >& Iterator< T, IsConst >::operator--() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->prev_;
    return *this;
  }

  template< typename T, bool IsConst >
  Iterator< T, IsConst > Iterator< T, IsConst >::operator--(int) noexcept
  {
    assert(node_ != nullptr);
    Iterator< T, IsConst > result(*this);
    --(*this);
    return result;
  }

  template< typename T, bool IsConst >
  typename Iterator< T, IsConst >::reference Iterator< T, IsConst >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T, bool IsConst >
  typename Iterator< T, IsConst >::pointer Iterator< T, IsConst >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T, bool IsConst >
  bool Iterator< T, IsConst >::operator==(const Iterator< T, IsConst >& it) const noexcept
  {
    return node_ == it.node_;
  }

  template< typename T, bool IsConst >
  bool Iterator< T, IsConst >::operator!=(const Iterator< T, IsConst >& it) const noexcept
  {
    return !(it == *this);
  }

}
#endif
