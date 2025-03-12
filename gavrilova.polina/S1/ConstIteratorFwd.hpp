#ifndef CONST_ITERATOR_HPP
#define CONST_ITERATOR_HPP

#include <iterator>
#include <cassert>
#include "NodeFwdList.hpp"

namespace gavrilova {
  template< class T >
  struct ConstIteratorFwd: public std::iterator< std::forward_iterator_tag, T >
  {
    const NodeFwdList< T >* node_;
    using this_t = ConstIteratorFwd< T >;

    ConstIteratorFwd() : node_(nullptr) {};
    ConstIteratorFwd(const NodeFwdList< T >* node) : node_(node) {};
    ~ConstIteratorFwd() = default;
    ConstIteratorFwd(const this_t&) = default;
    this_t& operator=(const this_t&) = default;

    this_t& operator++();
    this_t operator++(int);
    const T& operator*() const;
    const T* operator->() const;
    bool operator!=(const this_t&) const;
    bool operator==(const this_t&) const;
  };
}

template< class T >
gavrilova::ConstIteratorFwd< T >& gavrilova::ConstIteratorFwd< T >::operator++()
{
  node_ = node_->next;
  return *this;
}

template< class T >
gavrilova::ConstIteratorFwd<T> gavrilova::ConstIteratorFwd<T>::operator++(int)
{
  ConstIteratorFwd< T > result(*this);
  ++(*this);
  return result;
}

template< class T >
bool gavrilova::ConstIteratorFwd<T>::operator==(const this_t& rhs) const
{
  return node_ == rhs.node_;
}

template< class T >
bool gavrilova::ConstIteratorFwd<T>::operator!=(const this_t& rhs) const
{
  return !(rhs == *this);
}

template< class T >
const T& gavrilova::ConstIteratorFwd<T>::operator*() const
{
  assert(node_ != nullptr);
  return node_->data;
}

template< class T >
const T* gavrilova::ConstIteratorFwd<T>::operator->() const
{
  assert(node_ != nullptr);
  return std::addressof(node_->data);
}

#endif
