#ifndef MAP_BASE_INSERT_HPP
#define MAP_BASE_INSERT_HPP

#include "declaration.hpp"

#include <utility>
#include <tuple>

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1 >
std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator, bool >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::find_hint_pair(const K1& key) const
{
  const_iterator hint = lower_bound_impl(key).first;
  if (IsMulti || (hint.pointed_ >= hint.node_->size()) || compare_with_key(key, *hint))
  {
    return {hint, true};
  }
  return {hint, false};
}

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1 >
std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator, bool >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::correct_hint(const_iterator hint, const K1& key) const
{
  if (empty())
  {
    return {end(), true};
  }
  bool correct = false;
  const bool right_order = (hint != end()) && !compare_with_key(key, *hint);
  if (hint == end())
  {
    --hint;
    if (!compare_with_key(key, *hint))
    {
      bool need_insert = IsMulti || !compare_with_key(*hint, key);
      hint.pointed_++;
      return {hint, need_insert};
    }
  }
  if (!right_order)
  {
    return correct_hint(const_reverse_iterator{hint.node_, hint.pointed_}, key);
  }
  while (true)
  {
    if (compare_with_key(key, *hint))
    {
      if (hint.node_->isleaf())
      {
        return {hint, true};
      }
      hint.node_ = hint.node_->children[hint.pointed_];
      hint.pointed_ = 0;
      correct = true;
    }
    else if (!compare_with_key(*hint, key))
    {
      if (!hint.node_->isleaf())
      {
        --hint;
        hint.pointed_++;
      }
      return {hint, IsMulti};
    }
    else
    {
      hint.pointed_++;
      if (hint.pointed_ == hint.node_->size())
      {
        if (!correct)
        {
          const_iterator backup = hint;
          hint.pointed_--;
          for (hint.move_up(); !hint.node_->isfake() && (hint.pointed_ == hint.node_->size()); hint.move_up())
          {}
          if (!hint.node_->isfake() && !comp_(key, *hint))
          {
            continue;
          }
          correct = true;
          hint = backup;
        }
        if (hint.node_->isleaf())
        {
          return {hint, true};
        }
        hint = {hint.node_->children[hint.pointed_], 0};
      }
    }
  }
}

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class K1 >
std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::const_iterator, bool >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::correct_hint(const_reverse_iterator hint, const K1& key) const
{
  bool correct = false;
  while (true)
  {
    if (compare_with_key(*hint, key))
    {
      if (hint.node_->isleaf())
      {
        return {{hint.node_, ++hint.pointed_}, true};
      }
      hint.node_ = hint.node_->children[hint.pointed_ + 1];
      hint.pointed_ = hint.node_->size() - 1;
      correct = true;
    }
    else if (!compare_with_key(key, *hint))
    {
      if (!hint.node_->isleaf())
      {
        --hint;
      }
      return {{hint.node_, hint.pointed_}, IsMulti};
    }
    else
    {
      if (hint.pointed_ == 0)
      {
        if (!correct)
        {
          const_reverse_iterator backup = hint;
          for (hint.move_up(); !hint.node_->isfake() && (hint.pointed_ == 0); hint.move_up())
          {}
          hint.pointed_--;
          if (!hint.node_->isfake() && !comp_(*hint, key))
          {
            continue;
          }
          correct = true;
          hint = backup;
        }
        if (hint.node_->isleaf())
        {
          return {{hint.node_, hint.pointed_}, true};
        }
        hint.node_ = hint.node_->children[hint.pointed_];
        hint.pointed_ = hint.node_->size() - 1;
        continue;
      }
      hint.pointed_--;
    }
  }
}

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class... Args >
std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator, bool >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::emplace(Args&&... args)
{
  real_value_type temp{std::forward< Args >(args)...};
  return emplace_hint_impl(find_hint_pair(get_key(temp)), std::move(temp));
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class... Args >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::emplace_hint(const_iterator hint, Args&&... args)
{
  real_value_type temp{std::forward< Args >(args)...};
  return emplace_hint_impl(correct_hint(hint, get_key(temp)), std::move(temp)).first;
}

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< bool IsSet2, class... Args >
std::enable_if_t< !IsSet && !IsSet2,
      std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator, bool > >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::try_emplace(const key_type& key, Args&&... args)
{
  return emplace_hint_impl(find_hint_pair(key), std::piecewise_construct,
        std::forward_as_tuple(key), std::forward_as_tuple(std::forward< Args >(args)...));
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< bool IsSet2, class... Args >
std::enable_if_t< !IsSet && !IsSet2,
      std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator, bool > >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::try_emplace(key_type&& key, Args&&... args)
{
  return emplace_hint_impl(find_hint_pair(key), std::piecewise_construct,
        std::forward_as_tuple(std::move(key)), std::forward_as_tuple(std::forward< Args >(args)...));
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< bool IsSet2, class K1, class... Args >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::transparent_compare_key_t
    < std::enable_if_t< !IsSet && !IsSet2,
        std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator, bool > >, K1 >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::try_emplace(K1&& key, Args&&... args)
{
  return emplace_hint_impl(find_hint_pair(key), std::piecewise_construct,
        std::forward_as_tuple(std::forward< K1 >(key)), std::forward_as_tuple(std::forward< Args >(args)...));
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< bool IsSet2, class... Args >
std::enable_if_t< !IsSet && !IsSet2, typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::try_emplace(const_iterator hint, const key_type& key, Args&&... args)
{
  return emplace_hint_impl(correct_hint(hint, key), key, std::forward< Args >(args)...).first;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< bool IsSet2, class... Args >
std::enable_if_t< !IsSet && !IsSet2, typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::try_emplace(const_iterator hint, key_type&& key, Args&&... args)
{
  return emplace_hint_impl(correct_hint(hint, key), std::move(key), std::forward< Args >(args)...).first;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< bool IsSet2, class K1, class... Args >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::transparent_compare_key_t
    < std::enable_if_t< !IsSet && !IsSet2,
        std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator, bool > >, K1 >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::try_emplace(const_iterator hint, K1&& key, Args&&... args)
{
  return emplace_hint_impl(correct_hint(hint, key), std::forward< K1 >(key), std::forward< Args >(args)...).first;
}

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator, bool >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::insert(const value_type& value)
{
  return emplace_hint_impl(find_hint_pair(get_key(value)), value);
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator, bool >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::insert(value_type&& value)
{
  return emplace_hint_impl(find_hint_pair(get_key(value)), std::move(value));
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class V >
std::enable_if_t< std::is_constructible< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::value_type, V&& >::value,
      std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator, bool > >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::insert(V&& value)
{
  return emplace(std::forward< V >(value));
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::insert(const_iterator hint, const value_type& value)
{
  return emplace_hint_impl(correct_hint(hint, get_key(value)), value).first;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::insert(const_iterator hint, value_type&& value)
{
  return emplace_hint_impl(correct_hint(hint, get_key(value)), std::move(value)).first;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class V >
std::enable_if_t< std::is_constructible< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::value_type, V&& >::value,
      typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::insert(const_iterator hint, V&& value)
{
  return emplace_hint(hint, std::forward< V >(value));
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class InputIter >
void rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::insert(InputIter from, InputIter to)
{
  for (; from != to; ++from)
  {
    insert(*from);
  }
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
void rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::insert(std::initializer_list< value_type > list)
{
  insert(list.begin(), list.end());
}

#endif
