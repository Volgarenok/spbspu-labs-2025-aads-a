#ifndef CITERATOR_HPP
#define CITERATOR_HPP
#include "list.hpp"

namespace abramov
{
  template< class T >
  struct List;

  template < class T >
  struct ConstIterator: std::iterator< std::bidirectional_iterator_tag, T >
  {
    ConstIterator();
    ConstIterator(Node< T > *node, const List< T > *list);
    ConstIterator(const ConstIterator &c_iter) = default;
    ~ConstIterator() = default;
    ConstIterator &operator=(const ConstIterator &c_iter) = default;
    ConstIterator &operator++() noexcept;
    ConstIterator operator++(int) noexcept;
    ConstIterator &operator--() noexcept;
    ConstIterator operator--(int) noexcept;
    bool operator==(const ConstIterator &c_iter) const noexcept;
    bool operator!=(const ConstIterator &c_iter) const noexcept;
    const T &operator*() const noexcept;
    const T *operator->() const noexcept;
  private:
    Node< T > *node_;
    const List< T > *list_;
    friend struct List< T >;
  };

  template< class T >
  ConstIterator< T >::ConstIterator():
    node_(nullptr)
  {}

  template< class T >
  ConstIterator< T >::ConstIterator(Node< T > *node, const List< T > *list):
    node_(node),
    list_(list)
  {}

  template< class T >
  ConstIterator< T > &ConstIterator< T >::operator++() noexcept
  {
    node_ = node_->next_;
    return *this;
  }

  template< class T >
  ConstIterator< T > ConstIterator< T >::operator++(int) noexcept
  {
    ConstIterator< T > c_iter(*this);
    ++(*this);
    return c_iter;
  }

  template< class T >
  ConstIterator< T > &ConstIterator< T >::operator--() noexcept
  {
    if (node_)
    {
      node_ = node_->prev_;
    }
    else
    {
      node_ = list_->tail_;
    }
    return *this;
  }

  template< class T >
  ConstIterator< T > ConstIterator< T >::operator--(int) noexcept
  {
    ConstIterator< T > c_iter(*this);
    --(*this);
    return c_iter;
  }

  template< class T >
  bool ConstIterator< T >::operator==(const ConstIterator &c_iter) const noexcept
  {
    return node_ == c_iter.node_;
  }

  template< class T >
  bool ConstIterator< T >::operator!=(const ConstIterator &c_iter) const noexcept
  {
    return !(c_iter == *this);
  }

  template< class T >
  const T &ConstIterator< T >::operator*() const noexcept
  {
    return node_->data_;
  }

  template< class T >
  const T *ConstIterator< T >::operator->() const noexcept
  {
    return std::addressof(node_->data_);
  }
}
#endif
