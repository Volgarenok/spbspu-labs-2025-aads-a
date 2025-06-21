#ifndef HASH_TABLE_ITERATOR_HPP
#define HASH_TABLE_ITERATOR_HPP

#include <cassert>
#include <iterator>
#include "hashNode.hpp"

namespace maslov
{
  template< class Key, class T, class HS1, class HS2, class EQ >
  struct HashTable;

  template< class Key, class T, class HS1, class HS2, class EQ >
  struct HashIterator
  {
    friend struct HashTable< Key, T, HS1, HS2, EQ >;
    using thisT = HashIterator< Key, T, HS1, HS2, EQ >;
    using node = HashNode< Key, T >;

    HashIterator();
    HashIterator(const thisT &) = default;
    ~HashIterator() = default;

    thisT & operator=(const thisT &) = default;
    std::pair< Key, T > & operator*();
    std::pair< Key, T > * operator->();
    thisT & operator++();
    thisT operator++(int);
    bool operator!=(const thisT & rhs) const;
    bool operator==(const thisT & rhs) const;
   private:
    explicit HashIterator(node * slots, size_t cap, size_t curr);
    node * slots_;
    size_t capacity_;
    size_t current_;
    void findOccupied();

  };

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashIterator< Key, T, HS1, HS2, EQ >::HashIterator():
    slots_(nullptr),
    capacity_(0),
    current_(0)
  {}

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashIterator< Key, T, HS1, HS2, EQ >::HashIterator(node * slots, size_t cap, size_t curr):
    slots_(slots),
    capacity_(cap),
    current_(curr)
  {
    findOccupied();
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  std::pair< Key, T > & HashIterator< Key, T, HS1, HS2, EQ >::operator*()
  {
    assert(slots_ != nullptr);
    assert(current_ < capacity_);
    assert(slots_[current_].occupied && !slots_[current_].deleted);
    return slots_[current_].data;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  std::pair< Key, T > * HashIterator< Key, T, HS1, HS2, EQ >::operator->()
  {
    return std::addressof(operator*());
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashIterator< Key, T, HS1, HS2, EQ > & HashIterator< Key, T, HS1, HS2, EQ >::operator++()
  {
    assert(slots_ != nullptr);
    assert(current_ < capacity_);
    if (current_ < capacity_)
    {
      ++current_;
      findOccupied();
    }
    return *this;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashIterator< Key, T, HS1, HS2, EQ > HashIterator< Key, T, HS1, HS2, EQ >::operator++(int)
  {
    HashIterator< Key, T, HS1, HS2, EQ > tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  bool HashIterator< Key, T, HS1, HS2, EQ >::operator==(const thisT & rhs) const
  {
    return slots_ == rhs.slots_ && current_ == rhs.current_;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  bool HashIterator< Key, T, HS1, HS2, EQ >::operator!=(const thisT & rhs) const
  {
    return !(*this == rhs);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  void HashIterator< Key, T, HS1, HS2, EQ >::findOccupied()
  {
    while (current_ < capacity_ && (!slots_[current_].occupied || slots_[current_].deleted))
    {
      ++current_;
    }
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  struct HashConstIterator
  {
    friend struct HashTable< Key, T, HS1, HS2, EQ >;
    using thisT = HashConstIterator< Key, T, HS1, HS2, EQ >;
    using node = HashNode< Key, T >;

    HashConstIterator();
    HashConstIterator(const thisT &) = default;
    ~HashConstIterator() = default;

    thisT & operator=(const thisT &) = default;
    const std::pair< Key, T > & operator*() const;
    const std::pair< Key, T > * operator->() const;
    thisT & operator++();
    thisT operator++(int);
    bool operator!=(const thisT & rhs) const;
    bool operator==(const thisT & rhs) const;
   private:
    explicit HashConstIterator(node * slots, size_t cap, size_t curr);
    node * slots_;
    size_t capacity_;
    size_t current_;
    void findOccupied();
  };

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashConstIterator< Key, T, HS1, HS2, EQ >::HashConstIterator():
    slots_(nullptr),
    capacity_(0),
    current_(0)
  {}

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashConstIterator< Key, T, HS1, HS2, EQ >::HashConstIterator(node * slots, size_t cap, size_t curr):
    slots_(slots),
    capacity_(cap),
    current_(curr)
  {
    findOccupied();
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  const std::pair< Key, T > & HashConstIterator< Key, T, HS1, HS2, EQ >::operator*() const
  {
    assert(slots_ != nullptr);
    assert(current_ < capacity_);
    assert(slots_[current_].occupied && !slots_[current_].deleted);
    return slots_[current_].data;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  const std::pair< Key, T > * HashConstIterator< Key, T, HS1, HS2, EQ >::operator->() const
  {
    return std::addressof(operator*());
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashConstIterator< Key, T, HS1, HS2, EQ > & HashConstIterator< Key, T, HS1, HS2, EQ >::operator++()
  {
    assert(slots_ != nullptr);
    assert(current_ < capacity_);
    if (current_ < capacity_)
    {
      ++current_;
      findOccupied();
    }
    return *this;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashConstIterator< Key, T, HS1, HS2, EQ > HashConstIterator< Key, T, HS1, HS2, EQ >::operator++(int)
  {
    HashConstIterator< Key, T, HS1, HS2, EQ > tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  bool HashConstIterator< Key, T, HS1, HS2, EQ >::operator==(const thisT & rhs) const
  {
    return slots_ == rhs.slots_ && current_ == rhs.current_;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  bool HashConstIterator< Key, T, HS1, HS2, EQ >::operator!=(const thisT & rhs) const
  {
    return !(*this == rhs);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  void HashConstIterator< Key, T, HS1, HS2, EQ >::findOccupied()
  {
    while (current_ < capacity_ && (!slots_[current_].occupied || slots_[current_].deleted))
    {
      ++current_;
    }
  }
}

#endif
