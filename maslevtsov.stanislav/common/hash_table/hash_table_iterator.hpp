#ifndef HASH_TABLE_ITERATOR_HPP
#define HASH_TABLE_ITERATOR_HPP

#include <type_traits>
#include <iterator>
#include "slot.hpp"

namespace maslevtsov {
  namespace detail {
    enum class HashTableIteratorType
    {
      CONSTANT,
      NONCONSTANT,
    };
  }

  template< class Key, class T, class Hash, class ProbeHash, class KeyEqual >
  class HashTable;

  template< class Key, class T, class Hash, class ProbeHash, class KeyEqual, detail::HashTableIteratorType it_type >
  class HashTableIterator final: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    using value_type = std::pair< Key, T >;
    using reference_type = typename std::conditional< it_type == detail::HashTableIteratorType::CONSTANT,
      const value_type&, value_type& >::type;
    using pointer_type = typename std::conditional< it_type == detail::HashTableIteratorType::CONSTANT,
      const value_type*, value_type* >::type;

    HashTableIterator() noexcept:
      hash_table_(nullptr),
      index_(0)
    {}

    HashTableIterator& operator++() noexcept
    {
      ++index_;
      size_t slots_size = hash_table_->slots_.size();
      while (index_ < slots_size && hash_table_->slots_[index_].state != detail::SlotState::OCCUPIED) {
        ++index_;
      }
      return *this;
    }

    HashTableIterator operator++(int) noexcept
    {
      HashTableIterator< Key, T, Hash, ProbeHash, KeyEqual, it_type > result(*this);
      ++(*this);
      return result;
    }

    reference_type operator*() const noexcept
    {
      return hash_table_->slots_[index_].data;
    }

    pointer_type operator->() const noexcept
    {
      return std::addressof(hash_table_->slots_[index_].data);
    }

    bool operator==(const HashTableIterator& rhs) const noexcept
    {
      return hash_table_ == rhs.hash_table_ && index_ == rhs.index_;
    }

    bool operator!=(const HashTableIterator& rhs) const noexcept
    {
      return !(*this == rhs);
    }

  private:
    friend class HashTable< Key, T, Hash, ProbeHash, KeyEqual >;

    using hash_table_type = typename std::conditional< it_type == detail::HashTableIteratorType::CONSTANT,
      const HashTable< Key, T, Hash, ProbeHash, KeyEqual >, HashTable< Key, T, Hash, ProbeHash, KeyEqual > >::type;

    hash_table_type* hash_table_;
    size_t index_;

    HashTableIterator(hash_table_type* hash_table, size_t index) noexcept:
      hash_table_(hash_table),
      index_(index)
    {}
  };
}

#endif
