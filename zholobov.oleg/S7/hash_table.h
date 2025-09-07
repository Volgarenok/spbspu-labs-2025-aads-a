#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <functional>

namespace zholobov {

  template < class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable {
  };

}

#endif
