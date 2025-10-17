#ifndef ITERATOR_HASH_HPP
#define ITERATOR_HASH_HPP
#include <vector>
#include <cstddef>
#include <memory>
#include "bucket.hpp"

namespace smirnov
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;
  template< class Key, class Value, class Hash, class Equal >
  class ConstIteratorHash;
  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class IteratorHash: public std::iterator< std::forward_iterator_tag, std::pair< const Key, Value > >
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class ConstIteratorHash< Key, Value, Hash, Equal >;
  public:
    IteratorHash();
    ~IteratorHash() = default;
    std::pair< Key, Value > & operator*();
    std::pair< Key, Value > * operator->();
    IteratorHash & operator++();
    IteratorHash operator++(int);
    bool operator!=(const IteratorHash & rhs) const;
    bool operator==(const IteratorHash & rhs) const;
  private:
    HashTable< Key, Value, Hash, Equal > * table_;
    size_t index_;
    explicit IteratorHash(HashTable< Key, Value, Hash, Equal > * table, size_t index);
    void advanceToOccupied();
  };

  template< class Key, class Value, class Hash, class Equal >
  IteratorHash< Key, Value, Hash, Equal >::IteratorHash():
    table_(nullptr),
    index_(0)
  {}

  template< class Key, class Value, class Hash, class Equal >
  IteratorHash< Key, Value, Hash, Equal >::IteratorHash(HashTable< Key, Value, Hash, Equal > * table, size_t index):
    table_(table),
    index_(index)
  {
    advanceToOccupied();
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< Key, Value > & IteratorHash< Key, Value, Hash, Equal >::operator*()
  {
    return table_->buckets_[index_].data;
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< Key, Value > * IteratorHash< Key, Value, Hash, Equal >::operator->()
  {
    return std::addressof(table_->buckets_[index_].data);
  }

  template < class Key, class Value, class Hash, class Equal >
  void IteratorHash< Key, Value, Hash, Equal >::advanceToOccupied()
  {
    while (index_ < table_->buckets_.size() && (!table_->buckets_[index_].occupied || table_->buckets_[index_].deleted))
    {
      ++index_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  IteratorHash< Key, Value, Hash, Equal > & IteratorHash< Key, Value, Hash, Equal >::operator++()
  {
    ++index_;
    advanceToOccupied();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  IteratorHash< Key, Value, Hash, Equal > IteratorHash< Key, Value, Hash, Equal >::operator++(int)
  {
    IteratorHash tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool IteratorHash< Key, Value, Hash, Equal >::operator==(const IteratorHash & rhs) const
  {
    return table_ == rhs.table_ && index_ == rhs.index_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool IteratorHash< Key, Value, Hash, Equal >::operator!=(const IteratorHash & rhs) const
  {
    return !(*this == rhs);
  }
}
#endif
