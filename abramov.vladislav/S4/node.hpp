#ifndef NODE_HPP
#define NODE_HPP
#include <utility>
#include <cstddef>

namespace abramov
{
  template< class Key, class Value >
  struct Node
  {
    std::pair< Key, Value > data_;
    Node< Key, Value > *parent_;
    Node< Key, Value > *left_;
    Node< Key, Value > *right_;
    char bfactor_;

    Node();
    Node(const std::pair< Key, Value > &p, const Node< Key, Value > *parent);
  };

  template< class Key, class Value >
  Node< Key, Value >::Node():
    data_(std::pair< Key, Value >()),
    parent_(nullptr),
    left_(nullptr),
    right_(nullptr),
    bfactor_(0)
  {}

  template< class Key, class Value >
  Node<Key, Value >::Node(const std::pair< Key, Value > &p, const Node< Key, Value > *parent):
    data_(p),
    parent_(parent),
    left_(parent->left_),
    right_(parent->right_),
    bfactor_(0)
  {}
}
#endif
