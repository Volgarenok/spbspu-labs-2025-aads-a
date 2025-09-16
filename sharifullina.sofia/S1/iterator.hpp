#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include <cassert>
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
    using pointer = T*;
    using reference = T&;

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

  private:
    Node< T >* node_;
    friend class List< T >;

    explicit Iterator(Node< T >* node);
  };

  template< typename T >
  Iterator< T >::Iterator() :
    node_(nullptr)
  {}

  template< typename T >
  Iterator< T >::Iterator(Node< T >* node) :
    node_(node)
  {}

  template< typename T >
  Iterator< T > & Iterator< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  Iterator< T > Iterator< T >::operator++(int)
  {
    assert(node_ != nullptr);
    Iterator temp(*this);
    ++(*this);
    return temp;
  }

  template< typename T >
  Iterator< T > & Iterator< T >::operator--()
  {
    assert(node_ != nullptr);
    node_ = node_->prev_;
    return *this;
  }

  template< typename T >
  Iterator< T > Iterator< T >::operator--(int)
  {
    assert(node_ != nullptr);
    Iterator temp(*this);
    --(*this);
    return temp;
  }

  template< typename T >
  typename Iterator< T >::reference Iterator< T >::operator*()
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  typename Iterator< T >::pointer Iterator< T >::operator->()
  {
    assert(node_ != nullptr);
    return &node_->data_;
  }

  template< typename T >
  typename Iterator< T >::reference Iterator< T >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  typename Iterator< T >::pointer Iterator< T >::operator->() const
  {
    assert(node_ != nullptr);
    return &node_->data_;
  }

  template< typename T >
  bool Iterator< T >::operator==(const Iterator& other) const
  {
    return node_ == other.node_;
  }

  template< typename T >
  bool Iterator< T >::operator!=(const Iterator& other) const
  {
    return !(*this == other);
  }

  template< typename T >
  bool Iterator< T >::operator==(const ConstIterator< T >& other) const
  {
    return node_ == other.node_;
  }

  template< typename T >
  bool Iterator< T >::operator!=(const ConstIterator< T >& other) const
  {
    return !(*this == other);
  }
}

#endif
