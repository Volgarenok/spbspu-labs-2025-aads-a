#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include "node.hpp"

namespace aleksandrov
{
  template< typename T >
  class Iterator: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    Iterator():
      node_(nullptr)
    {}
    Iterator(Node< T >* node):
      node_(node)
    {}
    ~Iterator() = default;
    Iterator(const Iterator< T >&) = default;
    Iterator< T >& operator=(const Iterator< T >&) = default;

    Iterator< T >& operator++();
    Iterator< T > operator++(int);

    T& operator*();
    T* operator->();

    bool operator!=(const Iterator< T >&) const;
    bool operator==(const Iterator< T >&) const;

    Node< T >* getNode() const;
  private:
    Node< T >* node_;
  };

  template< typename T >
  Iterator< T >& Iterator< T >::operator++()
  {
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  Iterator< T > Iterator< T >::operator++(int)
  {
    Iterator< T > result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  T& Iterator< T >::operator*()
  {
    return node_->data_;
  }

  template< typename T >
  T* Iterator< T >::operator->()
  {
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool Iterator< T >::operator==(const Iterator< T >& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool Iterator< T >::operator!=(const Iterator< T >& rhs) const
  {
    return !(rhs == *this);
  }

  template< typename T >
  Node< T >* Iterator< T >::getNode() const
  {
    return node_;
  }
}

#endif

