#ifndef TRAITS_HPP
#define TRAITS_HPP

#include <type_traits.hpp>

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
  namespace details
  {
    template< class T >
    constexpr size_t max_size_v = ((1ULL << (8 * sizeof(T) - 1)) - 1) * 2 + 1;

    template< size_t N, class PossibleSize_t, class... Ts >
    struct select_size_type
    {
      using type = std::conditional_t< (N < max_size_v< PossibleSize_t >),
            PossibleSize_t, typename select_size_type< N, Ts... >::type >;
    };
    template< size_t N, class PossibleSize_t >
    struct select_size_type< N, PossibleSize_t >
    {
      static_assert(N < max_size_v< PossibleSize_t >, "size type deduction failed");
      using type = PossibleSize_t;
    };
    template< size_t N, class... Ts >
    using select_size_type_t = typename select_size_type< N, Ts... >::type;
  }
}

#endif
