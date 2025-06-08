#ifndef CONST_ITERATOR_HPP
#define CONST_ITERATOR_HPP
#include <iterator>
#include <cassert>
#include "node.hpp"

namespace averenkov
{
  template< class T >
  class List;

  template< class T >
  struct ConstListIterator: public std::iterator< std::forward_iterator_tag, T >
  {
    friend class List< T >;
  public:
    ConstListIterator() = default;
    ~ConstListIterator() = default;

    ConstListIterator< T >& operator++();
    ConstListIterator< T > operator++(int);

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;

    bool operator==(const ConstListIterator< T >&) const;
    bool operator!=(const ConstListIterator< T >&) const;

  private:
    Node< T >* node_;
    explicit ConstListIterator(Node< T >* node) noexcept;

  };

  template< class T >
  ConstListIterator< T >::ConstListIterator(Node< T >* node) noexcept:
    node_(node)
  {
  }

  template< class T >
  ConstListIterator< T >& ConstListIterator< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }

  template< class T >
  ConstListIterator< T > ConstListIterator< T >::operator++(int)
  {
    assert(node_ != nullptr);
    ConstListIterator< T > result(*this);
    ++(*this);
    return result;
  }

  template< class T >
  const T& ConstListIterator< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< class T >
  const T* ConstListIterator< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< class T >
  bool ConstListIterator< T >::operator==(const ConstListIterator< T >& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class T >
  bool ConstListIterator< T >::operator!=(const ConstListIterator< T >& rhs) const
  {
    return !(rhs == *this);
  }
}

#endif
