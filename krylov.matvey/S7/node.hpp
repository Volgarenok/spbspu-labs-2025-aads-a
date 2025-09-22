#ifndef NODE_HPP
#define NODE_HPP
#include <utility>

namespace krylov
{
  template< class Key, class Value >
  struct Node
  {
    std::pair< Key, Value > data_;
    bool active_;
    Node(const Key& k, const Value& v);
  };

  template< class Key, class Value >
  Node< Key, Value >::Node(const Key& k, const Value& v):
    data_(std::make_pair< Key, Value >(k, v)),
    active_(true)
  {}
}

#endif
