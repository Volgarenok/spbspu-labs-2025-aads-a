#ifndef HASH_TABLE_ITERATORS_HPP
#define HASH_TABLE_ITERATORS_HPP

#include <cstddef>
#include <memory>

#include "bucket.hpp"

namespace zholobov {

  template < typename Key, typename T, bool isConst >
  class HashTableIterator {
    template < typename, typename, typename, typename >
    friend class HashTable;

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::conditional_t< isConst, const std::pair< Key, T >, std::pair< Key, T > >;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    HashTableIterator() noexcept;
    HashTableIterator& operator++() noexcept;
    HashTableIterator operator++(int) noexcept;
    reference operator*() const noexcept;
    pointer operator->() const noexcept;
    bool operator==(const HashTableIterator& other) const noexcept;
    bool operator!=(const HashTableIterator& other) const noexcept;

  private:
    using BucketPtr = std::conditional_t< isConst, const Bucket< Key, T >*, Bucket< Key, T >* >;
    HashTableIterator(BucketPtr current, BucketPtr end) noexcept;
    void advance() noexcept;
    BucketPtr current_;
    BucketPtr end_;
  };

  template < typename Key, typename T, bool isConst >
  HashTableIterator< Key, T, isConst >::HashTableIterator() noexcept:
    current_(nullptr),
    end_(nullptr)
  {}

  template < typename Key, typename T, bool isConst >
  typename HashTableIterator< Key, T, isConst >::HashTableIterator&
  HashTableIterator< Key, T, isConst >::operator++() noexcept
  {
    ++current_;
    advance();
    return *this;
  }

  template < typename Key, typename T, bool isConst >
  typename HashTableIterator< Key, T, isConst >::HashTableIterator
  HashTableIterator< Key, T, isConst >::operator++(int) noexcept
  {
    HashTableIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template < typename Key, typename T, bool isConst >
  typename HashTableIterator< Key, T, isConst >::reference
  HashTableIterator< Key, T, isConst >::operator*() const noexcept
  {
    return current_->data;
  }

  template < typename Key, typename T, bool isConst >
  typename HashTableIterator< Key, T, isConst >::pointer
  HashTableIterator< Key, T, isConst >::operator->() const noexcept
  {
    return std::addressof(current_->data);
  }

  template < typename Key, typename T, bool isConst >
  bool HashTableIterator< Key, T, isConst >::operator==(const HashTableIterator& other) const noexcept
  {
    return current_ == other.current_;
  }

  template < typename Key, typename T, bool isConst >
  bool HashTableIterator< Key, T, isConst >::operator!=(const HashTableIterator& other) const noexcept
  {
    return !(*this == other);
  }

  template < typename Key, typename T, bool isConst >
  HashTableIterator< Key, T, isConst >::HashTableIterator(BucketPtr current, BucketPtr end) noexcept:
    current_(current),
    end_(end)
  {
    advance();
  }

  template < typename Key, typename T, bool isConst >
  void HashTableIterator< Key, T, isConst >::advance() noexcept
  {
    while (current_ != end_ && (!current_->occupied || current_->deleted)) {
      ++current_;
    }
  }

}

#endif
