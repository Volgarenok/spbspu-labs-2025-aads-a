#ifndef CONST_ITERATOR_HASH_HPP
#define CONST_ITERATOR_HASH_HPP
#include <cstddef>
#include <iterator>
#include "bucket.hpp"

namespace smirnov
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;
  template< class Key, class Value, class Hash, class Equal >
  class IteratorHash;
  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class ConstIteratorHash: public std::iterator< std::forward_iterator_tag, std::pair< const Key, Value > >
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class IteratorHash< Key, Value, Hash, Equal >;
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
    const HashTable< Key, Value, Hash, Equal > * table_;
    size_t index_;
    explicit ConstIteratorHash(const HashTable< Key, Value, Hash, Equal > * table, size_t index);
    void advanceToOccupied();
  };

  template< class Key, class Value, class Hash, class Equal >
  ConstIteratorHash< Key, Value, Hash, Equal >::ConstIteratorHash():
    table_(nullptr),
    index_(0)
  {}

  template< class Key, class Value, class Hash, class Equal >
  ConstIteratorHash< Key, Value, Hash, Equal >::ConstIteratorHash(const HashTable< Key, Value, Hash, Equal > * table, size_t index):
    table_(table),
    index_(index)
  {
    advanceToOccupied();
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair< Key, Value > & ConstIteratorHash< Key, Value, Hash, Equal >::operator*() const
  {
    return table_->buckets_[index_].data;
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair< Key, Value > * ConstIteratorHash< Key, Value, Hash, Equal >::operator->() const
  {
    return std::addressof(table_->buckets_[index_].data);
  }

  template < class Key, class Value, class Hash, class Equal >
  void smirnov::ConstIteratorHash< Key, Value, Hash, Equal >::advanceToOccupied()
  {
    while (index_ < table_->buckets_.size() && (!table_->buckets_[index_].occupied || table_->buckets_[index_].deleted))
    {
      ++index_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIteratorHash< Key, Value, Hash, Equal > & ConstIteratorHash< Key, Value, Hash, Equal >::operator++()
  {
    ++index_;
    advanceToOccupied();
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
    return table_ == rhs.table_ && index_ == rhs.index_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool ConstIteratorHash< Key, Value, Hash, Equal >::operator!=(const ConstIteratorHash & rhs) const
  {
    return !(*this == rhs);
  }
}
#endif
