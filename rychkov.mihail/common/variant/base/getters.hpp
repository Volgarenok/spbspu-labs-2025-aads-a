#ifndef GETTERS_HPP
#define GETTERS_HPP

#include "../bases.hpp"
#include <utility>

template< size_t N, bool IsTrivDestr, class T, class... Types >
constexpr std::enable_if_t< N == 0, T& >
    rychkov::details::get(details::UnionBase< IsTrivDestr, T, Types... >& base)
{
  return base.current;
}
template< size_t N, bool IsTrivDestr, class T, class... Types >
constexpr std::enable_if_t< N == 0, const T& >
    rychkov::details::get(const details::UnionBase< IsTrivDestr, T, Types... >& base)
{
  return base.current;
}
template< size_t N, bool IsTrivDestr, class T, class... Types >
constexpr std::enable_if_t< N == 0, T&& >
    rychkov::details::get(details::UnionBase< IsTrivDestr, T, Types... >&& base)
{
  return std::move(base.current);
}
template< size_t N, bool IsTrivDestr, class T, class... Types >
constexpr std::enable_if_t< N == 0, const T&& >
    rychkov::details::get(const details::UnionBase< IsTrivDestr, T, Types... >&& base)
{
  return std::move(base.current);
}
template< size_t N, bool IsTrivDestr, class T, class... Types >
constexpr std::enable_if_t< N != 0, rychkov::nth_type_t< N - 1, Types... >& >
    rychkov::details::get(details::UnionBase< IsTrivDestr, T, Types... >& base)
{
  return get< N - 1 >(base.rest);
}
template< size_t N, bool IsTrivDestr, class T, class... Types >
constexpr std::enable_if_t< N != 0, const rychkov::nth_type_t< N - 1, Types... >& >
    rychkov::details::get(const details::UnionBase< IsTrivDestr, T, Types... >& base)
{
  return get< N - 1 >(base.rest);
}
template< size_t N, bool IsTrivDestr, class T, class... Types >
constexpr std::enable_if_t< N != 0, rychkov::nth_type_t< N - 1, Types... >&& >
    rychkov::details::get(details::UnionBase< IsTrivDestr, T, Types... >&& base)
{
  return get< N - 1 >(std::move(base.rest));
}
template< size_t N, bool IsTrivDestr, class T, class... Types >
constexpr std::enable_if_t< N != 0, const rychkov::nth_type_t< N - 1, Types... >&& >
    rychkov::details::get(const details::UnionBase< IsTrivDestr, T, Types... >&& base)
{
  return get< N - 1 >(std::move(base.rest));
}

template< size_t N, class... Types >
constexpr rychkov::nth_type_t< N, Types... >&
    rychkov::details::get(UnionStorageAlias< Types... >& base)
{
  return get< N >(base.storage);
}
template< size_t N, class... Types >
constexpr const rychkov::nth_type_t< N, Types... >&
    rychkov::details::get(const UnionStorageAlias< Types... >& base)
{
  return get< N >(base.storage);
}
template< size_t N, class... Types >
constexpr rychkov::nth_type_t< N, Types... >&&
    rychkov::details::get(UnionStorageAlias< Types... >&& base)
{
  return get< N >(std::move(base.storage));
}
template< size_t N, class... Types >
constexpr const rychkov::nth_type_t< N, Types... >&&
    rychkov::details::get(const UnionStorageAlias< Types... >&& base)
{
  return get< N >(std::move(base.storage));
}

#endif

