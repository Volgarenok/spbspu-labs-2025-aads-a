#ifndef HASHITERATOR_HPP
#define HASHITERATOR_HPP

#include "hashTable.hpp"

namespace karnauhova
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HashCIterator;

  template< typename Key, typename Value, typename Hash, typename Equal >
  struct HashIterator
  {
    using data = std::pair< Key, Value >;
    using Table = HashTable< Key, Value, Hash, Equal >;
    using this_t = HashIterator< Key, Value, Hash, Equal >;
  public:
    HashIterator();

    data& operator*() const noexcept;
    data* operator->() const noexcept;

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t& operator--() noexcept;
    this_t operator--(int) noexcept;

    bool operator==(const this_t&) const noexcept;
    bool operator!=(const this_t&) const noexcept;

  private:
    Table* table_;
    size_t index_;
    HashIterator(Table table, size_t index) noexcept;\

    friend class HashCIterator< Key, Value, Hash, Equal >;
    friend class HashTable< Key, Value, Hash, Equal >;
  };

  template< typename Key, typename Value, typename Hash, typename Equal >
  HashIterator< Key, Value, Hash, Equal >::HashIterator():
    table_(nullptr),
    index_(0)
  {}

  template< typename Key, typename Value, typename Hash, typename Equal >
  HashIterator< Key, Value, Hash, Equal >::HashIterator(Table table, size_t index) noexcept:
    table_(table),
    index_(index)
  {}

  template< typename Key, typename Value, typename Hash, typename Equal >
  typename HashIterator< Key, Value, Hash, Equal >::data& HashIterator< Key, Value, Hash, Equal >::operator*() const noexcept
  {
    return table_->slots_[index_].pair;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  typename HashIterator< Key, Value, Hash, Equal >::data* HashIterator< Key, Value, Hash, Equal >::operator->() const noexcept
  {
    return std::addressof(table_->slots_[index_].pair);
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  HashIterator< Key, Value, Hash, Equal >& HashIterator< Key, Value, Hash, Equal >::operator++() noexcept
  {
    ++index_;
    while (index_ < table_->slots_.size() && table_->slots_[index_].status != Table::Status::OCCUPIED)
    {
      ++index_;
    }
    return *this;
  }
  
  template< typename Key, typename Value, typename Hash, typename Equal >
  HashIterator< Key, Value, Hash, Equal > HashIterator< Key, Value, Hash, Equal >::operator++(int) noexcept
  {
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  HashIterator< Key, Value, Hash, Equal >& HashIterator< Key, Value, Hash, Equal >::operator--() noexcept
  {
    --index_;
    while (index_ > 0 && table_->slots_[index_].status != Table::Status::OCCUPIED)
    {
      --index_;
    }
    return *this;
  }
  
  template< typename Key, typename Value, typename Hash, typename Equal >
  HashIterator< Key, Value, Hash, Equal > HashIterator< Key, Value, Hash, Equal >::operator--(int) noexcept
  {
    this_t result(*this);
    --(*this);
    return result;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  bool HashIterator< Key, Value, Hash, Equal >::operator==(const this_t& oth) const noexcept
  {
    return table_ == oth.table_ && index_ == oth.index_;
  }

  template< typename Key, typename Value, typename Hash, typename Equal >
  bool HashIterator< Key, Value, Hash, Equal >::operator!=(const this_t& oth) const noexcept
  {
    return !(*this == oth);
  }
}

#endif
