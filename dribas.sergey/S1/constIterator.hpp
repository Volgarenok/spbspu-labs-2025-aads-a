#ifndef CONSTITERATOR_HPP
#define CONSTITERATOR_HPP

#include <iterator>

#include "node.hpp"
#include "iterator.hpp"

namespace dribas
{
  template< class T >
  class ConstIterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend class List< T >;
  public:
    using citer = ConstIterator< T >;

    ConstIterator() noexcept;
    ConstIterator(const citer&) = default;

    citer& operator=(const citer&) = default;
    citer& operator++() noexcept;
    citer& operator--() noexcept;
    citer operator++(int) noexcept;
    citer operator--(int) noexcept;

    bool operator==(const citer&) const noexcept;
    bool operator!=(const citer&) const noexcept;

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;
  private:
    const Node< T >* head_;
    explicit ConstIterator(const Node< T >*) noexcept;
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
  ConstIterator< T >::ConstIterator(const Node< T >* head) noexcept:
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
    return head_->data_;
  }

  template< class T >
  const T* ConstIterator< T >::operator->() const noexcept
  {
    return std::addressof(head_->data_);
  }
}

#endif
