#ifndef CONST_ITERATORS_HPP
#define CONST_ITERATORS_HPP
#include <cassert>
#include <memory>
#include "node_list.hpp"

namespace smirnov
{
  template < typename T >
  class List;
  template < typename T >
  class ConstIteratorList: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    using this_t = ConstIteratorList< T >;
    ConstIteratorList() noexcept;
    ~ConstIteratorList() = default;
    const T & operator*() const noexcept;
    const T * operator->() const noexcept;
    this_t & operator++() noexcept;
    this_t operator++(int) noexcept;
    bool operator==(const this_t &) const noexcept;
    bool operator!=(const this_t &) const noexcept;
  private:
    friend class List < T >;
    const ListNode< T > * node_;
    explicit ConstIteratorList(const ListNode< T > * node) noexcept;
  };

  template < typename T >
  ConstIteratorList< T >::ConstIteratorList() noexcept:
    node_(nullptr)
  {}

  template < typename T >
  ConstIteratorList< T >::ConstIteratorList(const ListNode< T > * node) noexcept:
    node_(node)
  {}

  template < typename T >
  const T & ConstIteratorList< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template < typename T >
  const T * ConstIteratorList< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template < typename T >
  ConstIteratorList< T > & ConstIteratorList< T >::operator++() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }

  template < typename T >
  ConstIteratorList< T > ConstIteratorList< T >::operator++(int) noexcept
  {
    assert(node_ != nullptr);
    this_t result(*this);
    ++(*this);
    return result;
  }

  template < typename T >
  bool ConstIteratorList< T >::operator==(const this_t & rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template < typename T >
  bool ConstIteratorList< T >::operator!=(const this_t & rhs) const noexcept
  {
    return !(rhs == *this);
  }
}
#endif
