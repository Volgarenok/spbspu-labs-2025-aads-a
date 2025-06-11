#ifndef COPY_MOVE_CONSTRUCT_BASE_HPP
#define COPY_MOVE_CONSTRUCT_BASE_HPP

#include "../bases.hpp"
#include <utility>

template< class... Types >
struct rychkov::details::CopyCtorBase< rychkov::details::TRIVIAL, Types... >:
  UnionStorageAlias< Types... >
{
  CopyCtorBase() = default;
  template< size_t N, class... Args >
  constexpr CopyCtorBase(in_place_index_t< N > i, Args&&... args):
    UnionStorageAlias< Types... >(i, std::forward< Args >(args)...)
  {}
};
template< class... Types >
struct rychkov::details::CopyCtorBase< rychkov::details::DELETED, Types... >:
  UnionStorageAlias< Types... >
{
  CopyCtorBase() = default;
  template< size_t N, class... Args >
  constexpr CopyCtorBase(in_place_index_t< N > i, Args&&... args):
    UnionStorageAlias< Types... >(i, std::forward< Args >(args)...)
  {}
  CopyCtorBase(const CopyCtorBase& rhs) = delete;
  CopyCtorBase(CopyCtorBase&& rhs) = default;
  CopyCtorBase& operator=(const CopyCtorBase& rhs) = delete;
  CopyCtorBase& operator=(CopyCtorBase&& rhs) = default;
};
template< class... Types >
struct rychkov::details::CopyCtorBase< rychkov::details::USER_DEFINED, Types... >:
  UnionStorageAlias< Types... >
{
  static constexpr bool is_nothrow_copy_ctor = variant_traits< Types... >::is_nothrow_copy_ctor;

  CopyCtorBase() = default;
  template< size_t N, class... Args >
  constexpr CopyCtorBase(in_place_index_t< N > i, Args&&... args):
    UnionStorageAlias< Types... >(i, std::forward< Args >(args)...)
  {}
  constexpr CopyCtorBase(const CopyCtorBase& rhs) noexcept(is_nothrow_copy_ctor)
  {
    if (!rhs.valueless_by_exception())
    {
      route_visit(Copier(), *this, rhs);
    }
  }
  CopyCtorBase(CopyCtorBase&& rhs) = default;
  CopyCtorBase& operator=(const CopyCtorBase& rhs) = default;
  CopyCtorBase& operator=(CopyCtorBase&& rhs) = default;
};

template< class... Types >
struct rychkov::details::MoveCtorBase< rychkov::details::TRIVIAL, Types... >:
  CopyCtorBaseAlias< Types... >
{
  MoveCtorBase() = default;
  template< size_t N, class... Args >
  constexpr MoveCtorBase(in_place_index_t< N > i, Args&&... args):
    CopyCtorBaseAlias< Types... >(i, std::forward< Args >(args)...)
  {}
};
template< class... Types >
struct rychkov::details::MoveCtorBase< rychkov::details::DELETED, Types... >:
  CopyCtorBaseAlias< Types... >
{
  MoveCtorBase() = default;
  MoveCtorBase(const MoveCtorBase& rhs) = default;
  MoveCtorBase(MoveCtorBase&& rhs) = delete;
  template< size_t N, class... Args >
  constexpr MoveCtorBase(in_place_index_t< N > i, Args&&... args):
    CopyCtorBaseAlias< Types... >(i, std::forward< Args >(args)...)
  {}
  MoveCtorBase& operator=(const MoveCtorBase& rhs) = default;
  MoveCtorBase& operator=(MoveCtorBase&& rhs) = delete;
};
template< class... Types >
struct rychkov::details::MoveCtorBase< rychkov::details::USER_DEFINED, Types... >:
  CopyCtorBaseAlias< Types... >
{
  static constexpr bool is_nothrow_move_ctor = variant_traits< Types... >::is_nothrow_move_ctor;

  MoveCtorBase() = default;
  constexpr MoveCtorBase(const MoveCtorBase& rhs) = default;
  constexpr MoveCtorBase(MoveCtorBase&& rhs) noexcept(is_nothrow_move_ctor)
  {
    if (!rhs.valueless_by_exception())
    {
      route_visit(Movier(), *this, std::move(rhs));
    }
  }
  template< size_t N, class... Args >
  constexpr MoveCtorBase(in_place_index_t< N > i, Args&&... args):
    CopyCtorBaseAlias< Types... >(i, std::forward< Args >(args)...)
  {}
  MoveCtorBase& operator=(const MoveCtorBase& rhs) = default;
  MoveCtorBase& operator=(MoveCtorBase&& rhs) = default;
};

#endif
