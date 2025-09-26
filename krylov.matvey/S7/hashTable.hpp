#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <functional>
#include <vector>
#include "node.hpp"
#include "constIterator.hpp"
#include "iterator.hpp"

namespace krylov
{
  template< class Key, class Value, class Hash, class Equal >
  class ConstIterator;

  template< class Key, class Value, class Hash, class Equal >
  class Iterator;

  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable
  {
    friend class ConstIterator< Key, Value, Hash, Equal >;
    friend class Iterator< Key, Value, Hash, Equal >;
  public:
    using iter = Iterator< Key, Value, Hash, Equal >;
    using cIter = ConstIterator< Key, Value, Hash, Equal >;
    using pair = const std::pair< Key, Value >;
    HashTable();
    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(HashTable< Key, Value, Hash, Equal > & rhs) noexcept;
    iter begin();
    iter end();
    cIter cbegin() const;
    cIter cend() const;
    cIter find(const Key& k) const;
    iter find(const Key& k);
    Value& operator[](const Key& key);
    const Value& operator[](const Key& key) const;
    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    size_t erase(const Key& key) noexcept;
    iter erase(iter) noexcept;
    std::pair< iter, bool > insert(pair& val);
    void rehash(size_t n);
  private:
    std::vector< Node< Key, Value > > table_;
    size_t size_;
    Hash hash_;
    Equal equal_;
    float max_load_factor_ = 0.7;
    size_t findIndex(const Key& k) const;
    size_t findIndexIn(const Key& k, const std::vector< Node< Key, Value > >& table) const;
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    table_(17),
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
    std::swap(hash_, rhs.hash_);
    std::swap(equal_, rhs.equal_);
    std::swap(max_load_factor_, rhs.max_load_factor_);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iter HashTable< Key, Value, Hash, Equal >::begin()
  {
    return iter{this, 0};
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iter HashTable< Key, Value, Hash, Equal >::end()
  {
    return iter{this, table_.size()};
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::cIter HashTable< Key, Value, Hash, Equal >::cbegin() const
  {
    return cIter{this, 0};
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::cIter HashTable< Key, Value, Hash, Equal >::cend() const
  {
    return cIter{this, table_.size()};
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::findIndex(const Key & k) const
  {
    size_t baseNode = hash_(k) % table_.size();
    size_t currNode = baseNode;
    size_t i = 1;
    while (i <= table_.size())
    {
      if (table_[currNode].occupied && equal_(table_[currNode].data.first, k))
      {
        return currNode;
      }
      if (!table_[currNode].occupied && !table_[currNode].deleted)
      {
        break;
      }
      currNode = (baseNode + i) % table_.size();
      ++i;
    }
    return table_.size();
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::findIndexIn(const Key& k, const std::vector< Node< Key, Value > >& table) const
  {
    size_t baseNode = hash_(k) % table.size();
    size_t currNode = baseNode;
    size_t i = 1;
    while (table[currNode].occupied)
    {
      currNode = (baseNode + i) % table.size();
      ++i;
    }
    return currNode;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t n)
  {
    if (n < table_.size())
    {
      return;
    }
    std::vector< Node< Key, Value > > temp(n);
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
  typename HashTable< Key, Value, Hash, Equal >::iter HashTable< Key, Value, Hash, Equal >::find(const Key& k)
  {
    return iter{this, findIndex(k)};
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::cIter HashTable< Key, Value, Hash, Equal >::find(const Key& k) const
  {
    return cIter{this, findIndex(k)};
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& key)
  {
    iter it = find(key);
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
    cIter it = find(key);
    return it->second;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key)
  {
    iter it = find(key);
    if (it == end())
    {
      throw std::out_of_range("<INVALID COMMAND>");
    }
    return it->second;
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key) const
  {
    cIter it = find(key);
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
  typename HashTable< Key, Value, Hash, Equal >::iter HashTable< Key, Value, Hash, Equal >::erase(iter it) noexcept
  {
    size_t Node = it.index_;
    table_[Node].occupied = false;
    table_[Node].deleted = true;
    --size_;
    return iter{this, it.index_ + 1};
  }


  template< class Key, class Value, class Hash, class Equal >
  std::pair< typename HashTable< Key, Value, Hash, Equal >::iter, bool > HashTable< Key, Value, Hash, Equal >::insert(pair& val)
  {
    if (table_.size() == 0)
    {
      rehash(16);
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
      if (table_[currNode].occupied && equal_(table_[currNode].data.first, val.first))
      {
        return std::make_pair(iter(this, currNode), false);
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
    table_[currNode].data = val;
    table_[currNode].occupied = true;
    table_[currNode].deleted = false;
    ++size_;
    return std::make_pair(iter(this, currNode), true);
  }
}

#endif
