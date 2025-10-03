#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace zholobov {

  template < typename Key, typename T >
  struct Node {
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair< Key, T >;

    template < typename... Args >
    Node(Args&&... args) noexcept:
      data(std::forward< Args >(args)...),
      parent(nullptr),
      left(nullptr),
      right(nullptr),
      height(1)
    {}

    value_type data;
    Node* parent;
    Node* left;
    Node* right;
    int height;
  };

}

#endif
