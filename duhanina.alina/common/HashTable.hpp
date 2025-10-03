#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <functional>
#include <stdexcept>
#include <utility>
#include "array.hpp"
#include "prime.hpp"
#include "bucket.hpp"
#include "iteratorHash.hpp"

namespace duhanina
{
  template <typename Key, typename Value, typename Hash, typename Equal, bool isConst>
  class IteratorHash;

  template < class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable
  {
    friend class IteratorHash< Key, Value, Hash, Equal, true >;
    friend class IteratorHash< Key, Value, Hash, Equal, false >;
  public:

    using iterator = IteratorHash< Key, Value, Hash, Equal, false >;
    using const_iterator = IteratorHash< Key, Value, Hash, Equal, true >;

    HashTable();

    template< typename InputIt >
    HashTable(InputIt, InputIt);

    explicit HashTable(std::initializer_list< std::pair< Key, Value > >);

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    Value& operator[](const Key& key);

    bool empty() const noexcept;
    size_t size() const noexcept;

    void clear() noexcept;
    std::pair< iterator, bool > insert(const std::pair< Key, Value >& value);

    template < typename InputIt >
    void insert(InputIt first, InputIt last);

    iterator erase(iterator pos) noexcept;
    size_t erase(const Key& key) noexcept;
    iterator erase(iterator first, iterator last) noexcept;

    template < typename K, typename V >
    std::pair< iterator, bool > emplace(K&& key, V&& value);

    void swap(HashTable& other) noexcept;

    iterator find(const Key& key);
    const_iterator find(const Key& key) const;
    size_t count(const Key& key) const;

    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float ml);
    void rehashFactor(size_t count);

  private:

    DynamicArray< Bucket< Key, Value > > buckets_;
    size_t size_ = 0;
    float max_load_factor_ = 0.75;
    Hash hasher_;
    Equal key_equal_;

    explicit HashTable(size_t bucket_count, const Hash& hash = Hash(), const Equal& equal = Equal());
    size_t probe(size_t hash, size_t i) const noexcept;
    void rehash(size_t new_capacity);
  };

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::probe(size_t hash, size_t i) const noexcept
  {
    return (hash + i * i) % buckets_.size();
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t new_capacity)
  {
    new_capacity = next_prime(new_capacity);
    DynamicArray< Bucket < Key, Value > > new_buckets(new_capacity);
    for (size_t i = 0; i < buckets_.size(); ++i)
    {
      if (buckets_[i].occupied && !buckets_[i].deleted)
      {
        const auto& key = buckets_[i].data.first;
        size_t hash = hasher_(key) % new_capacity;
        size_t j = 0;
        size_t index = probe(hash, j);
        while (new_buckets[index].occupied)
        {
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

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    HashTable(11)
  {}

  template< typename Key, typename Value, typename Hash, typename Equal >
  template< typename InputIt >
  HashTable< Key, Value, Hash, Equal >::HashTable(InputIt first, InputIt last):
    HashTable()
  {
    for (auto it = first; it != last; it++)
    {
      insert(*it);
    }
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(std::initializer_list< std::pair< Key, Value > > ilist):
    HashTable(ilist.begin(), ilist.end())
  {}

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t bucket_count, const Hash& hash, const Equal& equal):
    buckets_(next_prime(bucket_count)),
    hasher_(hash),
    key_equal_(equal)
  {}

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator HashTable< Key, Value, Hash, Equal >::begin() noexcept
  {
    return iterator(buckets_.data_, buckets_.data_ + buckets_.size());
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator HashTable< Key, Value, Hash, Equal >::end() noexcept
  {
    return iterator(buckets_.data_ + buckets_.size(), buckets_.data_ + buckets_.size());
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::const_iterator HashTable< Key, Value, Hash, Equal >::cbegin() const noexcept
  {
    return const_iterator(buckets_.data_, buckets_.data_ + buckets_.size());
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::const_iterator HashTable< Key, Value, Hash, Equal >::cend() const noexcept
  {
    return const_iterator(buckets_.data_ + buckets_.size(), buckets_.data_ + buckets_.size());
  }

  template < class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key)
  {
    auto it = find(key);
    if (it == end())
    {
      throw std::out_of_range("Key not found");
    }
    return it->second;
  }

  template < class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key) const
  {
    auto it = find(key);
    if (it == end())
    {
      throw std::out_of_range("Key not found");
    }
    return it->second;
  }

  template < class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& key)
  {
    auto it = find(key);
    if (it == end())
    {
      it = insert({ key, Value() }).first;
    }
    return it->second;
  }

  template < class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const noexcept
  {
    return size_;
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::clear() noexcept
  {
    for (size_t i = 0; i < buckets_.size(); ++i)
    {
      buckets_[i].occupied = false;
      buckets_[i].deleted = false;
    }
    size_ = 0;
  }

  template < class Key, class Value, class Hash, class Equal >
  std::pair< typename HashTable< Key, Value, Hash, Equal >::iterator, bool >
    HashTable< Key, Value, Hash, Equal >::insert(const std::pair< Key, Value >& value)
  {
    return emplace(value.first, value.second);
  }

  template < class Key, class Value, class Hash, class Equal >
  template < typename InputIt >
  void HashTable< Key, Value, Hash, Equal >::insert(InputIt first, InputIt last)
  {
    for (; first != last; ++first)
    {
      insert(*first);
    }
  }

  template < class Key, class Value, class Hash, class Equal >
  template < typename K, typename V >
  std::pair< typename HashTable< Key, Value, Hash, Equal >::iterator, bool >
    HashTable< Key, Value, Hash, Equal >::emplace(K&& key, V&& value)
  {
    if (size_ + 1 > max_load_factor_ * buckets_.size())
    {
      rehash(buckets_.size() * 2);
    }
    size_t hash = hasher_(key) % buckets_.size();
    size_t i = 0;
    size_t index = probe(hash, i);
    size_t first_deleted = buckets_.size();

    while (buckets_[index].occupied || buckets_[index].deleted)
    {
      if (buckets_[index].occupied && key_equal_(buckets_[index].data.first, key))
      {
        if (buckets_[index].deleted)
        {
          buckets_[index].data = { key, value };
          buckets_[index].occupied = true;
          buckets_[index].deleted = false;
          ++size_;
          return { iterator(buckets_.data_ + index, buckets_.data_ + buckets_.size()), true };
        }
        return { iterator(buckets_.data_ + index, buckets_.data_ + buckets_.size()), false };
      }
      if (buckets_[index].deleted && first_deleted == buckets_.size())
      {
        first_deleted = index;
      }
      ++i;
      index = probe(hash, i);
    }
    if (first_deleted != buckets_.size())
    {
      index = first_deleted;
    }
    buckets_[index].data = { key, value };
    buckets_[index].occupied = true;
    buckets_[index].deleted = false;
    ++size_;
    return { iterator(buckets_.data_ + index, buckets_.data_ + buckets_.size()), true };
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator HashTable< Key, Value, Hash, Equal >::erase(iterator pos) noexcept
  {
    if (pos == end())
    {
      return end();
    }
    size_t index = pos.current_ - buckets_.data_;
    buckets_[index].occupied = false;
    buckets_[index].deleted = true;
    --size_;
    return iterator(buckets_.data_ + index, buckets_.data_ + buckets_.size());
  }

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::erase(const Key& key) noexcept
  {
    auto it = find(key);
    if (it != end())
    {
      erase(it);
      return 1;
    }
    return 0;
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator
  HashTable< Key, Value, Hash, Equal >::erase(iterator first, iterator last) noexcept
  {
    iterator result = end();
    if (first == last)
    {
      return result;
    }
    while (first != last)
    {
      result = erase(first++);
    }
    return result;
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable& other) noexcept
  {
    std::swap(buckets_, other.buckets_);
    std::swap(size_, other.size_);
    std::swap(max_load_factor_, other.max_load_factor_);
    std::swap(hasher_, other.hasher_);
    std::swap(key_equal_, other.key_equal_);
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator
    HashTable< Key, Value, Hash, Equal >::find(const Key& key)
  {
    if (empty())
    {
      return end();
    }
    size_t hash = hasher_(key) % buckets_.size();
    size_t i = 0;
    size_t index = probe(hash, i);

    while (buckets_[index].occupied || buckets_[index].deleted)
    {
      if (buckets_[index].occupied && key_equal_(buckets_[index].data.first, key))
      {
        return iterator(buckets_.data_ + index, buckets_.data_ + buckets_.size());
      }
      ++i;
      index = probe(hash, i);
      if (i >= buckets_.size())
      {
        break;
      }
    }
    return end();
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::const_iterator
    HashTable< Key, Value, Hash, Equal >::find(const Key& key) const
  {
    if (empty())
    {
      return cend();
    }
    size_t hash = hasher_(key) % buckets_.size();
    size_t i = 0;
    size_t index = probe(hash, i);

    while (buckets_[index].occupied || buckets_[index].deleted)
    {
      if (buckets_[index].occupied && key_equal_(buckets_[index].data.first, key))
      {
        return const_iterator(buckets_.data_ + index, buckets_.data_ + buckets_.size());
      }
      ++i;
      index = probe(hash, i);
      if (i >= buckets_.size())
      {
        break;
      }
    }
    return cend();
  }

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::count(const Key& key) const
  {
    const_iterator found = find(key);
    if (found != cend())
    {
      return 1;
    }
    return 0;
  }

  template < class Key, class Value, class Hash, class Equal >
  float HashTable< Key, Value, Hash, Equal >::load_factor() const noexcept
  {
    if (buckets_.size() == 0)
    {
      return 0.0;
    }
    float current_size = size_;
    float table_size = buckets_.size();
    return current_size / table_size;
  }

  template < class Key, class Value, class Hash, class Equal >
  float HashTable< Key, Value, Hash, Equal >::max_load_factor() const noexcept
  {
    return max_load_factor_;
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::max_load_factor(float ml)
  {
    if (ml <= 0.0 || ml > 1.0)
    {
      throw std::invalid_argument("Invalid max load factor");
    }
    max_load_factor_ = ml;
    if (load_factor() > max_load_factor_)
    {
      rehash(buckets_.size() * 2);
    }
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehashFactor(size_t count)
  {
    size_t min_count = size_ / max_load_factor_;
    if (count < min_count)
    {
      count = min_count;
    }
    if (count != buckets_.size())
    {
      rehash(count);
    }
  }
}

#endif
