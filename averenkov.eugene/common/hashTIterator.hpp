#ifndef ITERATORHASH_HPP
#define ITERATORHASH_HPP

#include <iterator>
#include <type_traits>
#include "bucket.hpp"

namespace averenkov
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
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::conditional_t< isConst, const detail::Bucket< Key, Value >, detail::Bucket< Key, Value > >;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    IteratorHash() noexcept;

    value_type& operator*() const noexcept;
    value_type* operator->() const noexcept;

    IteratorHash& operator++() noexcept;
    IteratorHash operator++(int) noexcept;

    bool operator==(const IteratorHash& other) const noexcept;
    bool operator!=(const IteratorHash& other) const noexcept;

  private:
    using BucketPtr = std::conditional_t< isConst, const detail::Bucket< Key, Value >*, detail::Bucket< Key, Value >* >;

    BucketPtr current_;
    BucketPtr end_;
    Equal key_equal_;

    IteratorHash(BucketPtr ptr, BucketPtr end_ptr) noexcept;
    IteratorHash(BucketPtr ptr, BucketPtr end_ptr, Equal key_equal) noexcept;
    void skip_empty();
    bool is_valid_data(BucketPtr bucket);
  };
}

template < class Key, class Value, class Hash, class Equal, bool isConst >
averenkov::IteratorHash< Key, Value, Hash, Equal, isConst >::IteratorHash() noexcept:
  current_(nullptr),
  end_(nullptr),
  key_equal_(Equal())
{}


template < class Key, class Value, class Hash, class Equal, bool isConst >
averenkov::IteratorHash< Key, Value, Hash, Equal, isConst >::IteratorHash(BucketPtr ptr, BucketPtr end_ptr) noexcept:
  current_(ptr),
  end_(end_ptr)
{
  skip_empty();
}

template < class Key, class Value, class Hash, class Equal, bool isConst >
averenkov::IteratorHash< Key, Value, Hash, Equal, isConst >::IteratorHash(BucketPtr ptr, BucketPtr end_ptr, Equal key_equal) noexcept:
  current_(ptr),
  end_(end_ptr),
  key_equal_(key_equal)
{
  skip_empty();
}

template < class Key, class Value, class Hash, class Equal, bool isConst >
typename averenkov::IteratorHash< Key, Value, Hash, Equal, isConst >::reference
averenkov::IteratorHash< Key, Value, Hash, Equal, isConst >::operator*() const noexcept
{
  return *current_;
}

template < class Key, class Value, class Hash, class Equal, bool isConst >
typename averenkov::IteratorHash< Key, Value, Hash, Equal, isConst >::pointer
averenkov::IteratorHash< Key, Value, Hash, Equal, isConst >::operator->() const noexcept
{
  return current_;
}

template < class Key, class Value, class Hash, class Equal, bool isConst >
averenkov::IteratorHash< Key, Value, Hash, Equal, isConst >&
averenkov::IteratorHash< Key, Value, Hash, Equal, isConst >::operator++() noexcept
{
  ++current_;
  skip_empty();
  return *this;
}

template < class Key, class Value, class Hash, class Equal, bool isConst >
averenkov::IteratorHash< Key, Value, Hash, Equal, isConst >
averenkov::IteratorHash< Key, Value, Hash, Equal, isConst >::operator++(int) noexcept
{
  IteratorHash tmp = *this;
  ++(*this);
  return tmp;
}

template < class Key, class Value, class Hash, class Equal, bool isConst >
bool averenkov::IteratorHash< Key, Value, Hash, Equal, isConst >::operator==(const IteratorHash& other) const noexcept
{
  return current_ == other.current_;
}

template < class Key, class Value, class Hash, class Equal, bool isConst >
bool averenkov::IteratorHash< Key, Value, Hash, Equal, isConst >::operator!=(const IteratorHash& other) const noexcept
{
  return !(*this == other);
}

template < class Key, class Value, class Hash, class Equal, bool isConst >
void averenkov::IteratorHash< Key, Value, Hash, Equal, isConst >::skip_empty()
{
  while (current_ != end_ && (!current_->occupied || current_->deleted || !is_valid_data(current_)))
  {
    ++current_;
  }
}

template < class Key, class Value, class Hash, class Equal, bool isConst >
bool averenkov::IteratorHash< Key, Value, Hash, Equal, isConst >::is_valid_data(BucketPtr bucket)
{
  try
  {
    static Key dummy;
    key_equal_(bucket->key, dummy);
    return true;
  }
  catch (...)
  {
    return false;
  }
}

#endif
