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
  class IteratorHash
  {
    friend class HashTable< Key, Value, Hash, Equal >;
  public:
    IteratorHash();
    ~IteratorHash() = default;
    std::pair< Key, Value > & operator*() const;
    std::pair< Key, Value > * operator->() const;
    IteratorHash & operator++();
    IteratorHash operator++(int);
    bool operator!=(const IteratorHash & rhs) const;
    bool operator==(const IteratorHash & rhs) const;
  private:
    std::vector< Bucket< Key, Value > > * buckets_;
    size_t current_index_;
    explicit IteratorHash(std::vector< Bucket< Key, Value > > * buckets, size_t index);
    void skipInvalidBuckets();
  };

  template< class Key, class Value, class Hash, class Equal >
  IteratorHash< Key, Value, Hash, Equal >::IteratorHash():
    buckets_(nullptr),
    current_index_(0)
  {}

  template< class Key, class Value, class Hash, class Equal >
  IteratorHash< Key, Value, Hash, Equal >::IteratorHash(std::vector< Bucket< Key, Value > > * buckets, size_t index):
    buckets_(buckets),
    current_index_(index)
  {
    skipInvalidBuckets();
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< Key, Value > & IteratorHash< Key, Value, Hash, Equal >::operator*() const
  {
    return (*buckets_)[current_index_].data;
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< Key, Value > * IteratorHash< Key, Value, Hash, Equal >::operator->() const
  {
    return std::addressof(this->operator*());
  }

  template < class Key, class Value, class Hash, class Equal >
  void IteratorHash<Key, Value, Hash, Equal>::skipInvalidBuckets()
  {
    while (buckets_ && current_index_ < buckets_->size())
    {
      auto & bucket = (*buckets_)[current_index_];
      if (bucket.occupied && !bucket.deleted)
      {
        break;
      }
      ++current_index_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  IteratorHash< Key, Value, Hash, Equal > & IteratorHash< Key, Value, Hash, Equal >::operator++()
  {
    ++current_index_;
    skipInvalidBuckets();
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
    return buckets_ == rhs.buckets_ && current_index_ == rhs.current_index_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool IteratorHash< Key, Value, Hash, Equal >::operator!=(const IteratorHash & rhs) const
  {
    return !(*this == rhs);
  }
}
#endif
