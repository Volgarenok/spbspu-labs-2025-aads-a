#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <utility>
#include "iterator.hpp"
#include "cIterator.hpp"

namespace duhanina
{
  template < typename Key, typename Value, typename Compare >
  class Tree
  {
  public:
    using Iterator_t = Iterator< Key, Value, Compare >;
    using ConstIterator_t = ConstIterator< Key, Value, Compare >;

    Tree();

    template< typename InputIt >
    Tree(InputIt, InputIt);

    explicit Tree(std::initializer_list< std::pair< Key, Value > >);

    Tree(const Tree& other);
    Tree(Tree&& other) noexcept;
    ~Tree();

    Tree& operator=(const Tree& other);
    Tree& operator=(Tree&& other) noexcept;

    Iterator_t begin() const noexcept;
    ConstIterator_t cbegin() const noexcept;
    Iterator_t end() const noexcept;
    ConstIterator_t cend() const noexcept;

    void push(const Key& k, const Value& v);
    const Value& get(const Key& k) const;
    void drop(const Key& k);

    size_t size() const noexcept;
    bool empty() const noexcept;

    void clear() noexcept;
    void swap(Tree& other) noexcept;

    Iterator_t insert(const std::pair< const Key, Value >& value);
    Iterator_t insert(std::pair< const Key, Value >&& value);
    template< typename InputIt >
    void insert(InputIt first, InputIt last);

    Iterator_t erase(Iterator_t it) noexcept;
    size_t erase(const Key& key) noexcept;
    Iterator_t erase(Iterator_t first, Iterator_t last) noexcept;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    Value& operator[](const Key& key);
    Value& operator[](Key&& key);

    size_t count(const Key& key) const noexcept;
    Iterator_t find(const Key& key) noexcept;
    ConstIterator_t find(const Key& key) const noexcept;

    std::pair< Iterator_t, Iterator_t > equal_range(const Key& key) noexcept;
    std::pair< ConstIterator_t, ConstIterator_t > equal_range(const Key& key) const noexcept;
    Iterator_t lower_bound(const Key& key) noexcept;
    ConstIterator_t lower_bound(const Key& key) const noexcept;
    Iterator_t upper_bound(const Key& key) noexcept;
    ConstIterator_t upper_bound(const Key& key) const noexcept;

  private:
    using Node_t = Node< Key, Value >;

    Node_t* fakeRoot_;
    size_t size_;
    Compare comp_;

    Node_t* getRoot() const noexcept;
    void setRoot(Node_t* newRoot) noexcept;
    void updateHeight(Node_t* Node_t) noexcept;
    int getBalance(Node_t* node) noexcept;
    Node_t* rotateRight(Node_t* y) noexcept;
    Node_t* rotateLeft(Node_t* x) noexcept;
    Node_t* insert(Node_t* node, const Key& k, const Value& v, Node_t* parent);
    Node_t* find(Node_t* node, const Key& k) const noexcept;
    Node_t* findMin(Node_t* node) const noexcept;
    Node_t* findMax(Node_t* node) const noexcept;
    Node_t* remove(Node_t* node, const Key& k) noexcept;
    void clear(Node_t* node) noexcept;
    Node_t* copyTree(Node_t* node, Node_t* parent);
    Node_t* findNextNode(Node_t* node) const noexcept;
  };

  template < typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >::Tree():
    fakeRoot_(new Node_t(Key(), Value(), nullptr)),
    size_(0)
  {}

  template< typename Key, typename Value, typename Compare >
  template< typename InputIt >
  Tree< Key, Value, Compare >::Tree(InputIt first, InputIt last):
    Tree()
  {
    for (auto it = first; it != last; it++)
    {
      insert(*it);
    }
  }

  template< typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >::Tree(std::initializer_list< std::pair< Key, Value > > iList):
    Tree(iList.begin(), iList.end())
  {}

  template < typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >::Tree(const Tree& other):
    Tree(other.cbegin(), other.cend())
  {}

  template < typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >::Tree(Tree&& other) noexcept:
    fakeRoot_(std::exchange(other.fakeRoot_, new Node_t(Key(), Value(), nullptr))),
    size_(std::exchange(other.size_, 0))
  {}

  template < typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >::~Tree()
  {
    clear();
    delete fakeRoot_;
  }

  template < typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >& Tree< Key, Value, Compare >::operator=(const Tree< Key, Value, Compare >& other)
  {
    if (this != std::addressof(other))
    {
      Tree< Key, Value, Compare> temp(other);
      swap(temp);
    }
    return *this;
  }

  template < typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >& Tree< Key, Value, Compare >::operator=(Tree< Key, Value, Compare >&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      Tree< Key, Value, Compare > temp(std::move(other));
      swap(temp);
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::begin() const noexcept
  {
    if (empty())
    {
      return end();
    }
    return Iterator_t(findMin(getRoot()));
  }

  template< typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::ConstIterator_t Tree< Key, Value, Compare >::cbegin() const noexcept
  {
    if (empty())
    {
      return cend();
    }
    return ConstIterator_t(findMin(getRoot()));
  }

  template< typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::end() const noexcept
  {
    return Iterator_t(fakeRoot_);
  }

  template< typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::ConstIterator_t Tree< Key, Value, Compare >::cend() const noexcept
  {
    return ConstIterator_t(fakeRoot_);
  }

  template < typename Key, typename Value, typename Compare >
  void Tree< Key, Value, Compare >::push(const Key& k, const Value& v)
  {
    if (count(k))
    {
      Node_t* existing = find(getRoot(), k);
      existing->data.second = v;
      return;
    }
    Node_t* oldRoot = getRoot();
    size_t oldSize = size_;
    try
    {
      Node_t* newRoot = insert(getRoot(), k, v, fakeRoot_);
      setRoot(newRoot);
      size_ = oldSize + 1;
    }
    catch (...)
    {
      setRoot(oldRoot);
      size_ = oldSize;
      throw;
    }
  }

  template < typename Key, typename Value, typename Compare >
  const Value& Tree< Key, Value, Compare >::get(const Key& k) const
  {
    const Node_t* node = find(getRoot(), k);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    return node->data.second;
  }

  template < typename Key, typename Value, typename Compare >
  void Tree< Key, Value, Compare >::drop(const Key& k)
  {
    Node_t* node = find(getRoot(), k);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    setRoot(remove(getRoot(), k));
    size_--;
  }

  template < typename Key, typename Value, typename Compare >
  size_t Tree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template < typename Key, typename Value, typename Compare >
  bool Tree< Key, Value, Compare >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < typename Key, typename Value, typename Compare >
  void Tree< Key, Value, Compare >::clear() noexcept
  {
    clear(getRoot());
    setRoot(nullptr);
    size_ = 0;
  }

  template < typename Key, typename Value, typename Compare >
  void Tree< Key, Value, Compare >::swap(Tree< Key, Value, Compare >& other) noexcept
  {
    std::swap(fakeRoot_, other.fakeRoot_);
    std::swap(size_, other.size_);
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::insert(const std::pair< const Key, Value >& value)
  {
    push(value.first, value.second);
    return Iterator_t(find(getRoot(), value.first));
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::insert(std::pair< const Key, Value >&& value)
  {
    push(std::move(value.first), std::move(value.second));
    return Iterator_t(find(getRoot(), value.first));
  }

  template < typename Key, typename Value, typename Compare >
  template< typename InputIt >
  void Tree< Key, Value, Compare >::insert(InputIt first, InputIt last)
  {
    for (auto it = first; it != last; ++it)
    {
      push(it->first, it->second);
    }
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::erase(Iterator_t pos) noexcept
  {
    if (pos == end() || pos.current_ == nullptr)
    {
      return end();
    }
    Node_t* nextNode = findNextNode(pos.current_);
    setRoot(remove(getRoot(), pos.current_->data.first));
    size_--;
    if (nextNode != nullptr)
    {
      return Iterator_t(nextNode);
    }
    else
    {
      return end();
    }
  }

  template < typename Key, typename Value, typename Compare >
  size_t Tree< Key, Value, Compare >::erase(const Key& key) noexcept
  {
    Iterator_t it = find(key);
    if (it != end())
    {
      erase(it);
      return 1;
    }
    return 0;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::erase(Iterator_t first, Iterator_t last) noexcept
  {
    while (first != last)
    {
      first = erase(first);
    }
    return last;
  }

  template < typename Key, typename Value, typename Compare >
  Value& Tree< Key, Value, Compare >::at(const Key& key)
  {
    Node_t* node = find(getRoot(), key);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    return node->data.second;
  }

  template < typename Key, typename Value, typename Compare >
  const Value& Tree< Key, Value, Compare >::at(const Key& key) const
  {
    Node_t* node = find(getRoot(), key);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    return node->data.second;
  }

  template < typename Key, typename Value, typename Compare >
  Value& Tree< Key, Value, Compare >::operator[](const Key& key)
  {
    Node_t* node = find(getRoot(), key);
    if (!node)
    {
      push(key, Value());
      node = find(getRoot(), key);
    }
    return node->data.second;
  }

  template < typename Key, typename Value, typename Compare >
  Value& Tree< Key, Value, Compare >::operator[](Key&& key)
  {
    Node_t* node = find(getRoot(), key);
    if (!node)
    {
      push(std::move(key), Value());
      node = find(getRoot(), key);
    }
    return node->data.second;
  }

  template < typename Key, typename Value, typename Compare >
  size_t Tree< Key, Value, Compare >::count(const Key& key) const noexcept
  {
    if (find(getRoot(), key))
    {
      return 1;
    }
    return 0;
  }


  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::find(const Key& key) noexcept
  {
    Node_t* node = find(getRoot(), key);
    if (node != nullptr)
    {
      return Iterator_t(node);
    }
    else
    {
      return end();
    }
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::ConstIterator_t Tree< Key, Value, Compare >::find(const Key& key) const noexcept
  {
    const Node_t* node = find(getRoot(), key);
    if (node != nullptr)
    {
      return ConstIterator_t(node);
    }
    else
    {
      return cend();
    }
  }

  template < typename Key, typename Value, typename Compare >
  std::pair< typename Tree< Key, Value, Compare >::Iterator_t, typename Tree< Key, Value, Compare >::Iterator_t >
    Tree< Key, Value, Compare >::equal_range(const Key& key) noexcept
  {
    return { lower_bound(key), upper_bound(key) };
  }

  template < typename Key, typename Value, typename Compare >
  std::pair< typename Tree< Key, Value, Compare >::ConstIterator_t, typename Tree< Key, Value, Compare >::ConstIterator_t >
    Tree< Key, Value, Compare >::equal_range(const Key& key) const noexcept
  {
    return { lower_bound(key), upper_bound(key) };
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::lower_bound(const Key& key) noexcept
  {
    Node_t* current = getRoot();
    Node_t* result = nullptr;
    while (current)
    {
      if (!comp_(current->data.first, key))
      {
        result = current;
        current = current->left;
      }
      else
      {
        current = current->right;
      }
    }
    if (result != nullptr)
    {
      return Iterator_t(result);
    }
    else
    {
      return end();
    }
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::ConstIterator_t Tree< Key, Value, Compare >::lower_bound(const Key& key) const noexcept
  {
    const Node_t* current = getRoot();
    const Node_t* result = nullptr;
    while (current)
    {
      if (!comp_(current->data.first, key))
      {
        result = current;
        current = current->left;
      }
      else
      {
        current = current->right;
      }
    }
    if (result != nullptr)
    {
      return ConstIterator_t(result);
    }
    else
    {
      return cend();
    }
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::upper_bound(const Key& key) noexcept
  {
    Node_t* current = getRoot();
    Node_t* result = nullptr;
    while (current)
    {
      if (comp_(key, current->data.first))
      {
        result = current;
        current = current->left;
      }
      else
      {
        current = current->right;
      }
    }
    if (result != nullptr)
    {
      return Iterator_t(result);
    }
    else
    {
      return end();
    }
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::ConstIterator_t Tree< Key, Value, Compare >::upper_bound(const Key& key) const noexcept
  {
    const Node_t* current = getRoot();
    const Node_t* result = nullptr;
    while (current)
    {
      if (comp_(key, current->data.first))
      {
        result = current;
        current = current->left;
      }
      else
      {
        current = current->right;
      }
    }
    if (result != nullptr)
    {
      return ConstIterator_t(result);
    }
    else
    {
      return cend();
    }
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::getRoot() const noexcept
  {
    return fakeRoot_->left;
  }

  template < typename Key, typename Value, typename Compare >
  void Tree< Key, Value, Compare >::setRoot(Node_t* newRoot) noexcept
  {
    fakeRoot_->left = newRoot;
    if (newRoot)
    {
      newRoot->parent = fakeRoot_;
    }
  }

  template < typename Key, typename Value, typename Compare >
  void Tree< Key, Value, Compare >::updateHeight(Node_t* node) noexcept
  {
    if (node == nullptr)
    {
      return;
    }
    int leftHeight = 0;
    if (node->left != nullptr)
    {
      leftHeight = node->left->height;
    }
    int rightHeight = 0;
    if (node->right != nullptr)
    {
      rightHeight = node->right->height;
    }
    node->height = 1 + std::max(leftHeight, rightHeight);
  }

  template < typename Key, typename Value, typename Compare >
  int Tree< Key, Value, Compare >::getBalance(Node_t* node) noexcept
  {
    if (!node)
    {
      return 0;
    }
    int leftHeight = 0;
    if (node->left)
    {
      leftHeight = node->left->height;
    }
    int rightHeight = 0;
    if (node->right)
    {
      rightHeight = node->right->height;
    }
    return leftHeight - rightHeight;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::rotateRight(Node_t* root) noexcept
  {
    Node_t* leftChild = root->left;
    Node_t* leftRightSubtree = leftChild->right;
    leftChild->right = root;
    root->left = leftRightSubtree;
    if (leftRightSubtree)
    {
      leftRightSubtree->parent = root;
    }
    leftChild->parent = root->parent;
    root->parent = leftChild;
    updateHeight(root);
    updateHeight(leftChild);
    return leftChild;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::rotateLeft(Node_t* root) noexcept
  {
    Node_t* rightChild = root->right;
    Node_t* rightLeftSubtree = rightChild->left;
    rightChild->left = root;
    root->right = rightLeftSubtree;
    if (rightLeftSubtree)
    {
      rightLeftSubtree->parent = root;
    }
    rightChild->parent = root->parent;
    root->parent = rightChild;
    updateHeight(root);
    updateHeight(rightChild);
    return rightChild;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t*
    Tree< Key, Value, Compare >::insert(Node_t* node, const Key& k, const Value& v, Node_t* parent)
  {
    if (!node)
    {
      return new Node_t(k, v, parent);
    }
    if (comp_(k, node->data.first))
    {
      node->left = insert(node->left, k, v, node);
    }
    else if (comp_(node->data.first, k))
    {
      node->right = insert(node->right, k, v, node);
    }
    else
    {
      node->data.second = v;
      return node;
    }
    updateHeight(node);
    int balance = getBalance(node);
    if (balance > 1 && comp_(k, node->left->data.first))
    {
      return rotateRight(node);
    }
    if (balance < -1 && comp_(node->right->data.first, k))
    {
      return rotateLeft(node);
    }
    if (balance > 1 && comp_(node->left->data.first, k))
    {
      node->left = rotateLeft(node->left);
      return rotateRight(node);
    }
    if (balance < -1 && comp_(k, node->right->data.first))
    {
      node->right = rotateRight(node->right);
      return rotateLeft(node);
    }
    return node;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::find(Node_t* node, const Key& k) const noexcept
  {
    while (node)
    {
      if (comp_(k, node->data.first))
      {
        node = node->left;
      }
      else if (comp_(node->data.first, k))
      {
        node = node->right;
      }
      else
      {
        return node;
      }
    }
    return nullptr;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::findMin(Node_t* node) const noexcept
  {
    if (!node)
    {
      return nullptr;
    }
    while (node->left)
    {
      node = node->left;
    }
    return node;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::findMax(Node_t* node) const noexcept
  {
    if (!node)
    {
      return nullptr;
    }
    while (node->right)
    {
      node = node->right;
    }
    return node;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::remove(Node_t* node, const Key& k) noexcept
  {
    if (!node)
    {
      return nullptr;
    }
    if (comp_(k, node->data.first))
    {
      node->left = remove(node->left, k);
    }
    else if (comp_(node->data.first, k))
    {
      node->right = remove(node->right, k);
    }
    else
    {
      if (!node->left || !node->right)
      {
        Node_t* temp = node->left;
        if (!temp)
        {
          temp = node->right;
        }
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
        Node_t* temp = findMin(node->right);
        node->data = temp->data;
        node->right = remove(node->right, temp->data.first);
      }
    }
    if (!node)
    {
      return nullptr;
    }
    updateHeight(node);
    int balance = getBalance(node);
    if (balance > 1 && getBalance(node->left) >= 0)
    {
      return rotateRight(node);
    }
    if (balance > 1 && getBalance(node->left) < 0)
    {
      node->left = rotateLeft(node->left);
      return rotateRight(node);
    }
    if (balance < -1 && getBalance(node->right) <= 0)
    {
      return rotateLeft(node);
    }
    if (balance < -1 && getBalance(node->right) > 0)
    {
      node->right = rotateRight(node->right);
      return rotateLeft(node);
    }
    return node;
  }

  template < typename Key, typename Value, typename Compare >
  void Tree< Key, Value, Compare >::clear(Node_t* node) noexcept
  {
    if (node)
    {
      clear(node->left);
      clear(node->right);
      delete node;
    }
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::copyTree(Node_t* node, Node_t* parent)
  {
    if (!node)
    {
      return nullptr;
    }
    Node_t* left_copy = nullptr;
    Node_t* right_copy = nullptr;
    Node_t* newNode = nullptr;
    try
    {
      newNode = new Node_t(node->data.first, node->data.second, parent);
      left_copy = copyTree(node->left, nullptr);
      right_copy = copyTree(node->right, nullptr);
      newNode->left = left_copy;
      newNode->right = right_copy;
      if (left_copy)
      {
        left_copy->parent = newNode;
      }
      if (right_copy)
      {
        right_copy->parent = newNode;
      }
      newNode->height = node->height;
      return newNode;
    }
    catch (...)
    {
      clear(left_copy);
      clear(right_copy);
      delete newNode;
      throw;
    }
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t*
    Tree< Key, Value, Compare >::findNextNode(Node_t* node) const noexcept
  {
    if (node == nullptr)
    {
      return nullptr;
    }
    if (node->right != nullptr)
    {
      node = node->right;
      while (node->left != nullptr)
      {
        node = node->left;
      }
      return node;
    }
    while (node->parent != nullptr)
    {
      if (node == node->parent->left)
      {
        return node->parent;
      }
      node = node->parent;
    }
    return nullptr;
  }

}

#endif
