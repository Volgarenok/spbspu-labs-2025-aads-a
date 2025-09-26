#ifndef HASHTABLESLOT_HPP
#define HASHTABLESLOT_HPP

#include <utility>

namespace kushekbaev
{
  template< typename Key, typename Value >
  struct HashTableSlot
  {
    std::pair< Key, Value > data;
    bool deleted = false;
    bool occupied = false;
  };
}

#endif
