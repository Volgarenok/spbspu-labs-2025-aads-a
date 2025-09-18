#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>
#include "ConstIteratorHashTable.hpp"
#include "IteratorHashTable.hpp"

namespace {
  bool is_prime(size_t n)
  {
    if (n <= 1) {
      return false;
    }
    if (n <= 3) {
      return true;
    }
    if (n % 2 == 0 || n % 3 == 0) {
      return false;
    }

    for (size_t i = 5; i * i <= n; i += 6) {
      if (n % i == 0 || n % (i + 2) == 0) {
        return false;
      }
    }
    return true;
  }

  size_t find_next_prime(size_t n)
  {
    if (n <= 2) {
      return 2;
    }
    if (n % 2 == 0) {
      ++n;
    }

    while (!is_prime(n)) {
      n += 2;
    }
    return n;
  }
}

namespace gavrilova {
  template < typename Key, typename Value, typename Hash = std::hash< Key >, typename KeyEqual = std::equal_to< Key > >
  class HashTable {
  public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair< const Key, Value >;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = gavrilova::IteratorHashTable< Key, Value, Hash, KeyEqual >;
    using const_iterator = gavrilova::ConstIteratorHashTable< Key, Value, Hash, KeyEqual >;

    explicit HashTable(size_type initial_buckets = 19);
    HashTable(std::initializer_list< value_type > init_list);
    template < typename InputIt >
    HashTable(InputIt first, InputIt last);

    HashTable(const HashTable& other);
    HashTable& operator=(const HashTable& other);

    HashTable(HashTable&& other) noexcept;
    HashTable& operator=(HashTable&& other) noexcept;

    ~HashTable();

    std::pair< iterator, bool > insert(const value_type& value);
    std::pair< iterator, bool > insert(value_type&& value);

    template < typename InputIt >
    void insert(InputIt first, InputIt last);

    template < typename... Args >
    std::pair< iterator, bool > emplace(Args&&... args);

    size_type erase(const Key& key);
    iterator erase(const_iterator pos);

    void clear() noexcept;
    void swap(HashTable& other) noexcept;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;

    Value& operator[](const Key& key);
    Value& operator[](Key&& key);

    iterator find(const Key& key);
    const_iterator find(const Key& key) const;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type bucket_count() const noexcept;

    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float ml);

    void rehash(size_type count);

  private:
    enum class SlotState {
      EMPTY,
      OCCUPIED,
      DELETED
    };

    struct Slot {
      typename std::aligned_storage< sizeof(value_type), alignof(value_type) >::type value_buffer_;
      SlotState state_ = SlotState::EMPTY;

      value_type* get_value_ptr()
      {
        return reinterpret_cast< value_type* >(&value_buffer_);
      }
      const value_type* get_value_ptr() const
      {
        return reinterpret_cast< const value_type* >(&value_buffer_);
      }

      Slot() = default;

      ~Slot()
      {
        destroy();
      }

      Slot(const Slot& other):
        state_(other.state_)
      {
        if (state_ == SlotState::OCCUPIED) {
          new (&value_buffer_) value_type(*other.get_value_ptr());
        }
      }

      Slot(Slot&& other) noexcept:
        state_(other.state_)
      {
        if (state_ == SlotState::OCCUPIED) {
          new (&value_buffer_) value_type(std::move(*other.get_value_ptr()));
          other.destroy();
          other.state_ = SlotState::EMPTY;
        }
      }

      Slot& operator=(const Slot& other) = delete;

      Slot& operator=(Slot&& other) noexcept
      {
        if (this != &other) {
          destroy();

          state_ = other.state_;

          if (state_ == SlotState::OCCUPIED) {
            new (&value_buffer_) value_type(std::move(*other.get_value_ptr()));
            other.destroy();
            other.state_ = SlotState::EMPTY;
          }
        }
        return *this;
      }

      void destroy()
      {
        if (state_ == SlotState::OCCUPIED) {
          get_value_ptr()->~value_type();
        }
      }
    };

    std::vector< Slot > buckets_;
    size_type element_count_;
    float max_load_factor_;
    Hash hasher_;
    KeyEqual key_equal_;

    friend class IteratorHashTable< Key, Value, Hash, KeyEqual >;
    friend class ConstIteratorHashTable< Key, Value, Hash, KeyEqual >;

    std::pair< size_type, bool > find_slot_for_insert(const Key& key) const;
    std::pair< size_type, bool > find_slot_for_read(const Key& key) const;
    size_type find_next_occupied(size_type start_index) const;
  };

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  HashTable< Key, Value, Hash, KeyEqual >::HashTable(size_type initial_buckets):
    buckets_(::find_next_prime(initial_buckets)),
    element_count_(0),
    max_load_factor_(0.75f),
    hasher_(),
    key_equal_()
  {}

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  HashTable< Key, Value, Hash, KeyEqual >::HashTable(std::initializer_list< value_type > init_list):
    HashTable(::find_next_prime(init_list.size() / 0.75f + 1))
  {
    insert(init_list.begin(), init_list.end());
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  template < typename InputIt >
  HashTable< Key, Value, Hash, KeyEqual >::HashTable(InputIt first, InputIt last):
    HashTable(::find_next_prime(std::distance(first, last) / 0.75f + 1))
  {
    insert(first, last);
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  HashTable< Key, Value, Hash, KeyEqual >::HashTable(const HashTable& other):
    buckets_(other.buckets_.size()),
    element_count_(0),
    max_load_factor_(other.max_load_factor_),
    hasher_(other.hasher_),
    key_equal_(other.key_equal_)
  {
    for (size_type i = 0; i < other.buckets_.size(); ++i) {
      if (other.buckets_[i].state_ == SlotState::OCCUPIED) {
        emplace(*other.buckets_[i].get_value_ptr());
      }
    }
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  HashTable< Key, Value, Hash, KeyEqual >& HashTable< Key, Value, Hash, KeyEqual >::operator=(const HashTable& other)
  {
    if (this != &other) {
      HashTable temp(other);
      swap(temp);
    }
    return *this;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  HashTable< Key, Value, Hash, KeyEqual >::HashTable(HashTable&& other) noexcept:
    buckets_(std::move(other.buckets_)),
    element_count_(other.element_count_),
    max_load_factor_(other.max_load_factor_),
    hasher_(std::move(other.hasher_)),
    key_equal_(std::move(other.key_equal_))
  {
    other.element_count_ = 0;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  HashTable< Key, Value, Hash, KeyEqual >& HashTable< Key, Value, Hash, KeyEqual >::operator=(HashTable&& other) noexcept
  {
    if (this != &other) {
      clear();

      buckets_ = std::move(other.buckets_);
      element_count_ = other.element_count_;
      max_load_factor_ = other.max_load_factor_;
      hasher_ = std::move(other.hasher_);
      key_equal_ = std::move(other.key_equal_);

      other.element_count_ = 0;
    }
    return *this;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  HashTable< Key, Value, Hash, KeyEqual >::~HashTable()
  {
    clear();
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  std::pair< typename HashTable< Key, Value, Hash, KeyEqual >::iterator, bool >
  HashTable< Key, Value, Hash, KeyEqual >::insert(const value_type& value)
  {
    return emplace(value);
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  std::pair< typename HashTable< Key, Value, Hash, KeyEqual >::iterator, bool >
  HashTable< Key, Value, Hash, KeyEqual >::insert(value_type&& value)
  {
    return emplace(std::move(value));
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  template < typename InputIt >
  void HashTable< Key, Value, Hash, KeyEqual >::insert(InputIt first, InputIt last)
  {
    for (auto it = first; it != last; ++it) {
      insert(*it);
    }
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  template < typename... Args >
  std::pair< typename HashTable< Key, Value, Hash, KeyEqual >::iterator, bool >
  HashTable< Key, Value, Hash, KeyEqual >::emplace(Args&&... args)
  {
    value_type temp_pair(std::forward< Args >(args)...);
    const Key& key = temp_pair.first;

    auto pair_result = find_slot_for_insert(key);
    size_type index = pair_result.first;
    bool found_existing = pair_result.second;

    if (found_existing) {
      return {iterator(this, index), false};
    }

    if ((static_cast< float >(element_count_ + 1) / buckets_.size()) >= max_load_factor_) {
      size_type new_size = static_cast< size_type >((element_count_ + 1) / max_load_factor_ + 1);
      rehash(std::max(buckets_.size() * 2, new_size));
      pair_result = find_slot_for_insert(key);
      index = pair_result.first;
    }

    if (buckets_[index].state_ == SlotState::DELETED) {
      buckets_[index].destroy();
    }

    new (buckets_[index].get_value_ptr()) value_type(std::move(temp_pair));
    buckets_[index].state_ = SlotState::OCCUPIED;
    element_count_++;

    return {iterator(this, index), true};
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename HashTable< Key, Value, Hash, KeyEqual >::size_type
  HashTable< Key, Value, Hash, KeyEqual >::erase(const Key& key)
  {
    auto it = find(key);
    if (it == end()) {
      return 0;
    }
    erase(it);
    return 1;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename HashTable< Key, Value, Hash, KeyEqual >::iterator
  HashTable< Key, Value, Hash, KeyEqual >::erase(const_iterator pos)
  {
    if (pos == cend() || pos.table_ != this || buckets_[pos.index_].state_ != SlotState::OCCUPIED) {
      return end();
    }

    buckets_[pos.index_].destroy();
    buckets_[pos.index_].state_ = SlotState::DELETED;
    element_count_--;

    auto next_it = iterator(this, pos.index_);
    ++next_it;
    return next_it;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  void HashTable< Key, Value, Hash, KeyEqual >::clear() noexcept
  {
    for (size_type i = 0; i < buckets_.size(); ++i) {
      buckets_[i].destroy();
      buckets_[i].state_ = SlotState::EMPTY;
    }
    element_count_ = 0;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  void HashTable< Key, Value, Hash, KeyEqual >::swap(HashTable& other) noexcept
  {
    std::swap(buckets_, other.buckets_);
    std::swap(element_count_, other.element_count_);
    std::swap(max_load_factor_, other.max_load_factor_);
    std::swap(hasher_, other.hasher_);
    std::swap(key_equal_, other.key_equal_);
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  Value& HashTable< Key, Value, Hash, KeyEqual >::at(const Key& key)
  {
    auto it = find(key);
    if (it == end()) {
      throw std::out_of_range("HashTable::at: key not found");
    }
    return const_cast< Value& >(it->second);
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  const Value& HashTable< Key, Value, Hash, KeyEqual >::at(const Key& key) const
  {
    auto it = find(key);
    if (it == cend()) {
      throw std::out_of_range("HashTable::at: key not found");
    }
    return it->second;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  Value& HashTable< Key, Value, Hash, KeyEqual >::operator[](const Key& key)
  {
    auto pair_result = find_slot_for_insert(key);
    size_type index = pair_result.first;
    bool found_existing = pair_result.second;

    if (found_existing) {
      return const_cast< Value& >(buckets_[index].get_value_ptr()->second);
    } else {
      if ((static_cast< float >(element_count_ + 1) / buckets_.size()) >= max_load_factor_) {
        size_type new_size = static_cast< size_type >((element_count_ + 1) / max_load_factor_ + 1);
        rehash(std::max(buckets_.size() * 2, new_size));

        pair_result = find_slot_for_insert(key);
        index = pair_result.first;
      }

      if (buckets_[index].state_ == SlotState::DELETED) {
        buckets_[index].destroy();
      }

      new (buckets_[index].get_value_ptr()) value_type(key, Value{});
      buckets_[index].state_ = SlotState::OCCUPIED;
      element_count_++;
      return const_cast< Value& >(buckets_[index].get_value_ptr()->second);
    }
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  Value& HashTable< Key, Value, Hash, KeyEqual >::operator[](Key&& key)
  {
    auto pair_result = find_slot_for_insert(key);
    size_type index = pair_result.first;
    bool found_existing = pair_result.second;

    if (found_existing) {
      return const_cast< Value& >(buckets_[index].get_value_ptr()->second);
    } else {
      if ((static_cast< float >(element_count_ + 1) / buckets_.size()) >= max_load_factor_) {
        size_type new_size = static_cast< size_type >((element_count_ + 1) / max_load_factor_ + 1);
        rehash(std::max(buckets_.size() * 2, new_size));
        pair_result = find_slot_for_insert(key);
        index = pair_result.first;
      }

      if (buckets_[index].state_ == SlotState::DELETED) {
        buckets_[index].destroy();
      }

      new (buckets_[index].get_value_ptr()) value_type(std::move(key), Value{});
      buckets_[index].state_ = SlotState::OCCUPIED;
      element_count_++;
      return const_cast< Value& >(buckets_[index].get_value_ptr()->second);
    }
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename HashTable< Key, Value, Hash, KeyEqual >::iterator
  HashTable< Key, Value, Hash, KeyEqual >::find(const Key& key)
  {
    auto pair_result = find_slot_for_read(key);
    size_type index = pair_result.first;
    bool found = pair_result.second;
    return found ? iterator(this, index) : end();
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename HashTable< Key, Value, Hash, KeyEqual >::const_iterator
  HashTable< Key, Value, Hash, KeyEqual >::find(const Key& key) const
  {
    auto pair_result = find_slot_for_read(key);
    size_type index = pair_result.first;
    bool found = pair_result.second;
    return found ? const_iterator(this, index) : cend();
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename HashTable< Key, Value, Hash, KeyEqual >::iterator
  HashTable< Key, Value, Hash, KeyEqual >::begin()
  {
    return iterator(this, find_next_occupied(0));
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename HashTable< Key, Value, Hash, KeyEqual >::iterator
  HashTable< Key, Value, Hash, KeyEqual >::end()
  {
    return iterator(this, buckets_.size());
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename HashTable< Key, Value, Hash, KeyEqual >::const_iterator
  HashTable< Key, Value, Hash, KeyEqual >::begin() const
  {
    return cbegin();
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename HashTable< Key, Value, Hash, KeyEqual >::const_iterator
  HashTable< Key, Value, Hash, KeyEqual >::end() const
  {
    return cend();
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename HashTable< Key, Value, Hash, KeyEqual >::const_iterator
  HashTable< Key, Value, Hash, KeyEqual >::cbegin() const
  {
    return const_iterator(this, find_next_occupied(0));
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename HashTable< Key, Value, Hash, KeyEqual >::const_iterator
  HashTable< Key, Value, Hash, KeyEqual >::cend() const
  {
    return const_iterator(this, buckets_.size());
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  bool HashTable< Key, Value, Hash, KeyEqual >::empty() const noexcept
  {
    return element_count_ == 0;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename HashTable< Key, Value, Hash, KeyEqual >::size_type
  HashTable< Key, Value, Hash, KeyEqual >::size() const noexcept
  {
    return element_count_;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename HashTable< Key, Value, Hash, KeyEqual >::size_type
  HashTable< Key, Value, Hash, KeyEqual >::bucket_count() const noexcept
  {
    return buckets_.size();
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  float HashTable< Key, Value, Hash, KeyEqual >::load_factor() const noexcept
  {
    return buckets_.empty() ? 0.0f : static_cast< float >(element_count_) / buckets_.size();
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  float HashTable< Key, Value, Hash, KeyEqual >::max_load_factor() const noexcept
  {
    return max_load_factor_;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  void HashTable< Key, Value, Hash, KeyEqual >::max_load_factor(float ml)
  {
    if (ml <= 0.0f) {
      throw std::out_of_range("Max load factor must be positive.");
    }
    max_load_factor_ = ml;

    if (load_factor() >= max_load_factor_ && element_count_ > 0) {
      rehash(static_cast< size_type >(element_count_ / max_load_factor_) + 1);
    }
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  void HashTable< Key, Value, Hash, KeyEqual >::rehash(size_type count)
  {
    size_type new_num_buckets = ::find_next_prime(std::max(count,
        static_cast< size_type >(element_count_ / max_load_factor_ + 1)));

    if (new_num_buckets <= buckets_.size() && new_num_buckets != 0) {
      return;
    }

    HashTable temp_table(new_num_buckets);

    for (size_type i = 0; i < buckets_.size(); ++i) {
      if (buckets_[i].state_ == SlotState::OCCUPIED) {
        temp_table.emplace(std::move(*buckets_[i].get_value_ptr()));
        buckets_[i].destroy();
        buckets_[i].state_ = SlotState::EMPTY;
      }
    }

    swap(temp_table);
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  std::pair< typename HashTable< Key, Value, Hash, KeyEqual >::size_type, bool >
  HashTable< Key, Value, Hash, KeyEqual >::find_slot_for_insert(const Key& key) const
  {
    if (buckets_.empty()) {
      return {0, false};
    }

    size_t hash = hasher_(key);
    size_t index = hash % buckets_.size();
    size_t first_deleted = buckets_.size();

    for (size_t i = 0; i < buckets_.size(); ++i) {
      size_t current = (index + i) % buckets_.size();

      if (buckets_[current].state_ == SlotState::EMPTY) {
        return {first_deleted != buckets_.size() ? first_deleted : current, false};
      }

      if (buckets_[current].state_ == SlotState::OCCUPIED &&
          key_equal_(buckets_[current].get_value_ptr()->first, key)) {
        return {current, true};
      }

      if (buckets_[current].state_ == SlotState::DELETED && first_deleted == buckets_.size()) {
        first_deleted = current;
      }
    }

    return {first_deleted != buckets_.size() ? first_deleted : buckets_.size(), false};
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  std::pair< typename HashTable< Key, Value, Hash, KeyEqual >::size_type, bool >
  HashTable< Key, Value, Hash, KeyEqual >::find_slot_for_read(const Key& key) const
  {
    if (buckets_.empty()) {
      return {buckets_.size(), false};
    }

    size_t hash = hasher_(key);
    size_t index = hash % buckets_.size();

    for (size_t i = 0; i < buckets_.size(); ++i) {
      size_t current = (index + i) % buckets_.size();

      if (buckets_[current].state_ == SlotState::EMPTY) {
        return {buckets_.size(), false};
      }

      if (buckets_[current].state_ == SlotState::OCCUPIED &&
          key_equal_(buckets_[current].get_value_ptr()->first, key)) {
        return {current, true};
      }
    }
    return {buckets_.size(), false};
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename HashTable< Key, Value, Hash, KeyEqual >::size_type
  HashTable< Key, Value, Hash, KeyEqual >::find_next_occupied(size_type start_index) const
  {
    for (size_t i = start_index; i < buckets_.size(); ++i) {
      if (buckets_[i].state_ == SlotState::OCCUPIED) {
        return i;
      }
    }
    return buckets_.size();
  }

}

#endif
