#ifndef UNORDERED_SET_HPP
#define UNORDERED_SET_HPP

#include <functional>
#include "unordered_base.hpp"

namespace rychkov
{
  template< class K, class H = Hash<>, class E = std::equal_to<> >
  using UnorderedSet = UnorderedBase< K, K, H, E, true, false >;
  template< class K, class H = Hash<>, class E = std::equal_to<> >
  using UnorderedMultiSet = UnorderedBase< K, K, H, E, true, true >;
}

#endif
