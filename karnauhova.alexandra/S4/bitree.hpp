#ifndef BITREE_HPP
#define BITREE_HPP
#include "tree_node.hpp"
#include <iostream>
#include <cstddef>
#include <stdexcept>
#include "bitree_iterator.hpp"
namespace karnauhova
{
  template< typename Key, typename Value, typename Compare = std::less< Key >>
  class BiTree
  {
  public:
    using Node = TreeNode< Key, Value >;
    using Iter = BiTreeIterator< Key, Value>;
    BiTree();
    ~BiTree();

    void push(Key k, Value v);
    Node* get(Key k);
    Value get_value(Key k);
    Value drop(Key k);

    Iter begin() const noexcept;
    Iter end() const noexcept;

    void clear();
    Node* getNode();
    bool empty() const;
  private:
    Node* fake_;
    Node* root_;
    size_t size_;
    void clear_tree(Node* root);
  };

  template< typename Key, typename Value, typename Compare >
  BiTree< Key, Value, Compare >::BiTree():
    fake_(reinterpret_cast< Node* >(new char[sizeof(Node)])),
    root_(nullptr),
    size_(0)
  {}

  template< typename Key, typename Value, typename Compare >
  bool BiTree< Key, Value, Compare >::empty() const
  {
    return size_ == 0;
  }

  template< typename Key, typename Value, typename Compare >
  void BiTree< Key, Value, Compare >::clear_tree(Node* root)
  {
    if (!root || root == fake_)
    {
      return;
    }
    clear_tree(root->left);
    clear_tree(root->right);
    clear_tree(root->middle);
    delete root;
  }

  template< typename Key, typename Value, typename Compare >
  typename BiTree< Key, Value, Compare >::Node* BiTree< Key, Value, Compare >::getNode()
  {
    return root_;
  }

  template< typename Key, typename Value, typename Compare >
  void BiTree< Key, Value, Compare >::clear()
  {
    clear_tree(root_);
    size_ = 0;
  }

  template< typename Key, typename Value, typename Compare >
  BiTree< Key, Value, Compare >::~BiTree()
  {
    clear();
    delete[] reinterpret_cast<char*>(fake_);
  }

  template< typename Key, typename Value, typename Compare >
  void BiTree< Key, Value, Compare >::push(Key k, Value v)
  {
    if (!root_)
    {
      root_ = new Node{0, {k, v}, {}, fake_, fake_, fake_, fake_};
      size_++;
      return;
    }
    Node* current = root_;
    Node* parent = nullptr;
    while (current != fake_)
    {
      parent = current;
      if (!current->full)
      {
        if (k < current->data1.first)
        {
          current = current->left;
        }
        else
        {
          current = current->right;
        }
      }
      else
      {
        if (k < current->data1.first)
        {
          current = current->left;
        }
        else if (k > current->data2.first)
        {
          current = current->right;
        }
        else
        {
          current = current->middle;
        }
      }
    }
    if (!parent->full)
    {
        if (k < parent->data1.first)
        {
            parent->data2 = parent->data1;
            parent->data1 = {k, v};
        }
        else
        {
            parent->data2 = {k, v};
        }
        parent->full = 1;
    }
    else
    {
      Node* leftNode = new Node{0, {}, {}, fake_, fake_, fake_, parent};
      Node* rightNode = new Node{0, {}, {}, fake_, fake_, fake_, parent};
      if (k < parent->data1.first)
      {
        leftNode->data1 = {k, v};
        rightNode->data1 = parent->data2;
        parent->data2 = {};
      }
      else if (k > parent->data2.first)
      {
        leftNode->data1 = parent->data1;
        rightNode->data1 = {k, v};
        parent->data1 = parent->data2;
        parent->data2 = {};
      }
      else
      {
        leftNode->data1 = parent->data1;
        rightNode->data1 = parent->data2;
        parent->data1 = {k, v};
        parent->data2 = {};
      }
      parent->full = 0;
      parent->left = leftNode;
      parent->right = rightNode;
      parent->middle = fake_;
    }
    size_++;
  }

  template< typename Key, typename Value, typename Compare >
  typename BiTree< Key, Value, Compare >::Node* BiTree< Key, Value, Compare >::get(Key k)
  {
    Node* now = root_;
    if (!now)
    {
      throw std::logic_error("Key not found: tree is empty");
    }
    while (now != fake_)
    {
      if (now->data1.first == k)
      {
        return now;
      }
      if (now->full)
      {
        if (now->data2.first == k)
        {
          return now;
        }
        else if (k < now->data2.first)
        {
          now = now->middle;
        }
      }
      if (k < now->data1.first)
      {
        now = now->left;
      }
      else
      {
        now = now->right;
      }
    }
    throw std::logic_error("Key not found");
  }

  template< typename Key, typename Value, typename Compare >
  Value BiTree< Key, Value, Compare >::get_value(Key k)
  {
    Node* it = get(k);
    if (it->data1.first == k)
    {
      return it->data1.second;
    }
    return it->data2.second;
  }

  /* template< typename Key, typename Value, typename Compare >
  Value BiTree< Key, Value, Compare >::drop(Key k)
  {
    Node* it = get(k);
    if (it->right == fake_ && it->middle == fake_ && it->left == fake_)
    {
      if (it->full)
      {
        if (it->data1.first == k)
        {
          it->data1 = it->data2;
        }
        it->data2 = 0;
        it->full = 0;
      }
      else
      {

      }
    }
    if (!it->full)
    {
      Node* parent = it->parent;
      if (parent->left == it)
      {
        Node* right_brother = parent->right;
        if (right_brother->full && !parent->full)
        {
          it->data1 = parent->data1;
          parent->data1 = right_brother->data1;
          right_brother->data1 = right_brother->data2;
          right_brother->data2 = 0;
          right_brother->full = 0;
          while (right_brother->left != fake_)
          {

          }
        }
      }
    }
  }
 */
  template< typename Key, typename Value, typename Compare >
  typename BiTree< Key, Value, Compare >::Iter BiTree< Key, Value, Compare >::begin() const noexcept
  {
    return Iter(root_, 0);
  }

  template< typename Key, typename Value, typename Compare >
  typename BiTree< Key, Value, Compare >::Iter BiTree< Key, Value, Compare >::end() const noexcept
  {
    return Iter(fake_, 0);
  }
}

#endif
