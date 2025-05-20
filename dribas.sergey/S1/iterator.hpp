#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>

#include "node.hpp"

namespace dribas
{
  template< class T >
  class List;

  template< class T, bool ifConst >
  class Iterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend class List< T >;
  public:
    using iter = Iterator< T, ifConst >;

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

  template< class T, bool ifConst >
  bool Iterator< T,  ifConst >::operator==(const iter& other) const noexcept
  {
    return head_ == other.head_;
  }

  template< class T, bool ifConst >
  bool Iterator< T,  ifConst >::operator!=(const iter& other) const noexcept
  {
    return !(*this == other);
  }

  template< class T, bool ifConst >
  Iterator< T,  ifConst >::Iterator() noexcept:
    head_(nullptr)
  {}

  template< class T, bool ifConst >
  Iterator< T,  ifConst >::Iterator(Node< T >* head) noexcept:
    head_(head)
  {}

  template< class T, bool ifConst >
  typename Iterator< T,  ifConst >::iter& Iterator< T,  ifConst >::operator++() noexcept
  {
    head_ = head_->next_;
    return *this;
  }

  template< class T, bool ifConst >
  typename Iterator< T,  ifConst >::iter Iterator< T,  ifConst >::operator++(int) noexcept
  {
    iter decr(*this);
    ++(*this);
    return decr;
  }

  template< class T, bool ifConst >
  typename Iterator< T,  ifConst >::iter& Iterator< T,  ifConst >::operator--() noexcept
  {
    head_ = head_->prev_;
    return *this;
  }

  template< class T, bool ifConst >
  typename Iterator< T,  ifConst >::iter Iterator< T,  ifConst >::operator--(int) noexcept
  {
    iter precr(*this);
    --(*this);
    return precr;
  }

  template< class T, bool ifConst >
  T& Iterator< T,  ifConst >::operator*()
  {
    return head_->data_;
  }

  template< class T, bool ifConst >
  T* Iterator< T,  ifConst >::operator->()
  {
    return std::addressof(head_->data_);
  }
}

#endif
