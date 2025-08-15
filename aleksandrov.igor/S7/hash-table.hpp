#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <boost/container_hash/hash.hpp>
#include <cstddef>
#include "slot.hpp"
#include "iterator.hpp"

namespace aleksandrov
{
  constexpr size_t minCapacity = 61;
  constexpr float maxLoadFactorValue = 0.75f;

  template< class K, class V, class H = boost::hash< K >, class E = std::equal_to< K > >
  class HashTable
  {
  public:
    using Iter = Iterator< K, V, H, E, false >;
    using ConstIter = Iterator< K, V, H, E, true >;

    HashTable();
    HashTable(const HashTable&);
    HashTable(HashTable&&);
    ~HashTable() noexcept;

    HashTable& operator=(const HashTable&);
    HashTable& operator=(HashTable&&);

    bool empty() const noexcept;
    size_t size() const noexcept;

    void clear() noexcept;
    void swap(HashTable&) noexcept;

    V& at(const K&);
    const V& at(const K&) const;
    Iter find(const K&);
    ConstIter find(const K&) const;

    Iter begin();
    ConstIter begin() const;
    ConstIter cbegin() const;
    Iter end();
    ConstIter end() const;
    ConstIter cend() const;

    float loadFactor() const noexcept;
    float maxLoadFactor() const noexcept;
    void maxLoadFactor(float) noexcept;

    H hashFunction() const;
    E keyEq() const;

  private:
    template< class, class, class, class, bool >
    friend class Iterator;
    using Slot = detail::Slot< K, V >;

    Slot* data_;
    size_t size_;
    size_t capacity_;
    float maxLoadFactor_;
    H hasher_;
    E keyEqual_;

    Slot* copyData(const HashTable&);
  };

  template< class K, class V, class H, class E >
  HashTable< K, V, H, E >::HashTable():
    data_(new Slot[minCapacity]),
    size_(0),
    capacity_(minCapacity),
    maxLoadFactor_(maxLoadFactorValue),
    hasher_{},
    keyEqual_{}
  {}

  template< class K, class V, class H, class E >
  HashTable< K, V, H, E >::HashTable(const HashTable& rhs):
    data_(copyData(rhs)),
    size_(rhs.size_),
    capacity_(rhs.capacity_),
    maxLoadFactor_(rhs.maxLoadFactor_),
    hasher_(rhs.hasher_),
    keyEqual_(rhs.keyEqual_)
  {}

  template< class K, class V, class H, class E >
  HashTable< K, V, H, E >::HashTable(HashTable&& rhs):
    data_(std::exchange(rhs.data_, nullptr)),
    size_(std::exchange(rhs.size_, 0)),
    capacity_(std::exchange(rhs.capacity_, 0)),
    maxLoadFactor_(std::exchange(rhs.maxLoadFactor_, 0)),
    hasher_(std::exchange(rhs.hasher_, H{})),
    keyEqual_(std::exchange(rhs.keyEqual_, E{}))
  {}

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::operator=(const HashTable& rhs) -> HashTable&
  {
    HashTable temp(rhs);
    swap(temp);
    return *this;
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::operator=(HashTable&& rhs) -> HashTable&
  {
    HashTable temp(std::move(rhs));
    swap(temp);
    return *this;
  }

  template< class K, class V, class H, class E >
  HashTable< K, V, H, E >::~HashTable() noexcept
  {
    delete[] data_;
  }

  template< class K, class V, class H, class E >
  bool HashTable< K, V, H, E >::empty() const noexcept
  {
    return !size_;
  }

  template< class K, class V, class H, class E >
  size_t HashTable< K, V, H, E >::size() const noexcept
  {
    return size_;
  }

  template< class K, class V, class H, class E >
  void HashTable< K, V, H, E >::clear() noexcept
  {
    if (!empty())
    {
      for (size_t i = 0; i < capacity_; ++i)
      {
        data_[i].occupied = false;
      }
      size_ = 0;
    }
  }

  template< class K, class V, class H, class E >
  void HashTable< K, V, H, E >::swap(HashTable& rhs) noexcept
  {
    std::swap(data_, rhs.data_);
    std::swap(size_, rhs.size_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(maxLoadFactor_, rhs.maxLoadFactor_);
    std::swap(hasher_, rhs.hasher_);
    std::swap(keyEqual_, rhs.keyEqual_);
  }

  template< class K, class V, class H, class E >
  V& HashTable< K, V, H, E >::at(const K& key)
  {
    Iter it = find(key);
    if (it == end())
    {
      throw std::out_of_range("ERROR: No such element exists!");
    }
    return it->second;
  }

  template< class K, class V, class H, class E >
  const V& HashTable< K, V, H, E >::at(const K& key) const
  {
    ConstIter it = find(key);
    if (it == cend())
    {
      throw std::out_of_range("ERROR: No such element exists!");
    }
    return it->second;
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::find(const K& key) -> Iter
  {
    ConstIter it = static_cast< const HashTable& >(*this).find(key);
    if (it == cend())
    {
      return Iter();
    }
    return Iter(this, it.index_);
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::find(const K& key) const -> ConstIter
  {
    size_t currSlotIndex = hasher_(key) % capacity_;
    while (data_[currSlotIndex].occupied)
    {
      if (keyEqual_(data_[currSlotIndex].data.first, key))
      {
        return ConstIter(this, currSlotIndex);
      }
      currSlotIndex = (currSlotIndex + 1) % capacity_;
    }
    return ConstIter();
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::begin() -> Iter
  {
    return empty() ? Iter() : Iter(this);
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::begin() const -> ConstIter
  {
    return cbegin();
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::cbegin() const -> ConstIter
  {
    return empty() ? ConstIter() : ConstIter(this);
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::end() -> Iter
  {
    return Iter();
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::end() const -> ConstIter
  {
    return cend();
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::cend() const -> ConstIter
  {
    return ConstIter();
  }

  template< class K, class V, class H, class E >
  float HashTable< K, V, H, E >::loadFactor() const noexcept
  {
    assert(capacity_ != 0);
    return static_cast< float >(size_) / capacity_;
  }

  template< class K, class V, class H, class E >
  float HashTable< K, V, H, E >::maxLoadFactor() const noexcept
  {
    return maxLoadFactor_;
  }

  template< class K, class V, class H, class E >
  void HashTable< K, V, H, E >::maxLoadFactor(float ml) noexcept
  {
    maxLoadFactor_ = ml;
  }

  template< class K, class V, class H, class E >
  H HashTable< K, V, H, E >::hashFunction() const
  {
    return hasher_;
  }

  template< class K, class V, class H, class E >
  E HashTable< K, V, H, E >::keyEq() const
  {
    return keyEqual_;
  }

  template< class K, class V, class H, class E >
  auto HashTable< K, V, H, E >::copyData(const HashTable& rhs) -> Slot*
  {
    Slot* copy = new Slot[rhs.capacity_];
    try
    {
      for (size_t i = 0; i < rhs.capacity_; ++i)
      {
        if (rhs.data_[i].occupied)
        {
          copy[i] = rhs.data_[i];
        }
      }
    }
    catch (...)
    {
      delete[] copy;
      throw;
    }
    return copy;
  }
}

#endif

