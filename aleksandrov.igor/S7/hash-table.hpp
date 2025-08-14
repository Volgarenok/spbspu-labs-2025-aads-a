#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <boost/container_hash/hash.hpp>
#include <cstddef>
#include "slot.hpp"

namespace aleksandrov
{
  constexpr size_t minCapacity = 64;
  constexpr float maxLoadFactorValue = 0.75f;

  template< class K, class V, class H = boost::hash< K >, class E = std::equal_to< K > >
  class HashTable
  {
  public:
    HashTable();
    HashTable(const HashTable&);
    HashTable(HashTable&&);
    ~HashTable() noexcept;

    HashTable& operator=(const HashTable&);
    HashTable& operator=(HashTable&&);

    bool empty() const noexcept;
    size_t size() const noexcept;

    void swap(HashTable&) noexcept;

    float loadFactor() const noexcept;
    float maxLoadFactor() const noexcept;
    void maxLoadFactor(float) noexcept;

    H hashFunction() const;
    E keyEq() const;

  private:
    using Slot = detail::Slot< K, V >;

    Slot* data_;
    size_t size_;
    size_t capacity_;
    float maxLoadFactor_;

    Slot* copyData(const HashTable&);
  };

  template< class K, class V, class H, class E >
  HashTable< K, V, H, E >::HashTable():
    data_(new Slot[minCapacity]),
    size_(0),
    capacity_(minCapacity),
    maxLoadFactor_(maxLoadFactorValue)
  {}

  template< class K, class V, class H, class E >
  HashTable< K, V, H, E >::HashTable(const HashTable& rhs):
    data_(copyData(rhs)),
    size_(rhs.size_),
    capacity_(rhs.capacity_),
    maxLoadFactor_(rhs.maxLoadFactor_)
  {}

  template< class K, class V, class H, class E >
  HashTable< K, V, H, E >::HashTable(HashTable&& rhs):
    data_(std::exchange(rhs.data_, nullptr)),
    size_(std::exchange(rhs.size_, 0)),
    capacity_(std::exchange(rhs.capacity_, 0)),
    maxLoadFactor_(std::exchange(rhs.maxLoadFactor_, 0))
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
  void HashTable< K, V, H, E >::swap(HashTable& rhs) noexcept
  {
    std::swap(data_, rhs.data_);
    std::swap(size_, rhs.size_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(maxLoadFactor_, rhs.maxLoadFactor_);
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
    return H{};
  }

  template< class K, class V, class H, class E >
  E HashTable< K, V, H, E >::keyEq() const
  {
    return E{};
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

