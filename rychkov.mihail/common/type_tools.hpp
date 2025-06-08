#ifndef TYPE_TOOLS_HPP
#define TYPE_TOOLS_HPP

#include <cstddef>
#include <limits>
#include <type_traits.hpp>

namespace rychkov
{
  namespace details
  {
    template< size_t N, class PossibleSize_t, class... Ts >
    struct select_size_type
    {
      using type = std::conditional_t< (N < std::numeric_limits< PossibleSize_t >::max()),
            PossibleSize_t, typename select_size_type< N, Ts... >::type >;
    };
    template< size_t N, class PossibleSize_t >
    struct select_size_type< N, PossibleSize_t >
    {
      static_assert(N < std::numeric_limits< PossibleSize_t >::max(), "size type deduction failed");
      using type = PossibleSize_t;
    };
    template< size_t N, class... Ts >
    using select_size_type_t = typename select_size_type< N, Ts... >::type;
  }
  template< size_t N >
  using select_size_type_t = typename details::select_size_type< N, unsigned char, unsigned short, size_t >::type;

  namespace details
  {
    template< class AlwaysVoid, size_t N, class... Types >
    struct nth_type
    {};
    template< size_t N, class T, class... Types >
    struct nth_type< std::enable_if_t< (N != 0) && (N <= sizeof...(Types)), void >, N, T, Types... >
    {
      using type = typename nth_type< void, N - 1, Types... >::type;
    };
    template< class T, class... Types >
    struct nth_type< void, 0, T, Types... >
    {
      using type = T;
    };
  }
  template< size_t N, class... Types >
  using nth_type_t = typename details::nth_type< void, N, Types... >::type;

  template< class T, size_t... Lens >
  struct multidimensional_array;
  template< class T >
  struct multidimensional_array< T >
  {
    T data;
    constexpr T operator()() const
    {
      return data;
    }
  };
  template< class T, size_t Len, size_t... Lens >
  struct multidimensional_array< T, Len, Lens... >
  {
    multidimensional_array< T, Lens... > data[Len];
    template< class... Sizes >
    constexpr T operator()(size_t i, Sizes... sizes) const
    {
      return data[i](sizes...);
    }
  };
}

#endif
