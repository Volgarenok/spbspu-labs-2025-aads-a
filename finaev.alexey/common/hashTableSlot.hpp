#ifndef HASHTABLESLOT_HPP
#define HASHTABLESLOT_HPP
#include <utility>

namespace finaev
{
  template< class Key, class Value >
  struct Slot
  {
    std::pair< Key, Value > data;
    bool occupied = false;
    bool deleted = false;
  };
}

#endif
