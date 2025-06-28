#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <boost/hash2/xxhash.hpp>
#include "iterator.hpp"
#include "node.hpp"

namespace lanovenko
{
  template< class Key >
  struct XXHash
  {
    size_t operator()(const Key& key) const
    {
      boost::hash2::xxhash_64 hasher;
      hasher.update(std::addressof(key), sizeof(Key));
      return hasher.result();
    }
  };

  template< class Key, class T, class HS1 = std::hash< Key >, class HS2 = XXHash< Key >, class EQ = std::equal_to< Key > >
  class HashTable
  {
  public:
    using cIterator = HashConstIterator< Key, T, HS1, HS2, EQ >;
    using iterator = HashIterator< Key, T, HS1, HS2, EQ >;

    ~HashTable() noexcept;
    HashTable();
    HashTable(size_t capacity);
    HashTable(const HashTable& rhs);
    HashTable(HashTable&& rhs) noexcept;
    template< class InputIt >
    HashTable(InputIt firstIt, InputIt lastIt);
    HashTable(std::initializer_list< std::pair< Key, T > > init);
    HashTable& operator=(const HashTable& rhs);
    HashTable& operator=(HashTable&& rhs) noexcept;

    iterator begin() noexcept;
    cIterator cbegin() const noexcept;
    iterator end() noexcept;
    cIterator cend() const noexcept;
    T& at(const Key& key);
    const T& at(const Key& key) const;
    T& operator[](const Key& key);
    T& operator[](Key&& key);
    iterator find(const Key& key) noexcept;
    cIterator find(const Key& key) const noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    float loadFactor() const noexcept;
    void rehash(size_t newCapacity);
    float max_load_factor() const noexcept;
    void max_load_factor(float ml);
    std::pair< iterator, bool > insert(const Key& key, const T& value);
    template< class InputIt >
    void insert(InputIt first, InputIt last);
    iterator insert(cIterator, const std::pair< Key, T >& pair);
    iterator insert(cIterator, std::pair< Key, T >&& pair);
    iterator erase(iterator pos);
    iterator erase(cIterator pos);
    template< class InputIt >
    iterator erase(InputIt first, InputIt last);
    size_t erase(const Key& key);
    void clear() noexcept;
    void swap(HashTable& rhs) noexcept;
    template< class... Args >
    std::pair< iterator, bool > emplace(Args &&... args);
    template< class... Args >
    iterator emplace_hint(cIterator hint, Args &&... args);
  private:
    Node< Key, T >* slots_;
    size_t capacity_;
    size_t size_;
    float max_load_factor_ = 0.7f;
    std::pair< size_t, size_t > calculate_positions(const Key& key) const noexcept;
    std::pair< size_t, bool > find_position(const Key& key) const noexcept;
  };

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashTable< Key, T, HS1, HS2, EQ >::HashTable():
    HashTable(10)
  {}

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashTable< Key, T, HS1, HS2, EQ >::HashTable(size_t capacity):
    slots_(new Node< Key, T >[capacity]{}),
    capacity_(capacity),
    size_(0)
  {}

  template< class Key, class T, class HS1, class HS2, class EQ >
  template< class InputIt >
  HashTable< Key, T, HS1, HS2, EQ >::HashTable(InputIt firstIt, InputIt lastIt):
    HashTable()
  {
    insert(firstIt, lastIt);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashTable< Key, T, HS1, HS2, EQ >::HashTable(std::initializer_list< std::pair< Key, T > > init):
    HashTable(init.begin(), init.end())
  {}

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashTable< Key, T, HS1, HS2, EQ >::~HashTable() noexcept
  {
    delete[] slots_;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashTable< Key, T, HS1, HS2, EQ >::HashTable(const HashTable& rhs):
    HashTable(rhs.capacity_)
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (rhs.slots_[i].occupied && !rhs.slots_[i].deleted)
      {
        slots_[i] = rhs.slots_[i];
      }
    }
    size_ = rhs.size_;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashTable< Key, T, HS1, HS2, EQ >::HashTable(HashTable&& rhs) noexcept:
    slots_(rhs.slots_),
    capacity_(rhs.capacity_),
    size_(rhs.size_)
  {
    rhs.slots_ = nullptr;
    rhs.capacity_ = rhs.size_ = 0;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashTable< Key, T, HS1, HS2, EQ >& HashTable< Key, T, HS1, HS2, EQ >::operator=(const HashTable& rhs)
  {
    if (this != std::addressof(rhs))
    {
      HashTable tmp(rhs);
      swap(tmp);
    }
    return *this;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashTable< Key, T, HS1, HS2, EQ >& HashTable< Key, T, HS1, HS2, EQ >::operator=(HashTable&& rhs) noexcept
  {
    if (this != std::addressof(rhs))
    {
      HashTable tmp(std::move(rhs));
      swap(tmp);
    }
    return *this;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  bool HashTable< Key, T, HS1, HS2, EQ >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  size_t HashTable< Key, T, HS1, HS2, EQ >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  float HashTable< Key, T, HS1, HS2, EQ >::loadFactor() const noexcept
  {
    return static_cast< float >(size_) / capacity_;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  void HashTable< Key, T, HS1, HS2, EQ >::rehash(size_t newCapacity)
  {
    if (newCapacity <= capacity_)
    {
      return;
    }
    Node< Key, T >* tmp = nullptr;
    try
    {
      tmp = new Node< Key, T >[newCapacity];
      for (size_t i = 0; i < capacity_; ++i)
      {
        if (slots_[i].occupied && !slots_[i].deleted)
        {
          auto pos = find_position(slots_[i].data.first);
          tmp[pos.first] = slots_[i];
        }
      }
    }
    catch (const std::exception&)
    {
      delete[] tmp;
      throw;
    }
    delete[] slots_;
    slots_ = std::move(tmp);
    capacity_ = newCapacity;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  std::pair< HashIterator< Key, T, HS1, HS2, EQ >, bool > HashTable< Key, T, HS1, HS2, EQ >::insert(const Key& key, const T& value)
  {
    return emplace(key, value);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  std::pair< size_t, size_t > HashTable< Key, T, HS1, HS2, EQ >::calculate_positions(const Key& key) const noexcept
  {
    size_t h1 = HS1{}(key) % capacity_;
    size_t h2 = HS2{}(key) % (capacity_ - 1) + 1;
    return { h1, h2 };
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  std::pair< size_t, bool > HashTable< Key, T, HS1, HS2, EQ >::find_position(const Key& key) const noexcept
  {
    auto pos = calculate_positions(key);
    size_t deleted = capacity_;
    for (size_t i = 0; i < capacity_; ++i)
    {
      size_t index = (pos.first + i * pos.second) % capacity_;
      if (!slots_[index].occupied)
      {
        if (deleted != capacity_)
        {
          return { deleted, true };
        }
        else
        {
          return { index, true };
        }
      }
      else if (slots_[index].deleted && deleted == capacity_)
      {
        deleted = index;
      }
      else if (EQ{}(slots_[index].data.first, key))
      {
        return { index, false };
      }
    }
    return { capacity_, false };
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashIterator< Key, T, HS1, HS2, EQ > HashTable< Key, T, HS1, HS2, EQ >::begin() noexcept
  {
    return iterator(slots_, capacity_, 0);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashIterator< Key, T, HS1, HS2, EQ > HashTable< Key, T, HS1, HS2, EQ >::end() noexcept
  {
    return iterator(slots_, capacity_, capacity_);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashConstIterator< Key, T, HS1, HS2, EQ > HashTable< Key, T, HS1, HS2, EQ >::cbegin() const noexcept
  {
    return cIterator(slots_, capacity_, 0);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashConstIterator< Key, T, HS1, HS2, EQ > HashTable< Key, T, HS1, HS2, EQ >::cend() const noexcept
  {
    return cIterator(slots_, capacity_, capacity_);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashIterator< Key, T, HS1, HS2, EQ > HashTable< Key, T, HS1, HS2, EQ >::find(const Key& key) noexcept
  {
    auto pair = find_position(key);
    size_t pos = pair.first;
    bool hasFind = pair.second;
    if (hasFind || pos == capacity_)
    {
      return end();
    }
    return iterator(slots_, capacity_, pos);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashConstIterator< Key, T, HS1, HS2, EQ > HashTable<Key, T, HS1, HS2, EQ >::find(const Key& key) const noexcept
  {
    auto pair = find_position(key);
    size_t pos = pair.first;
    bool hasFind = pair.second;
    if (hasFind || pos == capacity_)
    {
      return cend();
    }
    return cIterator(slots_, capacity_, pos);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  T& HashTable< Key, T, HS1, HS2, EQ >::at(const Key& key)
  {
    auto it = find(key);
    if (it == end())
    {
      throw std::out_of_range("ERROR: key not found");
    }
    return it->second;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  const T& HashTable< Key, T, HS1, HS2, EQ >::at(const Key& key) const
  {
    auto it = find(key);
    if (it == cend())
    {
      throw std::out_of_range("ERROR: key not found");
    }
    return it->second;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  T& HashTable< Key, T, HS1, HS2, EQ >::operator[](const Key& key)
  {
    auto result = insert(key, T{});
    return result.first->second;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  T& HashTable< Key, T, HS1, HS2, EQ >::operator[](Key&& key)
  {
    auto result = insert(std::move(key), T{});
    return result.first->second;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  void HashTable< Key, T, HS1, HS2, EQ >::clear() noexcept
  {
    for (size_t i = 0; i < capacity_; i++)
    {
      slots_[i].occupied = false;
      slots_[i].deleted = false;
    }
    size_ = 0;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashIterator< Key, T, HS1, HS2, EQ > HashTable< Key, T, HS1, HS2, EQ >::erase(iterator pos)
  {
    if (pos == end())
    {
      return end();
    }
    size_t index = pos.current_;
    slots_[index].occupied = false;
    slots_[index].deleted = true;
    size_--;
    return iterator(slots_, capacity_, index);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashIterator< Key, T, HS1, HS2, EQ > HashTable< Key, T, HS1, HS2, EQ >::erase(cIterator pos)
  {
    return erase(iterator(slots_, capacity_, pos.current_));
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  template< class InputIt >
  HashIterator< Key, T, HS1, HS2, EQ > HashTable< Key, T, HS1, HS2, EQ >::erase(InputIt first, InputIt last)
  {
    iterator result;
    for (auto it = first; it != last;)
    {
      result = erase(it++);
    }
    return result;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  size_t HashTable< Key, T, HS1, HS2, EQ >::erase(const Key& key)
  {
    auto it = find(key);
    if (it != end())
    {
      erase(it);
      return 1;
    }
    return 0;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  void HashTable< Key, T, HS1, HS2, EQ >::swap(HashTable& rhs) noexcept
  {
    std::swap(slots_, rhs.slots_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(size_, rhs.size_);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  float HashTable< Key, T, HS1, HS2, EQ >::max_load_factor() const noexcept
  {
    return max_load_factor_;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  void HashTable< Key, T, HS1, HS2, EQ >::max_load_factor(float ml)
  {
    if (ml > 0.0f && ml <= 1.0f)
    {
      max_load_factor_ = ml;
      if (loadFactor() > max_load_factor_)
      {
        rehash(capacity_ * 2);
      }
    }
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  template< class InputIt >
  void HashTable< Key, T, HS1, HS2, EQ >::insert(InputIt first, InputIt last)
  {
    for (auto it = first; it != last; ++it)
    {
      insert(it->first, it->second);
    }
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  template< class... Args >
  std::pair< HashIterator< Key, T, HS1, HS2, EQ >, bool > HashTable< Key, T, HS1, HS2, EQ >::emplace(Args&&... args)
  {
    if (loadFactor() >= max_load_factor_)
    {
      rehash(capacity_ * 2);
    }
    std::pair< Key, T > temp(std::forward< Args >(args)...);
    const Key& key = temp.first;
    auto pair = find_position(key);
    size_t pos = pair.first;
    bool hasFind = pair.second;

    if (pos == capacity_)
    {
      throw std::runtime_error("ERROR: hash table is full");
    }
    if (hasFind)
    {
      slots_[pos].data = std::move(temp);
      slots_[pos].occupied = true;
      slots_[pos].deleted = false;
      size_++;
      return { iterator(slots_, capacity_, pos), true };
    }
    return { iterator(slots_, capacity_, pos), false };
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  template< class... Args >
  HashIterator< Key, T, HS1, HS2, EQ > HashTable< Key, T, HS1, HS2, EQ >::emplace_hint(cIterator hint, Args &&... args)
  {
    if (hint != cend())
    {
      const auto& slot = slots_[hint.current_];
      if (slot.occupied && !slot.deleted)
      {
        std::pair< Key, T > temp(std::forward< Args >(args)...);
        if (EQ{}(slot.data.first, temp.first))
        {
          return iterator(slots_, capacity_, hint.current_);
        }
        return emplace(std::move(temp)).first;
      }
    }
    return emplace(std::forward< Args >(args)...).first;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashIterator< Key, T, HS1, HS2, EQ > HashTable< Key, T, HS1, HS2, EQ >::insert(cIterator hint, const std::pair< Key, T >& pair)
  {
    return emplace_hint(hint, pair);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashIterator< Key, T, HS1, HS2, EQ > HashTable< Key, T, HS1, HS2, EQ >::insert(cIterator hint, std::pair< Key, T >&& pair)
  {
    return emplace_hint(hint, std::move(pair));
  }
}

#endif
