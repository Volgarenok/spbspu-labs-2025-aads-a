#ifndef HASH_TABLE_DECLARATION_HPP
#define HASH_TABLE_DECLARATION_HPP

#include <functional>
#include <initializer_list>
#include <vector/declaration.hpp>
#include "hash_table_iterator.hpp"
#include "slot.hpp"

namespace maslevtsov {
  template< class Key, class T, class Hash = std::hash< Key >, class ProbeHash = std::hash< Key >,
    class KeyEqual = std::equal_to< Key > >
  class HashTable
  {
  public:
    using value_type = std::pair< Key, T >;
    using size_type = std::size_t;
    using iterator = HashTableIterator< Key, T, Hash, ProbeHash, KeyEqual, detail::HashTableIteratorType::NONCONSTANT >;
    using const_iterator =
      HashTableIterator< Key, T, Hash, ProbeHash, KeyEqual, detail::HashTableIteratorType::CONSTANT >;

    HashTable() noexcept;
    template< class InputIt >
    HashTable(InputIt first, InputIt last);
    HashTable(std::initializer_list< value_type > ilist);

    T& operator[](const Key& key);
    const T& operator[](const Key& key) const;
    T& at(const Key& key);
    const T& at(const Key& key) const;
    iterator find(const Key& key) noexcept;
    const_iterator find(const Key& key) const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;
    size_type size() const noexcept;

    void clear() noexcept;
    std::pair< iterator, bool > insert(const value_type& value);
    iterator insert(const_iterator hint, const value_type& value);
    template< class InputIt >
    void insert(InputIt first, InputIt last);
    template< class... Args >
    std::pair< iterator, bool > emplace(Args&&... args);
    template< class... Args >
    iterator emplace_hint(const_iterator hint, Args&&... args);
    iterator erase(iterator pos) noexcept;
    iterator erase(const_iterator pos) noexcept;
    iterator erase(const_iterator first, const_iterator last) noexcept;
    size_type erase(const Key& key) noexcept;
    void swap(HashTable& other);

    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float ml);
    void rehash(size_type count);

  private:
    friend class HashTableIterator< Key, T, Hash, ProbeHash, KeyEqual, detail::HashTableIteratorType::CONSTANT >;
    friend class HashTableIterator< Key, T, Hash, ProbeHash, KeyEqual, detail::HashTableIteratorType::NONCONSTANT >;

    Vector< detail::Slot< value_type > > slots_;
    size_type size_;
    Hash hasher_;
    ProbeHash probe_hasher_;
    KeyEqual key_equal_;
    float max_load_factor_ = 1.0;

    size_type find_index(const Key& key) const noexcept;
  };
}

#endif
