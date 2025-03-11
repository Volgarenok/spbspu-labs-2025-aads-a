#ifndef FWD_ITERATOR_HPP
#define FWD_ITERATOR_HPP

#include <iterator>
#include <type_traits>
#include "fwd_list_node.hpp"

namespace maslevtsov {
  template< typename T >
  class FwdList;

  template< typename T, bool is_const >
  class FwdIterator final: public std::iterator< std::forward_iterator_tag, int >
  {
  public:
    FwdIterator() noexcept = default;
    FwdIterator(const FwdIterator& rhs) noexcept = default;
    ~FwdIterator() = default;
    FwdIterator& operator=(const FwdIterator& rhs) noexcept = default;

    FwdIterator& operator++() noexcept
    {
      node_ = node_->next_;
      return *this;
    }

    FwdIterator& operator++(int) noexcept
    {
      FwdIterator< T, is_const > result(*this);
      ++(*this);
      return result;
    }

    typename std::conditional< is_const, const T&, T& >::type operator*() const noexcept
    {
      return node_->data_;
    }

    typename std::conditional< is_const, const T*, T* >::type operator->() const noexcept
    {
      return std::addressof(node_->data_);
    }

    bool operator==(const FwdIterator& rhs) const noexcept
    {
      return node_ == rhs.node_;
    }

    bool operator!=(const FwdIterator& rhs) const noexcept
    {
      return !(*this == rhs);
    }

  private:
    friend class FwdList< T >;

    FwdListNode< T >* node_;

    FwdIterator(FwdListNode< T >* node) noexcept:
      node_(node)
    {}
  };
}

#endif
