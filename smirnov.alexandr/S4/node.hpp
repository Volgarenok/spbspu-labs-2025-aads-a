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
    int height;

    Node(const Key & key, const Value & value):
      data(key, value),
      left(nullptr),
      right(nullptr),
      height(1)
      {}
  };
}
#endif
