#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <utility>

#include "iterator.hpp"
#include "constIterator.hpp"

namespace dribas
{
  template< class Key, class T, class Compare > class Iterator;
  template< class Key, class T, class Compare > class ConstIterator;

  template< class Key, class T >
  struct Node
  {
    std::pair< Key, T > value;
    Node< Key, T >* left;
    Node< Key, T >* right;
    Node< Key, T >* parent;
    int height;
    bool isFake;
    Node(const std::pair< Key, T >&, Node< Key, T >*);
    Node();
    template< class... Args >
    Node(Node< Key, T >* fakeleaf, Args&&... args);
    ~Node() = default;
  };

  template< class Key, class T >
  template< class... Args >
  Node< Key, T >::Node(Node< Key, T >* fakeleaf, Args&&... args):
    value(std::forward< Args >(args)...),
    left(fakeleaf),
    right(fakeleaf),
    parent(nullptr),
    height(1),
    isFake(false)
  {
    left->parent = this;
    right->parent = this;
  }

  template< class Key, class T >
  Node< Key, T >::Node():
    left(nullptr),
    right(nullptr),
    parent(nullptr),
    height(0),
    isFake(true)
  {}

  template< class Key, class T >
  Node< Key, T >::Node(const std::pair< Key, T >& val, Node< Key, T >* fakeleaf):
    value(val),
    left(fakeleaf),
    right(fakeleaf),
    parent(nullptr),
    height(1),
    isFake(false)
  {
    left->parent = this;
    right->parent = this;
  }

  template< class Key, class T, class Compare = std::less< Key > >
  class AVLTree
  {
    friend class Iterator< Key, T, Compare >;
    friend class ConstIterator< Key, T, Compare >;
    using iterator = Iterator< Key, T, Compare >;
    using const_iterator = ConstIterator< Key, T, Compare >;
    using TreeType = AVLTree< Key, T, Compare >;
    using NodeType = Node< Key, T >;
  public:
    AVLTree();
    AVLTree(const TreeType&);
    AVLTree(TreeType&&) noexcept;
    template< typename InputIt >
    AVLTree(InputIt, InputIt);
    explicit AVLTree(std::initializer_list< std::pair< Key, T > >);
    ~AVLTree();

    TreeType& operator=(const TreeType&);
    TreeType& operator=(TreeType&&) noexcept;
    T& at(const Key&);
    const T& at(const Key&) const;
    T& operator[](const Key&);

    std::pair< iterator, bool > insert(const std::pair< Key, T >& val);
    std::pair< iterator, bool > insert(std::pair< Key, T >&& val);
    template< class InputIterator >
    void insert(InputIterator first, InputIterator last);
    void insert (std::initializer_list< std::pair< Key, T > > il);
    iterator  erase (const_iterator position);
    size_t erase (const Key& k);
    iterator  erase (const_iterator first, const_iterator last);

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    template< class... Args >
    std::pair< iterator, bool > emplace(Args&&... args);
    template< class... Args >
    iterator emplace_hint(const_iterator hint, Args&&... args);

    std::pair< const_iterator, const_iterator > equal_range (const Key& k)const;
    std::pair< iterator, iterator > equal_range (const Key& k);
    iterator upper_bound (const Key& k);
    const_iterator upper_bound (const Key& k) const;
    iterator lower_bound(const Key& key);
    const_iterator lower_bound(const Key& key) const;

    void swap(AVLTree&) noexcept;
    void clear() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    iterator find(const Key&);
    const_iterator find(const Key&) const;
    size_t count(const Key& k) const;

  private:
    NodeType* fakeleaf_;
    NodeType* root_;
    Compare cmp_;
    size_t size_;
    void balanceTree(NodeType*);
    NodeType* balance(NodeType*) noexcept;
    void updateHeight(NodeType*) noexcept;
    int getBalanceFactor(NodeType*) const noexcept;
    NodeType* rightRotate(NodeType*) noexcept;
    NodeType* leftRotate(NodeType*) noexcept;
    NodeType* findNode(const Key&) const;
    void clearSubtree(NodeType*) noexcept;
    NodeType* removeNode(NodeType*, const Key&);
    NodeType* findMin(NodeType*);
  };

  template< class Key, class T, class Compare >
  template< class... Args >
  Iterator< Key, T, Compare > AVLTree< Key, T, Compare >::emplace_hint(const_iterator hint, Args&&... args)
  {
    NodeType* newNode = new NodeType(std::forward< Args >(args)...);
    newNode->left = fakeleaf_;
    newNode->right = fakeleaf_;
    newNode->parent = nullptr;
    newNode->height = 1;
    if (root_ == fakeleaf_) {
      root_ = newNode;
      ++size_;
      return iterator(newNode, this);
    }
    if (hint != cend()) {
      const Key& newKey = newNode->value.first;
      const_iterator next = hint;
      ++next;
      bool first = hint == cbegin() || !cmp_(newKey, (--const_iterator(hint))->first);
      bool second = next == cend() || !cmp_(next->first, newKey);
      if (first && second) {
        NodeType* hintNode = const_cast< NodeType* >(hint.node_);
        if (cmp_(newKey, hintNode->value.first)) {
          if (hintNode->left == fakeleaf_) {
            hintNode->left = newNode;
            newNode->parent = hintNode;
            ++size_;
            balanceTree(hintNode);
            return iterator(newNode, this);
          }
        } else if (cmp_(hintNode->value.first, newKey)) {
          if (hintNode->right == fakeleaf_) {
            hintNode->right = newNode;
            newNode->parent = hintNode;
            ++size_;
            balanceTree(hintNode);
            return iterator(newNode, this);
          }
        } else {
          delete newNode;
          return iterator(hintNode, this);
        }
      }
    }
    NodeType* current = root_;
    NodeType* parent = nullptr;
    while (current != fakeleaf_) {
      parent = current;
      if (cmp_(newNode->value.first, current->value.first)) {
        current = current->left;
      } else if (cmp_(current->value.first, newNode->value.first)) {
        current = current->right;
      } else {
        delete newNode;
        return iterator(current, this);
      }
    }
    newNode->parent = parent;
    if (cmp_(newNode->value.first, parent->value.first)) {
      parent->left = newNode;
    } else {
      parent->right = newNode;
    }
    ++size_;
    balanceTree(newNode);
    return iterator(newNode, this);
  }

  template< class Key, class T, class Compare >
  std::pair< Iterator< Key, T, Compare >, Iterator< Key, T, Compare > >
  AVLTree< Key, T, Compare >::equal_range(const Key& k)
  {
    iterator lower = lower_bound(k);
    iterator upper = upper_bound(k);
    return std::make_pair(lower, upper);
  }

  template< class Key, class T, class Compare >
  std::pair< ConstIterator< Key, T, Compare >, ConstIterator< Key, T, Compare > >
  AVLTree< Key, T, Compare >::equal_range(const Key& k) const
  {
    const_iterator lower = lower_bound(k);
    const_iterator upper = upper_bound(k);
    return std::make_pair(lower, upper);
  }

  template< class Key, class T, class Compare >
  Iterator< Key, T, Compare > AVLTree< Key, T, Compare >::lower_bound(const Key& k)
  {
    NodeType* current = root_;
    NodeType* result = fakeleaf_;
    while (current != fakeleaf_) {
      if (!cmp_(current->value.first, k)) {
        result = current;
        current = current->left;
      }
      else {
        current = current->right;
      }
    }
    return iterator(result, this);
  }

  template< class Key, class T, class Compare >
  ConstIterator< Key, T, Compare > AVLTree< Key, T, Compare >::lower_bound(const Key& k) const
  {
    NodeType* current = root_;
    NodeType* result = fakeleaf_;
    while (current != fakeleaf_)
    {
      if (!cmp_(current->value.first, k)) {
        result = current;
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return const_iterator(result, this);
  }

  template< class Key, class T, class Compare >
  Iterator< Key, T, Compare > AVLTree< Key, T, Compare >::upper_bound(const Key& k)
  {
    NodeType* current = root_;
    NodeType* result = fakeleaf_;
    while (current != fakeleaf_)
    {
      if (cmp_(k, current->value.first)) {
        result = current;
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return iterator(result, this);
  }

  template< class Key, class T, class Compare >
  ConstIterator< Key, T, Compare > AVLTree< Key, T, Compare >::upper_bound(const Key& k) const
  {
    NodeType* current = root_;
    NodeType* result = fakeleaf_;
    while (current != fakeleaf_) {
      if (cmp_(k, current->value.first)) {
        result = current;
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return const_iterator(result, this);
  }

  template< class Key, class T, class Compare >
  size_t AVLTree< Key, T, Compare >::count(const Key& key) const
  {
    return find(key) != end();
  }

  template< class Key, class T, class Compare >
  ConstIterator< Key, T, Compare > AVLTree< Key, T, Compare >::cend() const noexcept
  {
    return end();
  }

  template< class Key, class T, class Compare >
  ConstIterator< Key, T, Compare > AVLTree< Key, T, Compare >::cbegin() const noexcept
  {
    return begin();
  }

  template< class Key, class T, class Compare >
  Iterator< Key, T, Compare > AVLTree< Key, T, Compare >::erase(const_iterator position)
  {
    if (position == cend() || position.node_ == fakeleaf_) {
      return end();
    }
    NodeType* nodeToDelete = const_cast< NodeType* >(position.node_);
    iterator result(nodeToDelete, this);
    ++result;
    NodeType* nodeToBalance = nullptr;

    if (nodeToDelete->left == fakeleaf_ && nodeToDelete->right == fakeleaf_) {
      nodeToBalance = nodeToDelete->parent;
      if (nodeToBalance) {
        if (nodeToBalance->left == nodeToDelete) {
          nodeToBalance->left = fakeleaf_;
        } else {
          nodeToBalance->right = fakeleaf_;
        }
      } else {
        root_ = fakeleaf_;
      }
      delete nodeToDelete;
      --size_;
    } else if (nodeToDelete->left == fakeleaf_ || nodeToDelete->right == fakeleaf_) {
      NodeType* child = (nodeToDelete->left != fakeleaf_) ? nodeToDelete->left : nodeToDelete->right;
      nodeToBalance = nodeToDelete->parent;
      if (nodeToBalance) {
        if (nodeToBalance->left == nodeToDelete) {
          nodeToBalance->left = child;
        } else {
          nodeToBalance->right = child;
        }
      } else {
        root_ = child;
      }
      child->parent = nodeToBalance;
      delete nodeToDelete;
      --size_;
    } else {
      NodeType* successor = nodeToDelete->right;
      while (successor->left != fakeleaf_) {
        successor = successor->left;
      }
      std::swap(nodeToDelete->value, successor->value);

      nodeToBalance = successor->parent;
      if (nodeToBalance->left == successor) {
        nodeToBalance->left = successor->right;
      } else {
        nodeToBalance->right = successor->right;
      }
      if (successor->right != fakeleaf_) {
        successor->right->parent = nodeToBalance;
      }
      delete successor;
      --size_;
    }

    if (nodeToBalance) {
      balanceTree(nodeToBalance);
    }

    return result;
  }

  template< class Key, class T, class Compare >
  size_t AVLTree< Key, T, Compare >::erase(const Key& k)
  {
    const_iterator it = find(k);
    if (it == cend()) {
      return 0;
    }
    erase(it);
    return 1;
  }

  template< class Key, class T, class Compare >
  Iterator< Key, T, Compare > AVLTree< Key, T, Compare >::erase(const_iterator first, const_iterator last) {
    while (first != last) {
      first = erase(first);
    }
    return iterator(const_cast<NodeType*>(first.node_), this);
  }

  template< class Key, class T, class Compare >
  AVLTree< Key, T, Compare >::AVLTree(std::initializer_list< std::pair< Key, T > > list):
    AVLTree()
  {
    for (auto it = list.begin(); it != list.end(); it++)
    {
      insert(*it);
    }
  }

  template< class Key, class T, class Compare >
  template< typename InputIt >
  AVLTree< Key, T, Compare >::AVLTree(InputIt first, InputIt last):
    AVLTree()
  {
    for (auto it = first; it != last; it++)
    {
      insert(*it);
    }
  }

  template< class Key, class T, class Compare >
  void AVLTree< Key, T, Compare >::swap(TreeType& other) noexcept
  {
    std::swap(fakeleaf_, other.fakeleaf_);
    std::swap(root_, other.root_);
    std::swap(cmp_, other.cmp_);
    std::swap(size_, other.size_);
  }

  template< class Key, class T, class Compare >
  AVLTree< Key, T, Compare >::AVLTree(const TreeType& other):
    fakeleaf_(new NodeType()),
    root_(fakeleaf_),
    cmp_(other.cmp_),
    size_(0)
  {
    for (const auto& pair: other) {
      this->insert(pair);
    }
  }

  template< class Key, class T, class Compare >
  AVLTree< Key, T, Compare >::AVLTree(TreeType&& other) noexcept:
    fakeleaf_(std::exchange(other.fakeleaf_, nullptr)),
    root_(std::exchange(other.root_, nullptr)),
    cmp_(std::move(other.cmp_)),
    size_(std::exchange(other.size_, 0))
  {}

  template< class Key, class T, class Compare >
  AVLTree< Key, T, Compare >& AVLTree< Key, T, Compare >::operator=(const TreeType& other)
  {
    if (this != std::addressof(other)) {
      AVLTree temp(other);
      swap(temp);
    }
    return *this;
  }

  template< class Key, class T, class Compare >
  AVLTree< Key, T, Compare >& AVLTree< Key, T, Compare >::operator=(TreeType&& other) noexcept
  {
    if (this != std::addressof(other)) {
      AVLTree temp(std::move(other));
      swap(temp);
    }
    return *this;
  }


  template< class Key, class T, class Compare >
  Iterator< Key, T, Compare > AVLTree< Key, T, Compare >::find(const Key& key)
  {
    NodeType* node = findNode(key);
    if (node != fakeleaf_) {
      return iterator( node, this );
    } else {
      return end();
    }
  }

  template< class Key, class T, class Compare >
  ConstIterator< Key, T, Compare > AVLTree< Key, T, Compare >::find(const Key& key) const
  {
    NodeType* node = findNode(key);
    if (node != fakeleaf_) {
      return const_iterator{ node, this };
    } else {
      return end();
    }
  }

  template< class Key, class T, class Compare >
  Node< Key, T >* AVLTree< Key, T, Compare >::findNode(const Key& key) const
  {
    NodeType* current = root_;
    while (current != fakeleaf_) {
      if (cmp_(key, current->value.first)) {
        current = current->left;
      } else if (cmp_(current->value.first, key)) {
        current = current->right;
      } else {
        return current;
      }
    }
    return fakeleaf_;
  }

  template< class Key, class T, class Cmp >
  T& AVLTree< Key, T, Cmp >::operator[](const Key& key)
  {
    auto result = insert(std::make_pair(key, T()));
    return result.first->second;
  }

  template< class Key, class T, class Cmp >
  const T& AVLTree< Key, T, Cmp >::at(const Key& key) const
  {
    NodeType* node = findNode(key);
    if (node == fakeleaf_) {
      throw std::out_of_range("Key not found in AVLTree");
    }
    return node->value.second;
  }

  template< class Key, class T, class Cmp >
  T& AVLTree< Key, T, Cmp >::at(const Key& key)
  {
    NodeType* node = findNode(key);
    if (node == fakeleaf_) {
      throw std::out_of_range("Key not found in AVLTree");
    }
    return node->value.second;
  }

  template< class Key, class T, class Cmp >
  void AVLTree< Key, T, Cmp >::insert(std::initializer_list< std::pair< Key, T > > il)
  {
    insert(il.begin(), il.end());
  }

  template< class Key, class T, class Cmp >
  template< class InputIterator >
  void AVLTree< Key, T, Cmp >::insert(InputIterator first, InputIterator last)
  {
    for (; first != last; ++first) {
      insert(*first);
    }
  }

  template< class Key, class T, class Cmp >
  std::pair< Iterator< Key, T, Cmp >, bool > AVLTree< Key, T, Cmp >::insert(std::pair< Key, T >&& value)
  {
    return emplace(std::move(value.first), std::move(value.second));
  }

  template< class Key, class T, class Cmp >
  std::pair< Iterator< Key, T, Cmp >, bool > AVLTree< Key, T, Cmp >::insert(const std::pair< Key, T >& value)
  {
    NodeType* current = root_;
    NodeType* parent = nullptr;
    while (current != fakeleaf_) {
      parent = current;
      if (value.first == current->value.first) {
        return {iterator(current, this), false};
      }
      if (cmp_(value.first, current->value.first)) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    NodeType* newNode = new NodeType(value, fakeleaf_);
    newNode->parent = parent;
    if (parent == nullptr) {
      root_ = newNode;
    } else if (cmp_(value.first, parent->value.first)) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    size_++;
    balanceTree(newNode);
    return {iterator(newNode, this), true};
}

  template< class Key, class T, class Cmp >
  bool AVLTree< Key, T, Cmp >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class T, class Cmp >
  size_t AVLTree< Key, T, Cmp >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class T, class Cmp >
  ConstIterator< Key, T, Cmp > AVLTree< Key, T, Cmp >::end() const noexcept
  {
    return const_iterator{ fakeleaf_, this};
  }

  template< class Key, class T, class Cmp >
  Iterator< Key, T, Cmp > AVLTree< Key, T, Cmp >::end() noexcept
  {
    return iterator{ fakeleaf_, this };
  }

  template< class Key, class T, class Cmp >
  ConstIterator< Key, T, Cmp > AVLTree< Key, T, Cmp >::begin() const noexcept
  {
    auto current = root_;
    if (current == fakeleaf_) {
      return end();
    }
    while (current->left != fakeleaf_) {
      current = current->left;
    }
    return const_iterator{ current, this };
  }

  template< class Key, class T, class Cmp >
  Iterator< Key, T, Cmp > AVLTree< Key, T, Cmp >::begin() noexcept
  {
    auto current = root_;
    if (current == fakeleaf_) {
      return end();
    }
    while (current->left != fakeleaf_) {
      current = current->left;
    }
    return iterator{ current, this };
  }

  template< class Key, class T, class Cmp >
  void AVLTree< Key, T, Cmp >::clearSubtree(NodeType* node) noexcept
  {
    if (node != fakeleaf_) {
      clearSubtree(node->left);
      clearSubtree(node->right);
      delete node;
    }
  }

  template< class Key, class T, class Cmp >
  void AVLTree< Key, T, Cmp >::clear() noexcept
  {
    clearSubtree(root_);
    root_ = fakeleaf_;
    size_ = 0;
  }

  template< class Key, class T, class Cmp >
  AVLTree< Key, T, Cmp >::~AVLTree()
  {
    clearSubtree(root_);
    delete fakeleaf_;
  }

  template< class Key, class T, class Cmp >
  AVLTree< Key, T, Cmp >::AVLTree():
    fakeleaf_(new NodeType()),
    root_(fakeleaf_),
    cmp_(),
    size_(0)
  {
    root_->left = fakeleaf_;
    root_->right = fakeleaf_;
    root_->isFake = true;
    fakeleaf_->parent = root_;
    root_->height = 0;
  }

  template< class Key, class T, class Cmp >
  template< class... Args >
  std::pair< Iterator< Key, T, Cmp >, bool > AVLTree< Key, T, Cmp >::emplace(Args&&... args)
  {
    NodeType* newNode = new NodeType(fakeleaf_, std::forward< Args >(args)...);
    const Key& key = newNode->value.first;
    newNode->right = fakeleaf_;
    newNode->left = fakeleaf_;
    if (root_ == fakeleaf_) {
      root_ = newNode;
      size_++;
      return { iterator(root_, this), true };
    }
    NodeType* current = root_;
    NodeType* parent = nullptr;
    while (current != fakeleaf_) {
      parent = current;
      if (cmp_(key, current->value.first)) {
        current = current->left;
      } else if (cmp_(current->value.first, key)) {
        current = current->right;
      } else {
        delete newNode;
        return { iterator(current, this), false };
      }
    }
    newNode->parent = parent;
    if (cmp_(key, parent->value.first)) {
      parent->left = newNode;
    } else {
      parent->right = newNode;
    }
    balanceTree(newNode);
    size_++;
    return { iterator(newNode, this), true };
  }

  template< class Key, class T, class Cmp >
  void AVLTree< Key, T, Cmp >::updateHeight(NodeType* node) noexcept
  {
    if (node != fakeleaf_) {
      node->height = std::max(node->left->height, node->right->height) + 1;
    }
  }

  template< class Key, class T, class Cmp >
  int AVLTree< Key, T, Cmp >::getBalanceFactor(NodeType* node) const noexcept
  {
    if (node == fakeleaf_ || node == nullptr) {
      return 0;
    }
    return node->left->height - node->right->height;
  }

  template< class Key, class T, class Cmp >
  Node< Key, T >* AVLTree< Key, T, Cmp >::rightRotate(NodeType* node) noexcept
  {
    NodeType* leftNode = node->left;
    node->left = leftNode->right;
    if (leftNode->right != fakeleaf_) {
      leftNode->right->parent = node;
    }

    leftNode->parent = node->parent;
    if (node->parent == nullptr) {
      root_ = leftNode;
    } else if (node->parent->left == node) {
      node->parent->left = leftNode;
    } else {
      node->parent->right = leftNode;
    }
    leftNode->right = node;
    node->parent = leftNode;

    updateHeight(node);
    updateHeight(leftNode);

    return leftNode;
  }

  template< class Key, class T, class Cmp >
  Node< Key, T >* AVLTree< Key, T, Cmp >::leftRotate(NodeType* node) noexcept
  {
    NodeType* rightNode = node->right;
    node->right = rightNode->left;
    if (rightNode->left != fakeleaf_) {
      rightNode->left->parent = node;
    }

    rightNode->parent = node->parent;
    if (node->parent == nullptr) {
      root_ = rightNode;
    } else if (node->parent->right == node) {
      node->parent->right = rightNode;
    } else {
      node->parent->left = rightNode;
    }
    rightNode->left = node;
    node->parent = rightNode;

    updateHeight(node);
    updateHeight(rightNode);

    return rightNode;
  }

  template< class Key, class T, class Cmp >
  Node< Key, T >* AVLTree< Key, T, Cmp >::balance(NodeType* node) noexcept
  {
    if (node == fakeleaf_) {
      return fakeleaf_;
    }
    updateHeight(node);
    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1) {
      if (getBalanceFactor(node->left) < 0) {
        node->left = leftRotate(node->left);
      }
      return rightRotate(node);
    }
    if (balanceFactor < -1) {
      if (getBalanceFactor(node->right) > 0) {
        node->right = rightRotate(node->right);
      }
      return leftRotate(node);
    }
    return node;
  }

  template< class Key, class T, class Cmp >
  void AVLTree< Key, T, Cmp >::balanceTree(NodeType* node)
  {
    while (node != nullptr && node != fakeleaf_) {
      node = balance(node);
      if (node->parent == nullptr) {
        root_ = node;
        break;
      }
      node = node->parent;
    }
  }

}
#endif
