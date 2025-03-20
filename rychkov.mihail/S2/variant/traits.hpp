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
        std::is_default_constructible< variant_alternative_t< 0, Types... > >::value;
    static constexpr bool is_copy_ctor =
        conjunction_v< std::is_copy_constructible< Types >::value... >;
    static constexpr bool is_move_ctor =
        conjunction_v< std::is_move_constructible< Types >::value... >;
    static constexpr bool is_copy_assign = is_copy_ctor
        && conjunction_v< std::is_copy_assignable< Types >::value... >;
    static constexpr bool is_move_assign = is_move_ctor
        && conjunction_v< std::is_copy_assignable< Types >::value... >;
    static constexpr bool is_destructible =
        conjunction_v< std::is_default_constructible< Types >::value... >;

    static constexpr bool is_nothrow_def_ctor =
        std::is_nothrow_default_constructible< variant_alternative_t< 0, Types... > >::value;
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
    static constexpr bool is_triv_copy_assign =
        conjunction_v< std::is_trivially_copy_assignable< Types >::value... >;
    static constexpr bool is_triv_move_assign =
        conjunction_v< std::is_trivially_move_assignable< Types >::value... >;
    static constexpr bool is_trivially_destructible =
        conjunction_v< std::is_trivially_destructible< Types >::value... >;
  };
  namespace details
  {
    template< size_t N, class PossibleSize_t, class... Ts >
    struct select_size_type
    {
      using type = std::conditional_t< (N < 256 * sizeof(PossibleSize_t) - 1),
            PossibleSize_t, typename select_size_type< N, Ts... >::type >;
    };
    template< size_t N, class PossibleSize_t >
    struct select_size_type< N, PossibleSize_t >
    {
      using type = PossibleSize_t;
    };
    template< size_t N, class... Ts >
    using select_size_type_t = typename select_size_type< N, Ts... >::type;

    enum EnableMethod: byte
    {
      DELETED,
      USER_DEFINED,
      TRIVIAL
    };
    constexpr EnableMethod enable(bool isTrivial, bool isViable)
    {
      return (isTrivial ? TRIVIAL : (isViable ? USER_DEFINED : DELETED));
    }
  }
}

#endif
