#ifndef PAIR_HASH_HPP
#define PAIR_HASH_HPP

#include <cstddef>
#include <utility>
#include <functional>

namespace maslevtsov {
  template< class Key >
  struct PairKeyHash
  {
    size_t operator()(const std::pair< Key, Key >& pair) const
    {
      size_t hash1 = std::hash< Key >{}(pair.first);
      size_t hash2 = std::hash< Key >{}(pair.second);
      return hash1 ^ hash2;
    }
  };
}

#endif
