#ifndef UNORDERED_MAP_HPP
#define UNORDERED_MAP_HPP

#include <functional>
#include "unordered_base.hpp"

namespace rychkov
{
  template< class K, class T, class H = Hash<>, class E = std::equal_to<> >
  using UnorderedMap = UnorderedBase< K, T, H, E, false, false >;
  template< class K, class T, class H = Hash<>, class E = std::equal_to<> >
  using UnorderedMultiMap = UnorderedBase< K, T, H, E, false, true >;
}

#endif
