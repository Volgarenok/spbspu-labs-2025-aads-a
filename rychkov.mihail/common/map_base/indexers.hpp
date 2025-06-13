#ifndef INDEXERS_HPP
#define INDEXERS_HPP

#include "declaration.hpp"

#include <utility>

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< bool IsSet2, bool IsMulti2 >
std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2,
      typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::mapped_type& >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::at(const key_type& key)
{
  iterator temp = find(key);
  if (temp == end())
  {
    throw std::out_of_range("at index out of range");
  }
  return temp->second;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< bool IsSet2, bool IsMulti2 >
std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2,
      const typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::mapped_type& >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::at(const key_type& key) const
{
  const_iterator temp = this->find(key);
  if (temp == this->end())
  {
    throw std::out_of_range("at index out of range");
  }
  return temp->second;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1, bool IsSet2, bool IsMulti2 >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::transparent_compare_key_t
    < std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2,
      typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::mapped_type& >, K1 >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::at(const K1& key)
{
  iterator temp = this->find(key);
  if (temp == this->end())
  {
    throw std::out_of_range("at index out of range");
  }
  return temp->second;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1, bool IsSet2, bool IsMulti2 >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::transparent_compare_key_t
    < std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2,
      const typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::mapped_type& >, K1 >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::at(const K1& key) const
{
  const_iterator temp = this->find(key);
  if (temp == this->end())
  {
    throw std::out_of_range("at index out of range");
  }
  return temp->second;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< bool IsSet2, bool IsMulti2 >
std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2,
      typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::mapped_type& >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::operator[](const key_type& key)
{
  return this->try_emplace(key).first->second;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< bool IsSet2, bool IsMulti2 >
std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2,
      typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::mapped_type& >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::operator[](key_type&& key)
{
  return this->try_emplace(std::move(key)).first->second;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1, bool IsSet2, bool IsMulti2 >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::transparent_compare_key_t
    < std::enable_if_t< !IsSet && !IsSet2 && !IsMulti && !IsMulti2,
      typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::mapped_type& >, K1 >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::operator[](K1&& key)
{
  return this->try_emplace(std::forward< K1 >(key)).first->second;
}

#endif
