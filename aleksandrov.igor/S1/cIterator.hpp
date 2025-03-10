#ifndef CITERATOR_HPP
#define CITERATOR_HPP

#include <iterator>
#include "list.hpp"

namespace aleksandrov
{
  template< typename T >
  class List;

  template< typename T >
  class cIterator: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    cIterator():
      node_(nullptr)
    {}
    cIterator(Node< T >* node):
      node_(node)
    {}
    cIterator(const cIterator< T >&) = default;
    ~cIterator() = default;
    cIterator< T >& operator=(const cIterator< T >&) = default;

    cIterator< T >& operator++();
    cIterator< T > operator++(int);

    const T& operator*() const;
    const T* operator->() const;

    bool operator!=(const cIterator< T >&) const;
    bool operator==(const cIterator< T >&) const;
  private:
    friend List< T >;
    Node< T >* node_;
  };

  template< typename T >
  cIterator< T >& cIterator< T >::operator++()
  {
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  cIterator< T > cIterator< T >::operator++(int)
  {
    cIterator< T > result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  const T& cIterator< T >::operator*() const
  {
    return node_->data_;
  }

  template< typename T >
  const T* cIterator< T >::operator->() const
  {
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool cIterator< T >::operator==(const cIterator< T >& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool cIterator< T >::operator!=(const cIterator< T >& rhs) const
  {
    return !(rhs == *this);
  }
}

#endif

