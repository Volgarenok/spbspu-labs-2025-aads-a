#ifndef FORWARD_LIST_CONST_ITERATOR_HPP
#define FORWARD_LIST_CONST_ITERATOR_HPP

#include <iterator>
#include "forward_list_node.hpp"

namespace lanovenko
{
  template < typename T >
  class ForwardList;

  template < typename T>
  class ForwardListConstIterator final: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    using const_this_t = ForwardListConstIterator< T >;
    ~ForwardListConstIterator() = default;
    ForwardListConstIterator();
    ForwardListConstIterator(ForwardListNode< T >*);
    ForwardListConstIterator(const const_this_t&) = default;
    const_this_t& operator=(const const_this_t&) = default;
    const_this_t& operator++() noexcept;
    const_this_t operator++(int) noexcept;
    const T& operator*() const noexcept;
    const T* operator->() const noexcept;
    bool operator==(const const_this_t&) const noexcept;
    bool operator!=(const const_this_t&) const noexcept;
  private:
    friend class ForwardList< T >;
    ForwardListNode< T >* node_;
    bool firstIteration_;
  };
}

template< typename T >
lanovenko::ForwardListConstIterator< T >::ForwardListConstIterator():
  node_(nullptr),
  firstIteration_(true)
{}

template<typename T>
lanovenko::ForwardListConstIterator< T >::ForwardListConstIterator(ForwardListNode< T >* node):
  node_(node),
  firstIteration_(true)
{}

template<typename T>
lanovenko::ForwardListConstIterator< T >& lanovenko::ForwardListConstIterator< T >::operator++() noexcept
{
  assert(node_ != nullptr);
  node_ = node_->next_;
  firstIteration_ = false;
  return *this;
}

template<typename T>
lanovenko::ForwardListConstIterator< T > lanovenko::ForwardListConstIterator< T >::operator++(int) noexcept
{
  assert(node_ != nullptr);
  const_this_t result(*this);
  ++(*this);
  return result;
}

template<typename T>
const T& lanovenko::ForwardListConstIterator< T >::operator*() const noexcept
{
  assert(node_ != nullptr);
  return node_->data_;
}

template<typename T>
const T* lanovenko::ForwardListConstIterator< T >::operator->() const noexcept
{
  assert(node_ != nullptr);
  return std::addressof(node_->data_);
}

template<typename T>
bool lanovenko::ForwardListConstIterator< T >::operator==(const const_this_t& rhs) const noexcept
{
  return (node_ == rhs.node_ && firstIteration_ == rhs.firstIteration_);
}

template<typename T>
bool lanovenko::ForwardListConstIterator< T >::operator!=(const const_this_t& rhs) const noexcept
{
  return !(*this == rhs);
}

#endif
