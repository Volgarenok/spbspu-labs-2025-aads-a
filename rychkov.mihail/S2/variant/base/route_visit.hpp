#ifndef ROUTE_VISIT_HPP
#define ROUTE_VISIT_HPP

#include "../bases.hpp"
#include <utility>

namespace rychkov
{
  namespace details
  {
    template< size_t N, class Visitor, class SrcUnion, class... Types >
    constexpr void invoke_route_visit(Visitor&& visitor, UnionStorageAlias< Types... >& dest, SrcUnion&& src)
        noexcept(is_nothrow_route_visit_v< Visitor&&, Types... >)
    {
      invoke(std::forward< Visitor >(visitor), dest, std::forward< SrcUnion >(src), in_place_index_t< N >());
    }
    template< class Visitor, class SrcUnion, class... Types >
    using route_vtable_t = multidimensional_array< void(*)(Visitor, UnionStorageAlias< Types... >&, SrcUnion),
          sizeof...(Types) >;
    template< class Visitor, class SrcUnion, class... Types, size_t... Indexes >
    constexpr route_vtable_t< Visitor, SrcUnion, Types... >
        generate_route_visit_table(std::index_sequence< Indexes... >)
    {
      return {{ {&invoke_route_visit< Indexes, Visitor, SrcUnion, Types... >}... }};
    }
    template< class Visitor, class SrcUnion, class... Types >
    constexpr route_vtable_t< Visitor, SrcUnion, Types... > route_vtable =
        generate_route_visit_table< Visitor, SrcUnion, Types... >(std::make_index_sequence< sizeof...(Types) >());
  }
}
template< class Visitor, class... Types, size_t... Indexes >
constexpr bool rychkov::details::is_nothrow_route_visit(std::index_sequence< Indexes... >)
{
  return conjunction_v< is_nothrow_invocable_v< Visitor&&, UnionStorageAlias< Types... >&,
        Types, in_place_index_t< Indexes > >... >;
}
template< class... Types, class Visitor, class SrcUnion >
constexpr void rychkov::details::route_visit(Visitor&& visitor, UnionStorageAlias< Types... >& dest, SrcUnion&& src)
    noexcept(is_nothrow_route_visit_v< Visitor&&, Types... >)
{
  route_vtable< Visitor&&, SrcUnion&&, Types... >(src.index())(std::forward< Visitor >(visitor),
        dest, std::forward< SrcUnion >(src));
}

#endif

