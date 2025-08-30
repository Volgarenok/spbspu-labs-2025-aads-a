#ifndef TREE_HPP
#define TREE_HPP

#include <queue>
#include <stack>
#include <cassert>
#include <stdexcept>
#include "tree_node.hpp"
#include "tree_iterator.hpp"
#include "tree_const_iterator.hpp"

namespace lanovenko
{
  template< typename Key, typename Value, typename Comparator >
  class TreeIterator;

  template< typename Key, typename Value, typename Comparator >
  class TreeConstIterator;

  template< typename Key, typename Value, typename Comparator >
  class Tree
  {
  public:
    using cIter = TreeConstIterator< Key, Value, Comparator >;
    using iter = TreeIterator< Key, Value, Comparator >;
    using pairIt = std::pair< iter, iter >;
    using pairCit = std::pair< cIter, cIter >;

    ~Tree() noexcept;
    Tree();
    Tree(const Tree< Key, Value, Comparator >& rhs);
    Tree(Tree< Key, Value, Comparator >&& rhs) noexcept;
    Tree< Key, Value, Comparator >& operator=(const Tree< Key, Value, Comparator >& rhs);
    Tree< Key, Value, Comparator >& operator=(Tree< Key, Value, Comparator >&& rhs) noexcept;

    iter begin() noexcept;
    cIter begin() const noexcept;
    iter end() noexcept;
    cIter end() const noexcept;
    cIter cbegin() const noexcept;
    cIter cend() const noexcept;
    pairIt equal_range(const Key& key);
    pairCit equal_range(const Key& key) const;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void clear() noexcept;
    void swap(Tree< Key, Value, Comparator >& rhs) noexcept;
    iter find(const Key& key);
    cIter find(const Key& key) const;
    iter insert(const std::pair< Key, Value >& value);
    iter erase(iter position) noexcept;
    size_t erase(const Key& key);
    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    size_t count(const Key& k) const;
    Value& operator[](const Key& key);
    const Value& operator[](const Key& key) const;
    template< typename F >
    F traverseLnr(F f);
    template< typename F >
    F traverseLnr(F f) const;
    template< typename F >
    F traverseRnl(F f);
    template< typename F >
    F traverseRnl(F f) const;
    template< typename F >
    F traverseBreadth(F f);
    template< typename F >
    F traverseBreadth(F f) const;
  private:
    using node = TreeNode< Key, Value >;
    node* root_;
    node* fakeLeaf_;
    size_t size_;
    unsigned short int height(node* node) const noexcept;
    void fixHeight(node* node) const noexcept;
    short int balanceFactor(node* node) const noexcept;
    node* balance(node* node) noexcept;
    node* rotateRight(node* root) const noexcept;
    node* rotateLeft(node* root) const noexcept;
    node* insert(node* node, const std::pair< Key, Value >& value, const Comparator& cmp);
    static node* minValueNode(node* node) noexcept;
    node* erase(node* root, node* node) noexcept;
    void clear(node* root) noexcept;
    friend class TreeIterator< Key, Value, Comparator >;
    friend class TreeConstIterator< Key, Value, Comparator >;
  };

  template< typename Key, typename Value, typename Comparator >
  Tree< Key, Value, Comparator >::~Tree() noexcept
  {
    clear();
    delete fakeLeaf_;
  }

  template< typename Key, typename Value, typename Comparator >
  Tree< Key, Value, Comparator >::Tree():
    root_(nullptr),
    fakeLeaf_(new TreeNode< Key, Value >{}),
    size_(0)
  {}

  template< typename Key, typename Value, typename Comparator >
  Tree< Key, Value, Comparator >::Tree(const Tree< Key, Value, Comparator >& rhs):
    Tree()
  {
    for (cIter it = rhs.cbegin(); it != rhs.cend(); it++)
    {
      insert(*it);
    }
  }

  template< typename Key, typename Value, typename Comparator >
  Tree< Key, Value, Comparator >::Tree(Tree< Key, Value, Comparator >&& rhs) noexcept:
    root_(rhs.root_),
    fakeLeaf_(rhs.fakeLeaf_),
    size_(rhs.size_)
  {
    rhs.root_ = rhs.fakeLeaf_ = nullptr;
    rhs.size_ = 0;
  }

  template< typename Key, typename Value, typename Comparator >
  Tree< Key, Value, Comparator >& Tree< Key, Value, Comparator >::operator=(const Tree< Key, Value, Comparator >& rhs)
  {
    if (this != std::addressof(rhs))
    {
      Tree< Key, Value, Comparator > temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename Key, typename Value, typename Comparator >
  Tree< Key, Value, Comparator >& Tree< Key, Value, Comparator >::operator=(Tree< Key, Value, Comparator >&& r) noexcept
  {
    if (this != std::addressof(r))
    {
      Tree< Key, Value, Comparator > temp(std::move(r));
      swap(temp);
    }
    return *this;
  }

  template< typename Key, typename Value, typename Comparator >
  typename Tree< Key, Value, Comparator >::iter Tree< Key, Value, Comparator >::begin() noexcept
  {
    return iter(minValueNode(root_));
  }

  template< typename Key, typename Value, typename Comparator >
  typename Tree< Key, Value, Comparator >::cIter Tree< Key, Value, Comparator >::begin() const noexcept
  {
    return cIter(minValueNode(root_));
  }

  template< typename Key, typename Value, typename Comparator >
  typename Tree< Key, Value, Comparator >::iter Tree< Key, Value, Comparator >::end() noexcept
  {
    return iter();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Tree< Key, Value, Comparator >::cIter Tree< Key, Value, Comparator >::end() const noexcept
  {
    return cIter();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Tree< Key, Value, Comparator >::cIter Tree< Key, Value, Comparator >::cbegin() const noexcept
  {
    return cIter(minValueNode(root_));
  }

  template< typename Key, typename Value, typename Comparator >
  typename Tree< Key, Value, Comparator >::cIter Tree< Key, Value, Comparator >::cend() const noexcept
  {
    return cIter();
  }

  template< typename Key, typename Value, typename Comparator >
  typename Tree< Key, Value, Comparator >::pairIt Tree< Key, Value, Comparator >::equal_range(const Key& key)
  {
    iter position = find(key);
    iter nextPosition = position;
    if (position == end() || ++nextPosition == end())
    {
      position = nextPosition = end();
    }
    return pairIt{ position, nextPosition };
  }

  template< typename Key, typename Value, typename Comparator >
  typename Tree< Key, Value, Comparator >::pairCit Tree< Key, Value, Comparator >::equal_range(const Key& key) const
  {
    cIter position = find(key);
    cIter nextPosition = position;
    if (position == end() || ++nextPosition == end())
    {
      position = nextPosition = end();
    }
    return pairCit{ position, nextPosition };
  }

  template< typename Key, typename Value, typename Comparator >
  inline size_t Tree< Key, Value, Comparator >::size() const noexcept
  {
    return size_;
  }

  template< typename Key, typename Value, typename Comparator >
  inline bool Tree< Key, Value, Comparator >::empty() const noexcept
  {
    return size_ == 0;
  }


  template< typename Key, typename Value, typename Comparator >
  void Tree< Key, Value, Comparator >::clear() noexcept
  {
    clear(root_);
    delete fakeLeaf_;
    fakeLeaf_ = root_ = nullptr;
  }

  template< typename Key, typename Value, typename Comparator >
  inline void Tree< Key, Value, Comparator >::swap(Tree< Key, Value, Comparator >& rhs) noexcept
  {
    std::swap(root_, rhs.root_);
    std::swap(fakeLeaf_, rhs.fakeLeaf_);
    std::swap(size_, rhs.size_);
  }

  template< typename Key, typename Value, typename Comparator >
  typename Tree< Key, Value, Comparator >::iter Tree< Key, Value, Comparator >::find(const Key& key)
  {
    TreeNode< Key, Value >* root = root_;
    Comparator cmp = Comparator{};
    while (root && key != root->data_.first)
    {
      if (cmp(key, root->data_.first))
      {
        root = root->left_;
      }
      else
      {
        root = root->right_;
      }
    }
    return iter(root);
  }

  template< typename Key, typename Value, typename Comparator >
  typename Tree< Key, Value, Comparator >::cIter Tree< Key, Value, Comparator >::find(const Key& key) const
  {
    TreeNode< Key, Value >* root = root_;
    Comparator cmp = Comparator{};
    while (root && key != root->data_.first)
    {
      if (cmp(key, root->data_.first))
      {
        root = root->left_;
      }
      else
      {
        root = root->right_;
      }
    }
    return cIter(root);
  }

  template< typename Key, typename Value, typename Comparator >
  typename Tree< Key, Value, Comparator >::iter Tree< Key, Value, Comparator >::insert
  (const std::pair< Key, Value >& value)
  {
    root_ = insert(root_, value, Comparator{});
    size_++;
    return find(value.first);
  }

  template< typename Key, typename Value, typename Comparator >
  typename Tree< Key, Value, Comparator >::iter Tree< Key, Value, Comparator >::erase(iter position) noexcept
  {
    root_ = erase(root_, position++.node_);
    size_--;
    return position;
  }

  template< typename Key, typename Value, typename Comparator >
  size_t Tree< Key, Value, Comparator >::erase(const Key& key)
  {
    size_t erasedCount = 0;
    iter position = find(key);
    if (position == end())
    {
      return erasedCount;
    }
    root_ = erase(root_, position.node_);
    size_--;
    return ++erasedCount;
  }

  template< typename Key, typename Value, typename Comparator >
  Value& Tree< Key, Value, Comparator >::at(const Key& key)
  {
    iter position = find(key);
    if (position != end())
    {
      return (*position).second;
    }
    throw std::out_of_range("Wrong key");
  }

  template< typename Key, typename Value, typename Comparator >
  const Value& Tree< Key, Value, Comparator >::at(const Key& key) const
  {
    cIter position = find(key);
    if (position != end())
    {
      return (*position).second;
    }
    throw std::out_of_range("Wrong key");
  }

  template< typename Key, typename Value, typename Comparator >
  size_t Tree< Key, Value, Comparator >::count(const Key& key) const
  {
    size_t count = 0;
    iter position = find(key);
    return position == end() ? count : ++count;
  }

  template< typename Key, typename Value, typename Comparator >
  Value& Tree< Key, Value, Comparator >::operator[](const Key& key)
  {
    iter node = insert({key, Value{}});
    return node->second;
  }

  template< typename Key, typename Value, typename Comparator >
  const Value& Tree< Key, Value, Comparator >::operator[](const Key& key) const
  {
    iter node = insert({key, Value{}});
    return node->second;
  }

  template< typename Key, typename Value, typename Comparator >
  inline unsigned short int Tree< Key, Value, Comparator >::height(node* node) const noexcept
  {
    return !node ? 0 : node->height_;
  }

  template< typename Key, typename Value, typename Comparator >
  inline void Tree< Key, Value, Comparator >::fixHeight(node* node) const noexcept
  {
    unsigned short int hl = height(node->left_), hr = height(node->right_);
    node->height_ = (hl > hr ? hl : hr) + 1;
  }

  template< typename Key, typename Value, typename Comparator >
  inline short int Tree< Key, Value, Comparator >::balanceFactor(node* node) const noexcept
  {
    return !node ? 0 : height(node->left_) - height(node->right_);
  }

  template< typename Key, typename Value, typename Comparator >
  TreeNode< Key, Value >* Tree< Key, Value, Comparator >::balance(node* node) noexcept
  {
    short int balance = balanceFactor(node);
    if (balance > 1)
    {
      if (balanceFactor(node->left_) < 0)
      {
        node->left_ = rotateLeft(node->left_);
      }
      node = rotateRight(node);
    }
    if (balance < -1)
    {
      if (balanceFactor(node->right_) > 0)
      {
        node->right_ = rotateRight(node->right_);
      }
      node = rotateLeft(node);
    }
    return node;
  }

  template< typename Key, typename Value, typename Comparator >
  TreeNode< Key, Value >* Tree< Key, Value, Comparator >::rotateRight(node* root) const noexcept
  {
    assert(root && root->left_ != fakeLeaf_);
    TreeNode< Key, Value  >* newRoot = root->left_;
    if (root->parent_)
    {
      newRoot->parent_ = root->parent_;
      if (root->parent_->right_ == root)
      {
        newRoot->parent_->right_ = newRoot;
      }
      else
      {
        newRoot->parent_->left_ = newRoot;
      }
    }
    else
    {
      newRoot->parent_ = nullptr;
    }
    root->parent_ = newRoot;
    if (newRoot->right_)
    {
      root->left_ = newRoot->right_;
      root->left_->parent_ = root;
    }
    else
    {
      root->left_ = nullptr;
    }
    newRoot->right_ = root;
    fixHeight(root);
    fixHeight(newRoot);
    return newRoot;
  }

  template< typename Key, typename Value, typename Comparator >
  TreeNode< Key, Value >* Tree< Key, Value, Comparator >::rotateLeft(node* root) const noexcept
  {
    assert(root && root->right_ != fakeLeaf_);
    TreeNode< Key, Value >* newRoot = root->right_;
    if (root->parent_)
    {
      newRoot->parent_ = root->parent_;
      if (root->parent_->right_ == root)
      {
        newRoot->parent_->right_ = newRoot;
      }
      else
      {
        newRoot->parent_->left_ = newRoot;
      }
    }
    else
    {
      newRoot->parent_ = nullptr;
    }
    root->parent_ = newRoot;
    if (newRoot->left_)
    {
      root->right_ = newRoot->left_;
      root->right_->parent_ = root;
    }
    else
    {
      root->right_ = nullptr;
    }
    newRoot->left_ = root;
    fixHeight(root);
    fixHeight(newRoot);
    return newRoot;
  }

  template< typename Key, typename Value, typename Comparator >
  TreeNode< Key, Value >* Tree< Key, Value, Comparator >::insert(
    node* node,
    const std::pair< Key, Value >& value,
    const Comparator& cmp)
  {
    if (!node || node == fakeLeaf_)
    {
      return new TreeNode< Key, Value >{ {value.first, value.second} };
    }
    else if (cmp(value.first, node->data_.first))
    {
      node->left_ = insert(node->left_, value, cmp);
      node->left_->parent_ = node;
    }
    else if (cmp(node->data_.first, value.first))
    {
      node->right_ = insert(node->right_, value, cmp);
      node->right_->parent_ = node;
    }
    fixHeight(node);
    return balance(node);
  }

  template< typename Key, typename Value, typename Comparator >
  typename Tree< Key, Value, Comparator >::node* Tree< Key, Value, Comparator >::minValueNode(node* node) noexcept
  {
    while (node && node->left_)
    {
      node = node->left_;
    }
    return node;
  }

  template< typename Key, typename Value, typename Comparator >
  typename Tree< Key, Value, Comparator >::node* Tree< Key, Value, Comparator >::erase(node* root, node* node) noexcept
  {
    TreeNode< Key, Value >* toBalance = nullptr;
    TreeNode< Key, Value >* subRoot = nullptr;
    if (!node->left_ && !node->right_)
    {
      toBalance = node->parent_;
      if (!node->parent_)
      {
        root = nullptr;
      }
    }
    else if (node->right_ && node->left_)
    {
      subRoot = minValueNode(node->right_);
      if (subRoot->parent_->left_ == subRoot)
      {
        subRoot->parent_->left_ = subRoot->right_;
        if (subRoot->right_)
        {
          subRoot->right_->parent_ = subRoot->parent_;
        }
        subRoot->right_ = node->right_;
        subRoot->right_->parent_ = subRoot;
      }
      toBalance = subRoot->parent_->right_ == subRoot ? subRoot : subRoot->parent_;
      subRoot->parent_ = node->parent_;
      subRoot->left_ = node->left_;
      subRoot->left_->parent_ = subRoot;
    }
    else
    {
      subRoot = node->left_ ? node->left_ : node->right_;
      toBalance = subRoot;
      subRoot->parent_ = node->parent_;
    }
    if (node->parent_)
    {
      (node->parent_->left_ == node ? node->parent_->left_ : node->parent_->right_) = subRoot;
    }
    delete node;
    while (toBalance)
    {
      fixHeight(toBalance);
      root = balance(toBalance);
      toBalance = toBalance->parent_;
    }
    return root;
  }

  template< typename Key, typename Value, typename Comparator >
  void Tree< Key, Value, Comparator >::clear(node* root) noexcept
  {
    if (!root || root == fakeLeaf_)
    {
      return;
    }
    clear(root->left_);
    clear(root->right_);
    delete root;
    size_--;
  }

  template< typename Key, typename Value, typename Comparator >
  template< typename F >
  inline F Tree< Key, Value, Comparator >::traverseLnr(F f)
  {
    if (empty())
    {
      throw std::logic_error("<EMPTY>");
    }
    std::stack< node* > stack;
    node* current = root_;
    bool leftDone = false;
    while (current != nullptr || !stack.empty())
    {
      if (!leftDone && current != nullptr)
      {
        stack.push(current);
        current = current->left_;
      }
      else
      {
        current = stack.top();
        stack.pop();
        f(current->data_);
        current = current->right_;
        leftDone = (current == nullptr);
      }
    }
    return f;
  }

  template< typename Key, typename Value, typename Comparator >
  template< typename F >
  inline F Tree< Key, Value, Comparator >::traverseLnr(F f) const
  {
    return const_cast< Tree < Key, Value, Comparator > * >(this)->traverseLnr(f);
  }

  template< typename Key, typename Value, typename Comparator >
  template< typename F >
  inline F Tree< Key, Value, Comparator >::traverseRnl(F f)
  {
    if (empty())
    {
      throw std::logic_error("<EMPTY>");
    }
    std::stack< node* > stack;
    node* current = root_;
    bool rightDone = false;
    while (current != nullptr || !stack.empty())
    {
      if (!rightDone && current != nullptr)
      {
        stack.push(current);
        current = current->right_;
      }
      else
      {
        current = stack.top();
        stack.pop();
        f(current->data_);
        current = current->left_;
        rightDone = (current == nullptr);
      }
    }
    return f;
  }

  template< typename Key, typename Value, typename Comparator >
  template< typename F >
  inline F Tree< Key, Value, Comparator >::traverseRnl(F f) const
  {
    return const_cast< Tree < Key, Value, Comparator > * >(this)->traverseRnl(f);
  }

  template< typename Key, typename Value, typename Comparator >
  template< typename F >
  inline F Tree< Key, Value, Comparator >::traverseBreadth(F f)
  {
    if (empty())
    {
      throw std::logic_error("<EMPTY>");
    }
    std::queue< node* > queue;
    queue.push(root_);
    while (!queue.empty())
    {
      node* current = queue.front();
      queue.pop();
      f(current->data_);
      if (current->left_ != nullptr)
      {
        queue.push(current->left_);
      }
      if (current->right_ != nullptr)
      {
        queue.push(current->right_);
      }
    }
    return f;
  }

  template< typename Key, typename Value, typename Comparator >
  template< typename F >
  inline F Tree< Key, Value, Comparator >::traverseBreadth(F f) const
  {
    return const_cast< Tree< Key, Value, Comparator > * >(this)->traverseBreadth(f);
  }
}

#endif
