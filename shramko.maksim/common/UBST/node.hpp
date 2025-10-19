#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace shramko
{
  template < typename Key, typename Value >
  struct Node
  {
    std::pair< const Key, Value > data;
    Node * left;
    Node * right;
    Node * parent;

    Node(const Key & key, const Value & value):
      data(key, value),
      left(nullptr),
      right(nullptr),
      parent(nullptr)
    {}
  };
}

#endif
