#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP
#include <utility>
#include <cstddef>

namespace sveshnikov
{
  template< class Key, class T >
  struct tree_node_t
  {
    std::pair< Key, T > data_;
    tree_node_t< Key, T > *right_;
    tree_node_t< Key, T > *left_;
    tree_node_t< Key, T > *parent_;
    size_t height_;

    explicit tree_node_t();
    explicit tree_node_t(const std::pair< Key, T > &val);
  };

  template< class Key, class T >
  tree_node_t< Key, T >::tree_node_t():
    data_(std::make_pair(Key(), T())),
    right_(nullptr),
    left_(nullptr),
    parent_(nullptr),
    height_(0)
  {}

  template< class Key, class T >
  tree_node_t< Key, T >::tree_node_t(const std::pair< Key, T > &val):
    data_(val),
    right_(nullptr),
    left_(nullptr),
    parent_(nullptr),
    height_(1)
  {}
}

#endif
