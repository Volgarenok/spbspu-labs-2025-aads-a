#ifndef AVLTREE_HPP
#define AVLTREE_HPP
#include <utility>
#include <functional>
#include "treeNode.hpp"
#include "iterator.hpp"
#include "constIterator.hpp"
#include "stack.hpp"
#include "queue.hpp"

namespace finaev
{
  template< class Key, class Value, class Cmp = std::less< Key > >
  class AVLtree
  {
  public:
    using node_t = TreeNode< Key, Value >;
    using iter = TreeIterator< Key, Value, Cmp >;
    using cIter = ConstTreeIterator< Key, Value, Cmp >;

    AVLtree();
    AVLtree(const AVLtree& other);
    AVLtree(AVLtree&& other);
    AVLtree< Key, Value, Cmp >& operator=(const AVLtree& other);
    AVLtree< Key, Value, Cmp>& operator=(AVLtree&& other);

    iter begin() noexcept;
    iter end() noexcept;
    cIter cBegin() const noexcept;
    cIter cEnd() const noexcept;

    iter find(const Key&);
    cIter find(const Key&) const;
    void swap(AVLtree< Key, Value, Cmp >&) noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    iter erase(iter) noexcept;
    size_t erase(const Key&) noexcept;
    size_t count(const Key&) const noexcept;

    Value& at(const Key &);
    const Value& at(const Key &) const;
    std::pair< iter, bool > insert(const std::pair< Key, Value >&);
    Value& operator[](const Key&);

    template< class F >
    F traverse_lnr(F f) const;
    template< class F >
    F traverse_rnl(F f) const;
    template< class F >
    F traverse_breadth(F f) const;

    ~AVLtree();
  private:
    node_t* fakeroot_;
    node_t* root_;
    Cmp cmp_;
    size_t size_;

    node_t* copyTree(node_t* root);
    void clear(node_t* root) noexcept;
    void updateHeight(node_t* root) noexcept;
    node_t* rotateRight(node_t* root) noexcept;
    node_t* rotateLeft(node_t* root) noexcept;
    node_t* rotateRightLeft(node_t* root) noexcept;
    node_t* rotateLeftRight(node_t* root) noexcept;
    int getBalance(node_t* root) noexcept;
    node_t* balance(node_t* root) noexcept;
    std::pair< iter, bool > treeInsert(const Key&, const Value&);
    node_t* findMin(node_t* node) noexcept;
    node_t* findMax(node_t* node) noexcept;
    node_t* findSuccessor(node_t* node) noexcept;
    node_t* deleteNode(node_t* node, const Key&) noexcept;
  };

  template< class Key, class Value, class Cmp >
  AVLtree< Key, Value, Cmp >::AVLtree():
    fakeroot_(new node_t(Key(), Value(), nullptr)),
    root_(nullptr),
    cmp_(),
    size_(0)
  {
    fakeroot_->left = nullptr;
  }

  template< class Key, class Value, class Cmp >
  AVLtree< Key, Value, Cmp >::AVLtree(const AVLtree& other):
    fakeroot_(new node_t(Key(), Value(), nullptr)),
    cmp_(other.cmp_),
    size_(other.size_)
  {
    try
    {
      fakeroot_->left = copyTree(other.fakeroot_->left);
      root_ = fakeroot_->left;
      if (root_)
      {
        root_->parent = fakeroot_;
      }
    }
    catch(...)
    {
      delete fakeroot_;
      throw;
    }
  }

  template< class Key, class Value, class Cmp >
  AVLtree< Key, Value, Cmp >::AVLtree(AVLtree&& other):
    fakeroot_(other.fakeroot_),
    root_(other.root_),
    cmp_(other.cmp_),
    size_(other.size_)
  {
    other.fakeroot_ = new node_t(Key(), Value(), nullptr);
    other.fakeroot_->left = nullptr;
    other.root_ = nullptr;
    other.size_ = 0;
  }

  template< class Key, class Value, class Cmp >
  AVLtree< Key, Value, Cmp >& AVLtree< Key, Value, Cmp >::operator=(AVLtree&& other)
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    clear(fakeroot_->left);
    delete fakeroot_;
    fakeroot_ = other.fakeroot_;
    root_ = other.root_;
    cmp_ = std::move(other.cmp_);
    size_ = other.size_;
    other.fakeroot_ = new node_t(Key(), Value(), nullptr);
    other.fakeroot_->left = nullptr;
    other.root_ = nullptr;
    other.size_ = 0;
    return *this;
  }

  template< class Key, class Value, class Cmp >
  AVLtree< Key, Value, Cmp >& AVLtree< Key, Value, Cmp>::operator=(const AVLtree& other)
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    AVLtree temp = other;
    swap(temp);
    return *this;
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp >::iter AVLtree< Key, Value, Cmp >::begin() noexcept
  {
    if (fakeroot_->left)
    {
      node_t* temp = fakeroot_->left;
      while(temp->left)
      {
        temp = temp->left;
      }
      return iter(temp);
    }
    else
    {
      return end();
    }
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp >::iter AVLtree< Key, Value, Cmp >::end() noexcept
  {
    return iter(fakeroot_);
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp >::cIter AVLtree< Key, Value, Cmp >::cBegin() const noexcept
  {
    if (fakeroot_->left)
    {
      const node_t* temp = fakeroot_->left;
      while(temp->left)
      {
        temp = temp->left;
      }
      return cIter(temp);
    }
    else
    {
      return cEnd();
    }
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp >::cIter AVLtree< Key, Value, Cmp >::cEnd() const noexcept
  {
    return cIter(fakeroot_);
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp >::iter AVLtree< Key, Value, Cmp >::find(const Key& k)
  {
    node_t* current = fakeroot_->left;
    while (current)
    {
      if (cmp_(k, current->data.first))
      {
        current = current->left;
      }
      else if (cmp_(current->data.first, k))
      {
        current = current->right;
      }
      else
      {
        return iter(current);
      }
    }
    return end();
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp >::cIter AVLtree< Key, Value, Cmp >::find(const Key& k) const
  {
    const node_t* current = fakeroot_->left;
    while (current)
    {
      if (cmp_(k, current->data.first))
      {
        current = current->left;
      }
      else if (cmp_(current->data.first, k))
      {
        current = current->right;
      }
      else
      {
        return cIter(current);
      }
    }
    return cEnd();
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp >::node_t* AVLtree< Key, Value, Cmp >::copyTree(node_t* root)
  {
    if (!root)
    {
      return nullptr;
    }
    node_t* leftPart = nullptr;
    node_t* rightPart = nullptr;
    try
    {
      leftPart = copyTree(root->left);
      rightPart = copyTree(root->right);
      node_t* newNode = new node_t(root->data.first, root->data.second, nullptr);
      newNode->left = leftPart;
      newNode->right = rightPart;
      newNode->height = root->height;
      if (leftPart)
      {
        leftPart->parent = newNode;
      }
      if (rightPart)
      {
        rightPart->parent = newNode;
      }
      return newNode;
    }
    catch (...)
    {
      clear(leftPart);
      clear(rightPart);
      throw;
    }
  }

  template< class Key, class Value, class Cmp >
  void AVLtree< Key, Value, Cmp >::updateHeight(node_t* root) noexcept
  {
    if (!root)
    {
      return;
    }
    int leftHeight = root->left ? root->left->height : 0;
    int rightHeight = root->right ? root->right->height : 0;
    root->height = 1 + std::max(leftHeight, rightHeight);
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp >::node_t* AVLtree< Key, Value, Cmp >::rotateRight(node_t* root) noexcept
  {
    if (!root || !root->left)
    {
      return root;
    }
    node_t* newRoot = root->left;
    root->left = newRoot->right;
    if (newRoot->right)
    {
      newRoot->right->parent = root;
    }
    newRoot->parent = root->parent;
    if (root == fakeroot_->left)
    {
      fakeroot_->left = newRoot;
      root_ = newRoot;
    }
    else if (root->parent)
    {
      if (root == root->parent->left)
      {
        root->parent->left = newRoot;
      }
      else
      {
        root->parent->right = newRoot;
      }
    }
    newRoot->right = root;
    root->parent = newRoot;
    updateHeight(root);
    updateHeight(newRoot);
    return newRoot;
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp >::node_t* AVLtree< Key, Value, Cmp >::rotateLeft(node_t* root) noexcept
  {
    if (!root || !root->right)
    {
      return root;
    }
    node_t* newRoot = root->right;
    root->right = newRoot->left;
    if (newRoot->left)
    {
      newRoot->left->parent = root;
    }
    newRoot->parent = root->parent;
    if (root == fakeroot_->left)
    {
      fakeroot_->left = newRoot;
      root_ = newRoot;
    }
    else if (root->parent)
    {
      if (root == root->parent->left)
      {
        root->parent->left = newRoot;
      }
      else
      {
        root->parent->right = newRoot;
      }
    }
    newRoot->left = root;
    root->parent = newRoot;
    updateHeight(root);
    updateHeight(newRoot);
    return newRoot;
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp >::node_t* AVLtree< Key, Value, Cmp >::rotateRightLeft(node_t* root) noexcept
  {
    if (!root || !root->right)
    {
      return root;
    }
    root->right = rotateRight(root->right);
    if (root->right)
    {
      root->right->parent = root;
    }
    return rotateLeft(root);
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp >::node_t* AVLtree< Key, Value, Cmp >::rotateLeftRight(node_t* root) noexcept
  {
    if (!root || !root->left)
    {
      return root;
    }
    root->left = rotateLeft(root->left);
    if (root->left)
    {
      root->left->parent = root;
    }
    return rotateRight(root);
  }

  template< class Key, class Value, class Cmp >
  int AVLtree< Key, Value, Cmp >::getBalance(node_t* root) noexcept
  {
    if (!root)
    {
      return 0;
    }
    int leftH = root->left ? root->left->height : 0;
    int rightH = root->right ? root->right->height : 0;
    return leftH - rightH;
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp>::node_t* AVLtree< Key, Value, Cmp >::balance(node_t* root) noexcept
  {
    if (!root || root == fakeroot_)
    {
      return root;
    }
    root->left = balance(root->left);
    root->right = balance(root->right);
    updateHeight(root);
    int balance = getBalance(root);

    if (balance > 1)
    {
      if (root->left && getBalance(root->left) >= 0)
      {
        return rotateRight(root);
      }
      else if (root->left)
      {
        return rotateLeftRight(root);
      }
    }
    else if (balance < -1)
    {
      if (root->right && getBalance(root->right) <= 0)
      {
        return rotateLeft(root);
      }
      else if (root->right)
      {
        return rotateRightLeft(root);
      }
    }
    return root;
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp >::node_t* AVLtree< Key, Value, Cmp >::findMin(node_t* node) noexcept
  {
    if (!node)
    {
      return nullptr;
    }
    while(node->left)
    {
      node = node->left;
    }
    return node;
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp >::node_t* AVLtree< Key, Value, Cmp >::findMax(node_t* node) noexcept
  {
    if (!node)
    {
      return nullptr;
    }
    while(node->right)
    {
      node = node->right;
    }
    return node;
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp >::node_t* AVLtree< Key, Value, Cmp >::findSuccessor(node_t* node) noexcept
  {
    if (!node)
    {
      return nullptr;
    }
    if (node->right)
    {
      return findMin(node->right);
    }
    while (node->parent)
    {
      if (node == node->parent->left)
      {
        return node->parent;
      }
      node = node->parent;
    }
    return nullptr;
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp >::node_t* AVLtree< Key, Value, Cmp >::deleteNode(node_t* node, const Key& key) noexcept
  {
    if (!node)
    {
      return nullptr;
    }
    if (cmp_(key, node->data.first))
    {
      node->left = deleteNode(node->left, key);
    }
    else if (cmp_(node->data.first, key))
    {
      node->right = deleteNode(node->right, key);
    }
    else
    {
      if (!node->left || !node->right)
      {
        node_t* temp = node->left ? node->left : node->right;
        if (!temp)
        {
          temp = node;
          node = nullptr;
        }
        else
        {
          *node = *temp;
        }
        delete temp;
      }
      else
      {
        node_t* temp = findMin(node->right);
        node->data = temp->data;
        node->right = deleteNode(node->right, temp->data.first);
      }
    }
    if (!node)
    {
      return nullptr;
    }
    return balance(node);
  }

  template< class Key, class Value, class Cmp >
  std::pair< typename AVLtree< Key, Value, Cmp >::iter, bool > AVLtree< Key, Value, Cmp >::treeInsert(const Key& k, const Value& v)
  {
    if (empty())
    {
      fakeroot_->left = new node_t{ k, v, fakeroot_ };
      root_ = fakeroot_->left;
      ++size_;
      return { iter(root_), true };
    }
    node_t* current = fakeroot_->left;
    node_t* parent = fakeroot_;
    while (current)
    {
      if (cmp_(k, current->data.first))
      {
        parent = current;
        current = current->left;
      }
      else if (cmp_(current->data.first, k))
      {
        parent = current;
        current = current->right;
      }
      else
      {
        return { iter(current), false };
      }
    }
    node_t* newNode = new node_t(k, v, parent);
    if (cmp_(k, parent->data.first))
    {
      parent->left = newNode;
    }
    else
    {
      parent->right = newNode;
    }
    ++size_;
    balance(fakeroot_->left);
    return { iter(newNode), true };
  }

  template< class Key, class Value, class Cmp >
  std::pair< typename AVLtree< Key, Value, Cmp >::iter, bool > AVLtree< Key, Value, Cmp >::insert(const std::pair< Key, Value >& elem)
  {
    return treeInsert(elem.first, elem.second);
  }

  template< class Key, class Value, class Cmp >
  Value& AVLtree< Key, Value, Cmp >::at(const Key& k)
  {
    iter i = find(k);
    if (i == end())
    {
      throw std::out_of_range("KEY NOT FOUND");
    }
    return (*i).second;
  }

  template< class Key, class Value, class Cmp >
  const Value& AVLtree< Key, Value, Cmp >::at(const Key& k) const
  {
    cIter i = find(k);
    if (i == cEnd())
    {
      throw std::out_of_range("KEY NOT FOUND");
    }
    return (*i).second;
  }

  template< class Key, class Value, class Cmp >
  Value& AVLtree< Key, Value, Cmp >::operator[](const Key& k)
  {
    iter i = find(k);
    if (i == end())
    {
      insert(std::make_pair(k, Value()));
      i = find(k);
    }
    return (*i).second;
  }

  template< class Key, class Value, class Cmp >
  void AVLtree< Key, Value, Cmp >::swap(AVLtree< Key, Value, Cmp >& other) noexcept
  {
    std::swap(fakeroot_, other.fakeroot_);
    std::swap(root_, other.root_);
    std::swap(cmp_, other.cmp_);
    std::swap(size_, other.size_);
  }

  template< class Key, class Value, class Cmp >
  bool AVLtree< Key, Value, Cmp >::empty() const noexcept
  {
    return !fakeroot_->left;
  }

  template< class Key, class Value, class Cmp >
  size_t AVLtree< Key, Value, Cmp >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Cmp >
  typename AVLtree< Key, Value, Cmp >::iter AVLtree< Key, Value, Cmp >::erase(iter it) noexcept
  {
    if (it == end())
    {
      return end();
    }
    node_t* node = it.node_;
    node_t* next = findSuccessor(node);
    fakeroot_->left = deleteNode(root_, node->data.first);
    if (fakeroot_->left)
    {
      fakeroot_->left->parent = fakeroot_;
    }
    --size_;
    if (next != nullptr)
    {
      return iter(next);
    }
    else
    {
      return end();
    }
  }

  template< class Key, class Value, class Cmp >
  size_t AVLtree< Key, Value, Cmp >::erase(const Key& key) noexcept
  {
    iter it = find(key);
    if (it != end())
    {
      erase(it);
      return 1;
    }
    return 0;
  }

  template<class Key, class Value, class Cmp>
  size_t AVLtree<Key, Value, Cmp>::count(const Key& key) const noexcept
  {
    return find(key) != cEnd() ? 1 : 0;
  }

  template< class Key, class Value, class Cmp >
  template< class F >
  F AVLtree< Key, Value, Cmp >::traverse_lnr(F f) const
  {
    if (empty())
    {
      throw std::logic_error("<EMPTY>");
    }
    Stack< node_t* > stack;
    node_t* current = fakeroot_->left;
    while (!stack.isEmpty() || current)
    {
      while (current)
      {
        stack.push(current);
        current = current->left;
      }
      current = stack.top();
      stack.pop();
      f(current->data);
      current = current->right;
    }
    return f;
  }

  template< class Key, class Value, class Cmp >
  template< class F >
  F AVLtree< Key, Value, Cmp >::traverse_rnl(F f) const
  {
    if (empty())
    {
      throw std::logic_error("<EMPTY>");
    }
    Stack< node_t* > stack;
    node_t* current = fakeroot_->left;
    while(!stack.isEmpty() || current)
    {
      while(current)
      {
        stack.push(current);
        current = current->right;
      }
      current = stack.top();
      stack.pop();
      f(current->data);
      current = current->left;
    }
    return f;
  }

  template< class Key, class Value, class Cmp >
  template< class F >
  F AVLtree< Key, Value, Cmp >::traverse_breadth(F f) const
  {
    if (empty())
    {
      throw std::logic_error("<EMPTY>");
    }
    Queue< node_t* > queue;
    queue.push(fakeroot_->left);
    while(!queue.isEmpty())
    {
      node_t* current = queue.top();
      queue.pop();
      f(current->data);
      if (current->left)
      {
        queue.push(current->left);
      }
      if (current->right)
      {
        queue.push(current->right);
      }
    }
    return f;
  }

  template< class Key, class Value, class Cmp >
  void AVLtree< Key, Value, Cmp >::clear(node_t* root) noexcept
  {
    if (root)
    {
      clear(root->left);
      clear(root->right);
      delete root;
    }
  }

  template< class Key, class Value, class Cmp >
  AVLtree< Key, Value, Cmp >::~AVLtree()
  {
    clear(fakeroot_->left);
    delete fakeroot_;
  }
}

#endif
