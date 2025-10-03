#ifndef BUCKET_HPP
#define BUCKET_HPP

#include <utility>

namespace zholobov {

  template < typename Key, typename T >
  struct Bucket {
    std::pair< Key, T > data;
    bool occupied = false;
    bool deleted = false;
  };

}

#endif
