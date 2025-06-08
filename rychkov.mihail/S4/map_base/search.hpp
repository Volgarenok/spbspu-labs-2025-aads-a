#ifndef MAP_BASE_SEARCH_HPP
#define MAP_BASE_SEARCH_HPP

#include "declaration.hpp"

#include <iterator>

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< bool IsSet2 >
bool rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::compare_with_key
    (std::enable_if_t< !IsSet && !IsSet2, const value_type >& lhs, const key_type& rhs) const
{
  return comp_.comp(lhs.first, rhs);
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< bool IsSet2 >
bool rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::compare_with_key
    (const key_type& lhs, std::enable_if_t< !IsSet && !IsSet2, const value_type >& rhs) const
{
  return comp_.comp(lhs, rhs.first);
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< bool IsSet2 >
std::enable_if_t< IsSet && IsSet2, bool > rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::compare_with_key
    (const key_type& lhs, const key_type& rhs) const
{
  return comp_(lhs, rhs);
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< bool IsSet2 >
std::enable_if_t< !IsSet && !IsSet2, bool > rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::compare_with_key
    (const key_type& lhs, const key_type& rhs) const
{
  return comp_.comp(lhs, rhs);
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class V >
const typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::key_type&
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::get_key(const V& value)
{
  return value.first;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
const typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::key_type&
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::get_key(const key_type& key)
{
  return key;
}

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1 >
std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator,
      typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::lower_bound_impl(const K1& key) const
{
  if (size_ == 0)
  {
    return {end(), end()};
  }
  const_iterator left = {fake_children_[0], 0}, right = end();
  while (true)
  {
    if (compare_with_key(key, *left))
    {
      right = left;
      if (left.node_->isleaf())
      {
        return {left, left};
      }
      left = {left.node_->children[left.pointed_], 0};
    }
    else if (!compare_with_key(*left, key))
    {
      if (IsMulti)
      {
        right = left;
        if (left.node_->isleaf())
        {
          return {left, left};
        }
        left = {left.node_->children[left.pointed_], 0};
        continue;
      }
      return {left, left};
    }
    else
    {
      left.pointed_++;
      if (left.pointed_ >= left.node_->size())
      {
        if (left.node_->isleaf())
        {
          return {left, right};
        }
        left = {left.node_->children[left.pointed_], 0};
      }
    }
  }
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::upper_bound_impl(const K1& key) const
{
  if (size_ == 0)
  {
    return end();
  }
  const_iterator left = {fake_children_[0], 0}, right = end();
  while (true)
  {
    node_size_type i = 0;
    while (i < left.node_->size())
    {
      if (compare_with_key(key, left.node_->operator[](i)))
      {
        right = {left.node_, i};
        if (left.node_->isleaf())
        {
          return right;
        }
        left = {left.node_->children[i], 0};
        i = 0;
        continue;
      }
      i++;
    }
    if (left.node_->isleaf())
    {
      return right;
    }
    left = {left.node_->children[i], 0};
  }
}

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::lower_bound(const key_type& key)
{
  const_iterator temp = lower_bound_impl(key).second;
  return {temp.node_, temp.pointed_};
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::lower_bound(const key_type& key) const
{
  return lower_bound_impl(key).second;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::upper_bound(const key_type& key)
{
  const_iterator temp = upper_bound_impl(key);
  return {temp.node_, temp.pointed_};
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::upper_bound(const key_type& key) const
{
  return upper_bound_impl(key);
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::find(const key_type& key)
{
  const_iterator temp = lower_bound(key);
  return ((temp != end()) && !compare_with_key(key, *temp)) ? iterator{temp.node_, temp.pointed_} : end();
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::find(const key_type& key) const
{
  const_iterator temp = lower_bound(key);
  return ((temp != end()) && !compare_with_key(key, *temp)) ? temp : end();
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
bool rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::contains(const key_type& key) const
{
  return find(key) != end();
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator,
      typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::equal_range(const key_type& key)
{
  return {lower_bound(key), upper_bound(key)};
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator,
      typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::equal_range(const key_type& key) const
{
  return {lower_bound(key), upper_bound(key)};
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::size_type
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::count(const key_type& key) const
{
  return std::distance(lower_bound(key), upper_bound(key));
}

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::transparent_compare_key_t
    < typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator, K1 >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::lower_bound(const K1& key)
{
  const_iterator temp = lower_bound_impl(key).second;
  return {temp.node_, temp.pointed_};
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::transparent_compare_key_t
    < typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator, K1 >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::lower_bound(const K1& key) const
{
  return lower_bound_impl(key).second;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::transparent_compare_key_t
    < typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator, K1 >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::upper_bound(const K1& key)
{
  const_iterator temp = upper_bound_impl(key);
  return {temp.node_, temp.pointed_};
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::transparent_compare_key_t
    < typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator, K1 >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::upper_bound(const K1& key) const
{
  return upper_bound_impl(key);
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::transparent_compare_key_t
    < typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator, K1 >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::find(const K1& key)
{
  const_iterator temp = lower_bound(key);
  return ((temp != end()) && !compare_with_key(key, *temp)) ? iterator{temp.node_, temp.pointed_} : end();
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::transparent_compare_key_t
    < typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator, K1 >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::find(const K1& key) const
{
  const_iterator temp = lower_bound(key);
  return ((temp != end()) && !compare_with_key(key, *temp)) ? temp : end();
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::transparent_compare_key_t< bool, K1 >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::contains(const K1& key) const
{
  return find(key) != end();
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::transparent_compare_key_t
    < std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator,
        typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator >, K1 >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::equal_range(const K1& key)
{
  return {lower_bound(key), upper_bound(key)};
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::transparent_compare_key_t
    < std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator,
        typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator >, K1 >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::equal_range(const K1& key) const
{
  return {lower_bound(key), upper_bound(key)};
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::transparent_compare_key_t
    < typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::size_type, K1 >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::count(const K1& key) const
{
  return std::distance(lower_bound(key), upper_bound(key));
}

#endif
