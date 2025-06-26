#ifndef NODE_TREE_HPP
#define NODE_TREE_HPP

#include <cstddef>
#include <utility>

namespace tkach
{
  template< class Key, class Value >
  struct TreeNode
  {
    template< class... Args >
    TreeNode(Args... args);
    std::pair< Key, Value > data;
    TreeNode< Key, Value >* parent;
    TreeNode< Key, Value >* right;
    TreeNode< Key, Value >* left;
    size_t height;
  };

  template< class Key, class Value >
  template< class... Args >
  TreeNode< Key, Value >::TreeNode(Args... args):
    data{std::pair< Key, Value >(std::forward< Args >(args)...)},
    parent(nullptr),
    right(nullptr),
    left(nullptr),
    height(0)
  {}
}

#endif
