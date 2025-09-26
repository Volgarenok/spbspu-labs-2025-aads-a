#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <functional>

template< typename Key, typename Value, typename Hash = std::hash< Key >, typename Equal = std::equal_to< Key > >
struct HashTable
{
  HashTable();
  HashTable(const HashTable& other);
  HashTable(HashTable&& other);
  ~HashTable();
};

#endif
