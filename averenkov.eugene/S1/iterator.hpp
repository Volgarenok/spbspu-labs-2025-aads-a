#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
#include <cassert>
#include "node.hpp"

namespace averenkov
{
  template< class T >
  struct ListIterator: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    ListIterator() = default;
    explicit ListIterator(Node< T >* node) noexcept;
    ~ListIterator() = default;

    ListIterator< T >& operator++();
    ListIterator< T > operator++(int);

    T& operator*() const noexcept;
    T* operator->() const noexcept;

    bool operator==(const ListIterator< T >&) const;
    bool operator!=(const ListIterator< T >&) const;

    Node< T >* getNode();

  private:
    Node< T >* node_;

  };


  template< class T >
  Node< T >*  ListIterator< T >::getNode()
  {
    return node_;
  }

  template< class T >
  ListIterator< T >::ListIterator(Node< T >* node) noexcept:
    node_(node)
  {
  }

  template< class T >
  ListIterator< T >& ListIterator< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
    return *this;
  }

  template< class T >
  ListIterator< T > ListIterator< T >::operator++(int)
  {
    assert(node_ != nullptr);
    ListIterator< T > result(*this);
    ++(*this);
    return result;
  }

  template< class T >
  T& ListIterator< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< class T >
  T* ListIterator< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< class T >
  bool ListIterator< T >::operator==(const ListIterator< T >& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class T >
  bool ListIterator< T >::operator!=(const ListIterator< T >& rhs) const
  {
    return !(rhs == *this);
  }
}

#endif
