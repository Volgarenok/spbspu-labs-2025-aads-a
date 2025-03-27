#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include <type_traits>
#include "node.hpp"

namespace bocharov
{
  template< typename T >
  class List;

  template< typename T, bool is_const >
  class Iterator final: public std::iterator< std::forward_iterator_tag, int >
  {
  public:
    Iterator() noexcept = default;
    Iterator(const Iterator & rhs) noexcept = default;
    ~Iterator() = default;
    Iterator & operator=(const Iterator & rhs) noexcept = default;

    Iterator & operator++() noexcept
    {
      node_ = node_->next_;
      return *this;
    }

    Iterator & operator++(int) noexcept
    {
      Iterator< T, is_const > result(*this);
      ++(*this);
      return result;
    }

    typename std::conditional< is_const, const T &, T & >::type operator*() const noexcept
    {
      return node_->data_;
    }

    typename std::conditional< is_const, const T *, T * >::type operator->() const noexcept
    {
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
    Iterator(Node< T > * node) noexcept:
      node_(node)
    {}
  };
}

#endif
