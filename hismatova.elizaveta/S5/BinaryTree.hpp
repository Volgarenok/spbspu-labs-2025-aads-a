#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include <iostream>
#include <fstream>
#include <limits>
#include <queue>

namespace hismatova
{

  template< typename TKey, typename TValue >
  class BinaryTree
  {
    struct Node
    {
      TKey key;
      TValue value;
      Node* left;
      Node* right;
    };

    Node* root = nullptr;

  public:
    BinaryTree() = default;
    ~BinaryTree()
    {
      clear(root);
    }

    void insert(TKey key, const TValue& value)
    {
      root = insertRec(root, key, value);
    }

    void loadFromFile(const char* filename)
    {
      std::ifstream file(filename);
      if (!file.is_open())
      {
        std::cerr << "ERROR: can not open file\n";
        exit(1);
      }
      TKey key;
      TValue value;
      while (file >> key >> value)
      {
        insert(key, value);
      }
    }

    bool empty() const
    {
      return root == nullptr;
    }

    template< typename Func >
    void traverseAscending(Func f) const
    {
      inorder(root, f);
    }

    template< typename Func >
    void traverseDescending(Func f) const
    {
      reverseOrder(root, f);
    }

    template< typename Func >
    void traverseBreadth(Func f) const
    {
      if (!root)
      {
        return;
      }
      std::queue<Node*> q;
      q.push(root);
      while (!q.empty())
      {
        Node* n = q.front(); q.pop();
        f(n->key, n->value);
        if (n->left)
        {
          q.push(n->left);
        }
        if (n->right)
        {
          q.push(n->right);
        }
      }
    }

  private:
    Node* insertRec(Node* node, TKey key, const TValue& value)
    {
      if (!node)
      {
        Node* n = new Node{key, value, nullptr, nullptr};
        return n;
      }
      if (key < node->key)
      {
        node->left = insertRec(node->left, key, value);
      }
      else if (key > node->key)
      {
        node->right = insertRec(node->right, key, value);
      }
      return node;
    }

    template< typename Func >
    void inorder(Node* node, Func f) const
    {
      if (!node)
      {
        return;
      }
      inorder(node->left, f);
      f(node->key, node->value);
      inorder(node->right, f);
    }

    template< typename Func >
    void reverseOrder(Node* node, Func f) const
    {
      if (!node)
      {
        return;
      }
      reverseOrder(node->right, f);
      f(node->key, node->value);
      reverseOrder(node->left, f);
    }

    void clear(Node* node)
    {
      if (!node)
      {
        return;
      }
      clear(node->left);
      clear(node->right);
      delete node;
    }
  };

}

#endif
