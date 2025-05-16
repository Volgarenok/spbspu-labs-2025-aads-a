#ifndef NODE_HPP
#define NODE_HPP
#include <utility>

namespace demehin
{
  template< typename Key, typename T >
  struct TreeNode
  {
    std::pair< Key, T > data;
    TreeNode* parent;
    TreeNode* left;
    TreeNode* right;
    int height;

    explicit TreeNode(const std::pair< Key, T >&) noexcept;
    TreeNode(const std::pair< Key, T >&, const TreeNode*) noexcept;
  };

  template< typename Key, typename T >
  TreeNode< Key, T >::TreeNode(const std::pair< Key, T >& value) noexcept:
    data(value),
    parent(nullptr),
    left(nullptr),
    right(nullptr),
    height(1)
  {}

  template< typename Key, typename T >
  TreeNode< Key, T >::TreeNode(const std::pair< Key, T >& value, const TreeNode* prnt) noexcept:
    TreeNode(value)
  {
    parent = prnt;
  }
}

#endif
