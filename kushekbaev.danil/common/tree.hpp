#ifndef TREE_HPP
#define TREE_HPP

#include <functional>
#include <utility>
#include "constiterator.hpp"
#include "iterator.hpp"
#include "stack.hpp"
#include "queue.hpp"

namespace kushekbaev
{
  template< typename Key, typename Value, typename Cmp = std::less< Key > >
  struct Tree
  {
    template< typename InputIterator >
    using enableIf = std::enable_if_t< std::is_convertible< decltype(*std::declval< InputIterator >()), Value >::value >;
    using It = Iterator< Key, Value, Cmp >;
    using cIt = ConstIterator< Key, Value, Cmp >;
    using pairIt = std::pair< Iterator< Key, Value, Cmp >, Iterator< Key, Value, Cmp > >;
    using paircIt = std::pair< ConstIterator< Key, Value, Cmp >, ConstIterator< Key, Value, Cmp > >;

    Tree();
    Tree(const Tree< Key, Value, Cmp >& other);
    Tree(Tree< Key, Value, Cmp >&& other);
    template< typename InputIterator >
    Tree(InputIterator first, InputIterator last);
    explicit Tree(std::initializer_list< std::pair< Key, Value > > il);
    ~Tree();

    Tree< Key, Value, Cmp >& operator=(const Tree< Key, Value, Cmp >& other);
    Tree< Key, Value, Cmp >& operator=(Tree< Key, Value, Cmp >&& other);
    bool operator==(const Tree< Key, Value, Cmp >& other) const noexcept;
    bool operator!=(const Tree< Key, Value, Cmp >& other) const noexcept;

    It begin() noexcept;
    It end() noexcept;
    cIt begin() const noexcept;
    cIt end() const noexcept;
    cIt cbegin() const noexcept;
    cIt cend() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    Value& operator[](const Key& key);
    const Value& operator[](const Key& key) const;

    void clear() noexcept;

    void swap(Tree& other);

    std::pair< It, bool > insert(const std::pair< Key, Value >& value);
    std::pair< It, bool > insert(std::pair< Key, Value >&& value);
    template< typename InputIterator >
    void insert(InputIterator first, InputIterator last);
    It insert(cIt hint, const std::pair< Key, Value >& value);
    It insert(It hint, const std::pair< Key, Value >& value);
    It erase(It position);
    It erase(cIt position);
    It erase(It first, It last);
    It erase(cIt first, cIt last);
    size_t erase(const Key& key);
    template< typename... Args >
    std::pair< It, bool > emplace(Args&&... args);
    template< typename... Args >
    It emplace_hint(cIt hint, Args&&... args);

    It find(const Key& key) noexcept;
    cIt find(const Key& key) const noexcept;

    size_t count(const Key& key) const noexcept;

    std::pair< It, It > equal_range(const Key& key);
    std::pair< cIt, cIt > equal_range(const Key& key) const;
    It lower_bound(const Key& key);
    cIt lower_bound(const Key& key) const;
    It upper_bound(const Key& key);
    cIt upper_bound(const Key& key) const;

    template< typename F >
    F traverse_lnr(F f) const;
    template< typename F >
    F traverse_rnl(F f) const;
    template< typename F >
    F traverse_breadth(F f) const;
    template< typename F >
    F traverse_lnr(F f);
    template< typename F >
    F traverse_rnl(F f);
    template< typename F >
    F traverse_breadth(F f);

    private:
      using node_t = kushekbaev::TreeNode< Key, Value, Cmp >;
      node_t* fakeroot_;
      node_t* root_;
      long long int size_;
      Cmp cmp_;
      void killChildrenOf(node_t* node);
      node_t* copySubtree(node_t* node, node_t* parent);
  };

  template< typename Key, typename Value, typename Cmp >
  Tree< Key, Value, Cmp>::Tree():
    fakeroot_(reinterpret_cast< node_t* >(std::malloc(sizeof(node_t)))),
    root_(fakeroot_),
    size_(0)
  {
    fakeroot_->left = fakeroot_;
    fakeroot_->right = fakeroot_;
    fakeroot_->parent = fakeroot_;
  }

  template< typename Key, typename Value, typename Cmp >
  Tree< Key, Value, Cmp>::Tree(const Tree< Key, Value, Cmp >& other):
    fakeroot_(reinterpret_cast< node_t* >(std::malloc(sizeof(node_t)))),
    root_(fakeroot_),
    size_(0)
  {
    if (other.root_ == other.fakeroot_)
    {
      return;
    }
    fakeroot_->left = fakeroot_;
    fakeroot_->right = fakeroot_;
    fakeroot_->parent = fakeroot_;
    if (!other.empty())
    {
      root_ = copySubtree(other.root_, fakeroot_);
      size_ = other.size_;
      node_t* minNode = root_;
      while (minNode->left && minNode->left != other.fakeroot_)
      {
        minNode = minNode->left;
      }
      fakeroot_->left = minNode;
      node_t* maxNode = root_;
      while (maxNode->right && maxNode->right != other.fakeroot_)
      {
        maxNode = maxNode->right;
      }
      fakeroot_->right = maxNode;
    }
  }

  template< typename Key, typename Value, typename Cmp >
  Tree< Key, Value, Cmp>::Tree(Tree< Key, Value, Cmp >&& other):
    fakeroot_(std::exchange(other.fakeroot_, nullptr)),
    root_(std::exchange(other.root_, nullptr)),
    size_(std::exchange(other.size_, 0)),
    cmp_(std::move(other.cmp_))
  {
    other.fakeroot_ = nullptr;
    other.root_ = nullptr;
    other.size_ = 0;
  }

  template< typename Key, typename Value, typename Cmp >
  template< typename InputIterator >
  Tree< Key, Value, Cmp >::Tree(InputIterator first, InputIterator last):
    Tree()
  {
    for (auto it = first; it != last; ++it)
    {
      insert(*it);
    }
  }

  template< typename Key, typename Value, typename Cmp >
  Tree< Key, Value, Cmp >::Tree(std::initializer_list< std::pair< Key, Value > > il):
    Tree()
  {
    for (auto it = il.begin(); it != il.end(); ++it)
    {
      insert(*it);
    }
  }

  template< typename Key, typename Value, typename Cmp >
  Tree< Key, Value, Cmp >::~Tree()
  {
    clear();
    std::free(fakeroot_);
  }

  template< typename Key, typename Value, typename Cmp >
  Tree<Key, Value, Cmp >& Tree< Key, Value, Cmp >::operator=(const Tree< Key, Value, Cmp >& other)
  {
    Tree< Key, Value, Cmp > tmp(other);
    swap(tmp);
    return *this;
  }

  template< typename Key, typename Value, typename Cmp >
  Tree<Key, Value, Cmp >& Tree< Key, Value, Cmp >::operator=(Tree< Key, Value, Cmp >&& other)
  {
    Tree< Key, Value, Cmp > tmp(std::move(other));
    swap(tmp);
    return *this;
  }

  template< typename Key, typename Value, typename Cmp >
  bool Tree< Key, Value, Cmp >::operator==(const Tree< Key, Value, Cmp >& other) const noexcept
  {
    if (size() != other.size())
    {
      return false;
    }
    auto it1 = begin();
    auto it2 = other.begin();
    while (it1 != end())
    {
      if (*it1 != *it2)
      {
        return false;
      }
      ++it1;
      ++it2;
    }
    return true;
  }

  template< typename Key, typename Value, typename Cmp >
  bool Tree< Key, Value, Cmp >::operator!=(const Tree< Key, Value, Cmp >& other) const noexcept
  {
    return !(*this == other);
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::It Tree< Key, Value, Cmp >::begin() noexcept
  {
    if (empty())
    {
      return end();
    }
    node_t* current = root_;
    while (current->left && current->left != fakeroot_)
    {
      current = current->left;
    }
    return It(current);
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::It Tree< Key, Value, Cmp >::end() noexcept
  {
    return It(fakeroot_);
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::cIt Tree< Key, Value, Cmp >::begin() const noexcept
  {
    if (empty())
    {
      return end();
    }
    node_t* current = root_;
    while (current->left && current->left != fakeroot_)
    {
      current = current->left;
    }
    return cIt(current);
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::cIt Tree< Key, Value, Cmp >::end() const noexcept
  {
    return cIt(fakeroot_);
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::cIt Tree< Key, Value, Cmp >::cbegin() const noexcept
  {
    if (empty())
    {
      return cend();
    }
    node_t* current = root_;
    while (current->left && current->left != fakeroot_)
    {
      current = current->left;
    }
    return cIt(current);
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::cIt Tree< Key, Value, Cmp >::cend() const noexcept
  {
    return cIt(fakeroot_);
  }

  template< typename Key, typename Value, typename Cmp >
  size_t Tree< Key, Value, Cmp >::size() const noexcept
  {
    return size_;
  }

  template< typename Key, typename Value, typename Cmp >
  bool Tree< Key, Value, Cmp >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename Key, typename Value, typename Cmp >
  Value& Tree< Key, Value, Cmp >::at(const Key& key)
  {
    return const_cast< Value& >(static_cast< const Tree* >(this)->at(key));
  }

  template< typename Key, typename Value, typename Cmp >
  const Value& Tree< Key, Value, Cmp >::at(const Key& key) const
  {
    auto current = find(key);
    if (current == cend())
    {
      throw std::out_of_range("Key hasn't been found!");
    }
    return current->second;
  }

  template< typename Key, typename Value, typename Cmp >
  Value& Tree< Key, Value, Cmp >::operator[](const Key& key)
  {
    auto it = find(key);
    if (it == end())
    {
      auto inserted = emplace(key, Value());
      return inserted.first->second;
    }
    return it->second;
  }

  template< typename Key, typename Value, typename Cmp >
  const Value& Tree< Key, Value, Cmp >::operator[](const Key& key) const
  {
    auto it = find(key);
    return it->second;
  }

  template< typename Key, typename Value, typename Cmp >
  void Tree< Key, Value, Cmp >::clear() noexcept
  {
    killChildrenOf(root_);
    root_ = fakeroot_;
    size_ = 0;
  }

  template< typename Key, typename Value, typename Cmp >
  void Tree< Key, Value, Cmp >::killChildrenOf(node_t* node)
  {
    if (!node || node == fakeroot_)
    {
      return;
    }
    killChildrenOf(node->left);
    killChildrenOf(node->right);
    delete node;
  }

  template< typename Key, typename Value, typename Cmp >
  void Tree< Key, Value, Cmp >::swap(Tree& other)
  {
    std::swap(fakeroot_, other.fakeroot_);
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
    std::swap(cmp_, other.cmp_);
  }

  template<typename Key, typename Value, typename Cmp>
  std::pair< Iterator< Key, Value, Cmp >, bool> Tree< Key, Value, Cmp >::insert(const std::pair< Key, Value >& value)
  {
    return emplace(value);
  }

  template<typename Key, typename Value, typename Cmp>
  std::pair< Iterator< Key, Value, Cmp >, bool> Tree< Key, Value, Cmp >::insert(std::pair< Key, Value >&& value)
  {
    return emplace(std::move(value));
  }

  template< typename Key, typename Value, typename Cmp >
  template< typename InputIterator >
  void Tree< Key, Value, Cmp >::insert(InputIterator first, InputIterator last)
  {
    Tree< Key, Value, Cmp > tmp(*this);
    while (first != last)
    {
      tmp.insert(*first);
      ++first;
    }
    swap(tmp);
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::It Tree< Key, Value, Cmp >::insert(cIt hint, const std::pair< Key, Value >& value)
  {
    return emplace_hint(hint, value);
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::It Tree< Key, Value, Cmp >::insert(It hint, const std::pair<Key, Value>& value)
  {
    return insert(ConstIterator<Key, Value, Cmp>(hint), value);
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::It Tree< Key, Value, Cmp >::erase(It position)
  {
    if (position == end())
    {
      return end();
    }
    node_t* todelete = position.node_;
    node_t* parent = todelete->parent;
    It result = It(parent);
    if (!(todelete->left) && !(todelete->right))
    {
      if (parent->left == todelete)
      {
        parent->left = nullptr;
      }
      else
      {
        parent->right = nullptr;
      }
      result = It(parent);
    }
    else if (!(todelete->left) || !(todelete->right))
    {
      node_t* child = (todelete->left) ? todelete->left : todelete->right;
      if (parent->left == todelete)
      {
        parent->left = child;
      }
      else
      {
        parent->right = child;
      }
      child->parent = parent;
      result = It(child);
    }
    else
    {
      node_t* next = todelete->right;
      while (next->left)
      {
        next = next->left;
      }
      todelete->data = next->data;
      return erase(It(next));
    }
    if (todelete == root_)
    {
      root_ = (parent == fakeroot_) ? parent : fakeroot_;
      result = begin();
    }

    delete todelete;
    --size_;
    return result;
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::It Tree< Key, Value, Cmp >::erase(cIt position)
  {
    if (position == cend())
    {
      return end();
    }
    node_t* todelete = position.node_;
    node_t* parent = todelete->parent;
    It result = It(parent);
    if (!(todelete->left) && !(todelete->right))
    {
      if (parent->left == todelete)
      {
        parent->left = nullptr;
      }
      else
      {
        parent->right = nullptr;
      }
      result = It(parent);
    }
    else if (!(todelete->left) || !(todelete->right))
    {
      node_t* child = (todelete->left) ? todelete->left : todelete->right;
      if (parent->left == todelete)
      {
        parent->left = child;
      }
      else
      {
        parent->right = child;
      }
      child->parent = parent;
      result = It(child);
    }
    else
    {
      node_t* next = todelete->right;
      while (next->left)
      {
        next = next->left;
      }
      todelete->data = next->data;
      return erase(It(next));
    }
    if (todelete == root_)
    {
      root_ = (parent == fakeroot_) ? parent : fakeroot_;
      result = begin();
    }

    delete todelete;
    --size_;
    return result;
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::It Tree< Key, Value, Cmp >::erase(It first, It last)
  {
    for (auto it = first; it != last;)
    {
      it = erase(it);
    }
    return last;
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::It Tree< Key, Value, Cmp >::erase(cIt first, cIt last)
  {
    for (auto it = first; it != last;)
    {
      it = erase(it);
    }
    return last;
  }

  template< typename Key, typename Value, typename Cmp >
  size_t Tree< Key, Value, Cmp >::erase(const Key& key)
  {
    It it = find(key);
    if (it == end())
    {
      return 0;
    }
    erase(it);
    return 1;
  }

  template< typename Key, typename Value, typename Cmp >
  template< typename... Args >
  std::pair< Iterator< Key, Value, Cmp >, bool > Tree< Key, Value, Cmp >::emplace(Args&&... args)
  {
    node_t* newNode = new node_t(std::forward< Args >(args)...);
    newNode->left = nullptr;
    newNode->right = nullptr;
    const Key& key = newNode->data.first;
    if (root_ == fakeroot_)
    {
      root_ = newNode;
      root_->parent = fakeroot_;
      fakeroot_->left = root_;
      fakeroot_->right = root_;
      ++size_;
      return{ It(root_), true };
    }
    node_t* current = root_;
    node_t* parent = nullptr;
    bool isLeft = false;
    while(current && current != fakeroot_)
    {
      parent = current;
      if (cmp_(key, current->data.first))
      {
        current = current->left;
        isLeft = true;
      }
      else if (cmp_(current->data.first, key))
      {
        current = current->right;
        isLeft = false;
      }
      else
      {
        delete newNode;
        return{ It(current), false };
      }
    }
    if (!parent)
    {
      delete newNode;
      throw std::logic_error("Parent is nullptr!");
    }
    newNode->parent = parent;
    if (isLeft)
    {
      parent->left = newNode;
    }
    else
    {
      parent->right = newNode;
    }
    if (cmp_(key, fakeroot_->left->data.first))
    {
      fakeroot_->left = newNode;
    }
    if (cmp_(fakeroot_->right->data.first, key))
    {
      fakeroot_->right = newNode;
    }
    newNode->left = newNode->right = nullptr;
    ++size_;
    return{ It(newNode), true };
  }

  template< typename Key, typename Value, typename Cmp >
  template< typename... Args >
  typename Tree< Key, Value, Cmp >::It Tree< Key, Value, Cmp >::emplace_hint(cIt hint, Args&&... args)
  {
    if (empty())
    {
      return emplace(std::forward< Args >(args)...).first;
    }
    std::pair<Key, Value> newData(std::forward<Args>(args)...);
    auto newKey = newData.first;
    if (hint != cend())
    {
      auto hintKey = hint->first;
      auto nextHint = hint;
      ++hint;
      if ((cmp_(hintKey, newKey) && (nextHint == cend())) || (cmp_(newKey, nextHint->first)))
      {
        node_t* newNode = new node_t(std::move(newData));
        node_t* hintNode = hint.node_;
        newNode->parent = hintNode;
        ++size_;
        return It(newNode);
      }
    }
    return emplace(std::move(newData)).first;
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::It Tree< Key, Value, Cmp >::find(const Key& key) noexcept
  {
    node_t* current = root_;
    while (current && current != fakeroot_)
    {
      if (cmp_(key, current->data.first))
      {
        current = current->left;
      }
      else if (cmp_(current->data.first, key))
      {
        current = current->right;
      }
      else
      {
        return It(current);
      }
    }
    return end();
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::cIt Tree< Key, Value, Cmp >::find(const Key& key) const noexcept
  {
    node_t* current = root_;
    while (current && current != fakeroot_)
    {
      if (cmp_(key, current->data.first))
      {
        current = current->left;
      }
      else if (cmp_(current->data.first, key))
      {
        current = current->right;
      }
      else
      {
        return cIt(current);
      }
    }
    return cend();
  }

  template< typename Key, typename Value, typename Cmp >
  size_t Tree< Key, Value, Cmp >::count(const Key& key) const noexcept
  {
    return (find(key) != cend());
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::pairIt Tree< Key, Value, Cmp >::equal_range(const Key& key)
  {
    return std::make_pair(lower_bound(key), upper_bound(key));
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::paircIt Tree< Key, Value, Cmp >::equal_range(const Key& key) const
  {
    return std::make_pair(lower_bound(key), upper_bound(key));
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::It Tree< Key, Value, Cmp >::lower_bound(const Key& key)
  {
    node_t* current = root_;
    node_t* result = fakeroot_;
    while (current != fakeroot_ && current)
    {
      if (cmp_(current->data.first, key))
      {
        result = current;
        current = current->right;
      }
      else
      {
        current = current->left;
      }
    }
    return It(result);
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::cIt Tree< Key, Value, Cmp >::lower_bound(const Key& key) const
  {
    return cIt(lower_bound(key));
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::It Tree< Key, Value, Cmp >::upper_bound(const Key& key)
  {
    node_t* current = root_;
    node_t* result = fakeroot_;
    while (current != fakeroot_ && current)
    {
      if (cmp_(key, current->data.first))
      {
        result = current;
        current = current->right;
      }
      else
      {
        current = current->left;
      }
    }
    return It(result);
  }

  template< typename Key, typename Value, typename Cmp >
  typename Tree< Key, Value, Cmp >::cIt Tree< Key, Value, Cmp >::upper_bound(const Key& key) const
  {
    return cIt(upper_bound(key));
  }

  template< typename Key, typename Value, typename Cmp >
  template< typename F >
  F Tree< Key, Value, Cmp>::traverse_lnr(F f) const
  {
    if (empty() || root_ == fakeroot_)
    {
      return f;
    }
    Stack< const node_t* > stack;
    const node_t* current = root_;
    while (current || !stack.empty())
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

  template< typename Key, typename Value, typename Cmp >
  template< typename F >
  F Tree< Key, Value, Cmp>::traverse_rnl(F f) const
  {
    if (empty() || root_ == fakeroot_)
    {
      return f;
    }
    Stack< const node_t* > stack;
    const node_t* current = root_;
    while (current || !stack.empty())
    {
      while (current)
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

  template< typename Key, typename Value, typename Cmp >
  template< typename F >
  F Tree< Key, Value, Cmp>::traverse_breadth(F f) const
  {
    if (empty() || root_ == fakeroot_)
    {
      return f;
    }
    Queue< const node_t* > queue;
    queue.push(root_);
    while (!queue.empty())
    {
      const node_t* current = queue.front();
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

  template< typename Key, typename Value, typename Cmp >
  template< typename F >
  F Tree< Key, Value, Cmp>::traverse_lnr(F f)
  {
    if (empty() || root_ == fakeroot_)
    {
      return f;
    }
    Stack< node_t* > stack;
    node_t* current = root_;
    while (current || !stack.empty())
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

  template< typename Key, typename Value, typename Cmp >
  template< typename F >
  F Tree< Key, Value, Cmp>::traverse_rnl(F f)
  {
    if (empty() || root_ == fakeroot_)
    {
      return f;
    }
    Stack< node_t* > stack;
    node_t* current = root_;
    while (current || !stack.empty())
    {
      while (current)
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

  template< typename Key, typename Value, typename Cmp >
  template< typename F >
  F Tree< Key, Value, Cmp>::traverse_breadth(F f)
  {
    if (empty() || root_ == fakeroot_)
    {
      return f;
    }
    Queue< node_t* > queue;
    queue.push(root_);
    while (!queue.empty())
    {
      node_t* current = queue.front();
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

  template<typename Key, typename Value, typename Cmp>
  TreeNode< Key, Value, Cmp >* Tree<Key, Value, Cmp>::copySubtree(node_t* node, node_t* parent)
  {
    if (!node)
    {
      return nullptr;
    }
    node_t* newNode = new node_t(node->data);
    newNode->parent = parent;
    newNode->left = copySubtree(node->left, newNode);
    newNode->right = copySubtree(node->right, newNode);
    return newNode;
  }
}

#endif
