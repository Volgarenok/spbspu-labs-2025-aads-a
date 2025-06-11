#ifndef UNORDERED_BASE_ACCESS_HPP
#define UNORDERED_BASE_ACCESS_HPP

#include "declaration.hpp"

#include <limits>

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
bool rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::empty() const noexcept
{
  return size_ == 0;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::size_type
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::size() const noexcept
{
  return size_;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::size_type
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::maxsize() const noexcept
{
  return std::numeric_limits< difference_type >::max();
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
float rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::load_factor() const noexcept
{
  return capacity_ == 0 ? 1 : static_cast< float >(size_) / capacity_;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
float rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::max_load_factor() const noexcept
{
  return max_factor_;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
void rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::max_load_factor(float new_factor) noexcept
{
  max_factor_ = new_factor;
}

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::size_type
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::bucket_count() const noexcept
{
  return capacity_;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::size_type
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::max_bucket_count() const noexcept
{
  return maxsize();
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::size_type
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::bucket_size() const noexcept
{
  return 1;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::size_type
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::bucket(size_type index) const noexcept
{
  return index;
}

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::hasher
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::hash_function() const
{
  return hash_;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::key_equal
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::key_eq() const
{
  return equal_;
}

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::begin() noexcept
{
  return {cached_begin_, data_ + capacity_};
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::const_iterator
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::begin() const noexcept
{
  return cbegin();
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::const_iterator
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::cbegin() const noexcept
{
  return {cached_begin_, data_ + capacity_};
}

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::end() noexcept
{
  return {data_ + capacity_, data_ + capacity_};
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::const_iterator
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::end() const noexcept
{
  return cend();
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::const_iterator
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::cend() const noexcept
{
  return {data_ + capacity_, data_ + capacity_};
}

#endif
