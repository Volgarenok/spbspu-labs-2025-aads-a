#ifndef NODE_HPP
#define NODE_HPP

#include <utility>
#include <cstddef>

namespace mozhegova::detail
{
  template< typename Key, typename T >
  struct TreeNode
  {
    std::pair< Key, T > data;
    TreeNode * parent;
    TreeNode * left;
    TreeNode * right;
    size_t h;

    explicit TreeNode(std::pair< Key, T > val, TreeNode * fake):
      data(val),
      parent(nullptr),
      left(fake),
      right(fake),
      h(1)
    {}

    TreeNode():
      data(std::pair< Key, T >()),
      parent(nullptr),
      left(nullptr),
      right(nullptr),
      h(0)
    {}
  };
}

#endif
