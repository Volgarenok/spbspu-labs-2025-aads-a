#ifndef VISITOR_HELPERS_HPP
#define VISITOR_HELPERS_HPP

#include "../bases.hpp"
#include <utility>

struct rychkov::details::Destroyer
{
  template< class T >
  void operator()(T&& value) noexcept
  {
    value.~remove_cvref_t< T >();
  }
};
struct rychkov::details::Copier
{
  template< size_t N, class... Types >
  void operator()(UnionStorageAlias< Types... >& dest, const UnionStorageAlias< Types... >& src,
        in_place_index_t< N >) noexcept(variant_traits< Types... >::is_nothrow_copy_ctor)
  {
    dest.template emplace< N >(get< N >(src));
  }
};
struct rychkov::details::Movier
{
  template< size_t N, class... Types >
  constexpr void operator()(UnionStorageAlias< Types... >& dest, UnionStorageAlias< Types... >&& src,
        in_place_index_t< N >) noexcept(variant_traits< Types... >::is_nothrow_move_ctor)
  {
    dest.template emplace< N >(get< N >(std::move(src)));
  }
};
struct rychkov::details::CopyAssigner
{
  template< size_t N, class... Types >
  constexpr void operator()(UnionStorageAlias< Types... >& dest, const UnionStorageAlias< Types... >& src,
        in_place_index_t< N >) noexcept(variant_traits< Types... >::is_nothrow_copy_assign)
  {
    if (dest.index() != src.index())
    {
      dest.template emplace< N >(get< N >(src));
    }
    else
    {
      get< N >(dest) = get< N >(src);
    }
  }
};
struct rychkov::details::MoveAssigner
{
  template< size_t N, class... Types >
  constexpr void operator()(UnionStorageAlias< Types... >& dest, UnionStorageAlias< Types... >&& src,
        in_place_index_t< N >) noexcept(variant_traits< Types... >::is_nothrow_move_assign)
  {
    if (dest.index() != src.index())
    {
      dest.template emplace< N >(get< N >(std::move(src)));
    }
    else
    {
      get< N >(dest) = get< N >(std::move(src));
    }
  }
};

#endif

