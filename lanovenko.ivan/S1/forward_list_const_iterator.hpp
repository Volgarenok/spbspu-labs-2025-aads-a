#ifndef FORWARD_LIST_CONST_ITERATOR_HPP
#define FORWARD_LIST_CONST_ITERATOR_HPP

#include <iterator>
#include "forward_list_node.hpp"

namespace lanovenko
{
  template< typename T >
  class ForwardList;

  template< typename T >
  class ForwardListConstIterator final: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    using const_this_t = ForwardListConstIterator< T >;

    ~ForwardListConstIterator() = default;
    ForwardListConstIterator() noexcept;
    ForwardListConstIterator(const const_this_t& rhs) = default;

    const_this_t& operator=(const const_this_t& rhs) = default;
    const_this_t& operator++() noexcept;
    const_this_t operator++(int) noexcept;
    const T& operator*() const noexcept;
    const T* operator->() const noexcept;
    bool operator==(const const_this_t& rhs) const noexcept;
    bool operator!=(const const_this_t& rhs) const noexcept;
  private:
    ForwardListNode< T >* node_;
    bool firstIteration_;
    explicit ForwardListConstIterator(ForwardListNode< T >* node, bool firstIteration = true) noexcept;
    friend class ForwardList< T >;
  };


  template< typename T >
  ForwardListConstIterator< T >::ForwardListConstIterator() noexcept:
    node_(nullptr),
    firstIteration_(true)
  {}

  template< typename T >
  ForwardListConstIterator< T >::ForwardListConstIterator(ForwardListNode< T >* node, bool firstIteration) noexcept:
    node_(node),
    firstIteration_(firstIteration)
  {}

  template< typename T >
  ForwardListConstIterator< T >& ForwardListConstIterator< T >::operator++() noexcept
  {
    assert(node_);
    node_ = node_->next_;
    firstIteration_ = false;
    return *this;
  }

  template< typename T >
  ForwardListConstIterator< T > ForwardListConstIterator< T >::operator++(int) noexcept
  {
    assert(node_);
    const_this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  const T& ForwardListConstIterator< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  const T* ForwardListConstIterator< T >::operator->() const noexcept
  {
    assert(node_);
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool ForwardListConstIterator< T >::operator==(const const_this_t& rhs) const noexcept
  {
    return (node_ == rhs.node_ && firstIteration_ == rhs.firstIteration_);
  }

  template< typename T >
  bool ForwardListConstIterator< T >::operator!=(const const_this_t& rhs) const noexcept
  {
    return !(*this == rhs);
  }
}

#endif
