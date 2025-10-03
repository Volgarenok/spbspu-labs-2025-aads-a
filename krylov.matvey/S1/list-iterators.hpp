#ifndef LISTITERATORS_HPP
#define LISTITERATORS_HPP
#include "list.hpp"

namespace krylov
{
  template< typename T >
  class List;

  template< typename T >
  class Iterator
  {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    Iterator();
    T& operator*() noexcept;
    T* operator->() noexcept;
    Iterator& operator++() noexcept;
    Iterator operator++(int) noexcept;
    Iterator& operator--() noexcept;
    Iterator operator--(int) noexcept;
    bool operator!=(const Iterator& other) const noexcept;
    bool operator==(const Iterator& other) const noexcept;
  private:
    Node< T >* current_;
    const List< T >* list_;
    friend class List< T >;
    explicit Iterator(Node< T >* node) noexcept;
    Iterator(Node< T >* node, const List< T >* list) noexcept;
  };

  template< typename T >
  Iterator< T >::Iterator(Node< T >* node) noexcept:
    current_(node),
    list_(nullptr)
  {}

  template< typename T >
  Iterator< T >::Iterator(Node< T >* node, const List< T >* list) noexcept:
    current_(node),
    list_(list)
  {}

  template< typename T >
  T& Iterator< T >::operator*() noexcept
  {
    return current_->data_;
  }

  template< typename T >
  T* Iterator< T >::operator->() noexcept
  {
    return std::addressof(current_->data_);
  }

  template< typename T >
  Iterator< T >& Iterator< T >::operator++() noexcept
  {
    if (current_)
    {
      current_ = current_->next_;
    }
    return *this;
  }

  template< typename T >
  Iterator< T > Iterator< T >::operator++(int) noexcept
  {
    Iterator< T > temp = *this;
    ++(*this);
    return temp;
  }

  template< typename T >
  Iterator< T >& Iterator< T >::operator--() noexcept
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
  Iterator< T > Iterator< T >::operator--(int) noexcept
  {
    Iterator< T > temp = *this;
    --(*this);
    return temp;
  }

  template< typename T >
  bool Iterator< T >::operator!=(const Iterator& other) const noexcept
  {
    return current_ != other.current_;
  }

  template< typename T >
  bool Iterator < T >::operator==(const Iterator& other) const noexcept
  {
    return current_ == other.current_;
  }
}

#endif
