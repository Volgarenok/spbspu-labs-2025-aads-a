#ifndef ITERATORHASH_HPP
#define ITERATORHASH_HPP

#include "hashTable.hpp"

namespace karnauhova
{
  template< typename Key, typename Value, typename Hash1, typename Equal >
  struct HashCIterator
  {
    using data = std::pair< Key, Value >;
    using Table = HashTable< Key, Value, Hash1, Equal >;
    using this_t = HashCIterator< Key, Value, Hash1, Equal >;
  public:
    HashCIterator();
    HashCIterator(const this_t&);
    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t& operator--() noexcept;
    this_t operator--(int) noexcept;

    bool operator==(const this_t&) const noexcept;
    bool operator!=(const this_t&) const noexcept;

  private:
    const Table* table_;
    size_t index_;
    HashCIterator(Table table, size_t index) noexcept;
  };

  template< typename Key, typename Value, typename Hash1, typename Equal >
  HashCIterator< Key, Value, Hash1, Equal >::HashCIterator():
    table_(nullptr),
    index_(0)
  {}

  template< typename Key, typename Value, typename Hash1, typename Equal >
  HashCIterator< Key, Value, Hash1, Equal >::HashCIterator(const HashCIterator& oth):
    table_(oth.table_),
    index_(oth.index_)
  {}

  template< typename Key, typename Value, typename Hash1, typename Equal >
  HashCIterator< Key, Value, Hash1, Equal >& HashCIterator< Key, Value, Hash1, Equal >::operator++() noexcept
  {
    ++index_;
    while (index_ < table_->slots_.size() && table_->slots_[index_].status != Table::Status::OCCUPIED)
    {
      ++index_;
    }
    return *this;
  }
  
  template< typename Key, typename Value, typename Hash1, typename Equal >
  HashCIterator< Key, Value, Hash1, Equal > HashCIterator< Key, Value, Hash1, Equal >::operator++(int) noexcept
  {
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename Key, typename Value, typename Hash1, typename Equal >
  HashCIterator< Key, Value, Hash1, Equal >& HashCIterator< Key, Value, Hash1, Equal >::operator--() noexcept
  {
    --index_;
    while (index_ > 0 && table_->slots_[index_].status != Table::Status::OCCUPIED)
    {
      --index_;
    }
    return *this;
  }
  
  template< typename Key, typename Value, typename Hash1, typename Equal >
  HashCIterator< Key, Value, Hash1, Equal > HashCIterator< Key, Value, Hash1, Equal >::operator--(int) noexcept
  {
    this_t result(*this);
    --(*this);
    return result;
  }

  template< typename Key, typename Value, typename Hash1, typename Equal >
  bool HashCIterator< Key, Value, Hash1, Equal >::operator==(const this_t& oth) const noexcept
  {
    return table_ == oth.table_ && index_ == oth.index_;
  }

  template< typename Key, typename Value, typename Hash1, typename Equal >
  bool HashCIterator< Key, Value, Hash1, Equal >::operator!=(const this_t& oth) const noexcept
  {
    return !(*this == oth);
  }
}

#endif
