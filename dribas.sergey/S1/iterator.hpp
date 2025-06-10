#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>

#include "node.hpp"

namespace dribas
{
  template< class T >
  class List;

  template< class T >
  class Iterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend class List< T >;
  public:
    using iter = Iterator< T >;

    Iterator() noexcept;
    Iterator(const iter&) = default;

    iter& operator=(const iter&) = default;
    iter& operator++() noexcept;
    iter& operator--() noexcept;
    iter operator++(int) noexcept;
    iter operator--(int) noexcept;

    bool operator==(const iter& other) const noexcept;
    bool operator!=(const iter& other) const noexcept;

    T& operator*();
    T* operator->();
  private:
    Node< T >* head_;
    explicit Iterator(Node< T >* head) noexcept;
  };

  template< class T >
  bool Iterator< T >::operator==(const iter& other) const noexcept
  {
    return head_ == other.head_;
  }

  template< class T >
  bool Iterator< T >::operator!=(const iter& other) const noexcept
  {
    return !(*this == other);
  }

  template< class T >
  Iterator< T >::Iterator() noexcept:
    head_(nullptr)
  {}

  template< class T >
  Iterator< T >::Iterator(Node< T >* head) noexcept:
    head_(head)
  {}

  template< class T >
  typename Iterator< T >::iter& Iterator< T >::operator++() noexcept
  {
    head_ = head_->next_;
    return *this;
  }

  template< class T >
  typename Iterator< T >::iter Iterator< T >::operator++(int) noexcept
  {
    iter decr(*this);
    ++(*this);
    return decr;
  }

  template< class T >
  typename Iterator< T >::iter& Iterator< T >::operator--() noexcept
  {
    head_ = head_->prev_;
    return *this;
  }

  template< class T >
  typename Iterator< T >::iter Iterator< T >::operator--(int) noexcept
  {
    iter precr(*this);
    --(*this);
    return precr;
  }

  template< class T >
  T& Iterator< T >::operator*()
  {
    return head_->data_;
  }

  template< class T >
  T* Iterator< T >::operator->()
  {
    return std::addressof(head_->data_);
  }
}

#endif
