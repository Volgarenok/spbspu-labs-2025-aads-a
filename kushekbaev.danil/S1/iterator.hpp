#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <cassert>
#include <iterator>
#include "fwdlist.hpp"
#include "node.hpp"

namespace kushekbaev
{
  template< typename T >
  struct Iterator final: public std::iterator< std::forward_iterator_tag, T >
  {
    using this_t = Iterator< T >;
    Node< T >* node;

    Iterator();
    Iterator(Node< T > *);
    Iterator(const this_t &) = default;
    ~Iterator() = default;

    this_t& operator=(const this_t &) = default;
    this_t& operator++();
    this_t operator++(int);

    T& operator*();
    T* operator -> ();

    bool operator!=(const this_t &) const;
    bool operator==(const this_t &) const;

    Node< T > * getNode() const;

    private:
      Node< T >* node_;
  };

  template< typename T >
  Iterator< T >::Iterator():
    node_(nullptr)
  {}

  template< typename T >
  Iterator< T >::Iterator(Node< T > * node):
    node_(node)
  {}

  template< typename T >
  typename Iterator< T >::this_t & Iterator< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_ -> next;
    return *this;
  }

  template< typename T >
  typename Iterator< T >::this_t Iterator< T >::operator++(int)
  {
    assert(node_ != nullptr);
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  T & Iterator< T >::operator*()
  {
    assert(node_ != nullptr);
    return node_ -> data_;
  }

  template< typename T >
  T * Iterator< T >::operator->()
  {
    assert(node_ != nullptr);
    return std::addressof(node -> data_);
  }

  template< typename T >
  bool Iterator< T >::operator!=(const this_t & other) const
  {
    return !(other == *this);
  }

  template< typename T >
  bool Iterator< T >::operator==(const this_t & other) const
  {
    return node_ == other.node;
  }

  template< typename T >
  Node< T > * Iterator< T >::getNode() const
  {
    return node_;
  }
}

#endif
