#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace lanovenko
{
  template< class Key, class T >
  struct Node
  {
    std::pair< Key, T > data;
    bool occupied = false;
    bool deleted = false;
  };
}

#endif
