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
    ~HashTable() noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    float loadFactor() const noexcept;
    float maxLoadFactor() const noexcept;
    void maxLoadFactor(float) noexcept;

  private:
    using Slot = detail::Slot< K, V >;

    Slot* slots_;
    size_t size_;
    size_t capacity_;
    float maxLoadFactor_;
  };

  template< class K, class V, class H, class E >
  HashTable< K, V, H, E >::HashTable():
    slots_(new Slot[minCapacity]),
    size_(0),
    capacity_(minCapacity),
    maxLoadFactor_(maxLoadFactorValue)
  {}

  template< class K, class V, class H, class E >
  HashTable< K, V, H, E >::~HashTable() noexcept
  {
    delete[] slots_;
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
}

#endif

