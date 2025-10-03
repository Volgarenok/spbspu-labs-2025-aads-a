#ifndef ITERATORHASH_HPP
#define ITERATORHASH_HPP

#include "HashTable.hpp"
#include "bucket.hpp"

namespace duhanina
{
  template< typename Key, typename Value, typename Hash, typename Equal >
  class HashTable;

  template < class Key, class Value, class Hash, class Equal, bool isConst >
  class IteratorHash
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class IteratorHash< Key, Value, Hash, Equal, true >;
    friend class IteratorHash< Key, Value, Hash, Equal, false >;
  public:
    using value_type = std::conditional_t< isConst, const std::pair< Key, Value >, std::pair< Key, Value > >;
    using pointer = value_type*;
    using reference = value_type&;

    IteratorHash() noexcept;

    reference operator*() const noexcept;
    pointer operator->() const noexcept;

    IteratorHash& operator++() noexcept;
    IteratorHash operator++(int) noexcept;

    bool operator==(const IteratorHash& other) const noexcept;
    bool operator!=(const IteratorHash& other) const noexcept;

  private:
    using BucketPtr = std::conditional_t< isConst, const Bucket< Key, Value >*, Bucket< Key, Value >* >;
    BucketPtr current_;
    BucketPtr end_;

    IteratorHash(BucketPtr ptr, BucketPtr end_ptr) noexcept;
    void skip_empty();
  };

  template < class Key, class Value, class Hash, class Equal, bool isConst >
  void IteratorHash< Key, Value, Hash, Equal, isConst >::skip_empty()
  {
    while (current_ != end_ && (!current_->occupied || current_->deleted))
    {
      ++current_;
    }
  }

  template < class Key, class Value, class Hash, class Equal, bool isConst >
  IteratorHash< Key, Value, Hash, Equal, isConst >::IteratorHash() noexcept:
    current_(nullptr),
    end_(nullptr)
  {}

  template < class Key, class Value, class Hash, class Equal, bool isConst >
  IteratorHash< Key, Value, Hash, Equal, isConst >::IteratorHash(BucketPtr ptr, BucketPtr end_ptr) noexcept:
    current_(ptr),
    end_(end_ptr)
  {
    skip_empty();
  }

  template < class Key, class Value, class Hash, class Equal, bool isConst >
  typename IteratorHash< Key, Value, Hash, Equal, isConst >::reference
    IteratorHash< Key, Value, Hash, Equal, isConst >::operator*() const noexcept
  {
    return current_->data;
  }

  template < class Key, class Value, class Hash, class Equal, bool isConst >
  typename IteratorHash< Key, Value, Hash, Equal, isConst >::pointer
    IteratorHash< Key, Value, Hash, Equal, isConst >::operator->() const noexcept
  {
    return std::addressof(current_->data);
  }

  template < class Key, class Value, class Hash, class Equal, bool isConst >
  typename IteratorHash< Key, Value, Hash, Equal, isConst >::IteratorHash&
    IteratorHash< Key, Value, Hash, Equal, isConst >::operator++() noexcept
  {
    ++current_;
    skip_empty();
    return *this;
  }

  template < class Key, class Value, class Hash, class Equal, bool isConst >
  typename IteratorHash< Key, Value, Hash, Equal, isConst >::IteratorHash
    IteratorHash< Key, Value, Hash, Equal, isConst >::operator++(int) noexcept
  {
    IteratorHash tmp = *this;
    ++(*this);
    return tmp;
  }

  template < class Key, class Value, class Hash, class Equal, bool isConst >
  bool IteratorHash< Key, Value, Hash, Equal, isConst >::operator==(const IteratorHash& other) const noexcept
  {
    return current_ == other.current_;
  }

  template < class Key, class Value, class Hash, class Equal, bool isConst >
  bool IteratorHash< Key, Value, Hash, Equal, isConst >::operator!=(const IteratorHash& other) const noexcept
  {
    return !(*this == other);
  }
}

#endif
