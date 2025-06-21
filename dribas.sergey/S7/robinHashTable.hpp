#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>
#include <utility>

#include <boost/hash2/fnv1a.hpp>

#include "robinConstIterator.hpp"
#include "robinIterator.hpp"
#include "entryState.hpp"

namespace dribas
{
  struct fnv1a
  {
    template < class Key >
    size_t operator()(const Key& key) const noexcept
    {
      return boost::hash_value(key);
    }
    size_t operator()(const std::string& key) const noexcept
    {
      return boost::hash_value(key);
    }
    size_t operator()(const std::pair<std::string, std::string>& key) const noexcept
    {
      size_t seed = 0;
      boost::hash_combine(seed, key.first);
      boost::hash_combine(seed, key.second);
      return seed;
    }
  };

  template< class Key, class T >
  struct Entry
  {
    std::pair< Key, T > data;
    size_t probe_distance;
    EntryState state;

    Entry();
    Entry(const Key& k, const T& v, size_t pd, EntryState s);
  };

  template< class Key, class T >
  Entry< Key, T >::Entry():
    probe_distance(0),
    state(EntryState::EMPTY)
  {}

  template< class Key, class T >
  Entry< Key, T >::Entry(const Key& k, const T& v, size_t pd, EntryState s):
    data(k, v),
    probe_distance(pd),
    state(s)
  {}


  template< class Key, class T, class Hash = fnv1a >
  class RobinHoodHashTable
  {
  public:
    using value_type = std::pair< const Key, T >;
    using const_iterator = ConstIterator< Key, T, Hash >;
    using iterator = Iterator< Key, T, Hash >;

    explicit RobinHoodHashTable(size_t initial_capacity = 8);
    RobinHoodHashTable(std::initializer_list< value_type > init, size_t initial_capacity = 8);
    template< class InputIt >
    RobinHoodHashTable(InputIt first, InputIt last, size_t initial_capacity = 8);
    RobinHoodHashTable(const RobinHoodHashTable< Key, T, Hash >& other);
    RobinHoodHashTable(RobinHoodHashTable< Key, T, Hash >&& other) noexcept;

    RobinHoodHashTable& operator=(const RobinHoodHashTable< Key, T, Hash >& other);
    RobinHoodHashTable& operator=(RobinHoodHashTable< Key, T, Hash >&& other) noexcept;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    void insert(const Key& key, const T& value);
    template< class InputIt >
    void insert(InputIt first, InputIt last);
    bool erase(const Key& key);
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    void clear() noexcept;
    void swap(RobinHoodHashTable< Key, T, Hash >& other) noexcept;

    T& operator[](const Key& key);
    T& at(const Key& key);
    const T& at(const Key& key) const;

    iterator find(const Key& key);
    const_iterator find(const Key& key) const;

    double load_factor() const;
    double max_load_factor() const;
    void max_load_factor(double ml);
    void rehash(size_t new_capacity);

    size_t size() const noexcept;
    bool empty() const noexcept;

  private:
    std::vector< Entry< Key, T > > table_;
    size_t num_elements_;
    Hash hasher_;
    double max_load_;

    size_t hash_index(const Key& key, size_t capacity_param) const;
    bool insert(const Key& key, const T& value, std::vector< Entry< Key, T> >& target_table);
    void grow_and_rehash_if_needed();
  };

  template< class Key, class T, class Hash >
  RobinHoodHashTable< Key, T, Hash >::RobinHoodHashTable(const RobinHoodHashTable& other):
    table_(other.table_),
    num_elements_(other.num_elements_),
    hasher_(other.hasher_),
    max_load_(other.max_load_)
  {}

  template< class Key, class T, class Hash >
  RobinHoodHashTable< Key, T, Hash >::RobinHoodHashTable(RobinHoodHashTable&& other) noexcept:
    table_(std::move(other.table_)),
    num_elements_(std::exchange(other.num_elements_, 0)),
    hasher_(std::move(other.hasher_)),
    max_load_(std::exchange(other.max_load_, 0.0))
  {}

  template< class Key, class T, class Hash >
  RobinHoodHashTable< Key, T, Hash >&
  RobinHoodHashTable< Key, T, Hash >::operator=(RobinHoodHashTable&& other) noexcept
  {
    if (this != std::addressof(other)) {
      RobinHoodHashTable< Key, T, Hash > copy(std::move(other));
      swap(copy);
    }
    return *this;
  }

  template< class Key, class T, class Hash >
  RobinHoodHashTable< Key, T, Hash >&
  RobinHoodHashTable< Key, T, Hash >::operator=(const RobinHoodHashTable& other)
  {
    if (this != std::addressof(other)) {
      RobinHoodHashTable< Key, T, Hash > temp(other);
      swap(temp);
    }
    return *this;
  }

  template< class Key, class T, class Hash >
  RobinHoodHashTable< Key, T, Hash >::RobinHoodHashTable(size_t initial_capacity):
    num_elements_(0),
    hasher_(),
    max_load_(0.75)
  {
    if (initial_capacity == 0) {
      initial_capacity = 8;
    }
    table_.resize(initial_capacity);
  }

  template< class Key, class T, class Hash >
  RobinHoodHashTable< Key, T, Hash >::RobinHoodHashTable(std::initializer_list< value_type > init, size_t initial_capacity):
    RobinHoodHashTable(initial_capacity)
  {
    for (const auto& pair: init) {
      insert(pair.first, pair.second);
    }
  }

  template< class Key, class T, class Hash >
  template< class InputIt >
  RobinHoodHashTable< Key, T, Hash >::RobinHoodHashTable(InputIt first, InputIt last, size_t initial_capacity):
    RobinHoodHashTable(initial_capacity)
  {
    insert(first, last);
  }

  template< class Key, class T, class Hash >
  Iterator< Key, T, Hash > RobinHoodHashTable< Key, T, Hash >::begin()
  {
    Entry< Key, T >* first_occupied = table_.data();
    Entry< Key, T >* table_end_ptr = table_.data() + table_.size();

    while (first_occupied != table_end_ptr && first_occupied->state != EntryState::OCCUPIED) {
      first_occupied++;
    }
    return iterator(first_occupied, table_end_ptr);
  }

  template< class Key, class T, class Hash >
  Iterator< Key, T, Hash >
  RobinHoodHashTable< Key, T, Hash >::end()
  {
    return iterator(table_.data() + table_.size(), table_.data() + table_.size());
  }

  template< class Key, class T, class Hash >
  ConstIterator< Key, T, Hash >
  RobinHoodHashTable< Key, T, Hash >::begin() const
  {
    const Entry< Key, T >* first_occupied = table_.data();
    const Entry< Key, T >* table_end_ptr = table_.data() + table_.size();

    while (first_occupied != table_end_ptr && first_occupied->state != EntryState::OCCUPIED) {
      first_occupied++;
    }
    return const_iterator(first_occupied, table_end_ptr);
  }

  template< class Key, class T, class Hash >
  ConstIterator< Key, T, Hash >
  RobinHoodHashTable< Key, T, Hash >::end() const
  {
    return const_iterator(table_.data() + table_.size(), table_.data() + table_.size());
  }

  template< class Key, class T, class Hash >
  ConstIterator< Key, T, Hash >
  RobinHoodHashTable< Key, T, Hash >::cbegin() const
  {
    return static_cast<const RobinHoodHashTable*>(this)->begin();
  }

  template< class Key, class T, class Hash >
  ConstIterator< Key, T, Hash >
  RobinHoodHashTable< Key, T, Hash >::cend() const
  {
    return static_cast<const RobinHoodHashTable*>(this)->end();
  }

  template< class Key, class T, class Hash >
  void RobinHoodHashTable< Key, T, Hash >::insert(const Key& key, const T& value)
  {
    grow_and_rehash_if_needed();
    if (insert(key, value, table_)) {
      num_elements_++;
    }
  }

  template< class Key, class T, class Hash >
  bool RobinHoodHashTable< Key, T, Hash >::erase(const Key& key)
  {
    if (empty()) {
      return false;
    }

    size_t index = hash_index(key, table_.size());
    size_t initial_index = index;
    size_t current_probe_distance = 0;

    while (table_[index].state != EntryState::EMPTY) {
      if (table_[index].state == EntryState::OCCUPIED &&
        table_[index].data.first == key &&
          table_[index].probe_distance <= current_probe_distance) {
        table_[index].state = EntryState::DELETED;
        num_elements_--;
        return true;
      }
      if (table_[index].probe_distance < current_probe_distance) {
        return false;
      }

      index = (index + 1) % table_.size();
      current_probe_distance++;

      if (index == initial_index && current_probe_distance > 0) {
        break;
      }
    }
    return false;
  }

  template< class Key, class T, class Hash >
  Iterator< Key, T, Hash >
  RobinHoodHashTable< Key, T, Hash >::erase(iterator pos)
  {
    if (pos == end()) {
      return end();
    }
    Entry< Key, T >* entry_ptr = reinterpret_cast<Entry< Key, T >*>(&(*pos));
    if (entry_ptr->state == EntryState::OCCUPIED) {
      entry_ptr->state = EntryState::DELETED;
      num_elements_--;
    }
    return ++pos;
  }

  template< class Key, class T, class Hash >
  Iterator< Key, T, Hash >
  RobinHoodHashTable< Key, T, Hash >::erase(iterator first, iterator last)
  {
    while (first != last) {
      first = erase(first);
    }
    return first;
  }

  template< class Key, class T, class Hash >
  void RobinHoodHashTable< Key, T, Hash >::clear() noexcept
  {
    for (auto& entry : table_) {
      entry.state = EntryState::EMPTY;
      entry.probe_distance = 0;
    }
    num_elements_ = 0;
  }

  template< class Key, class T, class Hash >
  void RobinHoodHashTable< Key, T, Hash >::swap(RobinHoodHashTable& other) noexcept
  {
    std::swap(table_, other.table_);
    std::swap(num_elements_, other.num_elements_);
    std::swap(hasher_, other.hasher_);
    std::swap(max_load_, other.max_load_);
  }

  template< class Key, class T, class Hash >
  template< class InputIt >
  void RobinHoodHashTable< Key, T, Hash >::insert(InputIt first, InputIt last)
  {
    for (; first != last; first++) {
      insert(first->first, first->second);
    }
  }

  template< class Key, class T, class Hash >
  T& RobinHoodHashTable< Key, T, Hash >::operator[](const Key& key)
  {
    auto it = find(key);
    if (it != end()) {
      return it->second;
    }
    insert(key, T{});
    return find(key)->second;
  }

  template< class Key, class T, class Hash >
  T& RobinHoodHashTable< Key, T, Hash >::at(const Key& key)
  {
    auto it = find(key);
    if (it == end()) {
      throw std::out_of_range("Key not found in RobinHoodHashTable::at");
    }
    return it->second;
  }

  template< class Key, class T, class Hash >
  const T& RobinHoodHashTable< Key, T, Hash >::at(const Key& key) const
  {
    auto it = find(key);
    if (it == cend()) {
      throw std::out_of_range("Key not found in RobinHoodHashTable::at (const)");
    }
    return it->second;
  }

  template< class Key, class T, class Hash >
  Iterator< Key, T, Hash > RobinHoodHashTable< Key, T, Hash >::find(const Key& key)
  {
    if (empty()) {
      return end();
    }

    size_t index = hash_index(key, table_.size());
    size_t initial_index = index;
    size_t current_probe_distance = 0;

    while (table_[index].state != EntryState::EMPTY) {
      if (table_[index].state == EntryState::OCCUPIED &&
          table_[index].data.first == key &&
          table_[index].probe_distance <= current_probe_distance) {
        return iterator(&table_[index], table_.data() + table_.size());
      }
      if (table_[index].probe_distance < current_probe_distance) {
        return end();
      }

      index = (index + 1) % table_.size();
      current_probe_distance++;

      if (index == initial_index && current_probe_distance > 0) {
        break;
      }
    }
    return end();
  }

  template< class Key, class T, class Hash >
  ConstIterator< Key, T, Hash >
  RobinHoodHashTable< Key, T, Hash >::find(const Key& key) const
  {
    if (empty()) {
      return end();
    }
    size_t index = hash_index(key, table_.size());
    size_t initial_index = index;
    size_t current_probe_distance = 0;

    while (table_[index].state != EntryState::EMPTY) {
      if (table_[index].state == EntryState::OCCUPIED &&
          table_[index].data.first == key &&
          table_[index].probe_distance <= current_probe_distance) {
        return const_iterator(&table_[index], table_.data() + table_.size());
      }
      if (table_[index].probe_distance < current_probe_distance) {
        return end();
      }
      index = (index + 1) % table_.size();
      current_probe_distance++;
      if (index == initial_index && current_probe_distance > 0) {
        break;
      }
    }
    return end();
  }

  template< class Key, class T, class Hash >
  double RobinHoodHashTable< Key, T, Hash >::load_factor() const
  {
    if (table_.empty()) {
      return 0.0;
    }
    return static_cast<double>(num_elements_) / table_.size();
  }

  template< class Key, class T, class Hash >
  double RobinHoodHashTable< Key, T, Hash >::max_load_factor() const
  {
    return max_load_;
  }

  template< class Key, class T, class Hash >
  void RobinHoodHashTable< Key, T, Hash >::max_load_factor(double ml)
  {
    if (ml <= 0.0) {
      throw std::invalid_argument("Max load factor must be greater than 0.");
    }
    max_load_ = ml;
    grow_and_rehash_if_needed();
  }

  template< class Key, class T, class Hash >
  void RobinHoodHashTable< Key, T, Hash >::rehash(size_t new_capacity)
  {
    if (new_capacity < num_elements_) {
      throw std::runtime_error("New capacity is too small for existing elements.");
    }
    if (new_capacity < 8) {
      new_capacity = 8;
    }

    std::vector< Entry< Key, T > > new_table(new_capacity);
    size_t rehashed_elements_count = 0;

    for (const auto& entry : table_) {
      if (entry.state == EntryState::OCCUPIED) {
        insert(entry.data.first, entry.data.second, new_table);
        rehashed_elements_count++;
      }
    }
    table_ = std::move(new_table);
    num_elements_ = rehashed_elements_count;
  }

  template< class Key, class T, class Hash >
  size_t RobinHoodHashTable< Key, T, Hash >::size() const noexcept
  {
    return num_elements_;
  }

  template< class Key, class T, class Hash >
  bool RobinHoodHashTable< Key, T, Hash >::empty() const noexcept
  {
    return num_elements_ == 0;
  }

  template< class Key, class T, class Hash >
  size_t RobinHoodHashTable< Key, T, Hash >::hash_index(const Key& key, size_t capacity_param) const
  {
    return hasher_(key) % capacity_param;
  }

  template< class Key, class T, class Hash >
  bool RobinHoodHashTable< Key, T, Hash >::insert(const Key& key, const T& value, std::vector<Entry< Key, T > >& target_table)
  {
    size_t current_capacity = target_table.size();
    size_t index = hash_index(key, current_capacity);
    size_t current_probe_distance = 0;

    Entry< Key, T > new_entry(key, value, current_probe_distance, EntryState::OCCUPIED);

    bool new_key_inserted = false;

    while (true) {
      if (target_table[index].state == EntryState::EMPTY || target_table[index].state == EntryState::DELETED) {
        if (target_table[index].state == EntryState::EMPTY) {
          new_key_inserted = true;
        }
        target_table[index] = std::move(new_entry);
        return new_key_inserted;
      }

      if (target_table[index].state == EntryState::OCCUPIED && target_table[index].data.first == key) {
        target_table[index].data.second = value;
        return false;
      }

      if (target_table[index].probe_distance < new_entry.probe_distance) {
        Entry< Key, T > displaced_entry = std::move(target_table[index]);
        target_table[index] = std::move(new_entry);
        new_entry = std::move(displaced_entry);
      }
      new_entry.probe_distance++;
      index = (index + 1) % current_capacity;
      current_probe_distance++;
    }
  }

  template< class Key, class T, class Hash >
  void RobinHoodHashTable< Key, T, Hash >::grow_and_rehash_if_needed()
  {
    if (static_cast<double>(num_elements_) / table_.size() >= max_load_) {
      rehash(table_.size() * 2);
    }
  }

  template< class Key, class T, class Hash >
  void swap(RobinHoodHashTable< Key, T, Hash >& lhs, RobinHoodHashTable< Key, T, Hash >& rhs) noexcept
  {
    lhs.swap(rhs);
  }

}

#endif
