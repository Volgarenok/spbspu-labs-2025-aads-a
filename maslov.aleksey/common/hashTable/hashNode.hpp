#ifndef HASH_NODE_HPP
#define HASH_NODE_HPP

#include <utility>

namespace maslov
{
  template< class Key, class T >
  struct HashNode
  {
    std::pair< Key, T > data;
    bool occupied = false;
    bool deleted = false;
  };
}

#endif
