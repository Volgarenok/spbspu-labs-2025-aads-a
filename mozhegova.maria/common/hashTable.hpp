#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <functional>
#include "hashSlot.hpp"
#include "hashConstIter.hpp"
#include "hashIter.hpp"
#include "dynamicArray.hpp"

namespace mozhegova
{
  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable
  {
    friend class HashIter< Key, Value, Hash, Equal >;
    friend class HashConstIter< Key, Value, Hash, Equal >;
  public:
    using Iter = HashIter< Key, Value, Hash, Equal >;
    using cIter = HashConstIter< Key, Value, Hash, Equal >;

    HashTable();
    template< class InputIt >
    HashTable(InputIt begin, InputIt end);
    HashTable(std::initializer_list< std::pair< Key, Value > > il);

    Value & operator[](const Key & key);
    const Value & operator[](const Key & key) const;
    Value & at(const Key & key);
    const Value & at(const Key & key) const;

    Iter begin();
    cIter cbegin() const;
    Iter end();
    cIter cend() const;

    size_t size() const noexcept;
    bool empty() const noexcept;

    size_t erase(const Key &) noexcept;
    Iter erase(cIter) noexcept;
    Iter erase(Iter) noexcept;
    Iter erase(cIter first, cIter last) noexcept;

    std::pair< Iter, bool > insert(const std::pair< Key, Value > & val);
    Iter insert(cIter, const std::pair< Key, Value > & val);
    template < class InputIt >
    void insert(InputIt first, InputIt last);

    void clear();
    void swap(HashTable< Key, Value, Hash, Equal > & rhs) noexcept;

    template< class... Args >
    std::pair< Iter, bool > emplace(Args &&... args);

    template< class... Args >
    Iter emplace_hint(cIter hint, Args &&... args);

    Iter find(const Key & k);
    cIter find(const Key & k) const;

    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float val);
    void rehash(size_t n);
  private:
    DynamicArray< Slot< Key, Value > > table_;
    size_t size_;
    Hash hasher_;
    Equal equal_;
    float max_load_factor_ = 0.7;

    size_t findIndex(const Key & k) const;
    size_t findIndexIn(const Key & k, const DynamicArray< Slot< Key, Value > > & table) const;
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    table_(11),
    size_(0)
  {}

  template< class Key, class Value, class Hash, class Equal >
  template< class InputIt >
  HashTable< Key, Value, Hash, Equal >::HashTable(InputIt begin, InputIt end):
    HashTable()
  {
    insert(begin, end);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(std::initializer_list< std::pair< Key, Value > > il):
    HashTable(il.begin(), il.end())
  {}

  template< class Key, class Value, class Hash, class Equal >
  Value & HashTable< Key, Value, Hash, Equal >::operator[](const Key & key)
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
  const Value & HashTable< Key, Value, Hash, Equal >::operator[](const Key & key) const
  {
    cIter it = find(key);
    return it->second;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value & HashTable< Key, Value, Hash, Equal >::at(const Key & key)
  {
    Iter it = find(key);
    if (it == end())
    {
      throw std::out_of_range("<INVALID COMMAND>");
    }
    return it->second;
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value & HashTable< Key, Value, Hash, Equal >::at(const Key & key) const
  {
    cIter it = find(key);
    if (it == cend())
    {
      throw std::out_of_range("<INVALID COMMAND>");
    }
    return it->second;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::begin()
  {
    return Iter{this, 0};
  }

  template< class Key, class Value, class Hash, class Equal >
  HashConstIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::cbegin() const
  {
    return cIter{this, 0};
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::end()
  {
    return Iter{this, table_.size()};
  }

  template< class Key, class Value, class Hash, class Equal >
  HashConstIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::cend() const
  {
    return cIter{this, table_.size()};
  }

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
  void HashTable< Key, Value, Hash, Equal >::clear()
  {
    erase(cbegin(), cend());
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable< Key, Value, Hash, Equal > & rhs) noexcept
  {
    std::swap(table_, rhs.table_);
    std::swap(size_, rhs.size_);
    std::swap(hasher_, rhs.hasher_);
    std::swap(equal_, rhs.equal_);
    std::swap(max_load_factor_, rhs.max_load_factor_);
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::findIndex(const Key & k) const
  {
    size_t homeSlot = hasher_(k) % table_.size();
    size_t currSlot = homeSlot;
    size_t i = 1;
    while (table_[currSlot].occupied || table_[currSlot].deleted)
    {
      if (table_[currSlot].occupied && equal_(table_[currSlot].data.first, k))
      {
        return currSlot;
      }
      currSlot = (homeSlot + i * i) % table_.size();
      ++i;
      if (i >= table_.size())
      {
        break;
      }
    }
    return table_.size();
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::find(const Key & k)
  {
    return Iter{this, findIndex(k)};
  }

  template< class Key, class Value, class Hash, class Equal >
  HashConstIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::find(const Key & k) const
  {
    return cIter{this, findIndex(k)};
  }

  template< class Key, class Value, class Hash, class Equal >
  float HashTable< Key, Value, Hash, Equal >::load_factor() const noexcept
  {
    if (table_.size() == 0)
    {
      return 0.0;
    }
    return size_ / table_.size();
  }

  template< class Key, class Value, class Hash, class Equal >
  float HashTable< Key, Value, Hash, Equal >::max_load_factor() const noexcept
  {
    return max_load_factor_;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::max_load_factor(float val)
  {
    if (val <= 0.0 || val > 1.0)
    {
      throw std::invalid_argument("invalid max_load_factor");
    }
    max_load_factor_ = val;
    if (max_load_factor_ < load_factor())
    {
      rehash(table_.size() * 2);
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::findIndexIn(const Key & k, const DynamicArray< Slot< Key, Value > > & table) const
  {
    size_t homeSlot = hasher_(k) % table.size();
    size_t currSlot = homeSlot;
    size_t i = 1;
    while (table[currSlot].occupied)
    {
      currSlot = (homeSlot + i * i) % table.size();
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
    DynamicArray< Slot< Key, Value > > temp(n);
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
  size_t HashTable< Key, Value, Hash, Equal >::erase(const Key & k) noexcept
  {
    const HashTable & constThis = *this;
    cIter it = constThis.find(k);
    if (it == cend())
    {
      return 0;
    }
    erase(it);
    return 1;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::erase(cIter cit) noexcept
  {
    size_t currSlot = cit.index_;
    table_[currSlot].occupied = false;
    table_[currSlot].deleted = true;
    --size_;
    return Iter{this, cit.index_ + 1};
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::erase(Iter it) noexcept
  {
    size_t currSlot = it.index_;
    table_[currSlot].occupied = false;
    table_[currSlot].deleted = true;
    --size_;
    return Iter{this, it.index_ + 1};
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::erase(cIter first, cIter last) noexcept
  {
    Iter res = end();
    while (first != last)
    {
      res = erase(first++);
    }
    return res;
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< HashIter< Key, Value, Hash, Equal >, bool > HashTable< Key, Value, Hash, Equal >::insert(const std::pair< Key, Value > & val)
  {
    return emplace(val);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::insert(cIter cit, const std::pair< Key, Value > & val)
  {
    return emplace_hint(cit, val);
  }

  template< class Key, class Value, class Hash, class Equal >
  template < class InputIt >
  void HashTable< Key, Value, Hash, Equal >::insert(InputIt first, InputIt last)
  {
    for (auto it = first; it != last; ++it)
    {
      insert(*it);
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  template< class... Args >
  std::pair< HashIter< Key, Value, Hash, Equal >, bool > HashTable< Key, Value, Hash, Equal >::emplace(Args &&... args)
  {
    if ((size_ + 1) / table_.size() > max_load_factor_)
    {
      rehash(table_.size() * 2);
    }
    std::pair< Key, Value > pair(std::forward< Args >(args)...);
    size_t homeSlot = hasher_(pair.first) % table_.size();
    size_t currSlot = homeSlot;
    size_t firstDeleted = table_.size();
    size_t i = 1;
    while (table_[currSlot].occupied || table_[currSlot].deleted)
    {
      if (table_[currSlot].occupied && equal_(table_[currSlot].data.first, pair.first))
      {
        return {Iter{this, currSlot}, false};
      }
      if (table_[currSlot].deleted && firstDeleted == table_.size())
      {
        firstDeleted = currSlot;
      }
      currSlot = (homeSlot + i * i) % table_.size();
      ++i;
    }
    if (firstDeleted != table_.size())
    {
      currSlot = firstDeleted;
    }
    table_[currSlot].data = std::move(pair);
    table_[currSlot].occupied = true;
    table_[currSlot].deleted = false;
    ++size_;
    return {Iter{this, currSlot}, true};
  }

  template< class Key, class Value, class Hash, class Equal >
  template< class... Args >
  HashIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::emplace_hint(cIter hint, Args &&... args)
  {
    std::pair< Key, Value > pair(std::forward< Args >(args)...);
    if (hint != cend())
    {
      if (table_[hint.index_].occupied && key_equal_(table_[hint.index_].data.first, pair.first))
      {
        return hint;
      }
    }
    return emplace(std::move(pair)).first;
  }
}

#endif
