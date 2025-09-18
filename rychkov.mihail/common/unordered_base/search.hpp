#ifndef UNORDERED_BASE_SEARCH_HPP
#define UNORDERED_BASE_SEARCH_HPP

#include "declaration.hpp"

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class V >
const typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::key_type&
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::get_key(const V& value)
{
  return value.first;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
const typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::key_type&
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::get_key(const key_type& key)
{
  return key;
}

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::const_iterator
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::find_impl(const K1& key) const
{
  if (empty())
  {
    return end();
  }
  size_type slot = hash_(key) % capacity_;
  for (size_type i = 0; (data_[slot].first != ~0ULL) && (data_[slot].first >= i); i++,
        slot = (++slot < capacity_ ? slot : slot - capacity_))
  {
    if ((data_[slot].first == i) && equal_(get_key(data_[slot].second), key))
    {
      return {data_ + slot, data_ + capacity_};
    }
  }
  return end();
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::size_type
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::count_impl(const K1& key) const
{
  if (empty())
  {
    return 0;
  }
  size_type slot = hash_(key) % capacity_, result = 0;
  for (size_type i = 0; (data_[slot].first != ~0ULL) && (data_[slot].first >= i); i++,
        slot = (++slot < capacity_ ? slot : slot - capacity_))
  {
    if ((data_[slot].first == i) && equal_(get_key(data_[slot].second), key))
    {
      result++;
    }
  }
  return result;
}

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::find(const key_type& key)
{
  const_iterator result = find_impl(key);
  return {result.data_, result.end_};
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::const_iterator
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::find(const key_type& key) const
{
  return find_impl(key);
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
bool rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::contains(const key_type& key) const
{
  return find(key) != end();
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::size_type
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::count(const key_type& key) const
{
  return count_impl(key);
}

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::transparent_hash_key_t
    < typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator, K1 >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::find(const K1& key)
{
  const_iterator result = find_impl(key);
  return {result.data_, result.end_};
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::transparent_hash_key_t
    < typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::const_iterator, K1 >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::find(const K1& key) const
{
  return find_impl(key);
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::transparent_hash_key_t< bool, K1 >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::contains(const K1& key) const
{
  return find(key) != end();
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::transparent_hash_key_t
    < typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::size_type, K1 >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::count(const K1& key) const
{
  return count_impl(key);
}

#endif
