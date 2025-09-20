#ifndef ITERATORHASH_HPP
#define ITERATORHASH_HPP

#include "hashTable.hpp"

namespace karnauhova
{
  template< typename Key, typename Value, typename Hash1, typename Hash2, typename Equal >
  struct HashCIterator
  {
    using data = std::pair< Key, Value >;
    using Table = HashTable< Key, Value, Hash1, Hash2, Equal >;
    using this_t = HashCIterator< Key, Value, Hash1, Hash2, Equal >;
  public:
    HashCIterator();
    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;

  private:
    const Table* table_;
    size_t index_;
    HashCIterator(Table table, size_t index) noexcept;
  };

  template< typename Key, typename Value, typename Hash1, typename Hash2, typename Equal >
  HashCIterator< Key, Value, Hash1, Hash2, Equal >::HashCIterator():
    table_(nullptr),
    index_(0)
  {}

  template< typename Key, typename Value, typename Hash1, typename Hash2, typename Equal >
  HashCIterator< Key, Value, Hash1, Hash2, Equal >& HashCIterator< Key, Value, Hash1, Hash2, Equal >::operator++() noexcept
  {

  }
  
  template< typename Key, typename Value, typename Hash1, typename Hash2, typename Equal >
  HashCIterator< Key, Value, Hash1, Hash2, Equal > HashCIterator< Key, Value, Hash1, Hash2, Equal >::operator++(int) noexcept
  {
    
  }
}

#endif
