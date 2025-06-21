#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <functional>
#include "node_tree.hpp"
#include "c_iterator.hpp"
#include "iterator.hpp"
#include "stack.hpp"
#include "queue.hpp"

namespace tkach
{
  template< class Key, class Value, class Cmp = std::less< Key > >
  class AvlTree
  {
  public:
    AvlTree();
    AvlTree(const AvlTree< Key, Value, Cmp >& other);
    AvlTree(AvlTree< Key, Value, Cmp >&& other) noexcept;
    template< class InputIt >
    AvlTree(InputIt begin, InputIt end);
    AvlTree(std::initializer_list< std::pair< Key, Value > > init_list);
    ~AvlTree();
    AvlTree< Key, Value, Cmp >& operator=(const AvlTree< Key, Value, Cmp >& other);
    AvlTree< Key, Value, Cmp >& operator=(AvlTree< Key, Value, Cmp >&& other) noexcept;
    Value& operator[](const Key& key);
    const Value& operator[](const Key& key) const;
    Iterator< Key, Value, Cmp > insert(const std::pair< Key, Value > & pair);
    Iterator< Key, Value, Cmp > insert(std::pair< Key, Value > && pair);
    template< class InputIt >
    void insert(InputIt begin, InputIt end);
    void clear();
    Citerator< Key, Value, Cmp > find(const Key& key) const;
    Iterator< Key, Value, Cmp > find(const Key& key);
    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    Citerator< Key, Value, Cmp > cbegin() const;
    Iterator< Key, Value, Cmp > begin();
    Citerator< Key, Value, Cmp > cend() const;
    Iterator< Key, Value, Cmp > end();
    Iterator< Key, Value, Cmp > erase(Iterator< Key, Value, Cmp > it);
    size_t erase(const Key& key);
    Iterator< Key, Value, Cmp > erase(Citerator< Key, Value, Cmp > begin, Citerator< Key, Value, Cmp > end);
    Iterator< Key, Value, Cmp > erase(Iterator< Key, Value, Cmp > begin, Iterator< Key, Value, Cmp > end);
    Iterator< Key, Value, Cmp > lowerBound(const Key& key);
    Iterator< Key, Value, Cmp > upperBound(const Key& key);
    template< typename F >
    F traverseLnr(F f) const;
    template< typename F >
    F traverseRnl(F f) const;
    template< typename F >
    F traverseBreadth(F f) const;
    template< typename F >
    F traverseLnr(F f);
    template< typename F >
    F traverseRnl(F f);
    template< typename F >
    F traverseBreadth(F f);
    Citerator< Key, Value, Cmp > lowerBound(const Key& key) const;
    Citerator< Key, Value, Cmp > upperBound(const Key& key) const;
    std::pair< Iterator< Key, Value >, Iterator< Key, Value > > equalRange(const Key& key);
    std::pair< Citerator< Key, Value >, Citerator< Key, Value > > equalRange(const Key& key) const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t count(const Key& k) const;
  private:
    using pair_t = std::pair< TreeNode< Key, Value >*, std::pair< TreeNode< Key, Value >*, bool > >;
    TreeNode< Key, Value >* root_;
    size_t size_;
    Cmp cmp_;
    template< class... Args >
    Iterator< Key, Value, Cmp > insertSingle(const Key& key, Args&&... args);
    void clearFrom(TreeNode< Key, Value >* node);
    TreeNode< Key, Value >* findMin(TreeNode< Key, Value >* node) const;
    TreeNode< Key, Value >* findMax(TreeNode< Key, Value >* node) const;
    TreeNode< Key, Value >* rotateLeft(TreeNode< Key, Value >* const root);
    TreeNode< Key, Value >* rotateRight(TreeNode< Key, Value >* const root);
    void fixHeight(TreeNode< Key, Value >* node);
    size_t height(TreeNode< Key, Value >* node);
    void swap(AvlTree< Key, Value, Cmp >& other) noexcept;
    TreeNode< Key, Value >* eraseFrom(TreeNode< Key, Value >* root, const Key& key);
    TreeNode< Key, Value >* balance(TreeNode< Key, Value >* root);
    template< class... Args >
    pair_t insertCmp(TreeNode< Key, Value >* root, const Key& key, Args&&... args);
  };

  template< class Key, class Value, class Cmp >
  AvlTree< Key, Value, Cmp >::AvlTree():
    root_(nullptr),
    size_(0),
    cmp_(Cmp())
  {}

  template< class Key, class Value, class Cmp >
  template< class InputIt >
  AvlTree< Key, Value, Cmp >::AvlTree(InputIt begin, InputIt end):
    root_(nullptr),
    size_(0),
    cmp_(Cmp())
  {
    while (begin != end)
    {
      try
      {
        insert(std::make_pair(begin->first, begin->second));
        begin++;
      }
      catch (...)
      {
        clear();
        throw;
      }
    }
  }

  template< class Key, class Value, class Cmp >
  AvlTree< Key, Value, Cmp >::AvlTree(std::initializer_list< std::pair< Key, Value > > init_list):
    AvlTree(init_list.begin(), init_list.end())
  {}

  template< class Key, class Value, class Cmp >
  void AvlTree< Key, Value, Cmp >::swap(AvlTree< Key, Value, Cmp >& other) noexcept
  {
    std::swap(other.root_, root_);
    std::swap(size_, other.size_);
    std::swap(cmp_, other.cmp_);
  }

  template< class Key, class Value, class Cmp >
  bool AvlTree< Key, Value, Cmp >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Cmp >
  size_t AvlTree< Key, Value, Cmp >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Cmp >
  AvlTree< Key, Value, Cmp >& AvlTree< Key, Value, Cmp >::operator=(const AvlTree< Key, Value, Cmp >& other)
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    AvlTree< Key, Value, Cmp > temp(other);
    swap(temp);
    return *this;
  }

  template< class Key, class Value, class Cmp >
  Iterator< Key, Value, Cmp > AvlTree< Key, Value, Cmp >::lowerBound(const Key& key)
  {
    return Iterator< Key, Value, Cmp >((static_cast< const AvlTree< Key, Value, Cmp >* >(this)->lowerBound(key)).node_);
  }

  template< class Key, class Value, class Cmp >
  Iterator< Key, Value, Cmp > AvlTree< Key, Value, Cmp >::upperBound(const Key& key)
  {
    return Iterator< Key, Value, Cmp >((static_cast< const AvlTree< Key, Value, Cmp >* >(this)->upperBound(key)).node_);
  }

  template< class Key, class Value, class Cmp >
  Citerator< Key, Value, Cmp > AvlTree< Key, Value, Cmp >::lowerBound(const Key& key) const
  {
    const TreeNode< Key, Value >* node = root_;
    const TreeNode< Key, Value >* result = nullptr;
    while(node)
    {
      if (cmp_(node->data.first, key))
      {
        node = node->right;
      }
      else
      {
        result = node;
        node = node->left;
      }
    }
    return Citerator< Key, Value, Cmp >(const_cast< TreeNode< Key, Value >* >(result));
  }

  template< class Key, class Value, class Cmp >
  Citerator< Key, Value, Cmp > AvlTree< Key, Value, Cmp >::upperBound(const Key& key) const
  {
    const TreeNode< Key, Value >* node = root_;
    const TreeNode< Key, Value >* result = nullptr;
    while(node)
    {
      if (cmp_(key, node->data.first))
      {
        result = node;
        node = node->left;
      }
      else
      {
        node = node->right;
      }
    }
    return Citerator< Key, Value, Cmp >(const_cast< TreeNode< Key, Value >* >(result));
  }

  template< class Key, class Value, class Cmp >
  AvlTree< Key, Value, Cmp >& AvlTree< Key, Value, Cmp >::operator=(AvlTree< Key, Value, Cmp >&& other) noexcept
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    AvlTree< Key, Value, Cmp > temp(std::move(other));
    swap(temp);
    return *this;
  }

  template< class Key, class Value, class Cmp >
  TreeNode< Key, Value >* AvlTree< Key, Value, Cmp >::eraseFrom(TreeNode< Key, Value >* root, const Key& key)
  {
    if (!root)
    {
      return nullptr;
    }
    if (cmp_(key, root->data.first))
    {
      root->left = eraseFrom(root->left, key);
    }
    else if (cmp_(root->data.first, key))
    {
      root->right = eraseFrom(root->right, key);
    }
    else
    {
      if (root->left != nullptr)
      {
        TreeNode< Key, Value >* max_tree = findMax(root->left);
        root->data = max_tree->data;
        root->left = eraseFrom(root->left, max_tree->data.first);
      }
      else if (root->right != nullptr)
      {
        TreeNode< Key, Value >* min_tree = findMin(root->right);
        root->data = min_tree->data;
        root->right = eraseFrom(root->right, min_tree->data.first);
      }
      else
      {
        delete root;
        return nullptr;
      }
    }
    fixHeight(root);
    return balance(root);
  }

  template< class Key, class Value, class Cmp >
  template< class InputIt >
  void AvlTree< Key, Value, Cmp >::insert(InputIt begin, InputIt end)
  {
    while (begin != end)
    {
      insert(*begin);
      begin++;
    }
  }

  template< class Key, class Value, class Cmp >
  Iterator< Key, Value, Cmp > AvlTree< Key, Value, Cmp >::erase(Citerator< Key, Value, Cmp > begin, Citerator< Key, Value, Cmp > end)
  {
    return erase(Iterator< Key, Value, Cmp >(begin.node_), Iterator< Key, Value, Cmp >(end.node_));
  }

  template< class Key, class Value, class Cmp >
  Iterator< Key, Value, Cmp > AvlTree< Key, Value, Cmp >::erase(Iterator< Key, Value, Cmp > begin, Iterator< Key, Value, Cmp > end)
  {
    Iterator< Key, Value, Cmp > res;
    while (begin != end)
    {
      res = erase(begin);
      begin++;
    }
    return res;
  }

  template< class Key, class Value, class Cmp >
  AvlTree< Key, Value, Cmp >::~AvlTree()
  {
    clear();
  }

  template< class Key, class Value, class Cmp >
  std::pair< Iterator< Key, Value >, Iterator< Key, Value > > AvlTree< Key, Value, Cmp >::equalRange(const Key& key)
  {
    return std::make_pair(lowerBound(key), upperBound(key));
  }

  template< class Key, class Value, class Cmp >
  std::pair< Citerator< Key, Value >, Citerator< Key, Value > > AvlTree< Key, Value, Cmp >::equalRange(const Key& key) const
  {
    return std::make_pair(lowerBound(key), upperBound(key));
  }

  template< class Key, class Value, class Cmp >
  Iterator< Key, Value, Cmp > AvlTree< Key, Value, Cmp >::erase(Iterator< Key, Value, Cmp > it)
  {
    auto it2 = it;
    it2++;
    root_ = eraseFrom(root_, it->first);
    size_--;
    return it2;
  }

  template< class Key, class Value, class Cmp >
  size_t AvlTree< Key, Value, Cmp >::erase(const Key& key)
  {
    Iterator< Key, Value, Cmp > it = find(key);
    if (it != end())
    {
      erase(it);
      return 1;
    }
    return 0;
  }

  template< class Key, class Value, class Cmp >
  AvlTree< Key, Value, Cmp >::AvlTree(const AvlTree< Key, Value, Cmp >& other):
    AvlTree(other.cbegin(), other.cend())
  {}

  template< class Key, class Value, class Cmp >
  AvlTree< Key, Value, Cmp >::AvlTree(AvlTree< Key, Value, Cmp >&& other) noexcept:
    root_(other.root_),
    size_(other.size_),
    cmp_(other.cmp_)
  {
    other.root_ = nullptr;
    other.size_ = 0;
  }

  template< class Key, class Value, class Cmp >
  void AvlTree< Key, Value, Cmp >::fixHeight(TreeNode< Key, Value >* node)
  {
    node->height = 1 + std::max(height(node->left), height(node->right));
  }

  template< class Key, class Value, class Cmp >
  size_t AvlTree< Key, Value, Cmp >::height(TreeNode< Key, Value >* node)
  {
    return node ? node->height : 0;
  }

  template< class Key, class Value, class Cmp >
  TreeNode< Key, Value >* AvlTree< Key, Value, Cmp >::rotateLeft(TreeNode< Key, Value >* const root)
  {
    if (root == nullptr || root->right == nullptr)
    {
      throw std::logic_error("<INVALID ROTATE>");
    }
    TreeNode< Key, Value >* rotate_tree = root->right;
    if (root->parent != nullptr)
    {
      if (root->parent->left == root)
      {
        root->parent->left = rotate_tree;
      }
      else
      {
        root->parent->right = rotate_tree;
      }
    }
    root->right = rotate_tree->left;
    if (rotate_tree->left != nullptr)
    {
      rotate_tree->left->parent = root;
    }
    rotate_tree->left = root;
    rotate_tree->parent = root->parent;
    root->parent = rotate_tree;
    fixHeight(root);
    fixHeight(rotate_tree);
    return rotate_tree;
  }

  template< class Key, class Value, class Cmp >
  TreeNode< Key, Value >* AvlTree< Key, Value, Cmp >::rotateRight(TreeNode< Key, Value >* const root)
  {
    if (root == nullptr || root->left == nullptr)
    {
      throw std::logic_error("<INVALID ROTATE>");
    }
    TreeNode< Key, Value >* rotate_tree = root->left;
    if (root->parent != nullptr)
    {
      if (root->parent->left == root)
      {
        root->parent->left = rotate_tree;
      }
      else
      {
        root->parent->right = rotate_tree;
      }
    }
    root->left = rotate_tree->right;
    if (rotate_tree->right != nullptr)
    {
      rotate_tree->right->parent = root;
    }
    rotate_tree->right = root;
    rotate_tree->parent = root->parent;
    root->parent = rotate_tree;
    fixHeight(root);
    fixHeight(rotate_tree);
    return rotate_tree;
  }

  template< class Key, class Value, class Cmp >
  Citerator< Key, Value, Cmp > AvlTree< Key, Value, Cmp >::cbegin() const
  {
    return Citerator< Key, Value, Cmp >(findMin(root_));
  }

  template< class Key, class Value, class Cmp >
  Iterator< Key, Value, Cmp > AvlTree< Key, Value, Cmp >::begin()
  {
    return Iterator< Key, Value, Cmp >(findMin(root_));
  }

  template< class Key, class Value, class Cmp >
  Citerator< Key, Value, Cmp > AvlTree< Key, Value, Cmp >::cend() const
  {
    return Citerator< Key, Value, Cmp >();
  }

  template< class Key, class Value, class Cmp >
  Iterator< Key, Value, Cmp > AvlTree< Key, Value, Cmp >::end()
  {
    return Iterator< Key, Value, Cmp >();
  }

  template< class Key, class Value, class Cmp >
  TreeNode< Key, Value >* AvlTree< Key, Value, Cmp >::findMin(TreeNode< Key, Value >* root) const
  {
    while (root != nullptr && root->left != nullptr)
    {
      root = root->left;
    }
    return root;
  }

  template< class Key, class Value, class Cmp >
  TreeNode< Key, Value >* AvlTree< Key, Value, Cmp >::findMax(TreeNode< Key, Value >* root) const
  {
    while (root != nullptr && root->right != nullptr)
    {
      root = root->right;
    }
    return root;
  }

  template< class Key, class Value, class Cmp >
  Value& AvlTree< Key, Value, Cmp >::at(const Key& key)
  {
    return const_cast< Value& >(static_cast< const AvlTree< Key, Value, Cmp >* >(this)->at(key));
  }

  template< class Key, class Value, class Cmp >
  const Value& AvlTree< Key, Value, Cmp >::at(const Key& key) const
  {
    Citerator< Key, Value, Cmp > node = find(key);
    if (node != cend())
    {
      return node->second;
    }
    throw std::out_of_range("<INVALID COMMAND>");
  }

  template< class Key, class Value, class Cmp >
  Value& AvlTree< Key, Value, Cmp >::operator[](const Key& key)
  {
    Iterator< Key, Value, Cmp > node = insert(std::make_pair(key, Value()));
    return node->second;
  }

  template< class Key, class Value, class Cmp >
  const Value& AvlTree< Key, Value, Cmp >::operator[](const Key& key) const
  {
    Citerator< Key, Value, Cmp > node = find(key);
    return node->second;
  }

  template< class Key, class Value, class Cmp >
  Iterator< Key, Value, Cmp > AvlTree< Key, Value, Cmp >::insert(const std::pair< Key, Value > & pair)
  {
    return insertSingle(pair.first, pair);
  }

  template< class Key, class Value, class Cmp >
  Iterator< Key, Value, Cmp > AvlTree< Key, Value, Cmp >::insert(std::pair< Key, Value > && pair)
  {
    return insertSingle(pair.first, std::move(pair));
  }

  template< class Key, class Value, class Cmp >
  template< class... Args >
  Iterator< Key, Value, Cmp > AvlTree< Key, Value, Cmp >::insertSingle(const Key& key, Args&&... args)
  {
    auto pair = insertCmp(root_, key, std::forward< Args >(args)...);
    root_ = pair.first;
    if (pair.second.second)
    {
      size_++;
    }
    return Iterator< Key, Value >(pair.second.first);
  }

  template< class Key, class Value, class Cmp >
  TreeNode< Key, Value >* AvlTree< Key, Value, Cmp >::balance(TreeNode< Key, Value >* root)
  {
    int balance = height(root->left) - height(root->right);
    if (balance > 1)
    {
      if (height(root->left->left) < height(root->left->right))
      {
        root->left = rotateLeft(root->left);
      }
      return rotateRight(root);
    }
    if (balance < -1)
    {
      if (height(root->right->right) < height(root->right->left))
      {
        root->right = rotateRight(root->right);
      }
      return rotateLeft(root);
    }
    return root;
  }

  template< class Key, class Value, class Cmp >
  template< class... Args >
  auto AvlTree< Key, Value, Cmp >::insertCmp(TreeNode< Key, Value >* root, const Key& key, Args&&... args) -> pair_t
  {
    TreeNode< Key, Value >* inserted = nullptr;
    if (root == nullptr)
    {
      TreeNode< Key, Value >* new_node = new TreeNode< Key, Value >(std::forward< Args >(args)...);
      return std::make_pair(new_node, std::make_pair(new_node, true));
    }
    else if (cmp_(key, root->data.first))
    {
      auto pair = insertCmp(root->left, key, std::forward< Args >(args)...);
      root->left = pair.first;
      inserted = pair.second.first;
      if (root->left->parent == nullptr)
      {
        root->left->parent = root;
      }
      fixHeight(root);
      return std::make_pair(balance(root), std::make_pair(inserted, pair.second.second));
    }
    else if (cmp_(root->data.first, key))
    {
      auto pair = insertCmp(root->right, key, std::forward< Args >(args)...);
      root->right = pair.first;
      inserted = pair.second.first;
      if (root->right->parent == nullptr)
      {
        root->right->parent = root;
      }
      fixHeight(root);
      return std::make_pair(balance(root), std::make_pair(inserted, pair.second.second));
    }
    return std::make_pair(root, std::make_pair(root, false));
  }

  template< class Key, class Value, class Cmp >
  size_t AvlTree< Key, Value, Cmp >::count(const Key& k) const
  {
    size_t count = 0;
    for (auto it = cbegin(); it != cend(); ++it)
    {
      if (!cmp_(k, it->first) && !cmp_(it->first, k))
      {
        ++count;
      }
    }
    return count;
  }

  template< class Key, class Value, class Cmp >
  Citerator< Key, Value, Cmp > AvlTree< Key, Value, Cmp >::find(const Key& key) const
  {
    TreeNode< Key, Value >* root = root_;
    while (root != nullptr && key != root->data.first)
    {
      if (cmp_(key, root->data.first))
      {
        root = root->left;
      }
      else
      {
        root = root->right;
      }
    }
    return Citerator< Key, Value, Cmp >(root);
  }

  template< class Key, class Value, class Cmp >
  Iterator< Key, Value, Cmp > AvlTree< Key, Value, Cmp >::find(const Key& key)
  {
    TreeNode< Key, Value >* root = root_;
    while (root != nullptr && key != root->data.first)
    {
      if (cmp_(key, root->data.first))
      {
        root = root->left;
      }
      else
      {
        root = root->right;
      }
    }
    return Iterator< Key, Value, Cmp >(root);
  }

  template< class Key, class Value, class Cmp >
  void AvlTree< Key, Value, Cmp >::clear()
  {
    clearFrom(root_);
    root_ = nullptr;
    size_ = 0;
  }

  template< class Key, class Value, class Cmp >
  void AvlTree< Key, Value, Cmp >::clearFrom(TreeNode< Key, Value >* root)
  {
    if (root)
    {
      clearFrom(root->left);
      clearFrom(root->right);
      delete root;
    }
  }

  template< class Key, class Value, class Cmp >
  template< typename F >
  F AvlTree< Key, Value, Cmp >::traverseLnr(F f) const
  {
    if (empty())
    {
      throw std::logic_error("<EMPTY>");
    }
    Stack< const TreeNode< Key, Value >* > stack;
    const TreeNode< Key, Value >* temp = root_;
    while (!stack.empty() || temp)
    {
      while (temp)
      {
        stack.push(temp);
        temp = temp->left;
      }
      temp = stack.top();
      stack.pop();
      f(temp->data);
      temp = temp->right;
    }
    return f;
  }

  template< class Key, class Value, class Cmp >
  template< typename F >
  F AvlTree< Key, Value, Cmp >::traverseRnl(F f) const
  {
    if (empty())
    {
      throw std::logic_error("<EMPTY>");
    }
    Stack< const TreeNode< Key, Value >* > stack;
    const TreeNode< Key, Value >* temp = root_;
    while (!stack.empty() || temp)
    {
      while (temp)
      {
        stack.push(temp);
        temp = temp->right;
      }
      temp = stack.top();
      stack.pop();
      f(temp->data);
      temp = temp->left;
    }
    return f;
  }

  template< class Key, class Value, class Cmp >
  template< typename F >
  F AvlTree< Key, Value, Cmp >::traverseBreadth(F f) const
  {
    if (empty())
    {
      throw std::logic_error("<EMPTY>");
    }
    Queue< const TreeNode< Key, Value >* > queue;
    queue.push(root_);
    while (!queue.empty())
    {
      const TreeNode< Key, Value >* temp = queue.front();
      queue.pop();
      f(temp->data);
      if (temp->left)
      {
        queue.push(temp->left);
      }
      if (temp->right)
      {
        queue.push(temp->right);
      }
    }
    return f;
  }


  template< class Key, class Value, class Cmp >
  template< typename F >
  F AvlTree< Key, Value, Cmp >::traverseLnr(F f)
  {
    if (empty())
    {
      throw std::logic_error("<EMPTY>");
    }
    Stack< TreeNode< Key, Value >* > stack;
    TreeNode< Key, Value >* temp = root_;
    while (!stack.empty() || temp)
    {
      while (temp)
      {
        stack.push(temp);
        temp = temp->left;
      }
      temp = stack.top();
      stack.pop();
      f(temp->data);
      temp = temp->right;
    }
    return f;
  }

  template< class Key, class Value, class Cmp >
  template< typename F >
  F AvlTree< Key, Value, Cmp >::traverseRnl(F f)
  {
    if (empty())
    {
      throw std::logic_error("<EMPTY>");
    }
    Stack< TreeNode< Key, Value >* > stack;
    TreeNode< Key, Value >* temp = root_;
    while (!stack.empty() || temp)
    {
      while (temp)
      {
        stack.push(temp);
        temp = temp->right;
      }
      temp = stack.top();
      stack.pop();
      f(temp->data);
      temp = temp->left;
    }
    return f;
  }

  template< class Key, class Value, class Cmp >
  template< typename F >
  F AvlTree< Key, Value, Cmp >::traverseBreadth(F f)
  {
    if (empty())
    {
      throw std::logic_error("<EMPTY>");
    }
    Queue< TreeNode< Key, Value >* > queue;
    queue.push(root_);
    while (!queue.empty())
    {
      TreeNode< Key, Value >* temp = queue.front();
      queue.pop();
      f(temp->data);
      if (temp->left)
      {
        queue.push(temp->left);
      }
      if (temp->right)
      {
        queue.push(temp->right);
      }
    }
    return f;
  }
}

#endif
