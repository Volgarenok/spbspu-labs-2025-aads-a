
#ifndef ITERATOR_HASH_TABLE_HPP
#define ITERATOR_HASH_TABLE_HPP

#include <cassert>
#include <iterator>
#include <utility>

namespace gavrilova {
  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  class HashTable;

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  class ConstIteratorHashTable;
}

namespace gavrilova {

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  struct IteratorHashTable: public std::iterator< std::forward_iterator_tag, std::pair< const Key, Value > > {
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::pair< const Key, Value >;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    friend class HashTable< Key, Value, Hash, KeyEqual >;
    friend struct ConstIteratorHashTable< Key, Value, Hash, KeyEqual >;

    using this_t = IteratorHashTable< Key, Value, Hash, KeyEqual >;
    using Table = gavrilova::HashTable< Key, Value, Hash, KeyEqual >;

    IteratorHashTable();
    ~IteratorHashTable() = default;
    IteratorHashTable(const this_t&) = default;
    this_t& operator=(const this_t&) = default;

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;

    reference operator*() const;
    pointer operator->() const;

    bool operator!=(const this_t&) const;
    bool operator==(const this_t&) const;

    bool operator!=(const ConstIteratorHashTable< Key, Value, Hash, KeyEqual >& other) const;
    bool operator==(const ConstIteratorHashTable< Key, Value, Hash, KeyEqual >& other) const;

  private:
    Table* table_;
    size_t index_;

    explicit IteratorHashTable(HashTable< Key, Value, Hash, KeyEqual >* table, size_t index);
  };

  template < typename Key, typename Value, typename Hash, typename Equal >
  IteratorHashTable< Key, Value, Hash, Equal >::IteratorHashTable():
    table_(nullptr),
    index_(0)
  {}

  template < typename Key, typename Value, typename Hash, typename Equal >
  IteratorHashTable< Key, Value, Hash, Equal >::IteratorHashTable(HashTable< Key, Value, Hash, Equal >* table, size_t index):
    table_(table),
    index_(index)
  {}

  template < typename Key, typename Value, typename Hash, typename Equal >
  typename IteratorHashTable< Key, Value, Hash, Equal >::this_t&
  IteratorHashTable< Key, Value, Hash, Equal >::operator++() noexcept
  {
    assert(table_ != nullptr);

    index_ = table_->find_next_occupied(index_ + 1);
    return *this;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename IteratorHashTable< Key, Value, Hash, KeyEqual >::this_t
  IteratorHashTable< Key, Value, Hash, KeyEqual >::operator++(int) noexcept
  {
    this_t temp(*this);
    ++(*this);
    return temp;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename IteratorHashTable< Key, Value, Hash, KeyEqual >::reference
  IteratorHashTable< Key, Value, Hash, KeyEqual >::operator*() const
  {
    assert(table_ != nullptr);
    assert(index_ < table_->buckets_.size() && table_->buckets_[index_].state_ == Table::SlotState::OCCUPIED);

    return *table_->buckets_[index_].get_value_ptr();
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename IteratorHashTable< Key, Value, Hash, KeyEqual >::pointer
  IteratorHashTable< Key, Value, Hash, KeyEqual >::operator->() const
  {
    assert(table_ != nullptr);
    assert(index_ < table_->buckets_.size() && table_->buckets_[index_].state_ == Table::SlotState::OCCUPIED);

    return table_->buckets_[index_].get_value_ptr();
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  bool IteratorHashTable< Key, Value, Hash, KeyEqual >::operator==(const this_t& rhs) const
  {
    return table_ == rhs.table_ && index_ == rhs.index_;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  bool IteratorHashTable< Key, Value, Hash, KeyEqual >::operator!=(const this_t& rhs) const
  {
    return !(*this == rhs);
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  bool IteratorHashTable< Key, Value, Hash, KeyEqual >::operator==(
      const ConstIteratorHashTable< Key, Value, Hash, KeyEqual >& other) const
  {
    return table_ == other.table_ && index_ == other.index_;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  bool IteratorHashTable< Key, Value, Hash, KeyEqual >::operator!=(
      const ConstIteratorHashTable< Key, Value, Hash, KeyEqual >& other) const
  {
    return !(*this == other);
  }
}

#endif
