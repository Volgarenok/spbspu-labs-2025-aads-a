#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <functional>
#include <vector>
#include <list>
#include "HashIter.hpp"

namespace zakirov
{
  template< class K, class T, class H = std::hash< K >, class C = std::equal_to< K > >
  class HashTable
  {
  public:
    HashTable();
    HashTable(const HashTable< K, T, H, C > & other);
    ~HashTable();
    HashIter< K, T, H, C > begin();
    HashIter< K, T, H, C > end();

    size_t size();
    bool empty();

    HashIter< K, T, H, C > erase(HashIter< K, T, H, C >);
    std::pair< K, T > erase(const K & k);

    std::pair< HashIter< K, T, H, C >, bool > insert(const std::pair< K, T > & value);

    HashIter< K, T, H, C > find(const K & key);

    float load_factor() const;
    float max_load_factor() const;
    void max_load_factor(float ml);
    void rehash(size_t count);
  private:
    std::vector< std::list< std::pair< K, T > > > buckets_;
    size_t capacity_;
    size_t size_;
    float load_factor_;
    H hash_;
    C equal_;
  };
}

#endif
