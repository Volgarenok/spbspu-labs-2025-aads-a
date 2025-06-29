#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <functional>
#include "dynamicArr.hpp"
#include "hashTableSlot.hpp"
#include "hashTableconstIterator.hpp"
#include "hashTableIterator.hpp"

namespace finaev
{
  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable
  {
    friend class HashTableIterator< Key, Value, Hash, Equal >;
    friend class HashTableConstIterator< Key, Value, Hash, Equal >;
  public:
    using Iter = HashTableIterator< Key, Value, Hash, Equal >;
    using constIter = HashTableConstIterator< Key, Value, Hash, Equal >;
    using pair = const std::pair< Key, Value >;

    HashTable();

    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(HashTable< Key, Value, Hash, Equal > & rhs) noexcept;

    Iter begin();
    constIter cbegin() const;
    Iter end();
    constIter cend() const;

    Iter find(const Key& k);
    constIter find(const Key& k) const;

    Value& operator[](const Key& key);
    const Value& operator[](const Key& key) const;
    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    size_t erase(const Key& key) noexcept;
    Iter erase(Iter) noexcept;
    std::pair< Iter, bool > insert(pair& val);

    void rehash(size_t n);
  private:
    DynamicArr< Slot< Key, Value > > table_;
    size_t size_;
    Hash hasher_;
    Equal equal_;
    float max_load_factor_ = 0.7;

    size_t findIndex(const Key & k) const;
    size_t findIndexIn(const Key & k, const DynamicArr< Slot< Key, Value > >& table) const;
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    table_(16),
    size_(0)
  {}

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable< Key, Value, Hash, Equal >& rhs) noexcept
  {
    std::swap(table_, rhs.table_);
    std::swap(size_, rhs.size_);
    std::swap(hasher_, rhs.hasher_);
    std::swap(equal_, rhs.equal_);
    std::swap(max_load_factor_, rhs.max_load_factor_);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::Iter HashTable< Key, Value, Hash, Equal >::begin()
  {
    return Iter{this, 0};
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::Iter HashTable< Key, Value, Hash, Equal >::end()
  {
    return Iter{this, table_.size()};
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::constIter HashTable< Key, Value, Hash, Equal >::cbegin() const
  {
    return constIter{this, 0};
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::constIter HashTable< Key, Value, Hash, Equal >::cend() const
  {
    return constIter{this, table_.size()};
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::findIndex(const Key & k) const
  {
    size_t baseSlot = hasher_(k) % table_.size();
    size_t currSlot = baseSlot;
    size_t i = 1;
    while (i <= table_.size())
    {
      if (table_[currSlot].occupied && equal_(table_[currSlot].data.first, k))
      {
        return currSlot;
      }
      if (!table_[currSlot].occupied && !table_[currSlot].deleted)
      {
        break;
      }
      currSlot = (baseSlot + i) % table_.size();
      ++i;
    }
    return table_.size();
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::findIndexIn(const Key& k, const DynamicArr< Slot< Key, Value > >& table) const
  {
    size_t baseSlot = hasher_(k) % table.size();
    size_t currSlot = baseSlot;
    size_t i = 1;
    while (table[currSlot].occupied)
    {
      currSlot = (baseSlot + i) % table.size();
      ++i;
    }
    return currSlot;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t n)
  {
    if (n < table_.size())
    {
      return;
    }
    DynamicArr< Slot< Key, Value > > temp(n);
    for (size_t i = 0; i < table_.size(); ++i)
    {
      if (table_[i].occupied)
      {
        size_t newId = findIndexIn(table_[i].data.first, temp);
        temp[newId].data = table_[i].data;
        temp[newId].occupied = true;
        temp[newId].deleted = false;
      }
    }
    table_.swap(temp);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::Iter HashTable< Key, Value, Hash, Equal >::find(const Key& k)
  {
    return Iter{this, findIndex(k)};
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::constIter HashTable< Key, Value, Hash, Equal >::find(const Key& k) const
  {
    return constIter{this, findIndex(k)};
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& key)
  {
    Iter it = find(key);
    if (it == end())
    {
      insert(std::make_pair(key, Value()));
      it = find(key);
    }
    return it->second;
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& key) const
  {
    constIter it = find(key);
    return it->second;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key)
  {
    Iter it = find(key);
    if (it == end())
    {
      throw std::out_of_range("<INVALID COMMAND>");
    }
    return it->second;
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key) const
  {
    constIter it = find(key);
    if (it == cend())
    {
      throw std::out_of_range("<INVALID COMMAND>");
    }
    return it->second;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::erase(const Key& key) noexcept
  {
    size_t index = findIndex(key);
    if (index == table_.size())
    {
      return 0;
    }
    table_[index].occupied = false;
    table_[index].deleted = true;
    --size_;
    return 1;
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::Iter HashTable< Key, Value, Hash, Equal >::erase(Iter it) noexcept
  {
    size_t slot = it.index_;
    table_[slot].occupied = false;
    table_[slot].deleted = true;
    --size_;
    return Iter{this, it.index_ + 1};
  }


  template< class Key, class Value, class Hash, class Equal >
  std::pair< typename HashTable< Key, Value, Hash, Equal >::Iter, bool > HashTable< Key, Value, Hash, Equal >::insert(pair& val)
  {
    if (table_.size() == 0)
    {
      rehash(16);
    }
    if (size_ >= table_.size() * max_load_factor_)
    {
      rehash(table_.size() * 2);
    }
    size_t baseSlot = hasher_(val.first) % table_.size();
    size_t currSlot = baseSlot;
    size_t i = 1;
    size_t firstDeleted = table_.size();
    while (i <= table_.size())
    {
      if (table_[currSlot].occupied && equal_(table_[currSlot].data.first, val.first))
      {
        return std::make_pair(Iter(this, currSlot), false);
      }
      if (!table_[currSlot].occupied && table_[currSlot].deleted && firstDeleted == table_.size())
      {
        firstDeleted = currSlot;
      }
      if (!table_[currSlot].occupied && !table_[currSlot].deleted)
      {
        break;
      }
      currSlot = (baseSlot + i * i) % table_.size();
      ++i;
    }
    if (i > table_.size() && firstDeleted != table_.size())
    {
      currSlot = firstDeleted;
    }
    table_[currSlot].data = val;
    table_[currSlot].occupied = true;
    table_[currSlot].deleted = false;
    ++size_;
    return std::make_pair(Iter(this, currSlot), true);
  }
}

#endif
