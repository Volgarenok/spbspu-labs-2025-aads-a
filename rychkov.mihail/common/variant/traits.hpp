#ifndef TRAITS_HPP
#define TRAITS_HPP

#include <type_traits.hpp>
#include <type_tools.hpp>

namespace rychkov
{
  template< class... Types >
  struct variant_traits
  {
  public:
    static constexpr bool is_def_ctor =
        std::is_default_constructible< nth_type_t< 0, Types... > >::value;
    static constexpr bool is_copy_ctor =
        conjunction_v< std::is_copy_constructible< Types >::value... >;
    static constexpr bool is_move_ctor =
        conjunction_v< std::is_move_constructible< Types >::value... >;
    static constexpr bool is_copy_assign = is_copy_ctor
        && conjunction_v< std::is_copy_assignable< Types >::value... >;
    static constexpr bool is_move_assign = is_move_ctor
        && conjunction_v< std::is_move_assignable< Types >::value... >;

    static constexpr bool is_nothrow_def_ctor =
        std::is_nothrow_default_constructible< nth_type_t< 0, Types... > >::value;
    static constexpr bool is_nothrow_copy_ctor =
        conjunction_v< std::is_nothrow_copy_constructible< Types >::value... >;
    static constexpr bool is_nothrow_move_ctor =
        conjunction_v< std::is_nothrow_move_constructible< Types >::value... >;
    static constexpr bool is_nothrow_copy_assign = is_nothrow_copy_ctor
        && conjunction_v< std::is_nothrow_copy_assignable< Types >::value... >;
    static constexpr bool is_nothrow_move_assign = is_nothrow_move_ctor
        && conjunction_v< std::is_nothrow_move_assignable< Types >::value... >;

    static constexpr bool is_triv_copy_ctor =
        conjunction_v< std::is_trivially_copy_constructible< Types >::value... >;
    static constexpr bool is_triv_move_ctor =
        conjunction_v< std::is_trivially_move_constructible< Types >::value... >;
    static constexpr bool is_triv_copy_assign = is_triv_copy_ctor
        && conjunction_v< std::is_trivially_copy_assignable< Types >::value... >;
    static constexpr bool is_triv_move_assign = is_triv_move_ctor
        && conjunction_v< std::is_trivially_move_assignable< Types >::value... >;
    static constexpr bool is_trivially_destructible =
        conjunction_v< std::is_trivially_destructible< Types >::value... >;
  };
}

#endif
