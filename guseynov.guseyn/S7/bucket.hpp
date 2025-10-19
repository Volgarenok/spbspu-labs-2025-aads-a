#ifndef BUCKET_HPP
#define BUCKET_HPP
#include <utility>

namespace guseynov
{
  template< class Key, class Value >
  struct Bucket
  {
    std::pair< Key, Value > data;
    bool occupied = false;
    bool deleted = false;
  };
}
#endif
