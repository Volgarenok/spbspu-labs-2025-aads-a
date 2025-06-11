#ifndef UNORDERED_BASE_CONSTRUCT_DESTRUCT_HPP
#define UNORDERED_BASE_CONSTRUCT_DESTRUCT_HPP

#include "declaration.hpp"

#include <utility>

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::UnorderedBase()
    noexcept(noexcept_default):
  capacity_{0},
  size_{0},
  max_factor_{default_max_factor},
  data_{nullptr},
  raw_{nullptr},
  cached_begin_{nullptr}
{}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::UnorderedBase(size_type cnt, hasher hash, key_equal eq):
  capacity_{0},
  size_{0},
  max_factor_{default_max_factor},
  data_{nullptr},
  raw_{nullptr},
  cached_begin_{nullptr},
  hash_{std::move(hash)},
  equal_{std::move(eq)}
{
  allocate(cnt);
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::UnorderedBase(UnorderedBase&& rhs) noexcept(noexcept_move):
  capacity_{std::exchange(rhs.capacity_, 0)},
  size_{std::exchange(rhs.size_, 0)},
  max_factor_{std::exchange(rhs.max_factor_, default_max_factor + 0)},
  data_{std::exchange(rhs.data_, nullptr)},
  raw_{std::exchange(rhs.raw_, nullptr)},
  cached_begin_{std::exchange(rhs.cached_begin_, nullptr)},
  hash_{std::move(rhs.hash_)},
  equal_{std::move(rhs.equal_)}
{}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::UnorderedBase(const UnorderedBase& rhs):
  UnorderedBase(rhs.capacity_, rhs.hash_, rhs.equal_)
{
  insert(rhs.begin(), rhs.end());
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::UnorderedBase
    (std::initializer_list< value_type > init, hasher hash, key_equal eq):
  UnorderedBase(0, std::move(hash), std::move(eq))
{
  reserve(init.size());
  insert(init.begin(), init.end());
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class InputIt >
rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::UnorderedBase(InputIt from, InputIt to,
      hasher hash, key_equal eq):
  UnorderedBase(0, std::move(hash), std::move(eq))
{
  for (; from != to; ++from)
  {
    insert(*from);
  }
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >&
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::operator=(const UnorderedBase& rhs)
{
  UnorderedBase temp = rhs;
  swap(temp);
  return *this;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >&
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::operator=(UnorderedBase&& rhs)
    noexcept(noexcept(swap(std::declval< UnorderedBase& >())))
{
  UnorderedBase temp = std::move(rhs);
  swap(temp);
  return *this;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >&
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::operator=(std::initializer_list< value_type > init)
{
  UnorderedBase temp = init;
  swap(temp);
  return *this;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::~UnorderedBase()
{
  clear();
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
void rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::swap(UnorderedBase& rhs)
    noexcept(is_nothrow_swappable_v< hasher > && is_nothrow_swappable_v< key_equal >)
{
  static_assert(is_nothrow_swappable_v< hasher > || is_nothrow_swappable_v< key_equal >, "");
  if (is_nothrow_swappable_v< hasher >)
  {
    std::swap(equal_, rhs.equal_);
    std::swap(hash_, rhs.hash_);
  }
  else
  {
    std::swap(hash_, rhs.hash_);
    std::swap(equal_, rhs.equal_);
  }
  std::swap(capacity_, rhs.capacity_);
  std::swap(size_, rhs.size_);
  std::swap(max_factor_, rhs.max_factor_);
  std::swap(data_, rhs.data_);
  std::swap(raw_, rhs.raw_);
  std::swap(cached_begin_, rhs.cached_begin_);
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
void rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::clear() noexcept
{
  for (size_type i = 0; i < capacity_; i++)
  {
    if (data_[i].first == ~0ULL)
    {
      data_[i].first.~size_type();
    }
    else
    {
      data_[i].~stored_value();
    }
  }
  delete[] raw_;

  capacity_ = 0;
  size_ = 0;
  max_factor_ = default_max_factor;
  raw_ = nullptr;
  data_ = nullptr;
  cached_begin_ = nullptr;
}

#endif
