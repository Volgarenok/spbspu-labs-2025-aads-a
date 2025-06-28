#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <cstddef>
#include <functional.hpp>
#include <type_traits.hpp>
#include <type_tools.hpp>

namespace rychkov
{
  template< class... Types >
  class Variant;

  template< size_t N, class T >
  struct variant_alternative;
  template< size_t N, class... Types >
  struct variant_alternative< N, Variant< Types... > >
  {
    using type = nth_type_t< N, Types... >;
  };
  template< size_t N, class T >
  using variant_alternative_t = typename variant_alternative< N, T >::type;

  template< class T >
  struct variant_size;
  template< class... Types >
  struct variant_size< Variant< Types... > >: std::integral_constant< size_t, sizeof...(Types) >
  {};
  template< class T >
  constexpr size_t variant_size_v = variant_size< T >::value;

  namespace details
  {
    template< class T, class... Types >
    struct find_unique: std::integral_constant< size_t, 0 >
    {};
    template< class T, class U, class... Types >
    struct find_unique< T, U, Types... >
    {
      static constexpr bool is_same = std::is_same< T, U >::value;
      static constexpr size_t post = find_unique< T, Types... >::value;
      static constexpr size_t value = !is_same ? 1 + post : (post == sizeof...(Types) ? 0 : 1 + sizeof...(Types));
    };
  }
  template< class U, class... Types >
  constexpr size_t find_unique_v = details::find_unique< U, Types... >::value;
  template< class T, class... Types >
  constexpr bool exactly_once = find_unique_v< T, Types... > < sizeof...(Types);

  template< class T >
  struct in_place_type_t
  {};
  template< size_t N >
  struct in_place_index_t
  {};
  constexpr size_t variant_npos = -1;

  template< class T >
  struct is_in_place_tag: std::false_type
  {};
  template< class T >
  struct is_in_place_tag< in_place_type_t< T > >: std::true_type
  {};
  template< size_t N >
  struct is_in_place_tag< in_place_index_t< N > >: std::true_type
  {};

  namespace details
  {
    template< class T >
    struct ArrConstructHelper
    {
      T value[1];
    };
    template< class Dest, class Src, class = void >
    struct is_inarray_constructible: std::false_type
    {};
    template< class Dest, class Src >
    struct is_inarray_constructible< Dest, Src,
          void_t< decltype(ArrConstructHelper< Dest >{{std::declval< Src >()}}) > >: std::true_type
    {};
    template< class Dest, class Src >
    constexpr bool is_inarray_constructible_v = is_inarray_constructible< Dest, Src >::value;

    template< size_t N, class Dest, class Src, class = void >
    struct overload
    {
      void resolve() = delete;
    };
    template< size_t N, class Dest, class Src >
    struct overload< N, Dest, Src, std::enable_if_t< is_inarray_constructible_v< Dest, Src >, void > >
    {
      static std::integral_constant< size_t, N > resolve(Dest);
    };
    template< class T, class U, class... Types >
    struct resolve_overload: overload< 1 + sizeof...(Types), U, T >, resolve_overload< T, Types... >
    {
      using overload< 1 + sizeof...(Types), U, T >::resolve;
      using resolve_overload< T, Types... >::resolve;
    };
    template< class T, class U >
    struct resolve_overload< T, U >: overload< 1, U, T >
    {
      using overload< 1, U, T >::resolve;
    };
    template< class AlwaysVoid, class T, class... Types >
    struct resolve_overloaded_construct: std::integral_constant< size_t, 0 >
    {};
    template< class T, class... Types >
    struct resolve_overloaded_construct
        < void_t< decltype(resolve_overload< T, Types... >::resolve(std::declval< T >())) >, T, Types... >:
      decltype(resolve_overload< T, Types... >::resolve(std::declval< T >()))
    {};
  }
  template< class T, class... Types >
  constexpr size_t resolve_overloaded_construct_v = sizeof...(Types)
      - details::resolve_overloaded_construct< void, T, Types... >::value;

  template< class R, class F, class First, class... Variants >
  constexpr R visit(F&& func, First&& first, Variants&&... args);
  template< class R, class F >
  constexpr R visit(F&& func);
  template< class F, class... Variants >
  constexpr invoke_result_t< F, variant_alternative_t< 0, remove_cvref_t< Variants > >... >
      visit(F&& func, Variants&&... args);
}

#endif
