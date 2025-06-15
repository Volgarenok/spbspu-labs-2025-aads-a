#ifndef HASH_TABLE_ITERATOR_HPP
#define HASH_TABLE_ITERATOR_HPP

#include <type_traits>
#include <iterator>

namespace detail {
  enum class HashTableIteratorType
  {
    CONSTANT,
    NONCONSTANT,
  };
}

namespace maslevtsov {
  template< class Key, class T, class Hash, class KeyEqual >
  class HashTable;

  template< class Key, class T, class Hash, class KeyEqual, detail::HashTableIteratorType it_type >
  class HashTableIterator final: public std::iterator< std::forward_iterator_tag, T >
  {
  public:
    using value_type = std::pair< Key, T >;
    using reference_type = typename std::conditional< it_type == detail::HashTableIteratorType::CONSTANT,
      const value_type&, value_type& >::type;
    using pointer_type = typename std::conditional< it_type == detail::HashTableIteratorType::CONSTANT,
      const value_type*, value_type* >::type;

    HashTableIterator() noexcept;

    HashTableIterator& operator++() noexcept;
    HashTableIterator operator++(int) noexcept;

    reference_type operator*() const noexcept;
    pointer_type operator->() const noexcept;

    bool operator==(const HashTableIterator& rhs) const noexcept;
    bool operator!=(const HashTableIterator& rhs) const noexcept;

  private:
    friend class HashTable< Key, T, Hash, KeyEqual >;

    using hash_table_type = typename std::conditional< it_type == detail::HashTableIteratorType::CONSTANT,
      const HashTable< Key, T, Hash, KeyEqual >, HashTable< Key, T, Hash, KeyEqual > >::type;

    hash_table_type* hash_table_;
    size_t index_;

    HashTableIterator(hash_table_type* hash_table, size_t index_) noexcept;
  };

  template< class Key, class T, class Hash, class KeyEqual, detail::HashTableIteratorType it_type >
  HashTableIterator< Key, T, Hash, KeyEqual, it_type >::HashTableIterator() noexcept:
    hash_table_(nullptr),
    index_(0)
  {}

  template< class Key, class T, class Hash, class KeyEqual, detail::HashTableIteratorType it_type >
  typename HashTableIterator< Key, T, Hash, KeyEqual, it_type >::HashTableIterator&
    HashTableIterator< Key, T, Hash, KeyEqual, it_type >::operator++() noexcept
  {
    using hash_table_t = HashTable< Key, T, Hash, KeyEqual >;

    ++index_;
    size_t slots_size = hash_table_->slots_.size();
    while (index_ < slots_size && hash_table_->slots_[index_].state != hash_table_t::SlotState::OCCUPIED) {
      ++index_;
    }
    return *this;
  }

  template< class Key, class T, class Hash, class KeyEqual, detail::HashTableIteratorType it_type >
  typename HashTableIterator< Key, T, Hash, KeyEqual, it_type >::HashTableIterator
    HashTableIterator< Key, T, Hash, KeyEqual, it_type >::operator++(int) noexcept
  {
    HashTableIterator< Key, T, Hash, KeyEqual, it_type > result(*this);
    ++(*this);
    return result;
  }

  template< class Key, class T, class Hash, class KeyEqual, detail::HashTableIteratorType it_type >
  typename HashTableIterator< Key, T, Hash, KeyEqual, it_type >::reference_type
    HashTableIterator< Key, T, Hash, KeyEqual, it_type >::operator*() const noexcept
  {
    return hash_table_->slots_[index_].data;
  }

  template< class Key, class T, class Hash, class KeyEqual, detail::HashTableIteratorType it_type >
  typename HashTableIterator< Key, T, Hash, KeyEqual, it_type >::pointer_type
    HashTableIterator< Key, T, Hash, KeyEqual, it_type >::operator->() const noexcept
  {
    return std::addressof(hash_table_->slots_[index_].data);
  }

  template< class Key, class T, class Hash, class KeyEqual, detail::HashTableIteratorType it_type >
  bool HashTableIterator< Key, T, Hash, KeyEqual, it_type >::operator==(const HashTableIterator& rhs) const noexcept
  {
    return hash_table_ == rhs.hash_table_ && index_ == rhs.index_;
  }

  template< class Key, class T, class Hash, class KeyEqual, detail::HashTableIteratorType it_type >
  bool HashTableIterator< Key, T, Hash, KeyEqual, it_type >::operator!=(const HashTableIterator& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< class Key, class T, class Hash, class KeyEqual, detail::HashTableIteratorType it_type >
  HashTableIterator< Key, T, Hash, KeyEqual, it_type >::HashTableIterator(hash_table_type* hash_table,
    size_t index) noexcept:
    hash_table_(hash_table),
    index_(index)
  {}
}

#endif
