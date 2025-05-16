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
  };
}

#endif
