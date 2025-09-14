#ifndef TREENODE_HPP
#define TREENODE_HPP
#include <utility>

namespace krylov
{
  template< typename Key, typename T >
  struct BiTreeNode
  {
    std::pair< Key, T > data;
    BiTreeNode* left;
    BiTreeNode* right;
    BiTreeNode* parent;
  };
}

#endif
