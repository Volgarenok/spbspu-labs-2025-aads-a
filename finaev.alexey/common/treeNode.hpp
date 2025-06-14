#ifndef TREENODE_HPP
#define TREENODE_HPP
#include <utility>

namespace finaev
{
  template < class Key, class Value >
  struct treeNode
  {
    std::pair< Key, Value > data;
    treeNode< Key, Value >* left;
    treeNode< Key, Value >* right;
    treeNode< Key, Value >* parent;
    int height;
    treeNode(const Key& k, const Value& val, treeNode< Key, Value >* p);
  };

  template < class Key, class Value >
  treeNode< Key, Value >::treeNode(const Key& k, const Value& val, treeNode< Key, Value >* p):
    data(std::pair< Key, Value >(k, val)),
    left(nullptr),
    right(nullptr),
    parent(p),
    height(1)
  {}
}

#endif
