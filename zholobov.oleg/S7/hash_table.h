#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "hash_table_iterators.h"

namespace zholobov {

  template < class Key, class T, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable {
  public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair< const Key, T >;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = HashTableIterator< Key, T >;
    using const_iterator = HashTableConstIterator< Key, T >;
    using size_type = size_t;

    HashTable();

    template < typename InputIt >
    HashTable(InputIt, InputIt);

    explicit HashTable(std::initializer_list< std::pair< Key, T > >);

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    mapped_type& at(const Key& key);
    const mapped_type& at(const Key& key) const;
    mapped_type& operator[](const Key& key);

    bool empty() const noexcept;
    size_type size() const noexcept;
    void clear() noexcept;

    std::pair< iterator, bool > insert(const std::pair< Key, T >& value);
    template < typename InputIt >
    void insert(InputIt first, InputIt last);

    iterator erase(iterator pos) noexcept;
    size_type erase(const Key& key) noexcept;
    iterator erase(iterator first, iterator last) noexcept;

    template < typename K, typename V >
    std::pair< iterator, bool > emplace(K&& key, V&& value);

    void swap(HashTable& other) noexcept;

    iterator find(const Key& key);
    const_iterator find(const Key& key) const;
    size_type count(const Key& key) const;

    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float mlf);
    void rehashFactor(size_t count);
  };

}

#endif
