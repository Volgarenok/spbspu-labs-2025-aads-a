#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP

namespace maslevtsov {
  template< class T >
  struct TreeNode
  {
    T data1;
    T data2;
    TreeNode* parent;
    TreeNode* left;
    TreeNode* middle;
    TreeNode* right;
    bool is_two;
  };
}

#endif
