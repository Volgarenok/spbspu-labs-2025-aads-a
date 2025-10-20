#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
#include <cassert>
#include "node.hpp"

namespace averenkov
{
  template< class T >
  class List;

  template< class T >
  struct ListIterator: public std::iterator< std::forward_iterator_tag, T >
  {
    friend class List< T >;
  public:
    ListIterator() noexcept;
    ~ListIterator() = default;

    ListIterator< T >& operator++();
    ListIterator< T > operator++(int);

    T& operator*() noexcept;
    T* operator->() noexcept;
    const T& operator*() const noexcept;
    const T* operator->() const noexcept;

    bool operator==(const ListIterator< T >&) const;
    bool operator!=(const ListIterator< T >&) const;

  private:
    Node< T >* node_;
    explicit ListIterator(Node< T >* node) noexcept;

  };

  template< class T >
  ListIterator< T >::ListIterator() noexcept:
    node_(nullptr)
  {
  }

  template< class T >
  ListIterator< T >::ListIterator(Node< T >* node) noexcept:
    node_(node)
  {
  }

  template< class T >
  ListIterator< T >& ListIterator< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }

  template< class T >
  ListIterator< T > ListIterator< T >::operator++(int)
  {
    assert(node_ != nullptr);
    ListIterator< T > result(*this);
    ++(*this);
    return result;
  }

  template< class T >
  T& ListIterator< T >::operator*() noexcept
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< class T >
  T* ListIterator< T >::operator->() noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }
  template< class T >
  const T& ListIterator< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< class T >
  const T* ListIterator< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< class T >
  bool ListIterator< T >::operator==(const ListIterator< T >& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class T >
  bool ListIterator< T >::operator!=(const ListIterator< T >& rhs) const
  {
    return !(rhs == *this);
  }
}

#endif
