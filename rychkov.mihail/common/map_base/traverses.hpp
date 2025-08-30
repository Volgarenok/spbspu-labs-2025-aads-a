#ifndef MAP_BASE_TRAVERSES_HPP
#define MAP_BASE_TRAVERSES_HPP

#include "declaration.hpp"

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class Unary >
Unary rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::traverse_lnr(Unary f) const
{
  for (const_heavy_iterator i = cbegin(); i != cend(); ++i)
  {
    f(*i);
  }
  return f;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class Unary >
Unary rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::traverse_rnl(Unary f) const
{
  for (const_reverse_heavy_iterator i = crbegin(); i != crend(); ++i)
  {
    f(*i);
  }
  return f;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class Unary >
Unary rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::traverse_breadth(Unary f) const
{
  for (const_breadth_iterator i = cend(); i != const_breadth_iterator{}; ++i)
  {
    f(*i);
  }
  return f;
}

#endif
