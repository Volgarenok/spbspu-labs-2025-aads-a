#ifndef NODE_TTT_HPP
#define NODE_TTT_HPP

#include <cstddef>
#include <utility>

namespace gavrilova {
  template < class Key, class Value >
  struct NodeTwoThreeTree {
    using this_t = NodeTwoThreeTree< Key, Value >;
    using value_type = std::pair< Key, Value >;

    value_type data[2];
    this_t* children[3];
    this_t* parent;
    bool is_3_node;
    bool is_fake;

    NodeTwoThreeTree();
    NodeTwoThreeTree(const value_type& d1, const value_type& d2, bool is_3,
    this_t* p, this_t* c1, this_t* c2, this_t* c3);
    bool is_leaf() const;
  };

  template < class Key, class Value >
  NodeTwoThreeTree< Key, Value >::NodeTwoThreeTree():
    parent(nullptr),
    is_3_node(false),
    is_fake(false)
  {
    children[0] = children[1] = children[2] = nullptr;
    data[0] = data[1] = std::pair< Key, Value >();
  }

  template < class Key, class Value >
  NodeTwoThreeTree< Key, Value >::NodeTwoThreeTree(const value_type& d1, const value_type& d2,
  bool is_3, this_t* p, this_t* c0, this_t* c1, this_t* c2):
    data(), children(), parent(p), is_3_node(is_3), is_fake(false)
  {
    data[0] = d1;
    data[1] = d2;
    children[0] = c0;
    children[1] = c1;
    children[2] = c2;
  }

  template < class Key, class Value >
  bool NodeTwoThreeTree< Key, Value >::is_leaf() const
  {
    return children[0]->is_fake;
  }

}

#endif
