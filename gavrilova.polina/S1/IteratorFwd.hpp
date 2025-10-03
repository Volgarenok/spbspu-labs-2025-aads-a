#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <cassert>
#include <iterator>
#include "FwdList.hpp"

namespace gavrilova {
  template < class T >
  struct IteratorFwd: public std::iterator< std::forward_iterator_tag, T > {
    using this_t = IteratorFwd< T >;

    IteratorFwd();
    ~IteratorFwd() = default;
    IteratorFwd(const this_t&) = default;
    this_t& operator=(const this_t&) = default;

    this_t& operator++();
    this_t operator++(int);
    T& operator*();
    T* operator->();
    bool operator!=(const this_t&) const;
    bool operator==(const this_t&) const;

  private:
    NodeFwdList< T >* node_;
    friend class FwdList< T >;
    friend class ConstIteratorFwd< T >;
    explicit IteratorFwd(NodeFwdList< T >* node);
  };
}

template < class T >
gavrilova::IteratorFwd< T >::IteratorFwd():
  node_(nullptr)
{}

template < class T >
gavrilova::IteratorFwd< T >::IteratorFwd(NodeFwdList< T >* node):
  node_(node)
{}

template < class T >
gavrilova::IteratorFwd< T >& gavrilova::IteratorFwd< T >::operator++()
{
  node_ = node_->next;
  return *this;
}

template < class T >
gavrilova::IteratorFwd< T > gavrilova::IteratorFwd< T >::operator++(int)
{
  IteratorFwd< T > result(*this);
  ++(*this);
  return result;
}

template < class T >
bool gavrilova::IteratorFwd< T >::operator==(const this_t& rhs) const
{
  return node_ == rhs.node_;
}

template < class T >
bool gavrilova::IteratorFwd< T >::operator!=(const this_t& rhs) const
{
  return !(rhs == *this);
}

template < class T >
T& gavrilova::IteratorFwd< T >::operator*()
{
  assert(node_ != nullptr);
  return node_->data;
}

template < class T >
T* gavrilova::IteratorFwd< T >::operator->()
{
  assert(node_ != nullptr);
  return std::addressof(node_->data);
}

#endif
