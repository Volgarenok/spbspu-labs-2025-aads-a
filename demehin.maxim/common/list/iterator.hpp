#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <cassert>
#include <iterator>
#include "node.hpp"

namespace demehin
{
  template< typename T, bool isConst >
  struct ListIterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
  public:
    using this_t = ListIterator< T, isConst >;
    using Value = typename std::conditional< isConst, const T&, T& >::type;
    using Ptr = typename std::conditional< isConst, const T*, T* >::type;

    ListIterator() noexcept;
    explicit ListIterator(Node< T >*) noexcept;

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t& operator--() noexcept;
    this_t operator--(int) noexcept;

    Value operator*() const noexcept;
    Ptr operator->() const noexcept;

    bool operator==(const this_t& rhs) const noexcept;
    bool operator!=(const this_t& ths) const noexcept;

    Node< T >* getNode() const noexcept;

  private:
    Node< T >* node_;
  };

  template< typename T, bool isConst >
  ListIterator< T, isConst >::ListIterator() noexcept:
    node_(nullptr)
  {}

  template< typename T, bool isConst >
  ListIterator< T, isConst >::ListIterator(Node< T >* node) noexcept:
    node_(node)
  {}

  template< typename T, bool isConst >
  typename ListIterator< T, isConst >::this_t& ListIterator< T, isConst >::operator++() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }

  template< typename T, bool isConst >
  typename ListIterator< T, isConst >::this_t ListIterator< T, isConst >::operator++(int) noexcept
  {
    assert(node_ != nullptr);
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename T, bool isConst >
  typename ListIterator< T, isConst >::this_t& ListIterator< T, isConst >::operator--() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->prev;
    return *this;
  }

  template< typename T, bool isConst >
  typename ListIterator< T, isConst >::this_t ListIterator< T, isConst >::operator--(int) noexcept
  {
    assert(node_ != nullptr);
    this_t result(*this);
    --(*this);
    return result;
  }

  template< typename T, bool isConst >
  typename ListIterator< T, isConst >::Value ListIterator< T, isConst >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< typename T, bool isConst >
  typename ListIterator< T, isConst >::Ptr ListIterator< T, isConst >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename T, bool isConst >
  bool ListIterator< T, isConst >::operator==(const this_t& rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< typename T, bool isConst >
  bool ListIterator< T, isConst >::operator!=(const this_t& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< typename T, bool isConst >
  Node< T >* ListIterator< T, isConst >::getNode() const noexcept
  {
    return node_;
  }
}


#endif

