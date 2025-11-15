#ifndef CIRCULARFWDLISTITERATORS_HPP
#define CIRCULARFWDLISTITERATORS_HPP

#include <memory>

#include "FwdListNode.hpp"

namespace zholobov {

  template < typename T >
  class CircularFwdList;

  template < typename T >
  class CircularFwdListConstIterator;

  template < typename T >
  class CircularFwdListIterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    CircularFwdListIterator();
    explicit CircularFwdListIterator(const CircularFwdListConstIterator< T >& other);
    CircularFwdListIterator& operator++() noexcept;
    CircularFwdListIterator operator++(int) noexcept;
    reference operator*() const noexcept;
    pointer operator->() const noexcept;
    bool operator==(const CircularFwdListIterator& other) const noexcept;
    bool operator!=(const CircularFwdListIterator& other) const noexcept;

  private:
    explicit CircularFwdListIterator(FwdListNodeBase** node);
    FwdListNodeBase** node_;

    friend class CircularFwdList< T >;
    friend class CircularFwdListConstIterator< T >;
  };

  template < typename T >
  class CircularFwdListConstIterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    CircularFwdListConstIterator();
    explicit CircularFwdListConstIterator(const CircularFwdListIterator< T >& other);
    CircularFwdListConstIterator& operator++() noexcept;
    CircularFwdListConstIterator operator++(int) noexcept;
    reference operator*() const noexcept;
    pointer operator->() const noexcept;
    bool operator==(const CircularFwdListConstIterator& other) const noexcept;
    bool operator!=(const CircularFwdListConstIterator& other) const noexcept;

  private:
    explicit CircularFwdListConstIterator(FwdListNodeBase* const* node);
    FwdListNodeBase** node_;

    friend class CircularFwdList< T >;
    friend class CircularFwdListIterator< T >;
  };

}

template < typename T >
zholobov::CircularFwdListIterator< T >::CircularFwdListIterator():
  node_(nullptr)
{}

template < typename T >
zholobov::CircularFwdListIterator< T >::CircularFwdListIterator(const CircularFwdListConstIterator< T >& other):
  node_(const_cast< FwdListNodeBase** >(other.node_))
{}

template < typename T >
zholobov::CircularFwdListIterator< T >::CircularFwdListIterator(FwdListNodeBase** node):
  node_(node)
{}

template < typename T >
zholobov::CircularFwdListIterator< T >& zholobov::CircularFwdListIterator< T >::operator++() noexcept
{
  node_ = std::addressof((*node_)->next);
  return *this;
}

template < typename T >
zholobov::CircularFwdListIterator< T > zholobov::CircularFwdListIterator< T >::operator++(int) noexcept
{
  CircularFwdListIterator< T > temp(*this);
  node_ = std::addressof((*node_)->next);
  return temp;
}

template < typename T >
typename zholobov::CircularFwdListIterator< T >::reference
zholobov::CircularFwdListIterator< T >::operator*() const noexcept
{
  return static_cast< FwdListNode< value_type >* >(*node_)->value;
}

template < typename T >
typename zholobov::CircularFwdListIterator< T >::pointer
zholobov::CircularFwdListIterator< T >::operator->() const noexcept
{
  return std::addressof(static_cast< FwdListNode< value_type >* >(*node_)->value);
}

template < typename T >
bool zholobov::CircularFwdListIterator< T >::operator==(const CircularFwdListIterator& other) const noexcept
{
  return node_ == other.node_;
}

template < typename T >
bool zholobov::CircularFwdListIterator< T >::operator!=(const CircularFwdListIterator& other) const noexcept
{
  return !(*this == other);
}

template < typename T >
zholobov::CircularFwdListConstIterator< T >::CircularFwdListConstIterator():
  node_(nullptr)
{}

template < typename T >
zholobov::CircularFwdListConstIterator< T >::CircularFwdListConstIterator(const CircularFwdListIterator< T >& other):
  node_(other.node_)
{}

template < typename T >
zholobov::CircularFwdListConstIterator< T >::CircularFwdListConstIterator(FwdListNodeBase* const* node):
  node_(const_cast< FwdListNodeBase** >(node))
{}

template < typename T >
zholobov::CircularFwdListConstIterator< T >& zholobov::CircularFwdListConstIterator< T >::operator++() noexcept
{
  node_ = std::addressof((*node_)->next);
  return *this;
}

template < typename T >
zholobov::CircularFwdListConstIterator< T > zholobov::CircularFwdListConstIterator< T >::operator++(int) noexcept
{
  CircularFwdListConstIterator< T > temp(*this);
  node_ = std::addressof((*node_)->next);
  return temp;
}

template < typename T >
typename zholobov::CircularFwdListConstIterator< T >::reference
zholobov::CircularFwdListConstIterator< T >::operator*() const noexcept
{
  return static_cast< FwdListNode< value_type >* >(*node_)->value;
}

template < typename T >
typename zholobov::CircularFwdListConstIterator< T >::pointer
zholobov::CircularFwdListConstIterator< T >::operator->() const noexcept
{
  return std::addressof(static_cast< FwdListNode< value_type >* >(*node_)->value);
}

template < typename T >
bool zholobov::CircularFwdListConstIterator< T >::operator==(const CircularFwdListConstIterator& other) const noexcept
{
  return node_ == other.node_;
}

template < typename T >
bool zholobov::CircularFwdListConstIterator< T >::operator!=(const CircularFwdListConstIterator& other) const noexcept
{
  return !(*this == other);
}

#endif
