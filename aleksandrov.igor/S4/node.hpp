#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace aleksandrov
{
  namespace detail
  { 
    template< typename Key, typename Value >
    struct Node
    {
      std::pair< Key, Value > data[2]; 
      Node* left;
      Node* middle;
      Node* right;
      Node* parent;

      Node(const Key& key, const Value& value):
        data{ std::make_pair(key, value), std::pair< Key, Value >() },
	left(nullptr),
	middle(nullptr),
	right(nullptr),
	parent(nullptr)
      {}

      Node(const Key& key1, const Value& value1, const Key& key2, const Value& value2):
        data{ std::make_pair(key1, value1), std::make_pair(key2, value2) },
	left(nullptr),
        middle(nullptr),
        right(nullptr),
        parent(nullptr)
      {}
    };
  }
}

#endif

