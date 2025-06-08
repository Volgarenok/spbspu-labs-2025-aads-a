#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include <type_traits>
#include <cassert>
#include "node.hpp"

namespace bocharov
{
  template< class T >
  class List;

  template< class T, bool is_const >
  class Iterator final: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    using reference = std::conditional_t< is_const, const T &, T & >;
    using pointer = std::conditional_t< is_const, const T *, T * >;

    Iterator() noexcept:
      node_(nullptr)
    {}
    Iterator(const Iterator & rhs) noexcept = default;
    ~Iterator() = default;
    Iterator & operator=(const Iterator & rhs) noexcept = default;

    Iterator & operator++() noexcept
    {
      assert(node_ && "incrementing empty iterator");
      node_ = node_->next_;
      return *this;
    }

    Iterator & operator++(int) noexcept
    {
      Iterator< T, is_const > result(*this);
      ++(*this);
      return result;
    }

    reference operator*() const noexcept
    {
      assert(node_ && "dereferencing empty iterator");
      return node_->data_;
    }

    pointer operator->() const noexcept
    {
      assert(node_ && "dereferencing empty iterator");
      return std::addressof(node_->data_);
    }

    bool operator==(const Iterator & rhs) const noexcept
    {
      return node_ == rhs.node_;
    }

    bool operator!=(const Iterator & rhs) const noexcept
    {
      return !(*this == rhs);
    }

  private:
    friend class List< T >;
    Node< T > * node_;
    explicit Iterator(Node< T > * node) noexcept:
      node_(node)
    {}
  };
}

#endif
