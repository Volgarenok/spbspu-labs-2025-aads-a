#ifndef FORWARD_LIST_ITERATOR_HPP
#define FORWARD_LIST_ITERATOR_HPP

#include <iterator>
#include "forward_list_node.hpp"

namespace lanovenko
{
  template < typename T >
  class ForwardList;

  template < typename T >
  class ForwardListIterator final: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    using this_t = ForwardListIterator< T >;
    ~ForwardListIterator() = default;
    ForwardListIterator();
    ForwardListIterator(ForwardListNode< T >* node);
    ForwardListIterator(const this_t&) = default;
    this_t& operator=(const this_t&) = default;
    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    T& operator*() noexcept;
    T* operator->() noexcept;
    const T& operator*() const noexcept;
    const T* operator->() const noexcept;
    bool operator!=(const this_t&) const noexcept;
    bool operator==(const this_t&) const noexcept;
  private:
    friend class ForwardList < T >;
    ForwardListNode< T >* node_;
    bool firstIteration_;
  };
}

template < typename T >
lanovenko::ForwardListIterator< T >::ForwardListIterator():
  node_(nullptr),
  firstIteration_(true)
{}

template <typename T>
lanovenko::ForwardListIterator< T >::ForwardListIterator(ForwardListNode< T >* node):
  node_(node),
  firstIteration_(true)
{}

template <typename T>
typename lanovenko::ForwardListIterator< T >& lanovenko::ForwardListIterator< T >::operator++() noexcept
{
  assert(node_ != nullptr);
  node_ = node_->next_;
  firstIteration_ = false;
  return *this;
}

template <typename T>
typename lanovenko::ForwardListIterator< T > lanovenko::ForwardListIterator< T >::operator++(int) noexcept
{
  assert(node_ != nullptr);
  this_t result(*this);
  ++(*this);
  return result;
}

template <typename T>
bool lanovenko::ForwardListIterator< T >::operator==(const this_t& rhs) const noexcept
{
  return (node_ == rhs.node_ && firstIteration_ == rhs.firstIteration_);
}

template <typename T>
bool lanovenko::ForwardListIterator< T >::operator!=(const this_t& rhs) const noexcept
{
  return !(*this == rhs);
}

template <typename T>
T& lanovenko::ForwardListIterator< T >::operator*() noexcept
{
  assert(node_ != nullptr);
  return node_->data_;
}

template <typename T>
T* lanovenko::ForwardListIterator< T >::operator->() noexcept
{
  assert(node_ != nullptr);
  return std::addressof(node_->data_);
}

template <typename T>
const T& lanovenko::ForwardListIterator< T >::operator*() const noexcept
{
  assert(node_ != nullptr);
  return node_->data_;
}

template<typename T>
const T* lanovenko::ForwardListIterator< T >::operator->() const noexcept
{
  assert(node_ != nullptr);
  return std::addressof(node_->data_);
}

#endif
