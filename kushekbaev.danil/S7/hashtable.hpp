#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <functional>
#include <vector>
#include "hashtableslot.hpp"
#include "consthashiterator.hpp"
#include "hashiterator.hpp"

namespace kushekbaev
{
  template< typename Key, typename Value, typename Hash, typename Equal >
  struct ConstHashIterator;

  template< typename Key, typename Value, typename Hash, typename Equal >
  struct HashIterator;

  template< typename Key, typename Value, typename Hash = std::hash< Key >, typename Equal = std::equal_to< Key > >
  struct HashTable
  {
    using It = HashIterator< Key, Value, Hash, Equal >;
    using cIt = ConstHashIterator< Key, Value, Hash, Equal >;
    using pair = const std::pair< Key, Value >;

    HashTable();
    ~HashTable() = default;

    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(HashTable< Key, Value, Hash, Equal > & rhs) noexcept;

    It begin();
    It end();
    cIt cbegin() const;
    cIt cend() const;

    cIt find(const Key& k) const;
    It find(const Key& k);

    Value& operator[](const Key& key);
    const Value& operator[](const Key& key) const;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;

    size_t erase(const Key& key) noexcept;
    It erase(It) noexcept;

    std::pair< It, bool > insert(const pair& val);

    void rehash(size_t n);

    private:
      size_t DEFAULT_HASHTABLE_SIZE = 17;
      friend struct ConstHashIterator< Key, Value, Hash, Equal >;
      friend struct HashIterator< Key, Value, Hash, Equal >;
      std::vector< HashTableSlot< Key, Value > > table_;
      size_t size_;
      Hash hash_;
      Equal equal_;
      float max_load_factor_ = 0.7;
      size_t find_index(const Key& k) const;
      size_t find_index_in(const Key& k, const std::vector< HashTableSlot< Key, Value > >& table) const;
  };

  template< typename Key, typename Value, typename Hash, typename Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    table_(DEFAULT_HASHTABLE_SIZE),
    size_(0)
  {}

  template< typename Key, typename Value, typename Hash, typename Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const noexcept
  {
    return size_;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  bool HashTable< Key, Value, Hash, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable< Key, Value, Hash, Equal >& rhs) noexcept
  {
    std::swap(table_, rhs.table_);
    std::swap(size_, rhs.size_);
    std::swap(hash_, rhs.hash_);
    std::swap(equal_, rhs.equal_);
    std::swap(max_load_factor_, rhs.max_load_factor_);
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  typename HashTable< Key, Value, Hash, Equal >::It HashTable< Key, Value, Hash, Equal >::begin()
  {
    return It{ this, 0 };
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  typename HashTable< Key, Value, Hash, Equal >::It HashTable< Key, Value, Hash, Equal >::end()
  {
    return It{ this, table_.size() };
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  typename HashTable< Key, Value, Hash, Equal >::cIt HashTable< Key, Value, Hash, Equal >::cbegin() const
  {
    return cIt{ this, 0 };
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  typename HashTable< Key, Value, Hash, Equal >::cIt HashTable< Key, Value, Hash, Equal >::cend() const
  {
    return cIt{ this, table_.size() };
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  size_t HashTable< Key, Value, Hash, Equal >::find_index(const Key& k) const
  {
    if (table_.empty())
    {
      return table_.size();
    }
    size_t baseNode = hash_(k) % table_.size();
    size_t currNode = baseNode;
    size_t i = 1;
    while (i <= table_.size())
    {
      if (table_[currNode].occupied && equal_(table_[currNode].data_.first, k))
      {
        return currNode;
      }
      if (!table_[currNode].occupied && !table_[currNode].deleted)
      {
        break;
      }
      currNode = (baseNode + i * i) % table_.size();
      ++i;
    }
    return table_.size();
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  size_t HashTable< Key, Value, Hash, Equal >::find_index_in(const Key& k, const std::vector< HashTableSlot< Key, Value > >& table) const
  {
    if (table.empty())
    {
      return 0;
    }
    size_t baseNode = hash_(k) % table.size();
    size_t currNode = baseNode;
    size_t i = 1;
    while (table[currNode].occupied)
    {
      currNode = (baseNode + i * i) % table_.size();
      ++i;
    }
    return currNode;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t n)
  {
    if (n < table_.size())
    {
      return;
    }
    std::vector< HashTableSlot< Key, Value > > temp(n);
    for (size_t i = 0; i < table_.size(); ++i)
    {
      if (table_[i].occupied)
      {
        size_t newId = find_index_in(table_[i].data_.first, temp);
        if (newId < temp.size())
        {
          temp[newId].data_ = table_[i].data_;
          temp[newId].occupied = true;
          temp[newId].deleted = false;
        }
      }
    }
    table_.swap(temp);
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  typename HashTable< Key, Value, Hash, Equal >::It HashTable< Key, Value, Hash, Equal >::find(const Key& k)
  {
    return It{ this, find_index(k) };
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  typename HashTable< Key, Value, Hash, Equal >::cIt HashTable< Key, Value, Hash, Equal >::find(const Key& k) const
  {
    return cIt{ this, find_index(k) };
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& key)
  {
    It it = find(key);
    if (it == end())
    {
      insert(std::make_pair(key, Value()));
      it = find(key);
    }
    return it->second;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& key) const
  {
    cIt it = find(key);
    return it->second;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key)
  {
    It it = find(key);
    if (it == end())
    {
      throw std::out_of_range("<INVALID COMMAND>");
    }
    return it->second;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key) const
  {
    cIt it = find(key);
    if (it == cend())
    {
      throw std::out_of_range("<INVALID COMMAND>");
    }
    return it->second;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  size_t HashTable< Key, Value, Hash, Equal >::erase(const Key& key) noexcept
  {
    size_t index = find_index(key);
    if (index == table_.size())
    {
      return 0;
    }
    table_[index].occupied = false;
    table_[index].deleted = true;
    --size_;
    return 1;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  typename HashTable< Key, Value, Hash, Equal >::It HashTable< Key, Value, Hash, Equal >::erase(It it) noexcept
  {
    size_t HashTableSlot = it.index_;
    table_[HashTableSlot].occupied = false;
    table_[HashTableSlot].deleted = true;
    --size_;
    return It{this, it.index_ + 1};
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  std::pair< typename HashTable< Key, Value, Hash, Equal >::It, bool > HashTable< Key, Value, Hash, Equal >::insert(const pair& val)
  {
    if (table_.size() == 0)
    {
      rehash(DEFAULT_HASHTABLE_SIZE);
    }
    if (size_ >= table_.size() * max_load_factor_)
    {
      rehash(table_.size() * 2);
    }
    size_t baseNode = hash_(val.first) % table_.size();
    size_t currNode = baseNode;
    size_t i = 1;
    size_t firstDeleted = table_.size();
    while (i <= table_.size())
    {
      if (table_[currNode].occupied && equal_(table_[currNode].data_.first, val.first))
      {
        return std::make_pair(It(this, currNode), false);
      }
      if (!table_[currNode].occupied && table_[currNode].deleted && firstDeleted == table_.size())
      {
        firstDeleted = currNode;
      }
      if (!table_[currNode].occupied && !table_[currNode].deleted)
      {
        break;
      }
      currNode = (baseNode + i * i) % table_.size();
      ++i;
    }
    if (i > table_.size() && firstDeleted != table_.size())
    {
      currNode = firstDeleted;
    }
    table_[currNode].data_ = val;
    table_[currNode].occupied = true;
    table_[currNode].deleted = false;
    ++size_;
    return std::make_pair(It(this, currNode), true);
  }
}

#endif
