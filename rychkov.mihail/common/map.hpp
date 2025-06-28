#ifndef MAP_HPP
#define MAP_HPP

#include <functional>
#include "map_base.hpp"

namespace rychkov
{
  template< class K, class T, class C = std::less<>, size_t N = 2 >
  using Map = MapBase< K, T, C, N, false, false >;
  template< class K, class T, class C = std::less<>, size_t N = 2 >
  using MultiMap = MapBase< K, T, C, N, false, true >;
}

#endif
