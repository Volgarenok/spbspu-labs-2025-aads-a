#ifndef HASHITER_HPP
#define HASHITER_HPP
#include <utility>
#include <vector>
#include <list>

namespace zakirov
{
  template< class K, class T, class H, class C >
  class HashTable;

  template< class K, class T, class H, class C >
  class HashIter: std::iterator< std::forward_iterator_tag, std::pair< Key, Value > >
  {
  public:
    HashIter();
    HashIter(HashTable< K, T, H, C > * t, size_t i, Node< K, T > * n);
    HashIter(const HashIter< K, T, H, C > & iter) = default;
    ~HashIter() = default;
    HashIter< K, T, H, C > & operator=(const HashIter< K, T, H, C > &iter) = default;
    std::pair< K, T > & operator*() const;
    std::pair< K, T > & operator->() const;
    HashIter< K, T, H, C > & operator++();
    HashIter< K, T, H, C > & operator++(int);
    bool operator!=(const BiIter< K, T, H, C > & other) const noexcept;
    bool operator==(const BiIter< K, T, H, C > & other) const noexcept;
  private:
    friend class HashTable< K, T, H, C >;

    using BucketT = std::vector< std::list< std::pair< K, T > > >;
    using ListIter = typename std::list< std::pair< K, T > >::iterator;

    BucketT * buckets_;
    size_t bucketIndex_;
    ListIter listIterator_;

    HashIter(BucketT * buckets, size_t bucketIndex, ListIter listIterator);
    void findBucket();
  };
}

#endif
