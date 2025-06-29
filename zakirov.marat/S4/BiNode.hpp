#ifndef BINODE_HPP
#define BINODE_HPP
#include <utility>

namespace zakirov
{
  template < class K, class T >
  struct BiNode
  {
    BiNode() = default;
    BiNode(const BiNode< K, T > * other);
    BiNode(std::pair< K, T > value);

    BiNode< K, T > * left_;
    BiNode< K, T > * right_;
    BiNode< K, T > * parent_;
    std::pair< K, T > value_;
  };

  template < class K, class T >
  BiNode< K, T >::BiNode(const BiNode< K, T > * other):
    BiNode()
  {
    if (other)
    {
      left_ = other->left_;
      right_ = other->right_;
      parent_ = other->parent_;
      value_ = other->value_;
    }
  }

  template < class K, class T >
  BiNode< K, T >::BiNode(std::pair< K, T > value):
    left_(nullptr),
    right_(nullptr),
    parent_(nullptr),
    value_(std::make_pair(value.first, value.second))
  {}
}

#endif
