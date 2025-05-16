#ifndef MAP_BASE_ACCESS_HPP
#define MAP_BASE_ACCESS_HPP

#include "declaration.hpp"
#include <limits>

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::node_type*
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::fake_root() const noexcept
{
  return get_fake_pointer(fake_parent_, &node_type::parent);
}

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
bool rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::empty() const noexcept
{
  return size_ == 0;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::size_type
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::size() const noexcept
{
  return size_;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::size_type
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::maxsize() const noexcept
{
  return std::numeric_limits< difference_type >::max();
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< bool IsSet2 >
std::enable_if_t< IsSet2, typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::key_compare >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::key_comp() const
{
  return comp_;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< bool IsSet2 >
std::enable_if_t< !IsSet2, typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::key_compare >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::key_comp() const
{
  return comp_.comp;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::value_compare
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::value_comp() const
{
  return comp_;
}

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::begin() noexcept
{
  return {cached_begin_, 0};
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::begin() const noexcept
{
  return cbegin();
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::cbegin() const noexcept
{
  return {cached_begin_, 0};
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::reverse_iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::rbegin() noexcept
{
  return {cached_rbegin_, cached_rbegin_->size() - 1};
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_reverse_iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::rbegin() const noexcept
{
  return crbegin();
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_reverse_iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::crbegin() const noexcept
{
  return {cached_rbegin_, cached_rbegin_->size() - 1};
}

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::end() noexcept
{
  return {fake_root(), 0};
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::end() const noexcept
{
  return cend();
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::cend() const noexcept
{
  return {fake_root(), 0};
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::reverse_iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::rend() noexcept
{
  return {fake_root(), 0};
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_reverse_iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::rend() const noexcept
{
  return crend();
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_reverse_iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::crend() const noexcept
{
  return {fake_root(), 0};
}

#endif
