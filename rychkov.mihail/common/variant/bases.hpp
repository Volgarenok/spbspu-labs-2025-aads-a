#ifndef BASES_HPP
#define BASES_HPP

#include <utility>
#include "tools.hpp"
#include "traits.hpp"

namespace rychkov
{
  namespace details
  {
    enum EnableMethod
    {
      DELETED,
      USER_DEFINED,
      TRIVIAL
    };
    constexpr EnableMethod enable(bool isTrivial, bool isViable)
    {
      return isTrivial ? TRIVIAL : (isViable ? USER_DEFINED : DELETED);
    }

    template< bool IsTrivDestr, class... Types >
    union UnionBase;
    template< class T, class... Types >
    union UnionBase< true, T, Types... >;
    template< class T, class... Types >
    union UnionBase< false, T, Types... >;
    template< bool IsTrivDestr >
    union UnionBase< IsTrivDestr >;

    template< size_t N, bool IsTrivDestr, class T, class... Types >
    constexpr std::enable_if_t< N == 0, T& >
        get(details::UnionBase< IsTrivDestr, T, Types... >& base);
    template< size_t N, bool IsTrivDestr, class T, class... Types >
    constexpr std::enable_if_t< N == 0, const T& >
        get(const details::UnionBase< IsTrivDestr, T, Types... >& base);
    template< size_t N, bool IsTrivDestr, class T, class... Types >
    constexpr std::enable_if_t< N == 0, T&& >
        get(details::UnionBase< IsTrivDestr, T, Types... >&& base);
    template< size_t N, bool IsTrivDestr, class T, class... Types >
    constexpr std::enable_if_t< N == 0, const T&& >
        get(const details::UnionBase< IsTrivDestr, T, Types... >&& base);
    template< size_t N, bool IsTrivDestr, class T, class... Types >
    constexpr std::enable_if_t< N != 0, nth_type_t< N - 1, Types... >& >
        get(details::UnionBase< IsTrivDestr, T, Types... >& base);
    template< size_t N, bool IsTrivDestr, class T, class... Types >
    constexpr std::enable_if_t< N != 0, const nth_type_t< N - 1, Types... >& >
        get(const details::UnionBase< IsTrivDestr, T, Types... >& base);
    template< size_t N, bool IsTrivDestr, class T, class... Types >
    constexpr std::enable_if_t< N != 0, nth_type_t< N - 1, Types... >&& >
        get(details::UnionBase< IsTrivDestr, T, Types... >&& base);
    template< size_t N, bool IsTrivDestr, class T, class... Types >
    constexpr std::enable_if_t< N != 0, const nth_type_t< N - 1, Types... >&& >
        get(const details::UnionBase< IsTrivDestr, T, Types... >&& base);

    template< bool isTrivDestr, class... Types >
    struct UnionStorage;
    template< class... Types >
    struct UnionStorage< false, Types... >;
    template< class... Types >
    struct UnionStorage< true, Types... >;
    template< class... Types >
    using UnionStorageAlias = UnionStorage< variant_traits< Types... >::is_trivially_destructible, Types... >;

    template< size_t N, class... Types >
    constexpr nth_type_t< N, Types... >& get(details::UnionStorageAlias< Types... >& base);
    template< size_t N, class... Types >
    constexpr const nth_type_t< N, Types... >& get(const details::UnionStorageAlias< Types... >& base);
    template< size_t N, class... Types >
    constexpr nth_type_t< N, Types... >&& get(details::UnionStorageAlias< Types... >&& base);
    template< size_t N, class... Types >
    constexpr const nth_type_t< N, Types... >&& get(const details::UnionStorageAlias< Types... >&& base);

    template< class Visitor, class... Types, size_t... Indexes >
    constexpr bool is_nothrow_route_visit(std::index_sequence< Indexes... >);
    template< class Visitor, class... Types >
    constexpr bool is_nothrow_route_visit_v =
        is_nothrow_route_visit< Visitor&&, Types... >(std::make_index_sequence< sizeof...(Types) >());
    template< class... Types, class Visitor, class SrcUnion >
    constexpr void route_visit(Visitor&& visitor, UnionStorageAlias< Types... >& dest, SrcUnion&& src)
        noexcept(is_nothrow_route_visit_v< Visitor&&, Types... >);

    template< EnableMethod, class... Types >
    struct CopyCtorBase;
    template< class... Types >
    struct CopyCtorBase< TRIVIAL, Types... >;
    template< class... Types >
    struct CopyCtorBase< DELETED, Types... >;
    template< class... Types >
    struct CopyCtorBase< USER_DEFINED, Types... >;
    template< class... Types >
    using CopyCtorBaseAlias = CopyCtorBase< enable(variant_traits< Types... >::is_triv_copy_ctor,
            variant_traits< Types... >::is_copy_ctor), Types... >;

    template< EnableMethod, class... Types >
    struct MoveCtorBase;
    template< class... Types >
    struct MoveCtorBase< TRIVIAL, Types... >;
    template< class... Types >
    struct MoveCtorBase< DELETED, Types... >;
    template< class... Types >
    struct MoveCtorBase< USER_DEFINED, Types... >;
    template< class... Types >
    using MoveCtorBaseAlias = MoveCtorBase< enable(variant_traits< Types... >::is_triv_move_ctor,
            variant_traits< Types... >::is_move_ctor), Types... >;

    template< EnableMethod, class... Types >
    struct CopyAssignBase;
    template< class... Types >
    struct CopyAssignBase< TRIVIAL, Types... >;
    template< class... Types >
    struct CopyAssignBase< DELETED, Types... >;
    template< class... Types >
    struct CopyAssignBase< USER_DEFINED, Types... >;
    template< class... Types >
    using CopyAssignBaseAlias = CopyAssignBase< enable(variant_traits< Types... >::is_triv_copy_assign,
            variant_traits< Types... >::is_copy_assign), Types... >;

    template< EnableMethod, class... Types >
    struct MoveAssignBase;
    template< class... Types >
    struct MoveAssignBase< TRIVIAL, Types... >;
    template< class... Types >
    struct MoveAssignBase< DELETED, Types... >;
    template< class... Types >
    struct MoveAssignBase< USER_DEFINED, Types... >;
    template< class... Types >
    using MoveAssignBaseAlias = MoveAssignBase< enable(variant_traits< Types... >::is_triv_move_assign,
            variant_traits< Types... >::is_move_assign), Types... >;

    template< bool is_default_constructible, class... Types >
    struct VariantBase;
    template< class... Types >
    struct VariantBase< false, Types... >;
    template< class... Types >
    struct VariantBase< true, Types... >;
    template< class... Types >
    using VariantBaseAlias = VariantBase< variant_traits< Types... >::is_def_ctor, Types... >;

    struct Destroyer;
    struct Copier;
    struct Movier;
    struct CopyAssigner;
    struct MoveAssigner;
  }
  template< class... Types >
  struct variant_size< details::UnionStorageAlias< Types... > >;
  template< size_t N, class... Types >
  struct variant_alternative< N, details::UnionStorageAlias< Types... > >;
}

#endif
