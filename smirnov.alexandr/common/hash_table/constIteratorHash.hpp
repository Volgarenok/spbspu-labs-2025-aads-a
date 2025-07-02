#ifndef CONST_ITERATOR_HASH_HPP
#define CONST_ITERATOR_HASH_HPP
#include <vector>
#include <cstddef>
#include <memory>
#include "bucket.hpp"

namespace smirnov
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;
  template< class Key, class Value, class Hash, class Equal >
  class ConstIteratorHash
  {
    friend class HashTable< Key, Value, Hash, Equal >;
  public:
    ConstIteratorHash();
    ~ConstIteratorHash() = default;
    const std::pair< Key, Value > & operator*() const;
    const std::pair< Key, Value > * operator->() const;
    ConstIteratorHash & operator++();
    ConstIteratorHash operator++(int);
    bool operator!=(const ConstIteratorHash & rhs) const;
    bool operator==(const ConstIteratorHash & rhs) const;
  private:
    const std::vector< Bucket< Key, Value > > * buckets_;
    size_t current_index_;
    explicit ConstIteratorHash(const std::vector< Bucket< Key, Value > > * buckets, size_t index);
    void skipInvalidBuckets();
  };

  template< class Key, class Value, class Hash, class Equal >
  ConstIteratorHash< Key, Value, Hash, Equal >::ConstIteratorHash():
    buckets_(nullptr),
    current_index_(0)
  {}

  template< class Key, class Value, class Hash, class Equal >
  ConstIteratorHash< Key, Value, Hash, Equal >::ConstIteratorHash(const std::vector< Bucket< Key, Value > > * buckets, size_t index):
    buckets_(buckets),
    current_index_(index)
  {
    skipInvalidBuckets();
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair< Key, Value > & ConstIteratorHash< Key, Value, Hash, Equal >::operator*() const
  {
    return (*buckets_)[current_index_].data;
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair< Key, Value > * ConstIteratorHash< Key, Value, Hash, Equal >::operator->() const
  {
    return std::addressof(this->operator*());
  }

  template < class Key, class Value, class Hash, class Equal >
  void smirnov::ConstIteratorHash< Key, Value, Hash, Equal >::skipInvalidBuckets()
  {
    while (buckets_ && current_index_ < buckets_->size())
    {
      const auto & bucket = (*buckets_)[current_index_];
      if (bucket.occupied && !bucket.deleted)
      {
        break;
      }
      ++current_index_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIteratorHash< Key, Value, Hash, Equal > & ConstIteratorHash< Key, Value, Hash, Equal >::operator++()
  {
    ++current_index_;
    skipInvalidBuckets();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIteratorHash< Key, Value, Hash, Equal > ConstIteratorHash< Key, Value, Hash, Equal >::operator++(int)
  {
    ConstIteratorHash tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool ConstIteratorHash< Key, Value, Hash, Equal >::operator==(const ConstIteratorHash & rhs) const
  {
    return buckets_ == rhs.buckets_ && current_index_ == rhs.current_index_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool ConstIteratorHash< Key, Value, Hash, Equal >::operator!=(const ConstIteratorHash & rhs) const
  {
    return !(*this == rhs);
  }
}
#endif
