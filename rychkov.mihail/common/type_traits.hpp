#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <type_traits>

namespace rychkov
{
  template< bool Logic1, bool... Logics >
  struct conjunction: std::integral_constant< bool, Logic1 && conjunction< Logics... >::value >
  {};
  template< bool Logic1 >
  struct conjunction< Logic1 >: std::integral_constant< bool, Logic1 >
  {};
  template< bool... Logics >
  constexpr bool conjunction_v = conjunction< Logics... >::value;

  template< class T >
  using remove_cvref_t = std::remove_cv_t< std::remove_reference_t< T > >;

  template< class... T >
  using void_t = void;

  template< class T >
  struct is_nothrow_swappable: std::integral_constant< bool, noexcept(std::swap(std::declval< T& >(),
              std::declval< T& >())) >
  {};
  template< class T >
  constexpr bool is_nothrow_swappable_v = is_nothrow_swappable< T >::value;
}

#endif
