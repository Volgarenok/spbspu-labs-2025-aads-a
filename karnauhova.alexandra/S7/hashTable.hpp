#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <functional>
#include <vector>
#include "hashCIterator.hpp"

namespace karnauhova
{
  template < typename Key, typename Value, typename Hash1 = std::hash< Key >, typename Equal = std::equal_to< Key >>
  class HashTable
  {
  public:
    using value = std::pair< Key, Value >;
    using Iterator = HashCIterator< Key, Value, Hash1, Equal >;
    HashTable();
    explicit HashTable(size_t size):
    HashTable(const HashTable& table);
    HashTable(HashTable&& table);
    HashTable& operator=(const HashTable& table);
    HashTable& operator=(HashTable>&& table);

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
    size_t erase(const Key& key);

    Iterator find(const Key& key);
    ConstIterator find(const Key& key) const;

    std::pair< Iterator, bool > insert(const value& val);
    std::pair< Iterator, bool > insert(value&& val);

    void clear() noexcept;
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
  };

  template < typename Key, typename Value, typename Hash1, typename Equal >
  HashTable< Key, Value, Hash1, Equal >::HashTable():
    slots_(10),
    count_(0)
  {}

  template < typename Key, typename Value, typename Hash1, typename Equal >
  size_t HashTable< Key, Value, Hash1, Equal >::size() const noexcept
  {
    return count_;
  }

  template < typename Key, typename Value, typename Hash1, typename Equal >
  bool HashTable< Key, Value, Hash1, Equal >::empty() const noexcept
  {
    return count_ == 0;
  }

  template < typename Key, typename Value, typename Hash1, typename Equal >
  std::pair< HashCIterator< Key, Value, Hash1, Equal >, bool > HashTable< Key, Value, Hash1, Equal >::insert(const value& val)
  {

  }

  template < typename Key, typename Value, typename Hash1, typename Equal >
  float HashTable< Key, Value, Hash1, Equal >::loadFactor() const
  {
    return static_cast< float >(count_) / slots_.size();
  }

  template < typename Key, typename Value, typename Hash1, typename Equal >
  void HashTable< Key, Value, Hash1, Equal >::swap(HashTable& table) noexcept
  {
    std::swap(slots_, table.slots_);
    std::swap(count_, table.count_);
    std::swap(maxLoadFactor_, table.maxLoadFactor_);
  }

  template < typename Key, typename Value, typename Hash1, typename Equal >
  void HashTable< Key, Value, Hash1, Equal >::rehash(size_t count)
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
}

#endif
