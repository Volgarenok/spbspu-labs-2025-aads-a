#ifndef CONST_ITERATOR_HPP
#define CONST_ITERATOR_HPP

#include <iterator>
#include <iostream>
#include "node.hpp"

namespace sharifullina
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
    using pointer = const T *;
    using reference = const T &;

    ConstIterator();
    ConstIterator(const ConstIterator &) = default;
    ~ConstIterator() = default;
    ConstIterator & operator=(const ConstIterator &) = default;

    ConstIterator & operator++();
    ConstIterator operator++(int);

    ConstIterator & operator--();
    ConstIterator operator--(int);

    reference operator *() const;
    pointer operator->() const;

    bool operator==(const ConstIterator &) const;
    bool operator!=(const ConstIterator &) const;

    template< typename U >
    friend std::ostream & operator<<(std::ostream & os, const ConstIterator< U > & it);

  private:
    Node< T > * node_;
    friend class List< T >;

    explicit ConstIterator(Node< T > * node);
  };

  template< typename T >
  std::ostream& operator<<(std::ostream & os, const ConstIterator< T > & it)
  {
    if (it.node_ != nullptr)
    {
      os << "ConstIterator(" << it.node_->data_ << ")";
    }
    else
    {
      os << "ConstIterator(nullptr)";
    }
    return os;
  }

  template< typename T >
  ConstIterator< T >::ConstIterator():
    node_(nullptr)
  {}

  template< typename T >
  ConstIterator< T >::ConstIterator(Node< T > * node):
    node_(node)
  {}

  template< typename T >
  ConstIterator< T > & ConstIterator< T >::operator++()
  {
    if (node_ == nullptr)
    {
      throw std::runtime_error("Attempt to increment null iterator");
    }
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  ConstIterator< T > ConstIterator< T >::operator++(int)
  {
    ConstIterator temp(*this);
    ++(*this);
    return temp;
  }

  template< typename T >
  ConstIterator< T > & ConstIterator< T >::operator--()
  {
    if (node_ == nullptr)
    {
      throw std::runtime_error("Attempt to decrement null iterator");
    }
    node_ = node_->prev_;
    return *this;
  }

  template< typename T >
  ConstIterator< T > ConstIterator< T >::operator--(int)
  {
    ConstIterator temp(*this);
    --(*this);
    return temp;
  }

  template< typename T >
  typename ConstIterator< T >::reference ConstIterator< T >::operator*() const
  {
    if (node_ == nullptr)
    {
      throw std::runtime_error("Attempt to dereference null iterator");
    }
    return node_->data_;
  }

  template< typename T >
  typename ConstIterator< T >::pointer ConstIterator< T >::operator->() const
  {
    if (node_ == nullptr)
    {
      throw std::runtime_error("Attempt to dereference null iterator");
    }
    return &node_->data_;
  }

  template< typename T >
  bool ConstIterator< T >::operator==(const ConstIterator & other) const
  {
    return node_ == other.node_;
  }

  template< typename T >
  bool ConstIterator< T >::operator!=(const ConstIterator & other) const
  {
    return !(*this == other);
  }
}

#endif
