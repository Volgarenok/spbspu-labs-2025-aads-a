#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <functional>
#include <vector>
#include <cmath>
#include "hashCIterator.hpp"
#include "hashIterator.hpp"

namespace karnauhova
{
  template < typename Key, typename Value, typename Hash = std::hash< Key >, typename Equal = std::equal_to< Key >>
  class HashTable
  {
  public:
    using value = std::pair< Key, Value >;
    using Iterator = HashIterator< Key, Value, Hash, Equal >;
    using ConstIterator = HashCIterator< Key, Value, Hash, Equal >;
    HashTable();
    explicit HashTable(size_t size);
    HashTable(const HashTable& table);
    HashTable(HashTable&& table);
    HashTable& operator=(const HashTable& table);
    HashTable& operator=(HashTable&& table);

    Value& operator[](const Key& key);
    Value& at(const Key& key);
    const Value& at(const Key& key) const;

    Iterator begin() noexcept;
    ConstIterator cbegin() const noexcept;
    Iterator end() noexcept;
    ConstIterator cend() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    Iterator erase(Iterator pos);
    Iterator erase(ConstIterator pos);

    Iterator find(const Key& key);
    ConstIterator find(const Key& key) const;

    std::pair< Iterator, bool > insert(const value& val);
    template< class InputIt >
    void insert(InputIt first, InputIt last);

    void swap(HashTable& table) noexcept;
    float loadFactor() const;

    void rehash(size_t count);
  private:

    enum class Status
    {
      EMPTY,
      DELETED,
      OCCUPIED
    };

    struct Box
    {
      std::pair< Key, Value > pair;
      Status status = Status::EMPTY;
    };

    std::vector< Box > slots_;
    size_t count_;
    float maxLoadFactor_ = 0.6;
    Equal equal;

    friend class HashCIterator< Key, Value, Hash, Equal >;
    friend class HashIterator< Key, Value, Hash, Equal >;
  };

  template < typename Key, typename Value, typename Hash, typename Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    slots_(10),
    count_(0)
  {}

  template < typename Key, typename Value, typename Hash, typename Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t size):
    slots_(size),
    count_(0)
  {}

  template < typename Key, typename Value, typename Hash, typename Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& table):
    HashTable()
  {
    maxLoadFactor_ = table.maxLoadFactor_;
    for (ConstIterator it = table.cbegin(); it != table.cend(); ++it)
    {
      insert(*it);
    }
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& table):
    slots_(std::move(table.slots_)),
    count_(std::exchange(table.count_, 0)),
    maxLoadFactor_(std::exchange(table.maxLoadFactor_, 0))
  {}

  template < typename Key, typename Value, typename Hash, typename Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& table)
  {
    HashTable cpy(table);
    swap(cpy);
    return *this;
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& table)
  {
    HashTable temp(table);
    swap(temp);
    return *this;
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const noexcept
  {
    return count_;
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  bool HashTable< Key, Value, Hash, Equal >::empty() const noexcept
  {
    return count_ == 0;
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  float HashTable< Key, Value, Hash, Equal >::loadFactor() const
  {
    return static_cast< float >(count_) / slots_.size();
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable& table) noexcept
  {
    std::swap(slots_, table.slots_);
    std::swap(count_, table.count_);
    std::swap(maxLoadFactor_, table.maxLoadFactor_);
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  std::pair< typename HashTable< Key, Value, Hash, Equal >::Iterator, bool > HashTable< Key, Value, Hash, Equal >::insert(const value& val)
  {
    if (loadFactor() >= maxLoadFactor_)
    {
      rehash(slots_.size() * 2);
    }

    size_t posIn = slots_.size();
    size_t hash = Hash{}(val.first);

    for (size_t i = 0; i < slots_.size(); ++i)
    {
      size_t pos = (hash + i) % slots_.size();
      if (slots_[pos].status == Status::OCCUPIED)
      {
        if (equal(slots_[pos].pair.first, val.first))
        {
          return { Iterator(this, pos), false };
        }
      }
      else
      {
        if (posIn == slots_.size())
        {
          posIn = pos;
        }
        if (slots_[pos].status == Status::EMPTY)
        {
          posIn = pos;
          break;
        }
      }
    }

    if (posIn == slots_.size())
    {
      rehash(slots_.size() * 2);
      return insert(val);
    }

    slots_[posIn].pair = val;
    slots_[posIn].status = Status::OCCUPIED;
    count_++;
    return { Iterator(this, posIn), true };
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  template< class InputIt >
  void HashTable< Key, Value, Hash, Equal >::insert(InputIt first, InputIt last)
  {
    HashTable< Key, Value, Hash, Equal > temp(*this);
    for (; first != last; ++first)
    {
      temp.insert(*first);
    }
    swap(temp);
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  typename HashTable< Key, Value, Hash, Equal >::Iterator HashTable< Key, Value, Hash, Equal >::erase(ConstIterator pos)
  {
    if (pos == end())
    {
      return Iterator(this, pos.index_);
    }
    if (pos.index_ >= slots_.size() || slots_[pos.index_].status != Status::OCCUPIED)
    {
      return end();
    }
    slots_[pos.index_].status = Status::DELETED;
    --count_;
    ++pos;
    return Iterator(this, pos.index_);
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  typename HashTable< Key, Value, Hash, Equal >::Iterator HashTable< Key, Value, Hash, Equal >::erase(Iterator pos)
  {
    ConstIterator it(pos);
    return erase(it);
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t count)
  {
    size_t minSize = static_cast< size_t >(std::ceil(count_ / maxLoadFactor_));
    if (count < minSize)
    {
      count = minSize;
    }
    if (count <= slots_.size())
    {
      return;
    }
    HashTable newTable(count);
    newTable.insert(this->cbegin(), this->cend());
    swap(newTable);
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  typename HashTable< Key, Value, Hash, Equal >::Iterator HashTable< Key, Value, Hash, Equal >::begin() noexcept
  {
    size_t index = 0;
    while (index < slots_.size() && slots_[index].status != Status::OCCUPIED)
    {
      ++index;
    }
    return Iterator(this, index);
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  typename HashTable< Key, Value, Hash, Equal >::Iterator HashTable< Key, Value, Hash, Equal >::end() noexcept
  {
    return Iterator(this, slots_.size());
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  typename HashTable< Key, Value, Hash, Equal >::ConstIterator HashTable< Key, Value, Hash, Equal >::cbegin() const noexcept
  {
    size_t index = 0;
    while (index < slots_.size() && slots_[index].status != Status::OCCUPIED)
    {
      ++index;
    }
    return ConstIterator(this, index);
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  typename HashTable< Key, Value, Hash, Equal >::ConstIterator HashTable< Key, Value, Hash, Equal >::cend() const noexcept
  {
    return ConstIterator(this, slots_.size());
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  typename HashTable< Key, Value, Hash, Equal >::Iterator HashTable< Key, Value, Hash, Equal >::find(const Key& key)
  {
    if (slots_.empty())
    {
      return end();
    }

    size_t hash = Hash{}(key);

    for (size_t i = 0; i < slots_.size(); ++i)
    {
      size_t pos = (hash + i) % slots_.size();
      if (slots_[pos].status == Status::EMPTY)
      {
        return end();
      }

      if (slots_[pos].status == Status::OCCUPIED && equal(slots_[pos].pair.first, key))
      {
        return Iterator(this, pos);
      }
    }

    return end();
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  typename HashTable< Key, Value, Hash, Equal >::ConstIterator HashTable< Key, Value, Hash, Equal >::find(const Key& key) const
  {
    if (slots_.empty())
    {
      return cend();
    }

    size_t hash = Hash{}(key);

    for (size_t i = 0; i < slots_.size(); ++i)
    {
      size_t pos = (hash + i) % slots_.size();
      if (slots_[pos].status == Status::EMPTY)
      {
        return cend();
      }

      if (slots_[pos].status == Status::OCCUPIED && equal(slots_[pos].pair.first, key))
      {
        return ConstIterator(this, pos);
      }
    }

    return cend();
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& key)
  {
    Iterator res = insert({ key, Value() }).first;
    return res->second;
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key)
  {
    return const_cast< Value& >(static_cast< const HashTable< Key, Value, Hash, Equal > >(*this).at(key));
  }

  template < typename Key, typename Value, typename Hash, typename Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key) const
  {
    ConstIterator it = find(key);
    if (it == cend())
    {
      throw std::out_of_range("There is no such key");
    }
    return it->second;
  }
}

#endif
