#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_ENABLE_IF_INPUT_ITERATOR
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_ENABLE_IF_INPUT_ITERATOR

#include <iterator>
#include <type_traits>

namespace kizhin {
  namespace detail {
    template < typename... >
    using void_t = void;

    template < typename T, typename = void >
    struct is_input_iterator: std::false_type
    {};

    template < typename T >
    using iter_category = typename std::iterator_traits< T >::iterator_category;

    template < typename T >
    struct is_input_iterator< T, void_t< iter_category< T > > >:
      std::is_base_of< std::input_iterator_tag, iter_category< T > >
    {};

    template < typename T >
    constexpr bool is_input_iterator_v = is_input_iterator< T >::value;

    template < typename T >
    using enable_if_input_iterator = std::enable_if_t< is_input_iterator_v< T >, int >;
  }
}

#endif

