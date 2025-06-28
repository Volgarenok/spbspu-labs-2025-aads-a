#ifndef ITERATORS_HPP
#define ITERATORS_HPP
#include <cassert>
#include "list.hpp"
#include "listNode.hpp"

namespace alymova
{
  template< typename T >
  struct Iterator:
    public std::iterator< std::bidirectional_iterator_tag, T >
  {
  public:
    Iterator();
    Iterator< T >& operator++() noexcept;
    Iterator< T > operator++(int) noexcept;
    Iterator< T >& operator--() noexcept;
    Iterator< T > operator--(int) noexcept;
    bool operator==(const Iterator< T >& other) const noexcept;
    bool operator!=(const Iterator< T >& other) const noexcept;
    T& operator*() noexcept;
    T* operator->() noexcept;
  private:
    friend class List< T >;
    detail::ListNode< T >* node_;
    Iterator(detail::ListNode< T >* node);
  };
  template< typename T >
  struct ConstIterator:
    public std::iterator< std::bidirectional_iterator_tag, T >
  {
  public:
    ConstIterator();
    ConstIterator< T >& operator++() noexcept;
    ConstIterator< T > operator++(int) noexcept;
    ConstIterator< T >& operator--() noexcept;
    ConstIterator< T > operator--(int) noexcept;
    bool operator==(const ConstIterator< T >& other) const noexcept;
    bool operator!=(const ConstIterator< T >& other) const noexcept;
    const T& operator*() noexcept;
    const T* operator->() noexcept;
  private:
    friend class List< T >;
    const detail::ListNode< T >* node_;
    ConstIterator(detail::ListNode< T >* node);
  };

  template< typename T >
  Iterator< T >::Iterator():
    node_(nullptr)
  {}

  template< typename T >
  Iterator< T >::Iterator(detail::ListNode< T >* node):
    node_(node)
  {}

  template< typename T >
  Iterator< T >& Iterator< T >::operator++() noexcept
  {
    assert(node_ != nullptr && "You are trying to access beyond list's bounds");
    node_ = node_->next;
    return *this;
  }

  template< typename T >
  Iterator< T > Iterator< T >::operator++(int) noexcept
  {
    assert(node_ != nullptr && "You are trying to access beyond list's bounds");
    Iterator< T > old = *this;
    ++(*this);
    return old;
  }

  template< typename T >
  Iterator< T >& Iterator< T >::operator--() noexcept
  {
    assert(node_ != nullptr && "You are trying to access beyond list's bounds");
    node_ = node_->prev;
    return *this;
  }

  template< typename T >
  Iterator< T > Iterator< T >::operator--(int) noexcept
  {
    assert(node_ != nullptr && "You are trying to access beyond list's bounds");
    Iterator< T > old = *this;
    --(*this);
    return old;
  }

  template< typename T >
  bool Iterator< T >::operator==(const Iterator< T >& other)  const noexcept
  {
    return node_ == other.node_;
  }

  template< typename T >
  bool Iterator< T >::operator!=(const Iterator< T >& other) const noexcept
  {
    return node_ != other.node_;
  }

  template< typename T >
  T& Iterator< T >::operator*() noexcept
  {
    assert(node_ != nullptr && "You are trying to access beyond list's bounds");
    return node_->data;
  }

  template< typename T >
  T* Iterator< T >::operator->() noexcept
  {
    assert(node_ != nullptr && "You are trying to access beyond list's bounds");
    return std::addressof(node_->data);
  }

  template< typename T >
  ConstIterator< T >::ConstIterator():
    node_(nullptr)
  {}

  template< typename T >
  ConstIterator< T >::ConstIterator(detail::ListNode< T >* node):
    node_(node)
  {}

  template< typename T >
  ConstIterator< T >& ConstIterator< T >::operator++() noexcept
  {
    assert(node_ != nullptr && "You are trying to access beyond list's bounds");
    node_ = node_->next;
    return *this;
  }

  template< typename T >
  ConstIterator< T > ConstIterator< T >::operator++(int) noexcept
  {
    assert(node_ != nullptr && "You are trying to access beyond list's bounds");
    ConstIterator< T > old = *this;
    ++(*this);
    return old;
  }

  template< typename T >
  ConstIterator< T >& ConstIterator< T >::operator--() noexcept
  {
    assert(node_ != nullptr && "You are trying to access beyond list's bounds");
    node_ = node_->prev;
    return *this;
  }

  template< typename T >
  ConstIterator< T > ConstIterator< T >::operator--(int) noexcept
  {
    assert(node_ != nullptr && "You are trying to access beyond list's bounds");
    ConstIterator< T > old = *this;
    --(*this);
    return old;
  }

  template< typename T >
  bool ConstIterator< T >::operator==(const ConstIterator< T >& other) const noexcept
  {
    return node_ == other.node_;
  }

  template< typename T >
  bool ConstIterator< T >::operator!=(const ConstIterator< T >& other) const noexcept
  {
    return node_ != other.node_;
  }

  template< typename T >
  const T& ConstIterator< T >::operator*() noexcept
  {
    assert(node_ != nullptr && "You are trying to access beyond list's bounds");
    return node_->data;
  }

  template< typename T >
  const T* ConstIterator< T >::operator->() noexcept
  {
    assert(node_ != nullptr && "You are trying to access beyond list's bounds");
    return std::addressof(node_->data);
  }
}
#endif
