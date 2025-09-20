#ifndef UNORDERED_BASE_MEMORY_HPP
#define UNORDERED_BASE_MEMORY_HPP

#include "declaration.hpp"

#include <utility>
#include <cmath>
#include <memory>
#include <type_traits.hpp>

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
void rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::allocate(size_type new_capacity)
{
  if (new_capacity == 0)
  {
    raw_ = nullptr;
    data_ = nullptr;
    capacity_ = 0;
    return;
  }
  raw_ = new unsigned char[new_capacity * sizeof(stored_value) + alignof(stored_value) - 1];
  data_ = reinterpret_cast< stored_value* >(reinterpret_cast< size_t >(raw_ + alignof(stored_value) - 1)
        & ~(alignof(stored_value) - 1));
  capacity_ = new_capacity;
  cached_begin_ = data_ + capacity_;
  for (size_type i = 0; i < capacity_; i++)
  {
    new(&data_[i].first) size_type{~0ULL};
  }
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
bool rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::extend(size_type new_capacity)
{
  size_type required_size = std::ceil(new_capacity / max_factor_);
  if (required_size <= capacity_)
  {
    return false;
  }
  rehash(required_size * 2);
  return true;
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
void rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::reserve(size_type new_capacity)
{
  rehash(std::ceil(new_capacity / max_factor_));
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
void rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::rehash(size_type cnt)
{
  if (cnt <= capacity_)
  {
    return;
  }
  UnorderedBase temp{cnt, hash_, equal_};
  for (size_type i = 0; i < capacity_; i++)
  {
    if (data_[i].first != ~0ULL)
    {
      temp.insert(std::move_if_noexcept(*reinterpret_cast< temp_value* >(std::addressof(data_[i].second))));
    }
  }
  swap(temp);
}

template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
template< class... Args >
std::pair< typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator, bool >
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::emplace_hint_impl
    (std::pair< const_iterator, bool > hint, Args&&... args)
{
  static_assert(is_nothrow_swappable_v< temp_value >, "");
  iterator result = {hint.first.data_, hint.first.end_};
  if (!hint.second)
  {
    return {result, false};
  }

  temp_stored temp = {0, {std::forward< Args >(args)...}};
  size_type slot = hash_(get_key(temp.second)) % capacity_, id = hint.first.data_ - data_;
  temp.first = (id >= slot ? id - slot : id + capacity_ - slot);

  while (hint.first.data_->first != ~0ULL)
  {
    std::swap(temp, *reinterpret_cast< temp_stored* >(hint.first.data_));
    if (++hint.first.data_ == hint.first.end_)
    {
      hint.first.data_ = data_;
    }
    temp.first++;
  }
  new(reinterpret_cast< temp_value* >(std::addressof(hint.first.data_->second))) temp_value{std::move(temp.second)};
  hint.first.data_->first = temp.first;
  if ((cached_begin_ == nullptr) || (hint.first.data_ < cached_begin_))
  {
    cached_begin_ = hint.first.data_;
  }
  size_++;
  return {result, true};
}
template< class K, class T, class H, class E, bool IsSet, bool IsMulti >
typename rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::iterator
    rychkov::UnorderedBase< K, T, H, E, IsSet, IsMulti >::erase(const_iterator pos)
{
  static_assert(std::is_nothrow_move_constructible< temp_value >::value, "");
  pos.data_->second.~value_type();
  iterator result = {pos.data_, pos.end_};
  size_type expected_psl = pos.data_->first, shift = 0;
  pos.data_->first = ~0ULL;

  stored_value* erased = pos.data_;
  stored_value* prev = erased;
  while (true)
  {
    if (++pos.data_ == pos.end_)
    {
      pos.data_ = data_;
    }
    shift++;

    if ((pos.data_->first != expected_psl + shift) || (pos.data_->first == ~0ULL))
    {
      new(std::addressof(erased->second)) value_type(std::move(reinterpret_cast< temp_stored* >(prev)->second));
      prev->second.~value_type();
      erased->first = expected_psl;
      prev->first = ~0ULL;

      if ((pos.data_->first == ~0ULL) || (pos.data_->first == 0))
      {
        if (prev == cached_begin_)
        {
          cached_begin_ = (++begin()).data_;
        }
        size_--;
        if (result.data_->first == ~0ULL)
        {
          return ++result;
        }
        return result;
      }

      erased = prev;
      expected_psl = pos.data_->first - 1;
      shift = 0;
    }
    prev = pos.data_;
  }
}

#endif
