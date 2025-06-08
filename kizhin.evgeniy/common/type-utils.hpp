#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_TYPE_UTILS_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_TYPE_UTILS_HPP

#include <iterator>
#include <type_traits>

namespace kizhin {
  template < bool B >
  using bool_constant = std::integral_constant< bool, B >;

  template < typename... >
  using void_t = void;

  namespace detail {
    template < typename, typename = void >
    struct is_input_iterator: std::false_type
    {};

    template < typename T >
    using iter_category = typename std::iterator_traits< T >::iterator_category;

    template < typename T >
    struct is_input_iterator< T, void_t< iter_category< T > > >:
      std::is_base_of< std::input_iterator_tag, iter_category< T > >
    {};
  }

  template < typename T >
  constexpr bool is_input_iterator_v = detail::is_input_iterator< T >::value;

  template < typename T >
  using enable_if_input_iterator = std::enable_if_t< is_input_iterator_v< T >, int >;

  namespace detail {
    template < typename, typename = void >
    struct is_nothrow_default_constructible: std::false_type
    {};

    template < typename T >
    struct is_nothrow_default_constructible< T, void_t< decltype(T()) > >:
      bool_constant< noexcept(T()) >
    {};
  }

  template < typename T >
  constexpr bool is_nothrow_default_constructible_v =
      detail::is_nothrow_default_constructible< T >::value;

  namespace detail {
    template < typename, typename = void >
    struct has_adl_swap: std::false_type
    {};

    template < typename T >
    struct has_adl_swap< T,
        void_t< decltype(swap(std::declval< T& >(), std::declval< T& >())) > >:
      std::true_type
    {};

    template < typename T >
    constexpr bool has_adl_swap_v = has_adl_swap< T >::value;

    template < typename, typename = void >
    struct is_adl_swap_noexcept: std::false_type
    {};

    template < typename T >
    struct is_adl_swap_noexcept< T, std::enable_if_t< has_adl_swap_v< T > > >:
      bool_constant< noexcept(swap(std::declval< T& >(), std::declval< T& >())) >
    {};

    template < typename T >
    constexpr bool is_adl_swap_noexcept_v = is_adl_swap_noexcept< T >::value;

    template < typename T >
    constexpr bool is_std_swap_noexcept =
        noexcept(std::swap(std::declval< T& >(), std::declval< T& >()));

    template < typename T >
    constexpr bool use_std_swap = !has_adl_swap_v< T > && is_std_swap_noexcept< T >;

    template < typename T >
    constexpr bool use_adl_swap =
        is_adl_swap_noexcept_v< T > && is_std_swap_noexcept< T >;

    template < typename T >
    constexpr bool is_nothrow_swappable_v = use_std_swap< T > || use_adl_swap< T >;
  }

  template < typename T >
  constexpr bool is_nothrow_swappable_v = detail::is_nothrow_swappable_v< T >;
}

#endif

