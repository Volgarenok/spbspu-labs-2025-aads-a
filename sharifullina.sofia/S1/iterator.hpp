#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include <stdexcept>
#include <iostream>
#include "node.hpp"
#include "constIterator.hpp"

namespace sharifullina
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
    using pointer = T *;
    using reference = T &;

    Iterator();
    Iterator(const Iterator &) = default;
    ~Iterator() = default;
    Iterator & operator=(const Iterator &) = default;

    Iterator & operator++();
    Iterator operator++(int);

    Iterator & operator--();
    Iterator operator--(int);

    reference operator*();
    pointer operator->();
    reference operator*() const;
    pointer operator->() const;

    bool operator==(const Iterator &) const;
    bool operator!=(const Iterator &) const;
    bool operator==(const ConstIterator< T > &) const;
    bool operator!=(const ConstIterator< T > &) const;

    template< typename U >
    friend std::ostream & operator<<(std::ostream & os, const Iterator< U > & it);

  private:
    Node< T > * node_;
    friend class List< T >;

    explicit Iterator(Node< T > * node);
  };

  template< typename T >
  std::ostream & operator<<(std::ostream & os, const Iterator< T > & it)
  {
    if (it.node_ != nullptr)
    {
      os << "Iterator(" << it.node_->data_ << ")";
    }
    else
    {
      os << "Iterator(nullptr)";
    }
    return os;
  }

  template< typename T >
  Iterator< T >::Iterator():
    node_(nullptr)
  {}

  template< typename T >
  Iterator< T >::Iterator(Node< T > * node):
    node_(node)
  {}

  template< typename T >
  Iterator< T > & Iterator< T >::operator++()
  {
    if (node_ == nullptr)
    {
      throw std::runtime_error("Attempt to increment null iterator");
    }
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  Iterator< T > Iterator< T >::operator++(int)
  {
    Iterator temp(*this);
    ++(*this);
    return temp;
  }

  template< typename T >
  Iterator< T > & Iterator< T >::operator--()
  {
    if (node_ == nullptr)
    {
      throw std::runtime_error("Attempt to decrement null iterator");
    }
    node_ = node_->prev_;
    return *this;
  }

  template< typename T >
  Iterator< T > Iterator< T >::operator--(int)
  {
    Iterator temp(*this);
    --(*this);
    return temp;
  }

  template< typename T >
  typename Iterator< T >::reference Iterator< T >::operator*()
  {
    if (node_ == nullptr)
    {
      throw std::runtime_error("Attempt to dereference null iterator");
    }
    return node_->data_;
  }

  template< typename T >
  typename Iterator< T >::pointer Iterator< T >::operator->()
  {
    if (node_ == nullptr)
    {
      throw std::runtime_error("Attempt to dereference null iterator");
    }
    return &node_->data_;
  }

  template< typename T >
  typename Iterator< T >::reference Iterator< T >::operator*() const
  {
    if (node_ == nullptr)
    {
      throw std::runtime_error("Attempt to dereference null iterator");
    }
    return node_->data_;
  }

  template< typename T >
  typename Iterator< T >::pointer Iterator< T >::operator->() const
  {
    return &node_->data_;
  }

  template< typename T >
  bool Iterator< T >::operator==(const Iterator & other) const
  {
    return node_ == other.node_;
  }

  template< typename T >
  bool Iterator< T >::operator!=(const Iterator & other) const
  {
    return !(*this == other);
  }

  template< typename T >
  bool Iterator< T >::operator==(const ConstIterator< T > & other) const
  {
    return node_ == other.node_;
  }

  template< typename T >
  bool Iterator< T >::operator!=(const ConstIterator< T > & other) const
  {
    return !(*this == other);
  }
}

#endif
