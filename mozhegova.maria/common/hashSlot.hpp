#ifndef HASHSLOT_HPP
#define HASHSLOT_HPP

#include <utility>

namespace mozhegova
{
  template< typename Key, typename Value >
  struct Slot
  {
    std::pair< Key, Value > data;
    bool occupied = false;
    bool deleted = false;
  };
}

#endif
