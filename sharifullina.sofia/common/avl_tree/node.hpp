#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace sharifullina
{
  template< typename Key, typename T >
  struct TreeNode
  {
    std::pair< Key, T > data;
    TreeNode* parent;
    TreeNode* left;
    TreeNode* right;
    int height;

    TreeNode(const Key& k, const T& val, TreeNode* p);
  };

  template< typename Key, typename T >
  TreeNode< Key, T >::TreeNode(const Key& k, const T& val, TreeNode* p):
    data(std::pair< Key, T >(k, val)),
    parent(p),
    left(nullptr),
    right(nullptr),
    height(1)
  {}
}

#endif
