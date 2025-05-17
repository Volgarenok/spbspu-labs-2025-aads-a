#ifndef TREENODE_HPP
#define TREENODE_HPP
#include <utility>

namespace kiselev
{
  enum class Color
  {
    RED,
    BLACK
  };
  template< class Key, class Value >
  struct TreeNode
  {
    Color color;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
    std::pair< Key, Value > data;
  };
}
#endif
