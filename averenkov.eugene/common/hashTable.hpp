#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <functional>
#include <stdexcept>
#include <utility>
#include <iterator>
#include <algorithm>
#include <bucket.hpp>
#include <array.hpp>
#include <hashTIterator.hpp>
#include <prime.hpp>

namespace averenkov
{
  template < class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable
  {
    friend class IteratorHash< Key, Value, Hash, Equal, true >;
    friend class IteratorHash< Key, Value, Hash, Equal, false >;

  public:
    using iterator = IteratorHash< Key, Value, Hash, Equal, false >;
    using const_iterator = IteratorHash< Key, Value, Hash, Equal, true >;

    HashTable();
    HashTable(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    HashTable(std::initializer_list< std::pair< Key, Value > > init);
    template< typename InputIt >
    HashTable(InputIt first, InputIt last);

    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;
    HashTable& operator=(std::initializer_list< std::pair< Key, Value > > init);

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void clear() noexcept;
    std::pair< iterator, bool > insert(const std::pair< Key, Value >& value);
    template< typename InputIt >
    void insert(InputIt first, InputIt last);
    void insert(std::initializer_list< std::pair< Key, Value > > init);

    template < typename K, typename V >
    std::pair< iterator, bool > emplace(K&& key, V&& value);

    iterator erase(iterator pos);
    size_t erase(const Key& key);
    iterator erase(iterator first, iterator last);

    void swap(HashTable& other) noexcept;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    Value& operator[](const Key& key);
    Value& operator[](Key&& key);

    size_t count(const Key& key) const;
    iterator find(const Key& key);
    const_iterator find(const Key& key) const;

    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float ml);
    void rehash(size_t count);
    void reserve(size_t count);
    size_t probe(size_t hash, size_t i) const noexcept;

  private:
    Array< detail::Bucket< Key, Value > > table_;
    size_t size_ = 0;
    Hash hasher_;
    Equal key_equal_;
    float max_load_factor_ = 0.75;

    explicit HashTable(size_t bucket_count, const Hash& hash = Hash(), const Equal& equal = Equal());
    size_t hash_to_index(const Key& key) const;
    void rehash_if_needed();
  };

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::probe(size_t hash, size_t i) const noexcept
  {
    return (hash + i * i) % table_.size();
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t bucket_count, const Hash& hash, const Equal& equal):
    table_(next_prime(bucket_count)),
    hasher_(hash),
    key_equal_(equal)
  {}


  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    HashTable(11)
  {}

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& other):
    table_(other.table_),
    size_(other.size_),
    hasher_(other.hasher_),
    key_equal_(other.key_equal_),
    max_load_factor_(other.max_load_factor_)
  {}

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& other) noexcept:
    table_(std::move(other.table_)),
    size_(other.size_),
    hasher_(std::move(other.hasher_)),
    key_equal_(std::move(other.key_equal_)),
    max_load_factor_(other.max_load_factor_)
  {
    other.size_ = 0;
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(std::initializer_list< std::pair< Key, Value > > init):
    HashTable(init.begin(), init.end())
  {}

  template < class Key, class Value, class Hash, class Equal >
  template< typename InputIt >
  HashTable< Key, Value, Hash, Equal >::HashTable(InputIt first, InputIt last):
    HashTable()
  {
    insert(first, last);
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& other)
  {
    if (this != &other)
    {
      table_ = other.table_;
      size_ = other.size_;
      hasher_ = other.hasher_;
      key_equal_ = other.key_equal_;
      max_load_factor_ = other.max_load_factor_;
    }
    return *this;
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& other) noexcept
  {
    if (this != &other)
    {
      table_ = std::move(other.table_);
      size_ = other.size_;
      hasher_ = std::move(other.hasher_);
      key_equal_ = std::move(other.key_equal_);
      max_load_factor_ = other.max_load_factor_;
      other.size_ = 0;
    }
    return *this;
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(std::initializer_list< std::pair< Key, Value > > init)
  {
    clear();
    insert(init);
    return *this;
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator
  HashTable< Key, Value, Hash, Equal >::begin() noexcept
  {
    for (size_t i = 0; i < table_.size(); ++i)
    {
      if (table_[i].occupied && !table_[i].deleted)
      {
        return iterator(&table_[i], &table_[0] + table_.size());
      }
    }
    return end();
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator
  HashTable< Key, Value, Hash, Equal >::end() noexcept
  {
    return iterator(&table_[0] + table_.size(), &table_[0] + table_.size());
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::const_iterator
  HashTable< Key, Value, Hash, Equal >::begin() const noexcept
  {
    return cbegin();
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::const_iterator
  HashTable< Key, Value, Hash, Equal >::end() const noexcept
  {
    return cend();
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::const_iterator
  HashTable< Key, Value, Hash, Equal >::cbegin() const noexcept
  {
    for (size_t i = 0; i < table_.size(); ++i)
    {
      if (table_[i].occupied && !table_[i].deleted)
      {
        return const_iterator(&table_[i], &table_[0] + table_.size());
      }
    }
    return cend();
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::const_iterator
  HashTable< Key, Value, Hash, Equal >::cend() const noexcept
  {
    return const_iterator(&table_[0] + table_.size(), &table_[0] + table_.size());
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
    for (size_t i = 0; i < table_.size(); ++i)
    {
      table_[i].occupied = false;
      table_[i].deleted = false;
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
  template < typename K, typename V >
  std::pair< typename HashTable< Key, Value, Hash, Equal >::iterator, bool >
    HashTable< Key, Value, Hash, Equal >::emplace(K&& key, V&& value)
  {
    if (size_ + 1 > max_load_factor_ * table_.size())
    {
      rehash(table_.size() * 2);
    }
    size_t hash = hasher_(key) % table_.size();
    size_t i = 0;
    size_t index = probe(hash, i);
    size_t first_deleted = table_.size();

    while (table_[index].occupied || table_[index].deleted)
    {
      if (table_[index].occupied && key_equal_(table_[index].key, key))
      {
        if (table_[index].deleted)
        {
          table_[index].key = key;
          table_[index].value = value;
          table_[index].occupied = true;
          table_[index].deleted = false;
          ++size_;
          return { iterator(table_.get_data() + index, table_.get_data() + table_.size()), true };
        }
        return { iterator(table_.get_data() + index, table_.get_data() + table_.size()), false };
      }
      if (table_[index].deleted && first_deleted == table_.size())
      {
        first_deleted = index;
      }
      ++i;
      index = probe(hash, i);
    }
    if (first_deleted != table_.size())
    {
      index = first_deleted;
    }
    table_[index].key = key;
    table_[index].value = value;
    table_[index].occupied = true;
    table_[index].deleted = false;
    ++size_;
    return { iterator(table_.get_data() + index, table_.get_data() + table_.size()), true };
  }

  template < class Key, class Value, class Hash, class Equal >
  template< typename InputIt >
  void HashTable< Key, Value, Hash, Equal >::insert(InputIt first, InputIt last)
  {
    for (; first != last; ++first)
    {
      insert(*first);
    }
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::insert(std::initializer_list< std::pair< Key, Value > > init)
  {
    insert(init.begin(), init.end());
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator
  HashTable< Key, Value, Hash, Equal >::erase(iterator pos)
  {
    if (pos.current_ >= &table_[0] + table_.size())
    {
      return end();
    }

    size_t index = pos.current_ - &table_[0];
    table_[index].occupied = false;
    table_[index].deleted = true;
    size_--;

    auto next = pos;
    ++next;
    return next;
  }

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::erase(const Key& key)
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
  HashTable< Key, Value, Hash, Equal >::erase(iterator first, iterator last)
  {
    while (first != last)
    {
      first = erase(first);
    }
    return last;
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable& other) noexcept
  {
    table_.swap(other.table_);
    std::swap(size_, other.size_);
    std::swap(hasher_, other.hasher_);
    std::swap(key_equal_, other.key_equal_);
    std::swap(max_load_factor_, other.max_load_factor_);
  }

  template < class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key)
  {
    auto it = find(key);
    if (it == end())
    {
      throw std::out_of_range("Key not found");
    }
    return it->value;
  }

  template < class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key) const
  {
    auto it = find(key);
    if (it == end())
    {
      throw std::out_of_range("Key not found");
    }
    return it->value;
  }

  template < class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& key)
  {
    auto it = find(key);
    if (it == end())
    {
      it = insert({ key, Value() }).first;
    }
    return it->value;
  }

  template < class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](Key&& key)
  {
    auto it = find(key);
    if (it == end())
    {
      it = insert({ std::move(key), Value() }).first;
    }
    return it->value;
  }

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::count(const Key& key) const
  {
    return find(key) != end() ? 1 : 0;
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator
  HashTable< Key, Value, Hash, Equal >::find(const Key& key)
  {
    if (empty())
    {
      return end();
    }
    size_t hash = hasher_(key) % table_.size();
    size_t i = 0;
    size_t index = probe(hash, i);

    while (table_[index].occupied || table_[index].deleted)
    {
      if (table_[index].occupied && key_equal_(table_[index].key, key))
      {
        return iterator(table_.get_data() + index, table_.get_data() + table_.size());
      }
      ++i;
      index = probe(hash, i);
      if (i >= table_.size())
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
    size_t hash = hasher_(key) % table_.size();
    size_t i = 0;
    size_t index = probe(hash, i);

    while (table_[index].occupied || table_[index].deleted)
    {
      if (table_[index].occupied && key_equal_(table_[index].key, key))
      {
        return const_iterator(table_.get_data() + index, table_.get_data() + table_.size());
      }
      ++i;
      index = probe(hash, i);
      if (i >= table_.size())
      {
        break;
      }
    }
    return cend();
  }

  template < class Key, class Value, class Hash, class Equal >
  float HashTable< Key, Value, Hash, Equal >::load_factor() const noexcept
  {
    return table_.size() == 0 ? 0.0f : static_cast<float>(size_) / table_.size();
  }

  template < class Key, class Value, class Hash, class Equal >
  float HashTable< Key, Value, Hash, Equal >::max_load_factor() const noexcept
  {
    return max_load_factor_;
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::max_load_factor(float ml)
  {
    if (ml <= 0.0f || ml > 1.0f)
    {
      throw std::invalid_argument("Invalid max load factor");
    }
    max_load_factor_ = ml;
    rehash_if_needed();
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t count)
  {
    count = next_prime(count);
    Array< detail::Bucket < Key, Value > > new_table(count);
    for (size_t i = 0; i < count; ++i)
    {
      new_table.push_back(detail::Bucket< Key, Value >());
    }

    for (size_t i = 0; i < table_.size(); ++i)
    {
      auto& bucket = table_[i];
      if (bucket.occupied && !bucket.deleted)
      {
        size_t index = hasher_(bucket.key) % count;
        size_t j = 0;

        while (j < count)
        {
          size_t current_index = (index + j * j) % count;
          if (!new_table[current_index].occupied)
          {
            new_table[current_index].key = std::move(bucket.key);
            new_table[current_index].value = std::move(bucket.value);
            new_table[current_index].occupied = true;
            break;
          }
          j++;
        }
      }
    }
    table_ = std::move(new_table);
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::reserve(size_t count)
  {
    rehash(static_cast<size_t>(count / max_load_factor_) + 1);
  }

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::hash_to_index(const Key& key) const
  {
    if (table_.empty())
    {
      return 0;
    }
    return hasher_(key) % table_.size();
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash_if_needed()
  {
    if (load_factor() > max_load_factor_)
    {
      rehash(table_.size() * 2 + 1);
    }
  }

}
#endif

/*#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <functional>
#include <stdexcept>
#include <utility>
#include <iterator>
#include <algorithm>
#include <bucket.hpp>
#include <array.hpp>
#include <hashTIterator.hpp>
#include <prime.hpp>

namespace averenkov
{
  template < class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable
  {
    friend class IteratorHash< Key, Value, Hash, Equal, true >;
    friend class IteratorHash< Key, Value, Hash, Equal, false >;

  public:
    using iterator = IteratorHash< Key, Value, Hash, Equal, false >;
    using const_iterator = IteratorHash< Key, Value, Hash, Equal, true >;

    HashTable();
    HashTable(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    HashTable(std::initializer_list< std::pair< Key, Value > > init);
    template< typename InputIt >
    HashTable(InputIt first, InputIt last);

    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;
    HashTable& operator=(std::initializer_list< std::pair< Key, Value > > init);

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void clear() noexcept;
    std::pair< iterator, bool > insert(const std::pair< Key, Value >& value);
    template< typename InputIt >
    void insert(InputIt first, InputIt last);
    void insert(std::initializer_list< std::pair< Key, Value > > init);

    template < typename K, typename V >
    std::pair< iterator, bool > emplace(K&& key, V&& value);

    iterator erase(iterator pos);
    size_t erase(const Key& key);
    iterator erase(iterator first, iterator last);

    void swap(HashTable& other) noexcept;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    Value& operator[](const Key& key);
    Value& operator[](Key&& key);

    size_t count(const Key& key) const;
    iterator find(const Key& key);
    const_iterator find(const Key& key) const;

    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float ml);
    void rehash(size_t count);
    void reserve(size_t count);
    size_t probe(size_t hash, size_t i) const noexcept;

  private:
    Array< detail::Bucket< Key, Value > > table_;
    size_t size_ = 0;
    Hash hasher_;
    Equal key_equal_;
    float max_load_factor_ = 0.75;

    explicit HashTable(size_t bucket_count, const Hash& hash = Hash(), const Equal& equal = Equal());
    size_t hash_to_index(const Key& key) const;
    void rehash_if_needed();
  };

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::probe(size_t hash, size_t i) const noexcept
  {
    return (hash + i * i) % table_.size();
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t bucket_count, const Hash& hash, const Equal& equal):
    table_(next_prime(bucket_count)),
    hasher_(hash),
    key_equal_(equal)
  {}


  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    HashTable(11)
  {}

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& other):
    table_(other.table_),
    size_(other.size_),
    hasher_(other.hasher_),
    key_equal_(other.key_equal_),
    max_load_factor_(other.max_load_factor_)
  {}

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& other) noexcept:
    table_(std::move(other.table_)),
    size_(other.size_),
    hasher_(std::move(other.hasher_)),
    key_equal_(std::move(other.key_equal_)),
    max_load_factor_(other.max_load_factor_)
  {
    other.size_ = 0;
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(std::initializer_list< std::pair< Key, Value > > init):
    HashTable(init.begin(), init.end())
  {}

  template < class Key, class Value, class Hash, class Equal >
  template< typename InputIt >
  HashTable< Key, Value, Hash, Equal >::HashTable(InputIt first, InputIt last):
    HashTable()
  {
    insert(first, last);
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& other)
  {
    if (this != &other)
    {
      table_ = other.table_;
      size_ = other.size_;
      hasher_ = other.hasher_;
      key_equal_ = other.key_equal_;
      max_load_factor_ = other.max_load_factor_;
    }
    return *this;
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& other) noexcept
  {
    if (this != &other)
    {
      table_ = std::move(other.table_);
      size_ = other.size_;
      hasher_ = std::move(other.hasher_);
      key_equal_ = std::move(other.key_equal_);
      max_load_factor_ = other.max_load_factor_;
      other.size_ = 0;
    }
    return *this;
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(std::initializer_list< std::pair< Key, Value > > init)
  {
    clear();
    insert(init);
    return *this;
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator
  HashTable< Key, Value, Hash, Equal >::begin() noexcept
  {
    for (size_t i = 0; i < table_.size(); ++i)
    {
      if (table_[i].occupied && !table_[i].deleted)
      {
        return iterator(&table_[i], &table_[0] + table_.size());
      }
    }
    return end();
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator
  HashTable< Key, Value, Hash, Equal >::end() noexcept
  {
    return iterator(&table_[0] + table_.size(), &table_[0] + table_.size());
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::const_iterator
  HashTable< Key, Value, Hash, Equal >::begin() const noexcept
  {
    return cbegin();
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::const_iterator
  HashTable< Key, Value, Hash, Equal >::end() const noexcept
  {
    return cend();
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::const_iterator
  HashTable< Key, Value, Hash, Equal >::cbegin() const noexcept
  {
    for (size_t i = 0; i < table_.size(); ++i)
    {
      if (table_[i].occupied && !table_[i].deleted)
      {
        return const_iterator(&table_[i], &table_[0] + table_.size());
      }
    }
    return cend();
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::const_iterator
  HashTable< Key, Value, Hash, Equal >::cend() const noexcept
  {
    return const_iterator(&table_[0] + table_.size(), &table_[0] + table_.size());
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
    for (size_t i = 0; i < table_.size(); ++i)
    {
      table_[i].occupied = false;
      table_[i].deleted = false;
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
  template < typename K, typename V >
  std::pair< typename HashTable< Key, Value, Hash, Equal >::iterator, bool >
    HashTable< Key, Value, Hash, Equal >::emplace(K&& key, V&& value)
  {
    if (size_ + 1 > max_load_factor_ * table_.size())
    {
      rehash(table_.size() * 2);
    }
    size_t hash = hasher_(key) % table_.size();
    size_t i = 0;
    size_t index = probe(hash, i);
    size_t first_deleted = table_.size();

    while (table_[index].occupied || table_[index].deleted)
    {
      if (table_[index].occupied && key_equal_(table_[index].key, key))
      {
        if (table_[index].deleted)
        {
          table_[index].key = key;
          table_[index].value = value;
          table_[index].occupied = true;
          table_[index].deleted = false;
          ++size_;
          return { iterator(table_.data_ + index, table_.data_ + table_.size()), true };
        }
        return { iterator(table_.data_ + index, table_.data_ + table_.size()), false };
      }
      if (table_[index].deleted && first_deleted == table_.size())
      {
        first_deleted = index;
      }
      ++i;
      index = probe(hash, i);
    }
    if (first_deleted != table_.size())
    {
      index = first_deleted;
    }
    table_[index].key = key;
    table_[index].value = value;
    table_[index].occupied = true;
    table_[index].deleted = false;
    ++size_;
    return { iterator(table_.data_ + index, table_.data_ + table_.size()), true };
  }

  template < class Key, class Value, class Hash, class Equal >
  template< typename InputIt >
  void HashTable< Key, Value, Hash, Equal >::insert(InputIt first, InputIt last)
  {
    for (; first != last; ++first)
    {
      insert(*first);
    }
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::insert(std::initializer_list< std::pair< Key, Value > > init)
  {
    insert(init.begin(), init.end());
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator
  HashTable< Key, Value, Hash, Equal >::erase(iterator pos)
  {
    if (pos.current_ >= &table_[0] + table_.size())
    {
      return end();
    }

    size_t index = pos.current_ - &table_[0];
    table_[index].occupied = false;
    table_[index].deleted = true;
    size_--;

    auto next = pos;
    ++next;
    return next;
  }

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::erase(const Key& key)
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
  HashTable< Key, Value, Hash, Equal >::erase(iterator first, iterator last)
  {
    while (first != last)
    {
      first = erase(first);
    }
    return last;
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable& other) noexcept
  {
    table_.swap(other.table_);
    std::swap(size_, other.size_);
    std::swap(hasher_, other.hasher_);
    std::swap(key_equal_, other.key_equal_);
    std::swap(max_load_factor_, other.max_load_factor_);
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
  Value& HashTable< Key, Value, Hash, Equal >::operator[](Key&& key)
  {
    auto it = find(key);
    if (it == end())
    {
      it = insert({ std::move(key), Value() }).first;
    }
    return it->second;
  }

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::count(const Key& key) const
  {
    return find(key) != end() ? 1 : 0;
  }

  template < class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator
  HashTable< Key, Value, Hash, Equal >::find(const Key& key)
  {
    if (empty())
    {
      return end();
    }
    size_t hash = hasher_(key) % table_.size();
    size_t i = 0;
    size_t index = probe(hash, i);

    while (table_[index].occupied || table_[index].deleted)
    {
      if (table_[index].occupied && key_equal_(table_[index].key, key))
      {
        return iterator(table_.data_ + index, table_.data_ + table_.size());
      }
      ++i;
      index = probe(hash, i);
      if (i >= table_.size())
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
    size_t hash = hasher_(key) % table_.size();
    size_t i = 0;
    size_t index = probe(hash, i);

    while (table_[index].occupied || table_[index].deleted)
    {
      if (table_[index].occupied && key_equal_(table_[index].key, key))
      {
        return const_iterator(table_.data_ + index, table_.data_ + table_.size());
      }
      ++i;
      index = probe(hash, i);
      if (i >= table_.size())
      {
        break;
      }
    }
    return cend();
  }

  template < class Key, class Value, class Hash, class Equal >
  float HashTable< Key, Value, Hash, Equal >::load_factor() const noexcept
  {
    return table_.size() == 0 ? 0.0f : static_cast<float>(size_) / table_.size();
  }

  template < class Key, class Value, class Hash, class Equal >
  float HashTable< Key, Value, Hash, Equal >::max_load_factor() const noexcept
  {
    return max_load_factor_;
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::max_load_factor(float ml)
  {
    if (ml <= 0.0f || ml > 1.0f)
    {
      throw std::invalid_argument("Invalid max load factor");
    }
    max_load_factor_ = ml;
    rehash_if_needed();
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t count)
  {
    count = next_prime(count);
    Array< detail::Bucket < Key, Value > > new_table(count);
    for (size_t i = 0; i < count; ++i)
    {
      new_table.push_back(detail::Bucket< Key, Value >());
    }

    for (size_t i = 0; i < table_.size(); ++i)
    {
      auto& bucket = table_[i];
      if (bucket.occupied && !bucket.deleted)
      {
        size_t index = hasher_(bucket.key) % count;
        size_t j = 0;

        while (j < count)
        {
          size_t current_index = (index + j * j) % count;
          if (!new_table[current_index].occupied)
          {
            new_table[current_index].key = std::move(bucket.key);
            new_table[current_index].value = std::move(bucket.value);
            new_table[current_index].occupied = true;
            break;
          }
          j++;
        }
      }
    }
    table_ = std::move(new_table);
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::reserve(size_t count)
  {
    rehash(static_cast<size_t>(count / max_load_factor_) + 1);
  }

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::hash_to_index(const Key& key) const
  {
    if (table_.empty())
    {
      return 0;
    }
    return hasher_(key) % table_.size();
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash_if_needed()
  {
    if (load_factor() > max_load_factor_)
    {
      rehash(table_.size() * 2 + 1);
    }
  }

}
#endif
*/
