#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "array.hpp"

#include "bucket.hpp"
#include "hash_table_iterators.hpp"

namespace zholobov {

  template < class Key, class T, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable {
  public:
    using key_type = Key;
    using mapped_type = T;
    using iterator = HashTableIterator< Key, T, false >;
    using const_iterator = HashTableIterator< Key, T, true >;
    using size_type = size_t;

    HashTable();

    template < typename InputIt >
    HashTable(InputIt first, InputIt last);

    explicit HashTable(std::initializer_list< std::pair< Key, T > >);

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    T& at(const Key& key);
    const T& at(const Key& key) const;
    T& operator[](const Key& key);

    bool empty() const noexcept;
    size_type size() const noexcept;
    void clear() noexcept;

    std::pair< iterator, bool > insert(const std::pair< Key, T >& value);
    template < typename InputIt >
    void insert(InputIt first, InputIt last);

    iterator erase(iterator pos) noexcept;
    size_type erase(const Key& key) noexcept;
    iterator erase(iterator first, iterator last) noexcept;

    template < typename K, typename V >
    std::pair< iterator, bool > emplace(K&& key, V&& value);

    void swap(HashTable& other) noexcept;

    iterator find(const Key& key);
    const_iterator find(const Key& key) const;
    size_type count(const Key& key) const;

    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float mlf);
    void rehashFactor(size_t count);

  private:
    explicit HashTable(size_t bucketCount, const Hash& hash = Hash(), const Equal& equal = Equal());
    size_t probe(size_t hash, size_t i) const noexcept;
    void rehash(size_t new_capacity);
    static bool is_prime(size_t n) noexcept;
    static size_t next_prime(size_t n) noexcept;

    Array< Bucket< Key, T > > buckets_;
    size_type size_ = 0;
    float maxLoadFactor_ = 0.75f;
    Hash hash_;
    Equal equal_;
  };

  template < class Key, class T, class Hash, class Equal >
  HashTable< Key, T, Hash, Equal >::HashTable():
    HashTable(11)
  {}

  template < class Key, class T, class Hash, class Equal >
  template < typename InputIt >
  HashTable< Key, T, Hash, Equal >::HashTable(InputIt first, InputIt last)
  {
    for (auto it = first; it != last; it++) {
      insert(*it);
    }
  }

  template < class Key, class T, class Hash, class Equal >
  HashTable< Key, T, Hash, Equal >::HashTable(std::initializer_list< std::pair< Key, T > > initList):
    HashTable(initList.begin(), initList.end())
  {}

  template < class Key, class T, class Hash, class Equal >
  typename HashTable< Key, T, Hash, Equal >::iterator HashTable< Key, T, Hash, Equal >::begin() noexcept
  {
    auto beginPtr = std::addressof(buckets_.front());
    auto endPtr = beginPtr + buckets_.size();
    return iterator(beginPtr, endPtr);
  }

  template < class Key, class T, class Hash, class Equal >
  typename HashTable< Key, T, Hash, Equal >::iterator HashTable< Key, T, Hash, Equal >::end() noexcept
  {
    auto beginPtr = std::addressof(buckets_.front());
    auto endPtr = beginPtr + buckets_.size();
    return iterator(endPtr, endPtr);
  }

  template < class Key, class T, class Hash, class Equal >
  typename HashTable< Key, T, Hash, Equal >::const_iterator HashTable< Key, T, Hash, Equal >::cbegin() const noexcept
  {
    auto beginPtr = std::addressof(buckets_.front());
    auto endPtr = beginPtr + buckets_.size();
    return const_iterator(beginPtr, endPtr);
  }

  template < class Key, class T, class Hash, class Equal >
  typename HashTable< Key, T, Hash, Equal >::const_iterator HashTable< Key, T, Hash, Equal >::cend() const noexcept
  {
    auto beginPtr = std::addressof(buckets_.front());
    auto endPtr = beginPtr + buckets_.size();
    return const_iterator(endPtr, endPtr);
  }

  template < class Key, class T, class Hash, class Equal >
  T& HashTable< Key, T, Hash, Equal >::at(const Key& key)
  {
    auto it = find(key);
    if (it == end()) {
      throw std::out_of_range("Key not found");
    }
    return it->second;
  }

  template < class Key, class T, class Hash, class Equal >
  const T& HashTable< Key, T, Hash, Equal >::at(const Key& key) const
  {
    auto it = find(key);
    if (it == end()) {
      throw std::out_of_range("Key not found");
    }
    return it->second;
  }

  template < class Key, class T, class Hash, class Equal >
  T& HashTable< Key, T, Hash, Equal >::operator[](const Key& key)
  {
    auto it = find(key);
    if (it == end()) {
      it = insert({key, T()}).first;
    }
    return it->second;
  }

  template < class Key, class T, class Hash, class Equal >
  bool HashTable< Key, T, Hash, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < class Key, class T, class Hash, class Equal >
  typename HashTable< Key, T, Hash, Equal >::size_type HashTable< Key, T, Hash, Equal >::size() const noexcept
  {
    return size_;
  }

  template < class Key, class T, class Hash, class Equal >
  void HashTable< Key, T, Hash, Equal >::clear() noexcept
  {
    for (size_t i = 0; i < buckets_.size(); ++i) {
      buckets_[i].occupied = false;
      buckets_[i].deleted = false;
    }
    size_ = 0;
  }

  template < class Key, class T, class Hash, class Equal >
  std::pair< typename HashTable< Key, T, Hash, Equal >::iterator, bool >
  HashTable< Key, T, Hash, Equal >::insert(const std::pair< Key, T >& value)
  {
    return emplace(value.first, value.second);
  }

  template < class Key, class T, class Hash, class Equal >
  template < typename InputIt >
  void HashTable< Key, T, Hash, Equal >::insert(InputIt first, InputIt last)
  {
    for (; first != last; ++first) {
      insert(*first);
    }
  }

  template < class Key, class T, class Hash, class Equal >
  typename HashTable< Key, T, Hash, Equal >::iterator HashTable< Key, T, Hash, Equal >::erase(iterator pos) noexcept
  {
    if (pos == end()) {
      return end();
    }

    size_t index = pos.current_ - std::addressof(buckets_.front());
    buckets_[index].occupied = false;
    buckets_[index].deleted = true;
    --size_;
    return iterator(std::addressof(buckets_[index]), std::addressof(buckets_.front()) + buckets_.size());
  }

  template < class Key, class T, class Hash, class Equal >
  typename HashTable< Key, T, Hash, Equal >::size_type HashTable< Key, T, Hash, Equal >::erase(const Key& key) noexcept
  {
    auto it = find(key);
    if (it != end()) {
      erase(it);
      return 1;
    }
    return 0;
  }

  template < class Key, class T, class Hash, class Equal >
  typename HashTable< Key, T, Hash, Equal >::iterator
  HashTable< Key, T, Hash, Equal >::erase(iterator first, iterator last) noexcept
  {
    iterator result = end();
    if (first == last) {
      return result;
    }
    while (first != last) {
      result = erase(first++);
    }
    return result;
  }

  template < class Key, class T, class Hash, class Equal >
  template < typename K, typename V >
  std::pair< typename HashTable< Key, T, Hash, Equal >::iterator, bool >
  HashTable< Key, T, Hash, Equal >::emplace(K&& key, V&& value)
  {
    if (size_ + 1 > maxLoadFactor_ * buckets_.size()) {
      rehash(buckets_.size() * 2);
    }
    size_t hash = hash_(key) % buckets_.size();
    size_t i = 0;
    size_t index = probe(hash, i);
    size_t first_deleted = buckets_.size();

    while (buckets_[index].occupied || buckets_[index].deleted) {
      if (buckets_[index].occupied && equal_(buckets_[index].data.first, key)) {
        if (buckets_[index].deleted) {
          buckets_[index].data = {key, value};
          buckets_[index].occupied = true;
          buckets_[index].deleted = false;
          ++size_;
          return {iterator(std::addressof(buckets_[index]), std::addressof(buckets_.front()) + buckets_.size()), true};
        }
        return {iterator(std::addressof(buckets_[index]), std::addressof(buckets_.front()) + buckets_.size()), false};
      }
      if (buckets_[index].deleted && first_deleted == buckets_.size()) {
        first_deleted = index;
      }
      ++i;
      index = probe(hash, i);
    }
    if (first_deleted != buckets_.size()) {
      index = first_deleted;
    }
    buckets_[index].data = {key, value};
    buckets_[index].occupied = true;
    buckets_[index].deleted = false;
    ++size_;
    return {iterator(std::addressof(buckets_[index]), std::addressof(buckets_.front()) + buckets_.size()), true};
  }

  template < class Key, class T, class Hash, class Equal >
  void HashTable< Key, T, Hash, Equal >::swap(HashTable& other) noexcept
  {
    std::swap(buckets_, other.buckets_);
    std::swap(size_, other.size_);
    std::swap(maxLoadFactor_, other.maxLoadFactor_);
    std::swap(hash_, other.hash_);
    std::swap(equal_, other.equal_);
  }

  template < class Key, class T, class Hash, class Equal >
  typename HashTable< Key, T, Hash, Equal >::iterator HashTable< Key, T, Hash, Equal >::find(const Key& key)
  {
    if (empty()) {
      return end();
    }
    size_t hash = hash_(key) % buckets_.size();
    size_t i = 0;
    size_t index = probe(hash, i);

    while (buckets_[index].occupied || buckets_[index].deleted) {
      if (buckets_[index].occupied && equal_(buckets_[index].data.first, key)) {
        return iterator(std::addressof(buckets_[index]), std::addressof(buckets_.front()) + buckets_.size());
      }
      ++i;
      index = probe(hash, i);
      if (i >= buckets_.size()) {
        break;
      }
    }
    return end();
  }

  template < class Key, class T, class Hash, class Equal >
  typename HashTable< Key, T, Hash, Equal >::const_iterator HashTable< Key, T, Hash, Equal >::find(const Key& key) const
  {
    if (empty()) {
      return cend();
    }
    size_t hash = hash_(key) % buckets_.size();
    size_t i = 0;
    size_t index = probe(hash, i);

    while (buckets_[index].occupied || buckets_[index].deleted) {
      if (buckets_[index].occupied && equal_(buckets_[index].data.first, key)) {
        return const_iterator(std::addressof(buckets_[index]), std::addressof(buckets_.front()) + buckets_.size());
      }
      ++i;
      index = probe(hash, i);
      if (i >= buckets_.size()) {
        break;
      }
    }
    return cend();
  }

  template < class Key, class T, class Hash, class Equal >
  typename HashTable< Key, T, Hash, Equal >::size_type HashTable< Key, T, Hash, Equal >::count(const Key& key) const
  {
    const_iterator found = find(key);
    if (found != cend()) {
      return 1;
    }
    return 0;
  }

  template < class Key, class T, class Hash, class Equal >
  float HashTable< Key, T, Hash, Equal >::load_factor() const noexcept
  {
    if (buckets_.size() == 0) {
      return 0.0;
    }
    float current_size = size_;
    float table_size = buckets_.size();
    return current_size / table_size;
  }

  template < class Key, class T, class Hash, class Equal >
  float HashTable< Key, T, Hash, Equal >::max_load_factor() const noexcept
  {
    return maxLoadFactor_;
  }

  template < class Key, class T, class Hash, class Equal >
  void HashTable< Key, T, Hash, Equal >::max_load_factor(float mlf)
  {
    if (mlf <= 0.0 || mlf > 1.0) {
      throw std::invalid_argument("Invalid max load factor");
    }
    maxLoadFactor_ = mlf;
    if (load_factor() > maxLoadFactor_) {
      rehash(buckets_.size() * 2);
    }
  }

  template < class Key, class T, class Hash, class Equal >
  void HashTable< Key, T, Hash, Equal >::rehashFactor(size_t count)
  {
    size_t min_count = size_ / maxLoadFactor_;
    if (count < min_count) {
      count = min_count;
    }
    if (count != buckets_.size()) {
      rehash(count);
    }
  }

  template < class Key, class T, class Hash, class Equal >
  HashTable< Key, T, Hash, Equal >::HashTable(size_t bucketCount, const Hash& hash, const Equal& equal):
    buckets_(next_prime(bucketCount)),
    hash_(hash),
    equal_(equal)
  {}

  template < class Key, class T, class Hash, class Equal >
  size_t HashTable< Key, T, Hash, Equal >::probe(size_t hash, size_t i) const noexcept
  {
    return (hash + i * i) % buckets_.size();
  }

  template < class Key, class T, class Hash, class Equal >
  void HashTable< Key, T, Hash, Equal >::rehash(size_t new_capacity)
  {
    new_capacity = next_prime(new_capacity);
    Array< Bucket< Key, T > > new_buckets(new_capacity);
    for (size_t i = 0; i < buckets_.size(); ++i) {
      if (buckets_[i].occupied && !buckets_[i].deleted) {
        const auto& key = buckets_[i].data.first;
        size_t hash = hash_(key) % new_capacity;
        size_t j = 0;
        size_t index = probe(hash, j);
        while (new_buckets[index].occupied) {
          j++;
          index = probe(hash, j);
        }
        new_buckets[index].data = std::move(buckets_[i].data);
        new_buckets[index].occupied = true;
        new_buckets[index].deleted = false;
      }
    }
    buckets_ = std::move(new_buckets);
  }

  template < class Key, class T, class Hash, class Equal >
  bool HashTable< Key, T, Hash, Equal >::is_prime(size_t n) noexcept
  {
    if (n < 2) {
      return false;
    }
    if (n % 2 == 0) {
      return n == 2;
    }
    if (n % 3 == 0) {
      return n == 3;
    }
    for (size_t i = 5; i * i <= n; i += 6) {
      if (n % i == 0 || n % (i + 2) == 0) {
        return false;
      }
    }
    return true;
  }

  template < class Key, class T, class Hash, class Equal >
  size_t HashTable< Key, T, Hash, Equal >::next_prime(size_t n) noexcept
  {
    if (n < 2) {
      return 2;
    }
    if (n == 2) {
      return 3;
    }
    if (is_prime(n)) {
      return n;
    }
    n = (n % 2 == 0) ? (n + 1) : (n + 2);
    while (!is_prime(n)) {
      n += 2;
    }
    return n;
  }
}

#endif
