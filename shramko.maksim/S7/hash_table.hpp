#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "iterator.hpp"
#include "node.hpp"

namespace shramko
{
  template< class Key, class T, class Hash = std::hash< Key >, class Eq = std::equal_to< Key > >
  class HashTable
  {
  public:
    using cIterator = HashConstIterator< Key, T, Hash, Eq >;
    using iterator = HashIterator< Key, T, Hash, Eq >;

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
    iterator erase(iterator pos);
    iterator erase(cIterator pos);
    template< class InputIt >
    iterator erase(InputIt first, InputIt last);
    size_t erase(const Key& key);
    void clear() noexcept;
    void swap(HashTable& rhs) noexcept;

  private:
    Node< Key, T >* slots_;
    size_t capacity_;
    size_t size_;
    float max_load_factor_ = 0.7f;
    size_t compute_hash(const Key& key) const noexcept;
    size_t find_position(const Key& key) const noexcept;
    size_t get_insert_position(const Key& key) const noexcept;
  };

  template< class Key, class T, class Hash, class Eq >
  HashTable< Key, T, Hash, Eq >::HashTable()
    : HashTable(10)
  {}

  template< class Key, class T, class Hash, class Eq >
  HashTable< Key, T, Hash, Eq >::HashTable(size_t capacity)
    : slots_(new Node< Key, T >[capacity]{}),
      capacity_(capacity),
      size_(0)
  {}

  template< class Key, class T, class Hash, class Eq >
  template< class InputIt >
  HashTable< Key, T, Hash, Eq >::HashTable(InputIt firstIt, InputIt lastIt)
    : HashTable()
  {
    insert(firstIt, lastIt);
  }

  template< class Key, class T, class Hash, class Eq >
  HashTable< Key, T, Hash, Eq >::HashTable(std::initializer_list< std::pair< Key, T > > init)
    : HashTable(init.begin(), init.end())
  {}

  template< class Key, class T, class Hash, class Eq >
  HashTable< Key, T, Hash, Eq >::~HashTable() noexcept
  {
    delete[] slots_;
  }

  template< class Key, class T, class Hash, class Eq >
  HashTable< Key, T, Hash, Eq >::HashTable(const HashTable& rhs)
    : HashTable(rhs.capacity_)
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

  template< class Key, class T, class Hash, class Eq >
  HashTable< Key, T, Hash, Eq >::HashTable(HashTable&& rhs) noexcept
    : slots_(rhs.slots_),
      capacity_(rhs.capacity_),
      size_(rhs.size_)
  {
    rhs.slots_ = nullptr;
    rhs.capacity_ = 0;
    rhs.size_ = 0;
  }

  template< class Key, class T, class Hash, class Eq >
  HashTable< Key, T, Hash, Eq >& HashTable< Key, T, Hash, Eq >::operator=(const HashTable& rhs)
  {
    if (this != &rhs)
    {
      HashTable temp(rhs);

      swap(temp);
    }

    return *this;
  }

  template< class Key, class T, class Hash, class Eq >
  HashTable< Key, T, Hash, Eq >& HashTable< Key, T, Hash, Eq >::operator=(HashTable&& rhs) noexcept
  {
    if (this != &rhs)
    {
      delete[] slots_;

      slots_ = rhs.slots_;
      capacity_ = rhs.capacity_;
      size_ = rhs.size_;

      rhs.slots_ = nullptr;
      rhs.capacity_ = 0;
      rhs.size_ = 0;
    }

    return *this;
  }

  template< class Key, class T, class Hash, class Eq >
  HashIterator< Key, T, Hash, Eq > HashTable< Key, T, Hash, Eq >::begin() noexcept
  {
    return iterator(slots_, capacity_, 0);
  }

  template< class Key, class T, class Hash, class Eq >
  HashConstIterator< Key, T, Hash, Eq > HashTable< Key, T, Hash, Eq >::cbegin() const noexcept
  {
    return cIterator(slots_, capacity_, 0);
  }

  template< class Key, class T, class Hash, class Eq >
  HashIterator< Key, T, Hash, Eq > HashTable< Key, T, Hash, Eq >::end() noexcept
  {
    return iterator(slots_, capacity_, capacity_);
  }

  template< class Key, class T, class Hash, class Eq >
  HashConstIterator< Key, T, Hash, Eq > HashTable< Key, T, Hash, Eq >::cend() const noexcept
  {
    return cIterator(slots_, capacity_, capacity_);
  }

  template< class Key, class T, class Hash, class Eq >
  T& HashTable< Key, T, Hash, Eq >::at(const Key& key)
  {
    size_t pos = find_position(key);

    if (pos == capacity_)
    {
      throw std::out_of_range("Key not found");
    }

    return slots_[pos].data.second;
  }

  template< class Key, class T, class Hash, class Eq >
  const T& HashTable< Key, T, Hash, Eq >::at(const Key& key) const
  {
    size_t pos = find_position(key);

    if (pos == capacity_)
    {
      throw std::out_of_range("Key not found");
    }

    return slots_[pos].data.second;
  }

  template< class Key, class T, class Hash, class Eq >
  T& HashTable< Key, T, Hash, Eq >::operator[](const Key& key)
  {
    auto p = insert(key, T{});
    return p.first->second;
  }

  template< class Key, class T, class Hash, class Eq >
  T& HashTable< Key, T, Hash, Eq >::operator[](Key&& key)
  {
    auto p = insert(std::move(key), T{});
    return p.first->second;
  }

  template< class Key, class T, class Hash, class Eq >
  HashIterator< Key, T, Hash, Eq > HashTable< Key, T, Hash, Eq >::find(const Key& key) noexcept
  {
    size_t pos = find_position(key);

    if (pos != capacity_)
    {
      return iterator(slots_, capacity_, pos);
    }

    return end();
  }

  template< class Key, class T, class Hash, class Eq >
  HashConstIterator< Key, T, Hash, Eq > HashTable< Key, T, Hash, Eq >::find(const Key& key) const noexcept
  {
    size_t pos = find_position(key);

    if (pos != capacity_)
    {
      return cIterator(slots_, capacity_, pos);
    }

    return cend();
  }

  template< class Key, class T, class Hash, class Eq >
  bool HashTable< Key, T, Hash, Eq >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class T, class Hash, class Eq >
  size_t HashTable< Key, T, Hash, Eq >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class T, class Hash, class Eq >
  float HashTable< Key, T, Hash, Eq >::loadFactor() const noexcept
  {
    return static_cast< float >(size_) / static_cast< float >(capacity_);
  }

  template< class Key, class T, class Hash, class Eq >
  void HashTable< Key, T, Hash, Eq >::rehash(size_t newCapacity)
  {
    if (newCapacity < size_)
    {
      return;
    }

    HashTable temp(newCapacity);

    for (auto it = begin(); it != end(); ++it)
    {
      temp.insert(std::move(it->first), std::move(it->second));
    }

    swap(temp);
  }

  template< class Key, class T, class Hash, class Eq >
  void HashTable< Key, T, Hash, Eq >::clear() noexcept
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      slots_[i].occupied = false;
      slots_[i].deleted = false;
    }

    size_ = 0;
  }

  template< class Key, class T, class Hash, class Eq >
  size_t HashTable< Key, T, Hash, Eq >::compute_hash(const Key& key) const noexcept
  {
    return Hash{}(key) % capacity_;
  }

  template< class Key, class T, class Hash, class Eq >
  size_t HashTable< Key, T, Hash, Eq >::find_position(const Key& key) const noexcept
  {
    size_t h = compute_hash(key);

    for (size_t i = 0; i < capacity_; ++i)
    {
      size_t pos = (h + i * i) % capacity_;

      if (slots_[pos].occupied && !slots_[pos].deleted && Eq{}(slots_[pos].data.first, key))
      {
        return pos;
      }

      if (!slots_[pos].occupied && !slots_[pos].deleted)
      {
        return capacity_;
      }
    }

    return capacity_;
  }

  template< class Key, class T, class Hash, class Eq >
  size_t HashTable< Key, T, Hash, Eq >::get_insert_position(const Key& key) const noexcept
  {
    size_t h = compute_hash(key);

    for (size_t i = 0; i < capacity_; ++i)
    {
      size_t pos = (h + i * i) % capacity_;

      if (!slots_[pos].occupied)
      {
        return pos;
      }
    }

    return capacity_;
  }

  template< class Key, class T, class Hash, class Eq >
  std::pair< HashIterator< Key, T, Hash, Eq >, bool > HashTable< Key, T, Hash, Eq >::insert(const Key& key, const T& value)
  {
    if (loadFactor() >= max_load_factor_)
    {
      rehash(capacity_ * 2);
    }

    size_t found = find_position(key);

    if (found != capacity_)
    {
      return {iterator(slots_, capacity_, found), false};
    }

    size_t pos = get_insert_position(key);

    if (pos == capacity_)
    {
      throw std::runtime_error("ERROR: hash table is full");
    }

    slots_[pos].data = {key, value};
    slots_[pos].occupied = true;
    slots_[pos].deleted = false;
    size_++;

    return {iterator(slots_, capacity_, pos), true};
  }

  template< class Key, class T, class Hash, class Eq >
  template< class InputIt >
  void HashTable< Key, T, Hash, Eq >::insert(InputIt first, InputIt last)
  {
    for (auto it = first; it != last; ++it)
    {
      insert(it->first, it->second);
    }
  }

  template< class Key, class T, class Hash, class Eq >
  HashIterator< Key, T, Hash, Eq > HashTable< Key, T, Hash, Eq >::erase(iterator pos)
  {
    size_t index = pos.current_;

    slots_[index].occupied = false;
    slots_[index].deleted = true;
    size_--;

    return iterator(slots_, capacity_, index);
  }

  template< class Key, class T, class Hash, class Eq >
  HashIterator< Key, T, Hash, Eq > HashTable< Key, T, Hash, Eq >::erase(cIterator pos)
  {
    return erase(iterator(slots_, capacity_, pos.current_));
  }

  template< class Key, class T, class Hash, class Eq >
  template< class InputIt >
  HashIterator< Key, T, Hash, Eq > HashTable< Key, T, Hash, Eq >::erase(InputIt first, InputIt last)
  {
    iterator result;

    for (auto it = first; it != last;)
    {
      result = erase(it++);
    }

    return result;
  }

  template< class Key, class T, class Hash, class Eq >
  size_t HashTable< Key, T, Hash, Eq >::erase(const Key& key)
  {
    size_t pos = find_position(key);

    if (pos != capacity_)
    {
      slots_[pos].occupied = false;
      slots_[pos].deleted = true;
      size_--;
      return 1;
    }

    return 0;
  }

  template< class Key, class T, class Hash, class Eq >
  void HashTable< Key, T, Hash, Eq >::swap(HashTable& rhs) noexcept
  {
    std::swap(slots_, rhs.slots_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(size_, rhs.size_);
  }

  template< class Key, class T, class Hash, class Eq >
  float HashTable< Key, T, Hash, Eq >::max_load_factor() const noexcept
  {
    return max_load_factor_;
  }

  template< class Key, class T, class Hash, class Eq >
  void HashTable< Key, T, Hash, Eq >::max_load_factor(float ml)
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
}

#endif
