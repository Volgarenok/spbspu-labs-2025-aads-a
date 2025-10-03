#ifndef COPY_MOVE_ASSIGN_BASE_HPP
#define COPY_MOVE_ASSIGN_BASE_HPP

#include "../bases.hpp"
#include <utility>

template< class... Types >
struct rychkov::details::CopyAssignBase< rychkov::details::TRIVIAL, Types... >:
  MoveCtorBaseAlias< Types... >
{
  CopyAssignBase() = default;
  template< size_t N, class... Args >
  constexpr CopyAssignBase(in_place_index_t< N > i, Args&&... args):
    MoveCtorBaseAlias< Types... >(i, std::forward< Args >(args)...)
  {}
};
template< class... Types >
struct rychkov::details::CopyAssignBase< rychkov::details::DELETED, Types... >:
  MoveCtorBaseAlias< Types... >
{
  CopyAssignBase() = default;
  CopyAssignBase(const CopyAssignBase& rhs) = default;
  CopyAssignBase(CopyAssignBase&& rhs) = default;
  template< size_t N, class... Args >
  constexpr CopyAssignBase(in_place_index_t< N > i, Args&&... args):
    MoveCtorBaseAlias< Types... >(i, std::forward< Args >(args)...)
  {}
  CopyAssignBase& operator=(const CopyAssignBase& rhs) = delete;
  CopyAssignBase& operator=(CopyAssignBase&& rhs) = default;
};
template< class... Types >
struct rychkov::details::CopyAssignBase< rychkov::details::USER_DEFINED, Types... >:
  MoveCtorBaseAlias< Types... >
{
  static constexpr bool is_nothrow_copy_assign = variant_traits< Types... >::is_nothrow_copy_assign;

  CopyAssignBase() = default;
  CopyAssignBase(const CopyAssignBase& rhs) = default;
  CopyAssignBase(CopyAssignBase&& rhs) = default;
  template< size_t N, class... Args >
  constexpr CopyAssignBase(in_place_index_t< N > i, Args&&... args):
    MoveCtorBaseAlias< Types... >(i, std::forward< Args >(args)...)
  {}
  CopyAssignBase& operator=(const CopyAssignBase& rhs) noexcept(is_nothrow_copy_assign)
  {
    if (!this->valueless_by_exception() && (this->active != rhs.active))
    {
      this->destroy();
    }
    if (!rhs.valueless_by_exception())
    {
      route_visit(CopyAssigner(), *this, rhs);
    }
    return *this;
  }
  CopyAssignBase& operator=(CopyAssignBase&& rhs) = default;
};

template< class... Types >
struct rychkov::details::MoveAssignBase< rychkov::details::TRIVIAL, Types... >:
  CopyAssignBaseAlias< Types... >
{
  MoveAssignBase() = default;
  template< size_t N, class... Args >
  constexpr MoveAssignBase(in_place_index_t< N > i, Args&&... args):
    CopyAssignBaseAlias< Types... >(i, std::forward< Args >(args)...)
  {}
};
template< class... Types >
struct rychkov::details::MoveAssignBase< rychkov::details::DELETED, Types... >:
  CopyAssignBaseAlias< Types... >
{
  MoveAssignBase() = default;
  MoveAssignBase(const MoveAssignBase& rhs) = default;
  MoveAssignBase(MoveAssignBase&& rhs) = default;
  template< size_t N, class... Args >
  constexpr MoveAssignBase(in_place_index_t< N > i, Args&&... args):
    CopyAssignBaseAlias< Types... >(i, std::forward< Args >(args)...)
  {}
  MoveAssignBase& operator=(const MoveAssignBase& rhs) = default;
  MoveAssignBase& operator=(MoveAssignBase&& rhs) = delete;
};
template< class... Types >
struct rychkov::details::MoveAssignBase< rychkov::details::USER_DEFINED, Types... >:
  CopyAssignBaseAlias< Types... >
{
  static constexpr bool is_nothrow_move_assign = variant_traits< Types... >::is_nothrow_move_assign;

  MoveAssignBase() = default;
  MoveAssignBase(const MoveAssignBase& rhs) = default;
  MoveAssignBase(MoveAssignBase&& rhs) = default;
  template< size_t N, class... Args >
  constexpr MoveAssignBase(in_place_index_t< N > i, Args&&... args):
    CopyAssignBaseAlias< Types... >(i, std::forward< Args >(args)...)
  {}
  MoveAssignBase& operator=(const MoveAssignBase& rhs) = default;
  MoveAssignBase& operator=(MoveAssignBase&& rhs) noexcept(is_nothrow_move_assign)
  {
    if (!this->valueless_by_exception() && (this->active != rhs.active))
    {
      this->destroy();
    }
    if (!rhs.valueless_by_exception())
    {
      route_visit(MoveAssigner(), *this, std::move(rhs));
    }
    return *this;
  }
};

#endif
