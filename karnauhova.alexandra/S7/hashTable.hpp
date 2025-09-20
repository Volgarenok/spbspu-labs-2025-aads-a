#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <boost/hash2/hash_append.hpp>
#include <boost/functional/hash.hpp>

#include <functional>
#include <vector>


namespace karnauhova
{
  enum class Status
  {
    EMPTY,
    DELETED,
    OCCUPIED
  };

  template < typename Key, typename Value, typename Hash1, typename Hash2, typename Equal >
  struct Box
  {
    std::pair< Key, Value > pair;
    Status status = Status::EMPTY;
  };

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

  template < typename Key, typename Value, typename Hash1 = std::hash< Key >, typename Hash2 = XXHash< Key >, typename Equal >
  class HashTable
  {
  public:
    HashTable();
    std::pair< Iterator, bool > insert(const value& val);

    size_t size() const noexcept;
    bool empty() const noexcept;
    
  private:
    std::vector< Box > slots_;
    size_t count_;
  };

  template < typename Key, typename Value, typename Hash1, typename Hash2, typename Equal >
  HashTable< Key, Value, Hash1, Hash2, Equal >::HashTable():
    slots_(10),
    count_(0)
  {}

  template < typename Key, typename Value, typename Hash1, typename Hash2, typename Equal >
  size_t HashTable< Key, Value, Hash1, Hash2, Equal >::size() const noexcept
  {
    return count_;
  }

  template < typename Key, typename Value, typename Hash1, typename Hash2, typename Equal >
  bool HashTable< Key, Value, Hash1, Hash2, Equal >::empty() const noexcept
  {
    return count_ == 0;
  }
}

#endif
