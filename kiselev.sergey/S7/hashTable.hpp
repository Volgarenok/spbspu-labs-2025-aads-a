#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <utility>
#include <cmath>
#include <boost/hash2/fnv1a.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include "iteratorHash.hpp"

namespace kiselev
{
  template< class Key, class Hash = boost::hash2::fnv1a_64 >
  struct BoostHash
  {
    size_t operator()(Key const& key)
    {
      Hash h;
      boost::hash2::hash_append(h, {}, key);
      return boost::hash2::get_integral_result< size_t >(h);
    }
  };

  template< class Key, class Value, class Hash1 = std::hash< Key >, class Hash2 = BoostHash< Key > , class Equal = std::equal_to< Key > >
  class HashTable
  {
  public:
    using value = std::pair< Key, Value >;
    using Iterator = IteratorHash< Key, Value, Hash1, Hash2, Equal, false >;
    using ConstIterator = IteratorHash< Key, Value, Hash1, Hash2, Equal, true >;

    HashTable():
      slots_(10),
      count_(0)
    {}

    explicit HashTable(size_t size):
      slots_(size),
      count_(0)
    {}
    HashTable(const HashTable< Key, Value, Hash1, Hash2, Equal >& table):
      HashTable()
    {
      maxLoadFactor_ = table.maxLoadFactor_;
      for (ConstIterator it = table.cbegin(); it != table.cend(); ++it)
      {
        insert(*it);
      }
    }

    HashTable(HashTable< Key, Value, Hash1, Hash2, Equal >&& table):
      slots_(std::move(table.slots_)),
      count_(std::exchange(table.count_, 0)),
      maxLoadFactor_(std::exchange(table.maxLoadFactor_, 0))
    {}

    template< typename InputIt >
    HashTable(InputIt first, InputIt last):
      HashTable()
    {
      insert(first, last);
    }

    HashTable(std::initializer_list< value > il):
      HashTable(il.begin(), il.end())
    {}

    HashTable& operator=(const HashTable< Key, Value, Hash1, Hash2, Equal >& table)
    {
      HashTable< Key, Value, Hash1, Hash2, Equal > cpy(table);
      swap(cpy);
      return *this;
    }

    HashTable& operator=(HashTable< Key, Value, Hash1, Hash2, Equal >&& table)
    {
      HashTable< Key, Value, Hash1, Hash2, Equal > temp(table);
      swap(temp);
      return *this;
    }

    HashTable& operator=(std::initializer_list< value > il)
    {
      HashTable< Key, Value, Hash1, Hash2, Equal > temp(il);
      swap(temp);
      return *this;
    }

    const Value& operator[](const Key& key) const
    {
      ConstIterator it = find(key);
      return it->second;
    }
    Value& operator[](const Key& key)
    {
      Iterator res = insert({ key, Value() }).first;
      return res->second;
    }

    Value& at(const Key& key)
    {
      return const_cast< Value& >(static_cast< const HashTable< Key, Value, Hash1, Hash2, Equal > >(*this).at(key));
    }

    const Value& at(const Key& key) const
    {
      ConstIterator it = find(key);
      if (it == cend())
      {
        throw std::out_of_range("There is no such key");
      }
      return it->second;
    }

    Iterator begin() noexcept
    {
      size_t index = 0;
      while (index < slots_.size() && slots_[index].status != Status::OCCUPIED)
      {
        ++index;
      }
      return Iterator(this, index);
    }

    Iterator end() noexcept
    {
      return Iterator(this, slots_.size());
    }

    ConstIterator cbegin() const noexcept
    {
      size_t index = 0;
      while (index < slots_.size() && slots_[index].status != Status::OCCUPIED)
      {
        ++index;
      }
      return ConstIterator(this, index);
    }

    ConstIterator cend() const noexcept
    {
      return ConstIterator(this, slots_.size());
    }

    size_t size() const noexcept
    {
      return count_;
    }
    bool empty() const noexcept
    {
      return count_ == 0;
    }

    Iterator erase(Iterator pos)
    {
      ConstIterator it(pos);
      return erase(it);
    }
    Iterator erase(ConstIterator pos)
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
    Iterator erase(ConstIterator first, ConstIterator last)
    {
      while (first != last)
      {
        first = erase(first);
      }
      return Iterator(this, last.index_);
    }
    size_t erase(const Key& key)
    {
      Iterator it = find(key);
      if (it != end())
      {
        erase(it);
        return 1;
      }
      return 0;
    }

    template< typename... Args >
    std::pair< Iterator, bool > emplace(Args&&... args)
    {
      value newVal(std::forward< Args >(args)...);
      if (loadFactor() >= maxLoadFactor_)
      {
        rehash(slots_.size() * 2);
      }
      size_t posIn = slots_.size();
      size_t h1 = Hash1{}(newVal.first);
      size_t h2 = Hash2{}(newVal.first);
      for (size_t i = 0; i < slots_.size(); ++i)
      {
        size_t pos = (h1 + i * h2) % slots_.size();
        if (slots_[pos].status == Status::OCCUPIED)
        {
          if (equal(slots_[pos].pair.first, newVal.first))
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
        return (emplace(std::forward< Args >(args)...));
      }
      slots_[posIn].pair = std::move(newVal);
      slots_[posIn].status = Status::OCCUPIED;
      count_++;
      return { Iterator(this, posIn), true };
    }

    template< typename... Args >
    std::pair< Iterator, bool > emplaceHint(ConstIterator hint, Args... args)
    {
      if (hint == cend())
      {
        return emplace(std::forward< Args >(args)...);
      }
      if (loadFactor() >= maxLoadFactor_)
      {
        rehash(slots_.size() * 2);
        return emplace(std::forward<Args>(args)...);
      }
      value newVal(std::forward< Args >(args)...);
      if (slots_[hint.index_].status == Status::OCCUPIED && equal(slots_[hint.index_].pair.first, newVal.first))
      {
        return { hint, false };
      }
      size_t h1 = Hash1{}(newVal.first);
      size_t h2 = Hash2{}(newVal.first);
      size_t pos = (h1 + (hint.index_ - h1 % slots_.size()) * h2) % slots_.size();
      if (pos < slots_.size() && slots_[pos].status != Status::OCCUPIED)
      {
        slots_[pos].pair = std::move(newVal);
        slots_[pos].status = Status::OCCUPIED;
        count_++;
        return { Iterator(this, pos), true };
      }
      return emplace(std::forward< Args >(args)...);
    }

    std::pair< Iterator, bool > insert(const value& val)
    {
      return emplace(val);
    }

    std::pair< Iterator, bool > insert(value&& val)
    {
      return emplace(std::move(val));
    }

    std::pair< Iterator, bool > insert(ConstIterator hint, const value& val)
    {
      return emplaceHint(hint, val);
    }

    std::pair< Iterator, bool > insert(ConstIterator hint, value&& val)
    {
      return emplaceHint(hint, std::move(val));
    }

    template< class InputIt >
    void insert(InputIt first, InputIt last)
    {
      HashTable< Key, Value, Hash1, Hash2, Equal > temp(*this);
      for (; first != last; ++first)
      {
        temp.insert(*first);
      }
      swap(temp);
    }

    Iterator find(const Key& key)
    {
      size_t h1 = Hash1{}(key);
      size_t h2 = Hash2{}(key);
      for (size_t i = 0; i < slots_.size(); ++i)
      {
        size_t pos = (h1 + i * h2) % slots_.size();
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

    ConstIterator find(const Key& key) const
    {
      size_t h1 = Hash1{}(key);
      size_t h2 = Hash2{}(key);
      for (size_t i = 0; i < slots_.size(); ++i)
      {
        size_t pos = (h1 + i * h2) % slots_.size();
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

    void clear() noexcept
    {
      erase(cbegin(), cend());
    }
    void swap(HashTable< Key, Value, Hash1, Hash2, Equal >& table) noexcept
    {
      std::swap(slots_, table.slots_);
      std::swap(count_, table.count_);
      std::swap(maxLoadFactor_, table.maxLoadFactor_);
    }

    float loadFactor() const
    {
      return static_cast< float >(count_) / slots_.size();
    }

    float maxLoadFactor() const
    {
      return maxLoadFactor_;
    }

    void maxLoadFactor(float ml)
    {
      maxLoadFactor_ = ml;
      if (loadFactor() > maxLoadFactor_)
      {
        rehash(slots_.size() * 2);
      }
    }

    void rehash(size_t count)
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
      HashTable< Key, Value, Hash1, Hash2, Equal > newTable(count);
      newTable.insert(this->cbegin(), this->cend());
      swap(newTable);
    }

  private:
    enum class Status
    {
      EMPTY,
      DELETED,
      OCCUPIED
    };

    struct Slot
    {
      std::pair< Key, Value > pair;
      Status status = Status::EMPTY;
    };

    std::vector< Slot > slots_;
    size_t count_;
    float maxLoadFactor_ = 0.6;
    Equal equal;
    friend class IteratorHash< Key, Value, Hash1, Hash2, Equal, false >;
    friend class IteratorHash< Key, Value, Hash1, Hash2, Equal, true >;
  };
}
#endif
