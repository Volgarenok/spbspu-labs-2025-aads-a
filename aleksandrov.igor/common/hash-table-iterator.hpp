#ifndef HASH_TABLE_ITERATOR_HPP
#define HASH_TABLE_ITERATOR_HPP

#include <iterator>
#include <cassert>
#include "slot.hpp"

namespace aleksandrov
{
  template< class K, class V, class H, class E >
  class HashTable;

  template< class K, class V, class H, class E, bool isConst >
  class HashTableIterator: public std::iterator< std::forward_iterator_tag, std::pair< K, V > >
  {
  public:
    using ValueType = std::pair< K, V >;
    using Reference = std::conditional_t< isConst, const ValueType&, ValueType& >;
    using Pointer = std::conditional_t< isConst, const ValueType*, ValueType* >;

    HashTableIterator();

    HashTableIterator& operator++();
    HashTableIterator operator++(int);

    Reference operator*() const noexcept;
    Pointer operator->() const noexcept;

    bool operator==(const HashTableIterator&) const noexcept;
    bool operator!=(const HashTableIterator&) const noexcept;

  private:
    friend class HashTable< K, V, H, E >;
    using Table = HashTable< K, V, H, E >;
    using Slot = detail::Slot< K, V >;

    const Table* table_;
    size_t index_;

    HashTableIterator(const Table*);
    HashTableIterator(const Table*, size_t);
  };

  template< class K, class V, class H, class E, bool isConst >
  HashTableIterator< K, V, H, E, isConst >::HashTableIterator():
    table_(nullptr),
    index_(0)
  {}

  template< class K, class V, class H, class E, bool isConst >
  HashTableIterator< K, V, H, E, isConst >::HashTableIterator(const Table* table):
    table_(table),
    index_(0)
  {
    while (index_ < table_->capacity_ && !table_->data_[index_].occupied)
    {
      ++index_;
    }
  }

  template< class K, class V, class H, class E, bool isConst >
  HashTableIterator< K, V, H, E, isConst >::HashTableIterator(const Table* table, size_t index):
    table_(table),
    index_(index)
  {}

  template< class K, class V, class H, class E, bool isConst >
  auto HashTableIterator< K, V, H, E, isConst >::operator++() -> HashTableIterator&
  {
    assert(table_ != nullptr && "ERROR: Cannot iterate from null-table!");
    assert(index_ < table_->capacity_ && "ERROR: Cannot iterate from end() iterator!");
    index_ = table_->getNextIndex(index_);
    while (index_ < table_->capacity_ && !table_->data_[index_].occupied)
    {
      ++index_;
    }
    return *this;
  }

  template< class K, class V, class H, class E, bool isConst >
  auto HashTableIterator< K, V, H, E, isConst >::operator++(int) -> HashTableIterator
  {
    assert(table_ != nullptr && "ERROR: Cannot iterate from null-table!");
    assert(index_ < table_->capacity_ && "ERROR: Cannot iterate from end() iterator!");
    HashTableIterator temp(*this);
    ++(*this);
    return temp;
  }

  template< class K, class V, class H, class E, bool isConst >
  auto HashTableIterator< K, V, H, E, isConst >::operator*() const noexcept -> Reference
  {
    assert(table_ != nullptr && "ERROR: Access to null-table!");
    assert(index_ < table_->capacity_ && "ERROR: Access to end() iterator!");
    assert(table_->data_[index_].occupied && "ERROR: Access to empty slot!");
    return table_->data_[index_].data;
  }

  template< class K, class V, class H, class E, bool isConst >
  auto HashTableIterator< K, V, H, E, isConst >::operator->() const noexcept -> Pointer
  {
    assert(table_ != nullptr && "ERROR: Access to null-table!");
    assert(index_ < table_->capacity_ && "ERROR: Access to end() iterator!");
    assert(table_->data_[index_].occupied && "ERROR: Access to empty slot!");
    return std::addressof(table_->data_[index_].data);
  }

  template< class K, class V, class H, class E, bool isConst >
  bool HashTableIterator< K, V, H, E, isConst >::operator==(const HashTableIterator& rhs) const noexcept
  {
    return table_ == rhs.table_ && index_ == rhs.index_;
  }

  template< class K, class V, class H, class E, bool isConst >
  bool HashTableIterator< K, V, H, E, isConst >::operator!=(const HashTableIterator& rhs) const noexcept
  {
    return !(*this == rhs);
  }
}

#endif

