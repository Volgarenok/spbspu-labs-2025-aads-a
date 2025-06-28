#ifndef HASH_TABLE_DEFINITION_HPP
#define HASH_TABLE_DEFINITION_HPP

#include <utility>
#include <vector/definition.hpp>
#include "declaration.hpp"

namespace maslevtsov {
  namespace detail {
    template< class Key, class Hash >
    size_t get_odd_step(const Key& key, size_t capacity, Hash hasher)
    {
      return 1 + 2 * (hasher(key) % (capacity / 2));
    }
  }
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::HashTable() noexcept:
  slots_(16),
  size_(0)
{}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
template< class InputIt >
maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::HashTable(InputIt first, InputIt last):
  HashTable()
{
  insert(first, last);
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::HashTable(std::initializer_list< value_type > ilist):
  HashTable(ilist.begin(), ilist.end())
{}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
T& maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::operator[](const Key& key)
{
  return insert(std::make_pair(key, T())).first->second;
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
const T& maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::operator[](const Key& key) const
{
  return at(key);
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
T& maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::at(const Key& key)
{
  iterator it = find(key);
  if (it != end()) {
    return it->second;
  }
  throw std::out_of_range("invalid key");
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
const T& maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::at(const Key& key) const
{
  const_iterator it = find(key);
  if (it != cend()) {
    return it->second;
  }
  throw std::out_of_range("invalid key");
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::iterator
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::find(const Key& key) noexcept
{
  size_t index = find_index(key);
  return index == slots_.size() ? end() : iterator(this, index);
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::const_iterator
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::find(const Key& key) const noexcept
{
  size_t index = find_index(key);
  return index == slots_.size() ? cend() : const_iterator(this, index);
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::iterator
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::begin() noexcept
{
  return slots_[0].state == detail::SlotState::EMPTY ? ++iterator(this, 0) : iterator(this, 0);
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::const_iterator
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::begin() const noexcept
{
  return slots_[0].state == detail::SlotState::EMPTY ? ++const_iterator(this, 0) : const_iterator(this, 0);
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::const_iterator
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::cbegin() const noexcept
{
  return slots_[0].state == detail::SlotState::EMPTY ? ++const_iterator(this, 0) : const_iterator(this, 0);
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::iterator
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::end() noexcept
{
  return iterator(this, slots_.size());
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::const_iterator
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::end() const noexcept
{
  return const_iterator(this, slots_.size());
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::const_iterator
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::cend() const noexcept
{
  return const_iterator(this, slots_.size());
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
bool maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::empty() const noexcept
{
  return size_ == 0;
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::size_type
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::size() const noexcept
{
  return size_;
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
void maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::clear() noexcept
{
  for (size_t i = 0; i != slots_.size(); ++i) {
    slots_[i].state = detail::SlotState::EMPTY;
  }
  size_ = 0;
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
std::pair< typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::iterator, bool >
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::insert(const value_type& value)
{
  return emplace(value);
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::iterator
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::insert(const_iterator hint, const value_type& value)
{
  return emplace_hint(hint, std::forward< const value_type& >(value));
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
template< class InputIt >
void maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::insert(InputIt first, InputIt last)
{
  for (; first != last; ++first) {
    insert(*first);
  }
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
template< class... Args >
std::pair< typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::iterator, bool >
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::emplace(Args&&... args)
{
  value_type value(std::forward< Args >(args)...);
  const Key& key = value.first;
  if (load_factor() >= max_load_factor_) {
    rehash(slots_.size() * 2);
  }
  size_t index = find_index(key);
  if (index != slots_.size()) {
    return {iterator(this, index), false};
  }
  index = hasher_(key) % slots_.size();
  size_t odd_step = detail::get_odd_step(key, slots_.size(), probe_hasher_);
  size_t first_deleted = slots_.size();
  for (size_t i = 0; i < slots_.size(); ++i) {
    if (slots_[index].state == detail::SlotState::EMPTY) {
      if (first_deleted != slots_.size()) {
        slots_[first_deleted].data = value;
        slots_[first_deleted].state = detail::SlotState::OCCUPIED;
        ++size_;
        return {iterator(this, first_deleted), true};
      } else {
        slots_[index].data = value;
        slots_[index].state = detail::SlotState::OCCUPIED;
        ++size_;
        return {iterator(this, index), true};
      }
    } else if (slots_[index].state == detail::SlotState::DELETED) {
      if (first_deleted == slots_.size()) {
        first_deleted = index;
      }
    }
    index = (index + odd_step) % slots_.size();
  }
  if (first_deleted != slots_.size()) {
    slots_[first_deleted].data = value;
    slots_[first_deleted].state = detail::SlotState::OCCUPIED;
    ++size_;
    return {iterator(this, first_deleted), true};
  }
  throw std::runtime_error("hash-table is full");
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
template< class... Args >
typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::iterator
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::emplace_hint(const_iterator hint, Args&&... args)
{
  value_type value(std::forward< Args >(args)...);
  const Key& key = value.first;
  if (hint != cend()) {
    if (slots_[hint.index_].state == detail::SlotState::OCCUPIED && key_equal_(slots_[hint.index_].data.first, key)) {
      return iterator(this, hint.index_);
    }
  }
  return emplace(std::move(value)).first;
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::iterator
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::erase(iterator pos) noexcept
{
  slots_[pos.index_].state = detail::SlotState::DELETED;
  --size_;
  return ++pos;
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::iterator
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::erase(const_iterator pos) noexcept
{
  return erase(iterator(this, pos.index_));
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::iterator
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::erase(const_iterator first, const_iterator last) noexcept
{
  iterator first_it(this, first.index_);
  iterator last_it(this, last.index_);
  while (first_it != last_it) {
    first_it = erase(first_it);
  }
  return last_it;
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::size_type
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::erase(const Key& key) noexcept
{
  iterator it = find(key);
  if (it == end()) {
    return 0;
  }
  erase(it);
  return 1;
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
void maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::swap(HashTable& other)
{
  std::swap(slots_, other.slots_);
  std::swap(size_, other.size_);
  std::swap(hasher_, other.hasher_);
  std::swap(probe_hasher_, other.probe_hasher_);
  std::swap(key_equal_, other.key_equal_);
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
float maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::load_factor() const noexcept
{
  return static_cast< float >(size_) / slots_.size();
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
float maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::max_load_factor() const noexcept
{
  return max_load_factor_;
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
void maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::max_load_factor(float ml)
{
  max_load_factor_ = ml;
  while (load_factor() > max_load_factor_) {
    rehash(slots_.size() * 2);
  }
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
void maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::rehash(size_type count)
{
  if (count <= slots_.size()) {
    return;
  }
  if (count <= 0 || (count & (count - 1)) != 0) {
    size_t new_count = 1;
    while (new_count < count) {
      new_count *= 2;
    }
    count = new_count;
  }
  Vector< detail::Slot< value_type > > new_slots(count);
  for (auto it = slots_.begin(); it != slots_.end(); ++it) {
    if (it->state == detail::SlotState::OCCUPIED) {
      const Key& key = it->data.first;
      size_t index = hasher_(key) % new_slots.size();
      size_t odd_step = detail::get_odd_step(key, slots_.size(), probe_hasher_);
      while (new_slots[index].state == detail::SlotState::OCCUPIED) {
        index = (index + odd_step) % new_slots.size();
      }
      new_slots[index] = *it;
    }
  }
  slots_ = new_slots;
}

template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
typename maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::size_type
  maslevtsov::HashTable< Key, T, Hash, ProbeHash, KeyEqual >::find_index(const Key& key) const noexcept
{
  size_t index = hasher_(key) % slots_.size();
  size_t odd_step = detail::get_odd_step(key, slots_.size(), probe_hasher_);
  for (size_t i = 0; i < slots_.size(); ++i) {
    if (slots_[index].state == detail::SlotState::EMPTY) {
      return slots_.size();
    }
    if (slots_[index].state == detail::SlotState::OCCUPIED && key_equal_(slots_[index].data.first, key)) {
      return index;
    }
    index = (index + odd_step) % slots_.size();
  }
  return slots_.size();
}

#endif
