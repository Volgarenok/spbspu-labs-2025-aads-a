#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP

namespace maslevtsov {
  template< class T >
  struct TreeNode
  {
    TreeNode* parent;
    TreeNode* left;
    TreeNode* middle;
    TreeNode* right;
    bool is_two;
    T data1;
    T data2;
  };
}

#endif
