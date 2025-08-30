#ifndef HASH_NODE_HPP
#define HASH_NODE_HPP
#include <utility>

namespace abramov
{
  template< class Key, class Value >
  struct HashNode
  {
    std::pair< Key, Value > data_;
    HashNode< Key, Value > *next_;
    bool active_;

    HashNode(const Key &k, const Value &v);
  };
}

template< class Key, class Value >
abramov::HashNode< Key, Value >::HashNode(const Key &k, const Value &v):
  data_(std::make_pair(k, v)),
  next_(nullptr),
  active_(true)
{}
#endif
