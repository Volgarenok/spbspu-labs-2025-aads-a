#ifndef BUCKET_HPP
#define BUCKET_HPP

#include <utility>

namespace duhanina
{
  template< typename Key, typename Value >
  struct Bucket
  {
    std::pair< Key, Value > data;
    bool occupied = false;
    bool deleted = false;
  };
}

#endif
