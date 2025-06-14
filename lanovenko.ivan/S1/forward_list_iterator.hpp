#ifndef FORWARD_LIST_ITERATOR_HPP
#define FORWARD_LIST_ITERATOR_HPP

#include <iterator>
#include "forward_list_node.hpp"

namespace lanovenko
{
  template< typename T >
  class ForwardList;

  template< typename T >
  class ForwardListIterator final: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    using this_t = ForwardListIterator< T >;

    ~ForwardListIterator() = default;
    ForwardListIterator() noexcept;
    ForwardListIterator(const this_t& rhs) = default;

    this_t& operator=(const this_t& rhs) = default;
    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    T& operator*() noexcept;
    T* operator->() noexcept;
    bool operator!=(const this_t& rhs) const noexcept;
    bool operator==(const this_t& rhs) const noexcept;
  private:
    ForwardListNode< T >* node_;
    bool firstIteration_;
    explicit ForwardListIterator(ForwardListNode< T >* node, bool firstIteraion = true) noexcept;
    friend class ForwardList< T >;
  };

  template< typename T >
  ForwardListIterator< T >::ForwardListIterator() noexcept:
    node_(nullptr),
    firstIteration_(true)
  {}

  template< typename T >
  ForwardListIterator< T >::ForwardListIterator(ForwardListNode< T >* node, bool firstIteration) noexcept:
    node_(node),
    firstIteration_(firstIteration)
  {}

  template< typename T >
  ForwardListIterator< T >& ForwardListIterator< T >::operator++() noexcept
  {
    assert(node_);
    node_ = node_->next_;
    firstIteration_ = false;
    return *this;
  }

  template< typename T >
  ForwardListIterator< T > ForwardListIterator< T >::operator++(int) noexcept
  {
    assert(node_);
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  bool ForwardListIterator< T >::operator==(const this_t& rhs) const noexcept
  {
    return (node_ == rhs.node_ && firstIteration_ == rhs.firstIteration_);
  }

  template< typename T >
  bool ForwardListIterator< T >::operator!=(const this_t& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< typename T >
  T& ForwardListIterator< T >::operator*() noexcept
  {
    assert(node_);
    return node_->data_;
  }

  template< typename T >
  T* ForwardListIterator< T >::operator->() noexcept
  {
    assert(node_);
    return std::addressof(node_->data_);
  }
}

#endif
