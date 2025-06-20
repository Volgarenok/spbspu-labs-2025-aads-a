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

    Node();
    Node(std::pair< Key, Value > data, Node< Key, Value > *p, Node< Key, Value > *l, Node< Key, Value > *r);
    Node(const std::pair< Key, Value > &p, const Node< Key, Value > *parent);
  };

  template< class Key, class Value >
  Node< Key, Value >::Node():
    data_(std::pair< Key, Value >()),
    parent_(nullptr),
    left_(nullptr),
    right_(nullptr)
  {}

  template< class Key, class Value >
  Node< Key, Value >::Node(std::pair< Key, Value > d, Node< Key, Value > *p, Node< Key, Value > *l, Node< Key, Value > *r):
    data_(d),
    parent_(p),
    left_(l),
    right_(r)
  {}

  template< class Key, class Value >
  Node<Key, Value >::Node(const std::pair< Key, Value > &p, const Node< Key, Value > *parent):
    data_(p),
    parent_(parent),
    left_(parent->left_),
    right_(parent->right_)
  {}
}
#endif
