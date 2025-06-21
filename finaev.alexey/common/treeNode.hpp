#ifndef TREENODE_HPP
#define TREENODE_HPP
#include <utility>

namespace finaev
{
  template < class Key, class Value >
  struct TreeNode
  {
    std::pair< Key, Value > data;
    TreeNode< Key, Value >* left;
    TreeNode< Key, Value >* right;
    TreeNode< Key, Value >* parent;
    int height;
    TreeNode(const Key& k, const Value& val, TreeNode< Key, Value >* p);
  };

  template < class Key, class Value >
  TreeNode< Key, Value >::TreeNode(const Key& k, const Value& val, TreeNode< Key, Value >* p):
    data(std::pair< Key, Value >(k, val)),
    left(nullptr),
    right(nullptr),
    parent(p),
    height(1)
  {}
}

#endif
