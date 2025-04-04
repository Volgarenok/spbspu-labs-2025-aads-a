#ifndef CONSTITERATOR_HPP
#define CONSTITERATOR_HPP

#include "node.hpp"

namespace dribas
{
  template< class T >
  class ConstIterator final
  {
  public:
    using citer = ConstIterator< T >;

    ConstIterator() noexcept;
    ConstIterator(Node< T >* head) noexcept;
    ConstIterator(const citer&) = default;
    citer& operator=(const citer&) = default;

    citer& operator++() noexcept;
    citer& operator--() noexcept;
    citer operator++(int) noexcept;
    citer operator--(int) noexcept;

    bool operator==(const citer& other) const noexcept;
    bool operator!=(const citer& other) const noexcept;

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;

  private:
    Node< T >* head_;
  };

  template< class T >
  bool ConstIterator< T >::operator==(const citer& other) const noexcept
  {
    return head_ == other.head_;
  }

  template< class T >
  bool ConstIterator< T >::operator!=(const citer& other) const noexcept
  {
    return !(*this == other);
  }

  template< class T >
  ConstIterator< T >::ConstIterator() noexcept:
    head_(nullptr)
  {}

  template< class T >
  ConstIterator< T >::ConstIterator(Node< T >* head) noexcept:
    head_(head)
  {}

  template< class T >
  typename ConstIterator< T >::citer& ConstIterator< T >::operator++() noexcept
  {
    head_ = head_->next_;
    return *this;
  }

  template< class T >
  typename ConstIterator< T >::citer ConstIterator< T >::operator++(int) noexcept
  {
    citer decr(*this);
    ++(*this);
    return decr;
  }

  template< class T >
  typename ConstIterator< T >::citer& ConstIterator< T >::operator--() noexcept
  {
    head_ = head_->prev_;
    return *this;
  }

  template< class T >
  typename ConstIterator< T >::citer ConstIterator< T >::operator--(int) noexcept
  {
    citer precr(*this);
    --(*this);
    return precr;
  }

  template< class T >
  const T& ConstIterator< T >::operator*() const noexcept
  {
    return head_->data;
  }

  template< typename T >
  const T* ConstIterator< T >::operator->() const noexcept
  {
    return std::addressof(head_->data);
  }

}

#endif
