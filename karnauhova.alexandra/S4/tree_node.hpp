#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP
#include <utility>
namespace karnauhova
{
  template< typename Key, typename Value >
  struct TreeNode
  {
    bool full;
    std::pair< Key, Value > data1;
    std::pair< Key, Value > data2;
    TreeNode< Key, Value >* left, * right, * middle, * parent;
  };
}

#endif
