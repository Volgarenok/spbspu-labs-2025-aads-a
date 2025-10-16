#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <cassert>
#include <iterator>
#include "node.hpp"

namespace shramko
{
  template< class Key, class T, class Hash, class Eq >
  class HashTable;

  template< class Key, class T, class Hash, class Eq >
  class HashIterator
  {
  public:
    using this_t = HashIterator< Key, T, Hash, Eq >;
    using node = Node< Key, T >;

    ~HashIterator() = default;
    HashIterator() noexcept;
    HashIterator(const this_t&) = default;
    this_t& operator=(const this_t&) = default;

    std::pair< Key, T >& operator*() noexcept;
    std::pair< Key, T >* operator->() noexcept;
    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    bool operator!=(const this_t& rhs) const noexcept;
    bool operator==(const this_t& rhs) const noexcept;

  private:
    node* slots_;
    size_t capacity_;
    size_t current_;
    void find_occupied() noexcept;
    HashIterator(node* slots, size_t cap, size_t curr) noexcept;
    friend class HashTable< Key, T, Hash, Eq >;
  };

  template< class Key, class T, class Hash, class Eq >
  HashIterator< Key, T, Hash, Eq >::HashIterator() noexcept
    : slots_(nullptr),
      capacity_(0),
      current_(0)
  {}

  template< class Key, class T, class Hash, class Eq >
  HashIterator< Key, T, Hash, Eq >::HashIterator(node* slots, size_t cap, size_t curr) noexcept
    : slots_(slots),
      capacity_(cap),
      current_(curr)
  {
    find_occupied();
  }

  template< class Key, class T, class Hash, class Eq >
  std::pair< Key, T >& HashIterator< Key, T, Hash, Eq >::operator*() noexcept
  {
    assert(slots_ != nullptr);
    assert(current_ < capacity_);
    assert(slots_[current_].occupied && !slots_[current_].deleted);

    return slots_[current_].data;
  }

  template< class Key, class T, class Hash, class Eq >
  std::pair< Key, T >* HashIterator< Key, T, Hash, Eq >::operator->() noexcept
  {
    return std::addressof(operator*());
  }

  template< class Key, class T, class Hash, class Eq >
  HashIterator< Key, T, Hash, Eq >& HashIterator< Key, T, Hash, Eq >::operator++() noexcept
  {
    assert(slots_ != nullptr);
    assert(current_ < capacity_);

    if (current_ < capacity_)
    {
      ++current_;
      find_occupied();
    }

    return *this;
  }

  template< class Key, class T, class Hash, class Eq >
  HashIterator< Key, T, Hash, Eq > HashIterator< Key, T, Hash, Eq >::operator++(int) noexcept
  {
    HashIterator< Key, T, Hash, Eq > tmp = *this;

    ++(*this);

    return tmp;
  }

  template< class Key, class T, class Hash, class Eq >
  bool HashIterator< Key, T, Hash, Eq >::operator==(const this_t& rhs) const noexcept
  {
    return slots_ == rhs.slots_ && current_ == rhs.current_;
  }

  template< class Key, class T, class Hash, class Eq >
  bool HashIterator< Key, T, Hash, Eq >::operator!=(const this_t& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< class Key, class T, class Hash, class Eq >
  void HashIterator< Key, T, Hash, Eq >::find_occupied() noexcept
  {
    while (current_ < capacity_ && (!slots_[current_].occupied || slots_[current_].deleted))
    {
      ++current_;
    }
  }

  template< class Key, class T, class Hash, class Eq >
  class HashConstIterator
  {
  public:
    using this_t = HashConstIterator< Key, T, Hash, Eq >;
    using node = Node< Key, T >;

    ~HashConstIterator() = default;
    HashConstIterator() noexcept;
    HashConstIterator(const this_t&) = default;
    this_t& operator=(const this_t&) = default;

    const std::pair< Key, T >& operator*() const noexcept;
    const std::pair< Key, T >* operator->() const noexcept;
    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    bool operator!=(const this_t& rhs) const noexcept;
    bool operator==(const this_t& rhs) const noexcept;

  private:
    node* slots_;
    size_t capacity_;
    size_t current_;
    void find_occupied() noexcept;
    HashConstIterator(node* slots, size_t cap, size_t curr) noexcept;
    friend class HashTable< Key, T, Hash, Eq >;
  };

  template< class Key, class T, class Hash, class Eq >
  HashConstIterator< Key, T, Hash, Eq >::HashConstIterator() noexcept
    : slots_(nullptr),
      capacity_(0),
      current_(0)
  {}

  template< class Key, class T, class Hash, class Eq >
  HashConstIterator< Key, T, Hash, Eq >::HashConstIterator(node* slots, size_t cap, size_t curr) noexcept
    : slots_(slots),
      capacity_(cap),
      current_(curr)
  {
    find_occupied();
  }

  template< class Key, class T, class Hash, class Eq >
  const std::pair< Key, T >& HashConstIterator< Key, T, Hash, Eq >::operator*() const noexcept
  {
    assert(slots_ != nullptr);
    assert(current_ < capacity_);
    assert(slots_[current_].occupied && !slots_[current_].deleted);

    return slots_[current_].data;
  }

  template< class Key, class T, class Hash, class Eq >
  const std::pair< Key, T >* HashConstIterator< Key, T, Hash, Eq >::operator->() const noexcept
  {
    return std::addressof(operator*());
  }

  template< class Key, class T, class Hash, class Eq >
  HashConstIterator< Key, T, Hash, Eq >& HashConstIterator< Key, T, Hash, Eq >::operator++() noexcept
  {
    assert(slots_ != nullptr);
    assert(current_ < capacity_);

    if (current_ < capacity_)
    {
      ++current_;
      find_occupied();
    }

    return *this;
  }

  template< class Key, class T, class Hash, class Eq >
  HashConstIterator< Key, T, Hash, Eq > HashConstIterator< Key, T, Hash, Eq >::operator++(int) noexcept
  {
    HashConstIterator< Key, T, Hash, Eq > tmp = *this;

    ++(*this);

    return tmp;
  }

  template< class Key, class T, class Hash, class Eq >
  bool HashConstIterator< Key, T, Hash, Eq >::operator==(const this_t& rhs) const noexcept
  {
    return slots_ == rhs.slots_ && current_ == rhs.current_;
  }

  template< class Key, class T, class Hash, class Eq >
  bool HashConstIterator< Key, T, Hash, Eq >::operator!=(const this_t& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< class Key, class T, class Hash, class Eq >
  void HashConstIterator< Key, T, Hash, Eq >::find_occupied() noexcept
  {
    while (current_ < capacity_ && (!slots_[current_].occupied || slots_[current_].deleted))
    {
      ++current_;
    }
  }
}

#endif
