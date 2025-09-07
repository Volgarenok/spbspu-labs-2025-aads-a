#ifndef HASH_TABLE_ITERATORS_H
#define HASH_TABLE_ITERATORS_H

#include <cstddef>
#include <memory>

namespace zholobov {

  template < typename Key, typename T >
  class HashTableIterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    HashTableIterator();
    HashTableIterator& operator++() noexcept;
    HashTableIterator operator++(int) noexcept;
    HashTableIterator& operator--() noexcept;
    HashTableIterator operator--(int) noexcept;
    reference operator*() const noexcept;
    pointer operator->() const noexcept;
    bool operator==(const HashTableIterator& other) const noexcept;
    bool operator!=(const HashTableIterator& other) const noexcept;
  };

  template < typename Key, typename T >
  class HashTableConstIterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    HashTableConstIterator();
    HashTableConstIterator& operator++() noexcept;
    HashTableConstIterator operator++(int) noexcept;
    HashTableConstIterator& operator--() noexcept;
    HashTableConstIterator operator--(int) noexcept;
    reference operator*() const noexcept;
    pointer operator->() const noexcept;
    bool operator==(const HashTableConstIterator& other) const noexcept;
    bool operator!=(const HashTableConstIterator& other) const noexcept;
  };

}

#endif
