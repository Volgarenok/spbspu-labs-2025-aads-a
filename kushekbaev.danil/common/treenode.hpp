#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <cstddef>
#include <utility>

namespace kushekbaev
{
  template< typename Key, typename Value, typename Cmp >
  struct TreeNode
  {
    template< typename... Args >
    explicit TreeNode(Args&&... args);

    std::pair< Key, Value > data;
    TreeNode< Key, Value, Cmp >* parent;
    TreeNode< Key, Value, Cmp >* left;
    TreeNode< Key, Value, Cmp >* right;
  };

  template< typename Key, typename Value, typename Cmp >
  template< typename... Args >
  TreeNode< Key, Value, Cmp >::TreeNode(Args&&... args):
    data{std::pair< Key, Value >(std::forward< Args >(args)...)},
    parent(nullptr),
    left(nullptr),
    right(nullptr)
  {}
}

#endif
