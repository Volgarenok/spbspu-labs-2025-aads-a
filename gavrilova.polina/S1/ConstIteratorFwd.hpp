#ifndef CONST_ITERATOR_HPP
#define CONST_ITERATOR_HPP

#include <cassert>
#include <iterator>
#include "FwdList.hpp"

namespace gavrilova {
  template < class T >
  struct ConstIteratorFwd: public std::iterator< std::forward_iterator_tag, T > {
    using this_t = ConstIteratorFwd< T >;

    ConstIteratorFwd();
    ~ConstIteratorFwd() = default;
    ConstIteratorFwd(const this_t&) = default;
    ConstIteratorFwd(IteratorFwd< T >& other);
    this_t& operator=(const this_t&) = default;

    this_t& operator++();
    this_t operator++(int);
    const T& operator*() const;
    const T* operator->() const;
    bool operator!=(const this_t&) const;
    bool operator==(const this_t&) const;

  private:
    NodeFwdList< T >* node_;
    friend class FwdList< T >;
    friend class IteratorFwd< T >;
    explicit ConstIteratorFwd(NodeFwdList< T >* node);
  };
}

template < class T >
gavrilova::ConstIteratorFwd< T >::ConstIteratorFwd():
  node_(nullptr)
{}

template < class T >
gavrilova::ConstIteratorFwd< T >::ConstIteratorFwd(IteratorFwd< T >& other):
  node_(other.node_)
{}

template < class T >
gavrilova::ConstIteratorFwd< T >::ConstIteratorFwd(NodeFwdList< T >* node):
  node_(node)
{}

template < class T >
gavrilova::ConstIteratorFwd< T >& gavrilova::ConstIteratorFwd< T >::operator++()
{
  node_ = node_->next;
  return *this;
}

template < class T >
gavrilova::ConstIteratorFwd< T > gavrilova::ConstIteratorFwd< T >::operator++(int)
{
  ConstIteratorFwd< T > result(*this);
  ++(*this);
  return result;
}

template < class T >
bool gavrilova::ConstIteratorFwd< T >::operator==(const this_t& rhs) const
{
  return node_ == rhs.node_;
}

template < class T >
bool gavrilova::ConstIteratorFwd< T >::operator!=(const this_t& rhs) const
{
  return !(rhs == *this);
}

template < class T >
const T& gavrilova::ConstIteratorFwd< T >::operator*() const
{
  assert(node_ != nullptr);
  return node_->data;
}

template < class T >
const T* gavrilova::ConstIteratorFwd< T >::operator->() const
{
  assert(node_ != nullptr);
  return std::addressof(node_->data);
}

#endif
