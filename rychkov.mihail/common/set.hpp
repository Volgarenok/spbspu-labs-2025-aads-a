#ifndef SET_HPP
#define SET_HPP

#include <functional>
#include "map_base.hpp"

namespace rychkov
{
  template< class K, class C = std::less<>, size_t N = 2 >
  using Set = MapBase< K, K, C, N, true, false >;
  template< class K, class C = std::less<>, size_t N = 2 >
  using MultiSet = MapBase< K, K, C, N, true, true >;
}

#endif
