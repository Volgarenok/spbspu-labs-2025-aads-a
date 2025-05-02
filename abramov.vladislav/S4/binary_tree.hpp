#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP
#include <cstddef>
#include <algorithm>
#include <functional>
#include "node.hpp"
#include "cIterator.hpp"
#include "iterator.hpp"

namespace abramov
{
  template< class Key, class Value >
  using range_t = std::pair< Iterator< Key, Value >, Iterator< Key, Value > >;
  template< class Key, class Value >
  using crange_t = std::pair< ConstIterator< Key, Value >, ConstIterator< Key, Value > >;

 template< class Key, class Value, class Cmp = std::less< Key > >
  struct BinarySearchTree
  {
    BinarySearchTree();
    void insert(const Key &key, const Value &value);
    Iterator< Key, Value > begin();
    Iterator< Key, Value > end();
    ConstIterator< Key, Value > cbegin() const;
    ConstIterator< Key, Value > cend() const;
    Iterator< Key, Value > find(const Key &k) noexcept;
    ConstIterator< Key, Value > find(const Key &k, bool b = true) noexcept;
    size_t count(const Key &k) const noexcept;
    range_t< Key, Value > equal_range(const Key &k) noexcept;
    crange_t< Key, Value > equal_range(const Key &k, bool b = true) noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(BinarySearchTree &rhs) noexcept;
    void clear() noexcept;
  private:
    Node< Key, Value > *root_;
    Node< Key, Value > *fake_;
    Cmp cmp_;
    size_t size_;

    void clearNodes(Node< Key, Value > *root) noexcept;
    Node< Key, Value > *getMin(Node< Key, Value > *root) noexcept;
    Node< Key, Value > *rotateLeft(Node< Key, Value > *root) noexcept;
    Node< Key, Value > *rotateRight(Node< Key, Value > *root) noexcept;
    Node< Key, Value > *findNode(const Key &k) noexcept;
  };

  template< class Key, class Value, class Cmp >
  BinarySearchTree< Key, Value, Cmp >::BinarySearchTree():
    root_(nullptr),
    fake_(new Node< Key, Value >),
    cmp_(Cmp()),
    size_(0)
  {}

  template< class Key, class Value, class Cmp >
  void BinarySearchTree< Key, Value, Cmp >::insert(const Key &key, const  Value &value)
  {
    if (empty())
    {
      root_ = new Node< Key, Value >(std::make_pair(key, value), nullptr, fake_, fake_, 1);
      ++size_;
      return;
    }
    Node< Key, Value > *root = root_;
    Node< Key, Value > *node = new Node< Key, Value >(std::make_pair(key, value), nullptr, fake_, fake_, -1);
    while (root->left != fake_ && root->right != fake_)
    {
      if (cmp(key, root->data_))
      {
        root = root->left;
      }
      else
      {
        root = root->right;
      }
    }
    if (cmp(key, root->data))
    {
      if (root->left != fake_)
      {
        root = root->left;
        if (cmp(key, root->data))
        {
          root->left = node;
        }
        else
        {
          root->rigth = node;
        }
      }
      else
      {
        root->left = node;
      }
    }
    else
    {
      if (root->right != fake_)
      {
        root = root->right;
        if (cmp(key, root->data_))
        {
          root->left = node;
        }
        else
        {
          root->right = node;
        }
      }
      else
      {
        root->right = node;
      }
    }
    node->parent = root;
    ++size_;
  }

  template< class Key, class Value, class Cmp >
  Iterator< Key, Value > BinarySearchTree< Key, Value, Cmp >::begin()
  {
    Node< Key, Value > *node = getMin(root_);
    return Iterator< Key, Value >(node);
  }

  template< class Key, class Value, class Cmp >
  Iterator< Key, Value > BinarySearchTree< Key, Value, Cmp >::end()
  {
    return Iterator< Key, Value >(fake_);
  }

  template< class Key, class Value, class Cmp >
  ConstIterator< Key, Value > BinarySearchTree< Key, Value, Cmp >::cbegin() const
  {
    Node< Key, Value > *node = getMin(root_);
    return ConstIterator< Key, Value >(node);
  }

  template< class Key, class Value, class Cmp >
  ConstIterator< Key, Value > BinarySearchTree< Key, Value, Cmp>::cend() const
  {
    return ConstIterator< Key, Value >(fake_);
  }

  template< class Key, class Value, class Cmp >
  Iterator< Key, Value > BinarySearchTree< Key, Value, Cmp >::find(const Key &k) noexcept
  {
    Node< Key, Value > *node = findNode(k);
    return Iterator< Key, Value >(node);
  }

  template< class Key, class Value, class Cmp >
  ConstIterator< Key, Value > BinarySearchTree< Key, Value, Cmp >::find(const Key &k, bool b) noexcept
  {
    Node< Key, Value > *node = findNode(k);
    return ConstIterator< Key, Value >(node);
  }

  template< class Key, class Value, class Cmp >
  size_t BinarySearchTree< Key, Value, Cmp >::count(const Key &k) const noexcept
  {
    if (find(k) != end())
    {
      return 1;
    }
    return 0;
  }

  template< class Key, class Value, class Cmp >
  range_t< Key, Value > BinarySearchTree< Key, Value, Cmp >::equal_range(const Key &k) noexcept
  {
    Iterator< Key, Value > iter = find(k);
    if (iter == end())
    {
      return std::make_pair(iter, iter);
    }
    return std::make_pair(iter, ++iter);
  }

  template< class Key, class Value, class Cmp >
  crange_t< Key, Value > BinarySearchTree< Key, Value, Cmp >::equal_range(const Key &k, bool b) noexcept
  {
    ConstIterator< Key, Value > c_iter = find(k);
    if (c_iter == cend())
    {
      return std::make_pair(c_iter, c_iter);
    }
    return std::make_pair(c_iter, ++c_iter);
  }

  template< class Key, class Value, class Cmp >
  size_t BinarySearchTree< Key, Value, Cmp >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Cmp >
  bool BinarySearchTree< Key, Value, Cmp >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Cmp >
  void BinarySearchTree< Key, Value, Cmp >::swap(BinarySearchTree< Key, Value, Cmp > &rhs) noexcept
  {
    std::swap(root_, rhs.root_);
    std::swap(fake_, rhs.fake_);
    std::swap(cmp_, rhs.cmp_);
    std::swap(size_, rhs.size_);
  }

  template< class Key, class Value, class Cmp >
  void BinarySearchTree< Key, Value, Cmp >::clearNodes(Node< Key, Value > *root) noexcept
  {
    if (root != fake_)
    {
      clearNodes(root->left_);
      clearNodes(root->rigth_);
      delete root;
    }
  }

  template< class Key, class Value, class Cmp >
  void BinarySearchTree< Key, Value, Cmp >::clear() noexcept
  {
    Node< Key, Value > *root = root_;
    clearNodes(root);
    delete fake_;
  }

  template< class Key, class Value, class Cmp >
  Node< Key, Value > *BinarySearchTree< Key, Value, Cmp >::getMin(Node< Key, Value > *root) noexcept
  {
    while (root->left_ != fake_)
    {
      root = root->left_;
    }
    return root;
  }

  template< class Key, class Value, class Cmp >
  Node< Key, Value > *BinarySearchTree< Key, Value, Cmp >::rotateLeft(Node< Key, Value > *root) noexcept
  {
    if (root == fake_ || !root || !root->right_)
    {
      return root;
    }
    Node< Key, Value > *new_root = root->right_;
    root->parent_ = new_root;
    new_root->parent_ = root->parent_;
    root->right_ = new_root->left_;
    new_root->left_ = root;
    return new_root;
  }

  template< class Key, class Value, class Cmp >
  Node< Key, Value > *BinarySearchTree< Key, Value, Cmp >::rotateRight(Node< Key, Value > *root) noexcept
  {
    if (root == fake_ || !root || !root->left_)
    {
      return root;
    }
    Node< Key, Value > *new_root = root->left_;
    root->parent_ = new_root;
    new_root->parent_ = root->parent_;
    root->left_ = new_root->right_;
    new_root->right = root;
    return new_root;
  }

  template< class Key, class Value, class Cmp >
  Node< Key, Value > *BinarySearchTree< Key, Value, Cmp >::findNode(const Key &k) noexcept
  {
    Node< Key, Value > *root = root_;
    while (root != fake_)
    {
      if (cmp(k, root->data_))
      {
        root = root->left;
      }
      else if (!cmp(k, root->data_))
      {
        root = root->right;
      }
      else
      {
        return root;
      }
    }
    return fake_;
  }
}
#endif
