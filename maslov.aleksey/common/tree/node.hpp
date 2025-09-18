#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP

#include <utility>

namespace maslov
{
  template< typename Key, typename T >
  struct BiTreeNode
  {
    std::pair< Key, T > data;
    BiTreeNode * left;
    BiTreeNode * right;
    BiTreeNode * parent;
  };
}

#endif
