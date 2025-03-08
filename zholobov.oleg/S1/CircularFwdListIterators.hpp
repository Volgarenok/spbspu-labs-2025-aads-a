#ifndef CIRCULARFWDLISTITERATORS_HPP
#define CIRCULARFWDLISTITERATORS_HPP

#include <memory>

#include "FwdListNode.hpp"

namespace zholobov {

  template < typename T >
  class CircularFwdListIterator {
  public:
    using value_type = T;

    CircularFwdListIterator();
    explicit CircularFwdListIterator(FwdListNode< value_type >** node);
    CircularFwdListIterator& operator++();
    CircularFwdListIterator operator++(int);
    value_type& operator*() const;
    value_type* operator->() const;
    bool operator==(const CircularFwdListIterator& other) const;
    bool operator!=(const CircularFwdListIterator& other) const;

  private:
    FwdListNode< value_type >** node_;
  };

  template < typename T >
  class CircularFwdListConstIterator {
  public:
    using value_type = const T;

    CircularFwdListConstIterator();
    explicit CircularFwdListConstIterator(FwdListNode< T >* const* node);
    CircularFwdListConstIterator& operator++();
    CircularFwdListConstIterator operator++(int);
    value_type& operator*() const;
    value_type* operator->() const;
    bool operator==(const CircularFwdListConstIterator& other) const;
    bool operator!=(const CircularFwdListConstIterator& other) const;

  private:
    FwdListNode< T >* const* node_;
  };

}

template < typename T >
zholobov::CircularFwdListIterator< T >::CircularFwdListIterator():
  node_(nullptr)
{}

template < typename T >
zholobov::CircularFwdListIterator< T >::CircularFwdListIterator(FwdListNode< value_type >** node):
  node_(node)
{}

template < typename T >
zholobov::CircularFwdListIterator< T >& zholobov::CircularFwdListIterator< T >::operator++()
{
  node_ = std::addressof((*node_)->next);
  return *this;
}

template < typename T >
zholobov::CircularFwdListIterator< T > zholobov::CircularFwdListIterator< T >::operator++(int)
{
  CircularFwdListIterator< T > temp(*this);
  node_ = std::addressof((*node_)->next);
  return temp;
}

template < typename T >
typename zholobov::CircularFwdListIterator< T >::value_type& zholobov::CircularFwdListIterator< T >::operator*() const
{
  return (*node_)->value;
}

template < typename T >
typename zholobov::CircularFwdListIterator< T >::value_type* zholobov::CircularFwdListIterator< T >::operator->() const
{
  return std::addressof((*node_)->value);
}

template < typename T >
bool zholobov::CircularFwdListIterator< T >::operator==(const CircularFwdListIterator& other) const
{
  return node_ == other.node_;
}

template < typename T >
bool zholobov::CircularFwdListIterator< T >::operator!=(const CircularFwdListIterator& other) const
{
  return !(*this == other);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

template < typename T >
zholobov::CircularFwdListConstIterator< T >::CircularFwdListConstIterator():
  node_(nullptr)
{}

template < typename T >
zholobov::CircularFwdListConstIterator< T >::CircularFwdListConstIterator(FwdListNode< T >* const* node):
  node_(node)
{}

template < typename T >
zholobov::CircularFwdListConstIterator< T >& zholobov::CircularFwdListConstIterator< T >::operator++()
{
  node_ = std::addressof((*node_)->next);
  return *this;
}

template < typename T >
zholobov::CircularFwdListConstIterator< T > zholobov::CircularFwdListConstIterator< T >::operator++(int)
{
  CircularFwdListConstIterator< T > temp(*this);
  node_ = std::addressof((*node_)->next);
  return temp;
}

template < typename T >
typename zholobov::CircularFwdListConstIterator< T >::value_type& zholobov::CircularFwdListConstIterator< T >::operator*() const
{
  return (*node_)->value;
}

template < typename T >
typename zholobov::CircularFwdListConstIterator< T >::value_type* zholobov::CircularFwdListConstIterator< T >::operator->() const
{
  return std::addressof((*node_)->value);
}

template < typename T >
bool zholobov::CircularFwdListConstIterator< T >::operator==(const CircularFwdListConstIterator& other) const
{
  return node_ == other.node_;
}

template < typename T >
bool zholobov::CircularFwdListConstIterator< T >::operator!=(const CircularFwdListConstIterator& other) const
{
  return !(*this == other);
}

#endif
