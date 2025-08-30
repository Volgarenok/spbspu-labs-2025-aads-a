#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_INTERNAL_FORWARD_LIST_ITERATOR_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_INTERNAL_FORWARD_LIST_ITERATOR_HPP

#include <cassert>
#include <iterator>
#include <type_traits>
#include "forward-list-fwd.hpp"
#include "forward-list-node.hpp"

namespace kizhin {
  namespace detail {
    template < typename T, bool IsConst >
    class ForwardListIterator final
    {
    public:
      using value_type = T;
      using difference_type = std::ptrdiff_t;
      using pointer = std::conditional_t< IsConst, const value_type*, value_type* >;
      using reference = std::conditional_t< IsConst, const value_type&, value_type& >;
      using iterator_category = std::forward_iterator_tag;

      ForwardListIterator() = default;
      template < bool IsRhsConst, std::enable_if_t< IsConst && !IsRhsConst, int > = 0 >
      ForwardListIterator(const ForwardListIterator< T, IsRhsConst >&) noexcept;

      pointer operator->() const noexcept;
      reference operator*() const noexcept;

      ForwardListIterator& operator++() noexcept;
      ForwardListIterator operator++(int) noexcept;

    private:
      using Node = detail::Node< value_type >;

      Node* node_ = nullptr;

      explicit ForwardListIterator(Node* node):
        node_(node)
      {}

      friend class ForwardListIterator< T, !IsConst >;
      friend class ::kizhin::ForwardList< value_type >;
      template < typename U, bool IsLhsConst, bool IsRhsConst >
      friend bool operator==(const ForwardListIterator< U, IsLhsConst >&,
          const ForwardListIterator< U, IsRhsConst >&) noexcept;
    };

    template < typename T, bool IsConst >
    template < bool IsRhsConst, std::enable_if_t< IsConst && !IsRhsConst, int > >
    ForwardListIterator< T, IsConst >::ForwardListIterator(
        const ForwardListIterator< T, IsRhsConst >& rhs) noexcept:
      node_(rhs.node_)
    {}

    template < typename T, bool IsConst >
    typename ForwardListIterator< T, IsConst >::pointer ForwardListIterator< T,
        IsConst >::operator->() const noexcept
    {
      assert(node_ && "ForwardListIterator: dereferencing empty iterator (operator->)");
      return std::addressof(node_->data);
    }

    template < typename T, bool IsConst >
    typename ForwardListIterator< T, IsConst >::reference ForwardListIterator< T,
        IsConst >::operator*() const noexcept
    {
      assert(node_ && "ForwardListIterator: dereferencing empty iterator (operator*)");
      return node_->data;
    }

    template < typename T, bool IsConst >
    ForwardListIterator< T, IsConst >& ForwardListIterator< T,
        IsConst >::operator++() noexcept
    {
      assert(node_ && "ForwardListIterator: incrementing empty iterator");
      node_ = node_->next;
      return *this;
    }

    template < typename T, bool IsConst >
    ForwardListIterator< T, IsConst > ForwardListIterator< T, IsConst >::operator++(
        int) noexcept
    {
      ForwardListIterator tmp(*this);
      ++(*this);
      return tmp;
    }

    template < typename T, bool IsLhsConst, bool IsRhsConst >
    bool operator==(const ForwardListIterator< T, IsLhsConst >& lhs,
        const ForwardListIterator< T, IsRhsConst >& rhs) noexcept
    {
      return lhs.node_ == rhs.node_;
    }

    template < typename T, bool IsLhsConst, bool IsRhsConst >
    bool operator!=(const ForwardListIterator< T, IsLhsConst >& lhs,
        const ForwardListIterator< T, IsRhsConst >& rhs) noexcept
    {
      return !(lhs == rhs);
    }
  }
}

#endif

