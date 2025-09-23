#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <cmath>
#include <cstddef>
#include <algorithm>
#include <functional>
#include "decls.hpp"
#include "hash_node.hpp"
#include "hash_iterator.hpp"
#include "hash_cIterator.hpp"

namespace abramov
{
  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  struct HashTable
  {
    using Iter = HashIterator< Key, Value, Hash, Equal >;
    using cIter = ConstHashIterator< Key, Value, Hash, Equal >;
    using Hash_t = HashTable< Key, Value, Hash, Equal >;
    using cHash = const HashTable< Key, Value, Hash, Equal >;

    HashTable();
    HashTable(const HashTable< Key, Value, Hash, Equal > &other);
    ~HashTable();
    HashTable< Key, Value, Hash, Equal > &operator=(const HashTable< Key, Value, Hash, Equal > &other);
    void insert(const Key &k, const Value &v);
    double loadFactor() const noexcept;
    void rehash(size_t k);
    size_t erase(const Key &k);
    Value &at(const Key &k);
    Value &operator[](const Key &k);
    HashIterator< Key, Value, Hash, Equal > begin();
    HashIterator< Key, Value, Hash, Equal > end();
    HashIterator< Key, Value, Hash, Equal > find(const Key &k);
    ConstHashIterator< Key, Value, Hash, Equal > cbegin() const;
    ConstHashIterator< Key, Value, Hash, Equal > cend() const;
    ConstHashIterator< Key, Value, Hash, Equal > cfind(const Key &k) const;
    size_t size() const noexcept;
    bool empty() const noexcept;

  private:
    HashNode< Key, Value > **table_;
    size_t capacity_;
    size_t size_;
    Hash hash_;
    Equal equal_;
    friend struct HashIterator< Key, Value, Hash, Equal >;
    friend struct ConstHashIterator< Key, Value, Hash, Equal >;

    void initTable();
    void resizeIfNeed();
    void swap(HashTable< Key, Value, Hash, Equal > &other) noexcept;
    size_t findInsertPosition(const Key &k) const;
    bool isPrime(size_t k) const noexcept;
    size_t getLargerPrimeCapacity(size_t k) const noexcept;
  };
}

template< class Key, class Value, class Hash, class Equal >
abramov::HashTable< Key, Value, Hash, Equal >::HashTable():
  table_(nullptr),
  capacity_(17),
  size_(0),
  hash_(std::hash< Key >()),
  equal_(std::equal_to< Key >())
{
  initTable();
}

template< class Key, class Value, class Hash, class Equal >
void abramov::HashTable< Key, Value, Hash, Equal >::initTable()
{
  table_ = new HashNode< Key, Value >*[capacity_];
  for (size_t i = 0; i < capacity_; ++i)
  {
    table_[i] = nullptr;
  }
}

template< class Key, class Value, class Hash, class Equal >
abramov::HashTable< Key, Value, Hash, Equal >::HashTable(cHash &other):
  table_(nullptr),
  capacity_(other.capacity_),
  size_(0),
  hash_(other.hash_),
  equal_(other.equal_)
{
  initTable();
  for (size_t i = 0; i < other.capacity_; ++i)
  {
    HashNode< Key, Value > *curr = other.table_[i];
    while (curr)
    {
      insert(curr->data_.first, curr->data_.second);
      curr = curr->next_;
    }
  }
}

template< class Key, class Value, class Hash, class Equal >
void abramov::HashTable< Key, Value, Hash, Equal >::swap(Hash_t &other) noexcept
{
  std::swap(table_, other.table_);
  std::swap(capacity_, other.capacity_);
  std::swap(size_, other.size_);
  std::swap(hash_, other.hash_);
  std::swap(equal_, other.equal_);
}

template< class Key, class Value, class Hash, class Equal >
typename abramov::HashTable< Key, Value, Hash, Equal >::Hash_t&
abramov::HashTable< Key, Value, Hash, Equal >::operator=(cHash &other)
{
  if (this != std::addressof(other))
  {
    HashTable< Key, Value, Hash, Equal > temp(other);
    swap(temp);
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
abramov::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  for (size_t i = 0; i < capacity_; ++i)
  {
    HashNode< Key, Value > *curr = table_[i];
    while (curr)
    {
      HashNode< Key, Value > *next = curr->next_;
      delete curr;
      curr = next;
    }
  }
  delete[] table_;
}

template< class Key, class Value, class Hash, class Equal >
void abramov::HashTable< Key, Value, Hash, Equal >::insert(const Key &k, const Value &v)
{
  resizeIfNeed();
  size_t pos = findInsertPosition(k);
  if (!table_[pos])
  {
    table_[pos] = new HashNode< Key, Value >(k, v);
  }
  else
  {
    HashNode< Key, Value > *new_node = new HashNode< Key, Value >(k, v);
    new_node->next_ = table_[pos];
    table_[pos] = new_node;
  }
  ++size_;
}

template< class Key, class Value, class Hash, class Equal >
void abramov::HashTable< Key, Value, Hash, Equal >::resizeIfNeed()
{
  constexpr double max = 0.8;
  if (loadFactor() > max)
  {
    rehash(capacity_ * 2);
  }
}

template< class Key, class Value, class Hash, class Equal >
double abramov::HashTable< Key, Value, Hash, Equal >::loadFactor() const noexcept
{
  return static_cast< double >(size_) / capacity_;
}

template< class Key, class Value, class Hash, class Equal >
void abramov::HashTable< Key, Value, Hash, Equal >::rehash(size_t k)
{
  HashNode< Key, Value > **old_table = table_;
  size_t old_capacity = capacity_;
  capacity_ = k;
  size_ = 0;
  initTable();
  for (size_t i = 0; i < old_capacity; ++i)
  {
    HashNode< Key, Value > *curr = old_table[i];
    while (curr)
    {
      insert(curr->data_.first, curr->data_.second);
      HashNode< Key, Value > *next = curr->next_;
      delete curr;
      curr = next;
    }
  }
  delete[] old_table;
}

template< class Key, class Value, class Hash, class Equal >
size_t abramov::HashTable< Key, Value, Hash, Equal >::findInsertPosition(const Key &k) const
{
  size_t pos = hash_(k) % capacity_;
  size_t att = 0;
  while (table_[pos] && !equal_(table_[pos]->data_.first, k))
  {
    pos = (pos + att * att) % capacity_;
    ++att;
  }
  return pos;
}

template< class Key, class Value, class Hash, class Equal >
size_t abramov::HashTable< Key, Value, Hash, Equal >::erase(const Key &k)
{
  size_t removed = 0;
  size_t orig_pos = hash_(k) % capacity_;
  size_t pos = orig_pos;
  size_t att = 0;
  do
  {
    HashNode< Key, Value > *curr = table_[pos];
    HashNode< Key, Value > *prev = nullptr;
    while (curr)
    {
      if (equal_(curr->data_.first, k))
      {
        HashNode< Key, Value > *del = curr;
        if (prev)
        {
          prev->next_ = curr->next_;
        }
        else
        {
          table_[pos] = curr->next_;
        }
        curr = curr->next_;
        delete del;
        ++removed;
        --size_;
      }
      else
      {
        prev = curr;
        curr = curr->next_;
      }
    }
    ++att;
    pos = (orig_pos + att * att) % capacity_;
  } while (pos != orig_pos);
  return removed;
}

template< class Key, class Value, class Hash, class Equal >
Value &abramov::HashTable< Key, Value, Hash, Equal >::operator[](const Key &k)
{
  auto it = find(k);
  if (it != end())
  {
    return it->second;
  }
  insert(k, Value());
  return find(k)->second;
}

template< class Key, class Value, class Hash, class Equal >
typename abramov::HashTable< Key, Value, Hash, Equal >::Iter
abramov::HashTable< Key, Value, Hash, Equal >::end()
{
  return HashIterator< Key, Value, Hash, Equal >(this, capacity_, nullptr);
}

template< class Key, class Value, class Hash, class Equal >
typename abramov::HashTable< Key, Value, Hash, Equal >::Iter
abramov::HashTable< Key, Value, Hash, Equal >::begin()
{
  for (size_t i = 0; i < capacity_; ++i)
  {
    if (table_[i])
    {
      return HashIterator< Key, Value, Hash, Equal >(this, i, table_[i]);
    }
  }
  return end();
}

template< class Key, class Value, class Hash, class Equal >
typename abramov::HashTable< Key, Value, Hash, Equal >::Iter
abramov::HashTable< Key, Value, Hash, Equal >::find(const Key & k)
{
  if (empty())
  {
    return end();
  }
  size_t pos = hash_(k) % capacity_;
  size_t att = 0;
  size_t orig_pos = pos;
  do
  {
    HashNode< Key, Value > *curr = table_[pos];
    while (curr)
    {
      if (equal_(curr->data_.first, k))
      {
        return HashIterator< Key, Value, Hash, Equal >(this, pos, curr);
      }
      curr = curr->next_;
    }
    ++att;
    pos = (orig_pos + att * att) % capacity_;
  } while (pos != orig_pos);
  return end();
}

template< class Key, class Value, class Hash, class Equal >
typename abramov::HashTable< Key, Value, Hash, Equal >::cIter
abramov::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return ConstHashIterator< Key, Value, Hash, Equal >(this, capacity_, nullptr);
}

template< class Key, class Value, class Hash, class Equal >
typename abramov::HashTable< Key, Value, Hash, Equal >::cIter
abramov::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  for (size_t i = 0; i < capacity_; ++i)
  {
    if (table_[i])
    {
      return ConstHashIterator< Key, Value, Hash, Equal >(this, i, table_[i]);
    }
  }
  return cend();
}

template< class Key, class Value, class Hash, class Equal >
typename abramov::HashTable< Key, Value, Hash, Equal >::cIter
abramov::HashTable< Key, Value, Hash, Equal >::cfind(const Key & k) const
{
  if (empty())
  {
    return cend();
  }
  size_t pos = hash_(k) % capacity_;
  size_t att = 0;
  size_t orig_pos = pos;
  do
  {
    HashNode< Key, Value > *curr = table_[pos];
    while (curr)
    {
      if (equal_(curr->data_.first, k))
      {
        return ConstHashIterator< Key, Value, Hash, Equal >(this, pos, curr);
      }
      curr = curr->next_;
    }
    ++att;
    pos = (orig_pos + att * att) % capacity_;
  } while (pos != orig_pos);
  return cend();
}

template< class Key, class Value, class Hash, class Equal >
Value &abramov::HashTable< Key, Value, Hash, Equal >::at(const Key &k)
{
  auto it = find(k);
  if (it == end())
  {
    throw std::out_of_range("There is no such element\n");
  }
  return it->second;
}


template< class Key, class Value, class Hash, class Equal >
size_t abramov::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Hash, class Equal >
bool abramov::HashTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return size_ == 0;
}

template< class Key, class Value, class Hash, class Equal >
bool abramov::HashTable< Key, Value, Hash, Equal >::isPrime(size_t k) const noexcept
{
  if (k % 2 == 0)
  {
    return true;
  }
  for (size_t i = 3; i < static_cast< int >(std::ceil(std::sqrt(k))); ++i)
  {
    if (k % i == 0)
    {
      return false;
    }
  }
  return true;
}

template< class Key, class Value, class Hash, class Equal >
size_t abramov::HashTable< Key, Value, Hash, Equal >::getLargerPrimeCapacity(size_t k) const noexcept
{
  size_t cand = 2 * cand + 1;
  while (true)
  {
    if (isPrime(cand))
    {
      return cand;
    }
    ++cand;
  }
}
#endif

