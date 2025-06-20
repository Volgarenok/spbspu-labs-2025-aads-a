#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace duhanina
{
  template < typename Key, typename Value >
  struct Node
  {
    std::pair< Key, Value > data;
    Node* left;
    Node* right;
    Node* parent;
    int height;

    Node(const Key& k, const Value& v, Node* p);
  };

  template < typename Key, typename Value >
  Node< Key, Value >::Node(const Key& k, const Value& v, Node* p):
    data(std::make_pair(k, v)),
    left(nullptr),
    right(nullptr),
    parent(p),
    height(1)
  {}
}

#endif
