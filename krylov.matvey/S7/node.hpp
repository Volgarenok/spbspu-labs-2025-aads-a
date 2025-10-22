#ifndef NODE_HPP
#define NODE_HPP
#include <utility>

namespace krylov
{
  template< class Key, class Value >
  struct Node
  {
    std::pair< Key, Value > data;
    bool occupied = false;
    bool deleted = false;
  };
}

#endif
