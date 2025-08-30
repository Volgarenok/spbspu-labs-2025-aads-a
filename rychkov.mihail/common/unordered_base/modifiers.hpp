#ifndef UNORDERED_BASE_MODIFIERS_HPP
#define UNORDERED_BASE_MODIFIERS_HPP

#include "declaration.hpp"

#include <utility>
#include <tuple>

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class K1 >
std::pair< typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::const_iterator, bool >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::find_hint_pair(const K1& key) const
{
  size_type slot = hash_(key) % capacity_;
  for (size_type i = 0; (data_[slot].first != ~0ULL) && (data_[slot].first >= i); i++,
        slot = (++slot < capacity_ ? slot : slot - capacity_))
  {
    if ((data_[slot].first == i) && equal_(get_key(data_[slot].second), key))
    {
      return {{data_ + slot, data_ + capacity_}, IsMulti};
    }
  }
  return {{data_ + slot, data_ + capacity_}, true};
}

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class K1 >
std::pair< typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::const_iterator, bool >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::correct_hint(const_iterator hint, const K1& key)
{
  if (extend(size_ + 1))
  {
    return find_hint_pair(key);
  }
  if (hint.data_ == hint.end_)
  {
    hint.data_--;
  }
  size_type slot = hash_(key) % capacity_, psl = hint.data_ - data_;
  psl = (psl >= slot ? psl - slot : psl + capacity_ - slot);
  if ((hint.data_->first != ~0ULL) && (hint.data_->first >= psl))
  {
    for (; (hint.data_->first != ~0ULL) && (hint.data_->first >= psl); psl++,
          hint.data_ = (++hint.data_ == hint.end_ ? data_ : hint.data_))
    {
      if ((hint.data_->first == psl) && equal_(get_key(hint.data_->second), key))
      {
        return {hint, IsMulti};
      }
    }
    return {hint, true};
  }
  stored_value* prev = hint.data_;
  while (((hint.data_->first == ~0ULL) || (hint.data_->first <= psl)) && (psl != 0))
  {
    hint.data_ = (hint.data_ == data_ ? hint.end_ - 1 : hint.data_ - 1);
    --psl;
    prev = ((hint.data_->first < psl) || (hint.data_->first == ~0ULL) ? hint.data_ : prev);
    if ((hint.data_->first == psl) && equal_(get_key(hint.data_->second), key))
    {
      return {hint, IsMulti};
    }
  }
  return {{prev, hint.end_}, true};
}

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class... Args >
std::pair< typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator, bool >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::emplace(Args&&... args)
{
  extend(size_ + 1);
  temp_value temp{std::forward< Args >(args)...};
  return emplace_hint_impl(find_hint_pair(get_key(temp)), std::move(temp));
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class... Args >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::emplace_hint(const_iterator hint, Args&&... args)
{
  temp_value temp{std::forward< Args >(args)...};
  return emplace_hint_impl(correct_hint(hint, get_key(temp)), std::move(temp)).first;
}

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< bool IsSet2, class... Args >
std::enable_if_t< !IsSet && !IsSet2,
      std::pair< typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator, bool > >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::try_emplace(const key_type& key, Args&&... args)
{
  extend(size_ + 1);
  return emplace_hint_impl(find_hint_pair(key), std::piecewise_construct,
        std::forward_as_tuple(key), std::forward_as_tuple(std::forward< Args >(args)...));
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< bool IsSet2, class... Args >
std::enable_if_t< !IsSet && !IsSet2,
      std::pair< typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator, bool > >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::try_emplace(key_type&& key, Args&&... args)
{
  extend(size_ + 1);
  return emplace_hint_impl(find_hint_pair(key), std::piecewise_construct,
        std::forward_as_tuple(std::move(key)), std::forward_as_tuple(std::forward< Args >(args)...));
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< bool IsSet2, class K1, class... Args >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::transparent_hash_key_t
    < std::enable_if_t< !IsSet && !IsSet2,
        std::pair< typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator, bool > >, K1 >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::try_emplace(K1&& key, Args&&... args)
{
  extend(size_ + 1);
  return emplace_hint_impl(find_hint_pair(key), std::piecewise_construct,
        std::forward_as_tuple(std::forward< K1 >(key)), std::forward_as_tuple(std::forward< Args >(args)...));
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< bool IsSet2, class... Args >
std::enable_if_t< !IsSet && !IsSet2, typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::try_emplace(const_iterator hint,
      const key_type& key, Args&&... args)
{
  return emplace_hint_impl(correct_hint(hint, key), key, std::forward< Args >(args)...).first;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< bool IsSet2, class... Args >
std::enable_if_t< !IsSet && !IsSet2, typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::try_emplace(const_iterator hint,
      key_type&& key, Args&&... args)
{
  return emplace_hint_impl(correct_hint(hint, key), std::move(key), std::forward< Args >(args)...).first;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< bool IsSet2, class K1, class... Args >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::transparent_hash_key_t
    < std::enable_if_t< !IsSet && !IsSet2,
        std::pair< typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator, bool > >, K1 >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::try_emplace(const_iterator hint, K1&& key, Args&&... args)
{
  return emplace_hint_impl(correct_hint(hint, key), std::forward< K1 >(key), std::forward< Args >(args)...).first;
}

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
std::pair< typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator, bool >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::insert(const value_type& value)
{
  extend(size_ + 1);
  return emplace_hint_impl(find_hint_pair(get_key(value)), value);
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
std::pair< typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator, bool >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::insert(value_type&& value)
{
  extend(size_ + 1);
  return emplace_hint_impl(find_hint_pair(get_key(value)), std::move(value));
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class V >
std::enable_if_t< std::is_constructible
      < typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::value_type, V&& >::value,
      std::pair< typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator, bool > >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::insert(V&& value)
{
  extend(size_ + 1);
  return emplace(std::forward< V >(value));
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::insert(const_iterator hint, const value_type& value)
{
  return emplace_hint_impl(correct_hint(hint, get_key(value)), value).first;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::insert(const_iterator hint, value_type&& value)
{
  return emplace_hint_impl(correct_hint(hint, get_key(value)), std::move(value)).first;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class V >
std::enable_if_t< std::is_constructible
      < typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::value_type, V&& >::value,
      typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::insert(const_iterator hint, V&& value)
{
  return emplace_hint(hint, std::forward< V >(value));
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class InputIter >
void rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::insert(InputIter from, InputIter to)
{
  for (; from != to; ++from)
  {
    insert(*from);
  }
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
void rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::insert(std::initializer_list< value_type > list)
{
  extend(size_ + list.size());
  insert(list.begin(), list.end());
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::erase(const_iterator from, const_iterator to)
{
  iterator result;
  while (from != to)
  {
    --to;
    result = erase(to);
  }
  return result;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::size_type
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::erase(const key_type& key)
{
  iterator cur = find(key);
  size_type result = 0;
  while (cur != end())
  {
    erase(cur);
    result++;
    cur = find(key);
  }
  return result;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class K1 >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::transparent_hash_key_t
    < typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::size_type, K1 >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::erase(const K1& key)
{
  iterator cur = find(key);
  size_type result = 0;
  while (cur != end())
  {
    erase(cur);
    result++;
    cur = find(key);
  }
  return result;
}

#endif
