#ifndef NODE_HPP
#define NODE_HPP
#include <utility>

namespace smirnov
{
  template < typename Key, typename Value >
  struct Node
  {
    std::pair< const Key, Value > data;
    Node * left;
    Node * right;
    Node * parent;
    int height;

    Node(const Key & key, const Value & value):
      data(key, value),
      left(nullptr),
      right(nullptr),
      parent(nullptr),
      height(1)
    {}
  };
}
#endif
