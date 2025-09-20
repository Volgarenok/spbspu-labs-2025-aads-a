
#ifndef CONST_ITERATOR_HASH_TABLE_HPP
#define CONST_ITERATOR_HASH_TABLE_HPP

#include <cassert>
#include <iterator>
#include <utility>

namespace gavrilova {
  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  class HashTable;

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  class IteratorHashTable;
}

namespace gavrilova {

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  struct ConstIteratorHashTable {
    using iterator_category = std::forward_iterator_tag;
    using value_type = const std::pair< const Key, Value >;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    friend class HashTable< Key, Value, Hash, KeyEqual >;
    friend struct IteratorHashTable< Key, Value, Hash, KeyEqual >;

    using this_t = ConstIteratorHashTable< Key, Value, Hash, KeyEqual >;
    using Table = gavrilova::HashTable< Key, Value, Hash, KeyEqual >;

    ConstIteratorHashTable();
    ~ConstIteratorHashTable() = default;
    ConstIteratorHashTable(const this_t&) = default;
    this_t& operator=(const this_t&) = default;

    ConstIteratorHashTable(const IteratorHashTable< Key, Value, Hash, KeyEqual >& other);

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;

    reference operator*() const;
    pointer operator->() const;

    bool operator!=(const this_t&) const;
    bool operator==(const this_t&) const;

    bool operator==(const IteratorHashTable< Key, Value, Hash, KeyEqual >& other) const;
    bool operator!=(const IteratorHashTable< Key, Value, Hash, KeyEqual >& other) const;

  private:
    const Table* table_;
    size_t index_;

    explicit ConstIteratorHashTable(const Table* table, size_t index);
  };

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  ConstIteratorHashTable< Key, Value, Hash, KeyEqual >::ConstIteratorHashTable():
    table_(nullptr),
    index_(0)
  {}

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  ConstIteratorHashTable< Key, Value, Hash, KeyEqual >::ConstIteratorHashTable(const Table* table, size_t index):
    table_(table),
    index_(index)
  {}

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  ConstIteratorHashTable< Key, Value, Hash, KeyEqual >::ConstIteratorHashTable(
      const IteratorHashTable< Key, Value, Hash, KeyEqual >& other):
    table_(other.table_),
    index_(other.index_)
  {}

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename ConstIteratorHashTable< Key, Value, Hash, KeyEqual >::this_t&
  ConstIteratorHashTable< Key, Value, Hash, KeyEqual >::operator++() noexcept
  {
    assert(table_ != nullptr);

    index_ = table_->find_next_occupied(index_ + 1);
    return *this;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename ConstIteratorHashTable< Key, Value, Hash, KeyEqual >::this_t
  ConstIteratorHashTable< Key, Value, Hash, KeyEqual >::operator++(int) noexcept
  {
    this_t temp(*this);
    ++(*this);
    return temp;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename ConstIteratorHashTable< Key, Value, Hash, KeyEqual >::reference
  ConstIteratorHashTable< Key, Value, Hash, KeyEqual >::operator*() const
  {
    assert(table_ != nullptr);
    assert(index_ < table_->buckets_.size() && table_->buckets_[index_].state_ == Table::SlotState::OCCUPIED);

    return *table_->buckets_[index_].get_value_ptr();
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  typename ConstIteratorHashTable< Key, Value, Hash, KeyEqual >::pointer
  ConstIteratorHashTable< Key, Value, Hash, KeyEqual >::operator->() const
  {
    assert(table_ != nullptr);
    assert(index_ < table_->buckets_.size() && table_->buckets_[index_].state_ == Table::SlotState::OCCUPIED);

    return table_->buckets_[index_].get_value_ptr();
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  bool ConstIteratorHashTable< Key, Value, Hash, KeyEqual >::operator==(const this_t& rhs) const
  {
    return table_ == rhs.table_ && index_ == rhs.index_;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  bool ConstIteratorHashTable< Key, Value, Hash, KeyEqual >::operator!=(const this_t& rhs) const
  {
    return !(*this == rhs);
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  bool ConstIteratorHashTable< Key, Value, Hash, KeyEqual >::operator==(
      const IteratorHashTable< Key, Value, Hash, KeyEqual >& other) const
  {
    return table_ == other.table_ && index_ == other.index_;
  }

  template < typename Key, typename Value, typename Hash, typename KeyEqual >
  bool ConstIteratorHashTable< Key, Value, Hash, KeyEqual >::operator!=(
      const IteratorHashTable< Key, Value, Hash, KeyEqual >& other) const
  {
    return !(*this == other);
  }
}

#endif
