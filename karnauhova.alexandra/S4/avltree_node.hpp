#ifndef AVLTREE_NODE_HPP
#define AVLTREE_NODE_HPP
#include <utility>

namespace karnauhova::detail
{
  template< typename Key, typename Value >
  struct AvlTreeNode
  {
    std::pair< Key, Value > data;
    AvlTreeNode< Key, Value >* left, * right, * parent;
    int height;
  };
}

#endif
