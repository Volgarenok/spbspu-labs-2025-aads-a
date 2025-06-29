#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP
#include <queue>
#include <cstddef>
#include <algorithm>
#include <functional>
#include "node.hpp"
#include "cIterator.hpp"
#include "iterator.hpp"

namespace abramov
{
  template< class Key, class Value >
  struct Iterator;

  template< class Key, class Value >
  using range_t = std::pair< Iterator< Key, Value >, Iterator< Key, Value > >;

  template< class Key, class Value >
  using crange_t = std::pair< ConstIterator< Key, Value >, ConstIterator< Key, Value > >;


 template< class Key, class Value, class Cmp = std::less< Key > >
  struct BinarySearchTree
  {
    using node_t = Node< Key, Value >;
    using tree_t = BinarySearchTree< Key, Value, Cmp >;

    BinarySearchTree();
    BinarySearchTree(const BinarySearchTree< Key, Value, Cmp > &tree);
    BinarySearchTree(BinarySearchTree< Key, Value, Cmp > &&tree) noexcept;
    ~BinarySearchTree() noexcept;
    tree_t &operator=(const tree_t &tree);
    tree_t &operator=(tree_t &&tree) noexcept;
    Value &operator[](const Key &key) noexcept;
    void insert(const Key &key, const Value &value);
    Iterator< Key, Value > begin();
    Iterator< Key, Value > end();
    ConstIterator< Key, Value > cbegin() const;
    ConstIterator< Key, Value > cend() const;
    Iterator< Key, Value > find(const Key &k) noexcept;
    ConstIterator< Key, Value > cfind(const Key &k) const noexcept;
    size_t count(const Key &k) const noexcept;
    range_t< Key, Value > equal_range(const Key &k) noexcept;
    crange_t< Key, Value > cequal_range(const Key &k) const noexcept;
    void erase(Iterator< Key, Value > pos) noexcept;
    void erase(const Key &k) noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(BinarySearchTree &rhs) noexcept;
    void clear() noexcept;
    template< class F >
    F traverse_lnr(F f) const;
    template< class F >
    F traverse_lnr(F f);
    template< class F >
    F traverse_rnl(F f) const;
    template< class F >
    F traverse_rnl(F f);
    template< class F >
    F traverse_breadth(F f) const;
    template< class F >
    F traverse_breadth(F f);
  private:
    Node< Key, Value > *root_;
    Node< Key, Value > *fake_;
    Cmp cmp_;
    size_t size_;

    void clearNodes(Node< Key, Value > *root) noexcept;
    node_t *copyTree(node_t *node, node_t *parent, node_t *old_fake, node_t *fake);
    Node< Key, Value > *getMin(Node< Key, Value > *root) noexcept;
    const Node< Key, Value > *cgetMin(const Node< Key, Value > *root) const noexcept;
    Node< Key, Value > *findNode(const Key &k) noexcept;
    const Node< Key, Value > *cfindNode(const Key &k) const noexcept;
  };

  template< class Key, class Value, class Cmp >
  BinarySearchTree< Key, Value, Cmp >::BinarySearchTree():
    root_(nullptr),
    fake_(new Node< Key, Value >),
    cmp_(Cmp()),
    size_(0)
  {}

  template< class Key, class Value, class Cmp >
  BinarySearchTree< Key, Value, Cmp >::BinarySearchTree(const BinarySearchTree< Key, Value, Cmp > &tree):
    root_(nullptr),
    fake_(new Node< Key, Value >),
    cmp_(tree.cmp_),
    size_(tree.size_)
  {
    if (tree.root_ && tree.root_ != tree.fake_)
    {
      try
      {
        root_ = copyTree(tree.root_, nullptr, tree.fake_, fake_);
      }
      catch (const std::bad_alloc &)
      {
        delete fake_;
        throw;
      }
    }
    else
    {
      size_ = 0;
    }
  }

  template< class Key, class Value, class Cmp >
  typename BinarySearchTree< Key, Value, Cmp >::tree_t&
  BinarySearchTree< Key, Value, Cmp >::operator=(const tree_t &tree)
  {
    if (this != std::addressof(tree))
    {
      BinarySearchTree< Key, Value, Cmp > tmp(tree);
      swap(tmp);
    }
    return *this;
  }

  template< class Key, class Value, class Cmp >
  BinarySearchTree< Key, Value, Cmp >::BinarySearchTree(BinarySearchTree< Key, Value, Cmp > &&tree) noexcept:
    root_(tree.root_),
    fake_(tree.fake_),
    cmp_(std::move(tree.cmp_)),
    size_(tree.size_)
  {
    tree.root_ = nullptr;
    tree.fake_ = nullptr;
    tree.size_ = 0;
  }

  template< class Key, class Value, class Cmp >
  typename BinarySearchTree< Key, Value, Cmp >::tree_t&
  BinarySearchTree< Key, Value, Cmp >::operator=(tree_t &&tree) noexcept
  {
    if (this != std::addressof(tree))
    {
      clear();
      delete fake_;
      root_ = tree.root_;
      fake_ = tree.fake_;
      cmp_ = std::move(tree.cmp_);
      size_ = tree.size_;
      tree.root_ = nullptr;
      tree.root_ = nullptr;
      tree.size_ = 0;
    }
    return *this;
  }

  template< class Key, class Value, class Cmp >
  BinarySearchTree< Key, Value, Cmp >::~BinarySearchTree() noexcept
  {
    clear();
    delete fake_;
  }

  template< class Key, class Value, class Cmp >
  void BinarySearchTree< Key, Value, Cmp >::insert(const Key &key, const  Value &value)
  {
    Node< Key, Value > *node = new Node< Key, Value >(std::make_pair(key, value), nullptr, fake_, fake_);
    if (empty())
    {
      root_ = node;
      ++size_;
      return;
    }
    Node< Key, Value > *curr = root_;
    Node< Key, Value > *parent = nullptr;
    while (curr && curr != fake_)
    {
      parent = curr;
      if (cmp_(key, curr->data_.first))
      {
        curr = curr->left_;
      }
      else if (cmp_(curr->data_.first, key))
      {
        curr = curr->right_;
      }
    }
    node->parent_ = parent;
    if (cmp_(key, parent->data_.first))
    {
      parent->left_ = node;
    }
    else
    {
      parent->right_ = node;
    }
    ++size_;
  }

  template< class Key, class Value, class Cmp >
  Value &BinarySearchTree< Key, Value, Cmp >::operator[](const Key &key) noexcept
  {
    if (empty())
    {
      insert(key, Value());
    }
    else if (find(key) == end())
    {
      insert(key, Value());
    }
    return find(key)->second;
  }

  template< class Key, class Value, class Cmp >
  Iterator< Key, Value > BinarySearchTree< Key, Value, Cmp >::begin()
  {
    Node< Key, Value > *node = getMin(root_);
    return Iterator< Key, Value >(node, fake_, root_);
  }

  template< class Key, class Value, class Cmp >
  Iterator< Key, Value > BinarySearchTree< Key, Value, Cmp >::end()
  {
    return Iterator< Key, Value >(fake_, fake_, root_);
  }

  template< class Key, class Value, class Cmp >
  ConstIterator< Key, Value > BinarySearchTree< Key, Value, Cmp >::cbegin() const
  {
    const Node< Key, Value > *node = cgetMin(root_);
    return ConstIterator< Key, Value >(node, fake_, root_);
  }

  template< class Key, class Value, class Cmp >
  ConstIterator< Key, Value > BinarySearchTree< Key, Value, Cmp>::cend() const
  {
    return ConstIterator< Key, Value >(fake_, fake_, root_);
  }

  template< class Key, class Value, class Cmp >
  Iterator< Key, Value > BinarySearchTree< Key, Value, Cmp >::find(const Key &k) noexcept
  {
    Node< Key, Value > *node = findNode(k);
    return Iterator< Key, Value >(node, fake_, root_);
  }

  template< class Key, class Value, class Cmp >
  ConstIterator< Key, Value > BinarySearchTree< Key, Value, Cmp >::cfind(const Key &k) const noexcept
  {
    const Node< Key, Value > *node = cfindNode(k);
    return ConstIterator< Key, Value >(node, fake_, root_);
  }

  template< class Key, class Value, class Cmp >
  size_t BinarySearchTree< Key, Value, Cmp >::count(const Key &k) const noexcept
  {
    auto it = cend();
    if (cfind(k) != cend())
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
  crange_t< Key, Value > BinarySearchTree< Key, Value, Cmp >::cequal_range(const Key &k) const noexcept
  {
    ConstIterator< Key, Value > c_iter = cfind(k);
    if (c_iter == cend())
    {
      return std::make_pair(c_iter, c_iter);
    }
    return std::make_pair(c_iter, ++c_iter);
  }

  template< class Key, class Value, class Cmp >
  void BinarySearchTree< Key, Value, Cmp >::erase(Iterator< Key, Value > pos) noexcept
  {
    if (pos == end())
    {
      return;
    }
    Node< Key, Value > *node = pos.node_;
    Node< Key, Value > *parent = node->parent_;
    if (node->left_ == fake_ && node->right_ == fake_)
    {
      if (parent)
      {
        if (parent->left_ == node)
        {
          parent->left_ = fake_;
        }
        else
        {
          parent->right_ = fake_;
        }
      }
      else
      {
        root_ = nullptr;
      }
      delete node;
      --size_;
      return;
    }
    if (node->left_ == fake_ || node->right_ == fake_)
    {
      Node< Key, Value > *child = (node->left_ != fake_) ? node->left_ : node->right_;
      if (parent)
      {
        if (parent->left_ == node)
        {
          parent->left_ = child;
        }
        else
        {
          parent->right_ = child;
        }
        child->parent_ = parent;
      }
      else
      {
        root_ = child;
        child->parent_ = nullptr;
      }
      delete node;
      --size_;
      return;
    }
    Node< Key, Value > *successor = node->right_;
    while (successor->left_ != fake_)
    {
      successor = successor->left_;
    }
    auto data = successor->data_;
    erase(Iterator< Key, Value >(successor, fake_, root_));
    node->data_ = data;
  }

  template< class Key, class Value, class Cmp >
  void BinarySearchTree< Key, Value, Cmp >::erase(const Key &k) noexcept
  {
    Iterator< Key, Value > pos = find(k);
    erase(pos);
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
    if (root != fake_ && root)
    {
      clearNodes(root->left_);
      clearNodes(root->right_);
      delete root;
    }
  }

  template< class Key, class Value, class Cmp >
  void BinarySearchTree< Key, Value, Cmp >::clear() noexcept
  {
    clearNodes(root_);
    size_ = 0;
    root_ = nullptr;
  }

  template< class Key, class Value, class Cmp >
  typename BinarySearchTree< Key, Value, Cmp >::node_t*
  BinarySearchTree< Key, Value, Cmp >::copyTree(node_t *node, node_t *parent, node_t *old_fake, node_t *fake)
  {
    if (!node || node == old_fake)
    {
      return fake;
    }
    node_t *new_node = nullptr;
    try
    {
      new_node = new Node< Key, Value >(node->data_, parent, fake, fake);
      new_node->left_ = copyTree(node->left_, new_node, old_fake, fake);
      new_node->right_ = copyTree(node->right_, new_node, old_fake, fake);
    }
    catch (const std::bad_alloc &)
    {
      delete new_node;
      throw;
    }
    return new_node;
  }

  template< class Key, class Value, class Cmp >
  const Node< Key, Value > *BinarySearchTree< Key, Value, Cmp >::cgetMin(const Node< Key, Value > *root) const noexcept
  {
    if (empty())
    {
      return fake_;
    }
    while (root->left_ != fake_)
    {
      root = root->left_;
    }
    return root;
  }

  template< class Key, class Value, class Cmp >
  Node< Key, Value > *BinarySearchTree< Key, Value, Cmp >::getMin(Node< Key, Value > *root) noexcept
  {
    using Tree = BinarySearchTree< Key, Value, Cmp >;
    return const_cast< Node< Key, Value >* >(const_cast< Tree* >(this)->cgetMin(root));
  }

  template< class Key, class Value, class Cmp >
  const Node< Key, Value > *BinarySearchTree< Key, Value, Cmp >::cfindNode(const Key &k) const noexcept
  {
    const Node< Key, Value > *root = root_;
    while (root && root != fake_)
    {
      if (cmp_(k, root->data_.first))
      {
        root = root->left_;
      }
      else if (cmp_(root->data_.first, k))
      {
        root = root->right_;
      }
      else
      {
        return root;
      }
    }
    return fake_;
  }

  template< class Key, class Value, class Cmp >
  Node< Key, Value > *BinarySearchTree< Key, Value, Cmp >::findNode(const Key &k) noexcept
  {
    using Tree = BinarySearchTree< Key, Value, Cmp >;
    return const_cast< Node< Key, Value >* >(const_cast< Tree* >(this)->cfindNode(k));
  }

  template< class Key, class Value, class Cmp >
  template< class F >
  F BinarySearchTree< Key, Value, Cmp >::traverse_lnr(F f) const
  {
    if (empty())
    {
      throw std::logic_error("Empty tree\n");
    }
    for (auto it = cbegin(); it != cend(); ++it)
    {
      f(*it);
    }
    return f;
  }

  template< class Key, class Value, class Cmp >
  template< class F >
  F BinarySearchTree< Key, Value, Cmp >::traverse_lnr(F f)
  {
    return const_cast< const BinarySearchTree< Key, Value, Cmp >* >(this)->traverse_lnr(f);
  }

  template< class Key, class Value, class Cmp >
  template< class F >
  F BinarySearchTree< Key, Value, Cmp >::traverse_rnl(F f) const
  {
    if (empty())
    {
      throw std::logic_error("Empty tree\n");
    }
    for (auto it = cend(); it != cbegin();)
    {
      --it;
      f(*it);
    }
    return f;
  }

  template< class Key, class Value, class Cmp >
  template< class F >
  F BinarySearchTree< Key, Value, Cmp >::traverse_rnl(F f)
  {
    return const_cast< const BinarySearchTree< Key, Value, Cmp >* >(this)->traverse_rnl(f);
  }

  template< class Key, class Value, class Cmp >
  template< class F >
  F BinarySearchTree< Key, Value, Cmp >::traverse_breadth(F f) const
  {
    if (empty())
    {
      throw std::logic_error("Empty tree\n");
    }
    std::queue< const Node< Key, Value >* > q;
    q.push(root_);
    while (!q.empty())
    {
      const Node< Key, Value > *curr = q.front();
      q.pop();
      if (curr != fake_)
      {
        f(curr->data_);
        if (curr->left_ != fake_)
        {
          q.push(curr->left_);
        }
        if (curr->right_ != fake_)
        {
          q.push(curr->right_);
        }
      }
    }
    return f;
  }

  template< class Key, class Value, class Cmp >
  template< class F >
  F BinarySearchTree< Key, Value, Cmp >::traverse_breadth(F f)
  {
    return const_cast< const BinarySearchTree< Key, Value, Cmp >* >(this)->traverse_breadth(f);
  }
}
#endif
