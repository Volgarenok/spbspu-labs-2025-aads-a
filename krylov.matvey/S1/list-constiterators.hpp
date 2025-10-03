#ifndef LISTCONSTITERATORS_HPP
#define LISTCONSTITERATORS_HPP
#include "list.hpp"

namespace krylov
{
  template< typename T >
  class List;

  template< typename T >
  class ConstIterator
  {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;
    ConstIterator();
    const T& operator*() const noexcept;
    const T* operator->() const noexcept;
    ConstIterator& operator++() noexcept;
    ConstIterator operator++(int) noexcept;
    ConstIterator& operator--() noexcept;
    ConstIterator operator--(int) noexcept;
    bool operator!=(const ConstIterator& other) const noexcept;
    bool operator==(const ConstIterator& other) const noexcept;
  private:
    Node< T >* current_;
    const List< T >* list_;
    friend class List< T >;
    explicit ConstIterator(Node< T >* node) noexcept;
    ConstIterator(Node< T >* node, const List< T >* list) noexcept;
  };

  template< typename T >
  ConstIterator< T >::ConstIterator(Node< T >* node) noexcept:
    current_(node),
    list_(nullptr)
  {}

  template< typename T >
  ConstIterator< T >::ConstIterator(Node< T >* node, const List< T >* list) noexcept:
    current_(node),
    list_(list)
  {}

  template< typename T >
  const T& ConstIterator< T >::operator*() const noexcept
  {
    return current_->data_;
  }

  template< typename T >
  const T* ConstIterator< T >::operator->() const noexcept
  {
    return std::addressof(current_->data_);
  }

  template< typename T >
  ConstIterator< T >& ConstIterator< T >::operator++() noexcept
  {
    if (current_)
    {
      current_ = current_->next_;
    }
    return *this;
  }

  template< typename T >
  ConstIterator< T > ConstIterator< T >::operator++(int) noexcept
  {
    ConstIterator< T > temp = *this;
    ++(*this);
    return temp;
  }

  template< typename T >
  ConstIterator< T >& ConstIterator< T >::operator--() noexcept
  {
    if (!current_ && list_)
    {
      current_ = list_->tail_;
    }
    else
    {
      current_ = current_->prev_;
    }
    return *this;
  }

  template< typename T >
  ConstIterator< T > ConstIterator< T >::operator--(int) noexcept
  {
    ConstIterator< T > temp = *this;
    --(*this);
    return temp;
  }

  template< typename T >
  bool ConstIterator< T >::operator!=(const ConstIterator& other) const noexcept
  {
    return current_ != other.current_;
  }

  template< typename T >
  bool ConstIterator< T >::operator==(const ConstIterator& other) const noexcept
  {
    return current_ == other.current_;
  }
}

#endif
