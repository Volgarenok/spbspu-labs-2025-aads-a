#ifndef TREE_HPP
#define TREE_HPP

#include <functional>
#include <stdexcept>

#include <queue.hpp>
#include <stack.hpp>
#include "node.hpp"
#include "tree_iterators.hpp"

namespace zholobov {

  template < typename Key, typename T, typename Compare = std::less< Key > >
  class Tree {
  public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair< const Key, T >;
    using reference = value_type&;
    using const_reference = const value_type&;
    using node_type = Node< const Key, T >;
    using iterator = TreeIterator< node_type >;
    using const_iterator = TreeConstIterator< node_type >;
    using size_type = size_t;

    Tree();
    explicit Tree(const Compare& comp);
    Tree(const Tree< Key, T, Compare >& other);
    Tree(Tree< Key, T, Compare >&& other);

    template < typename InputIt >
    Tree(InputIt first, InputIt last, const Compare& comp = Compare());

    explicit Tree(std::initializer_list< value_type > init, const Compare& comp = Compare());

    ~Tree();

    Tree& operator=(const Tree& other);
    Tree& operator=(Tree&& other) noexcept;

    Tree& operator=(std::initializer_list< value_type > ilist);

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;
    size_type size() const noexcept;

    mapped_type& operator[](const key_type& key);
    mapped_type& at(const key_type& key);
    const mapped_type& at(const key_type& key) const;

    template < class... Args >
    std::pair< iterator, bool > emplace(Args&&... args);
    template < class... Args >
    iterator emplace_hint(const_iterator position, Args&&... args);

    std::pair< iterator, bool > insert(const value_type& value);
    iterator insert(const_iterator pos, const value_type& value);
    template < class InputIt >
    void insert(InputIt first, InputIt last);

    iterator erase(iterator position);
    iterator erase(const_iterator position);
    size_type erase(const key_type& key);
    iterator erase(const_iterator first, const_iterator last);

    void swap(Tree&) noexcept;
    void clear() noexcept;

    iterator find(const key_type& key);
    const_iterator find(const key_type& key) const;

    size_type count(const key_type& x) const;

    iterator lower_bound(const key_type& key);
    const_iterator lower_bound(const key_type& key) const;
    iterator upper_bound(const key_type& key);
    const_iterator upper_bound(const key_type& key) const;

    std::pair< iterator, iterator > equal_range(const key_type& key);
    std::pair< const_iterator, const_iterator > equal_range(const key_type& key) const;

    template < typename F >
    F traverse_lnr(F f);
    template < typename F >
    F traverse_lnr(F f) const;
    template < typename F >
    F traverse_rnl(F f);
    template < typename F >
    F traverse_rnl(F f) const;
    template < typename F >
    F traverse_breadth(F f);
    template < typename F >
    F traverse_breadth(F f) const;

  private:
    int height(node_type* node) const;
    void updateHeight(node_type* node);
    int getBalance(node_type* node) const;
    node_type* rotateRight(node_type* node);
    node_type* rotateLeft(node_type* node);
    node_type* rebalance(node_type* node);
    void rebalanceToRoot(node_type* fromNode);
    void transplant(node_type* u, node_type* v);
    void clearTree(node_type* node);
    node_type* findByKey(key_type key) const;
    node_type* findLowerBound(key_type key) const;
    node_type* findUpperBound(key_type key) const;

    node_type* fakeRoot_;
    Compare cmp_;
    size_type size_;
  };

  template < typename Key, typename T, typename Compare >
  Tree< Key, T, Compare >::Tree():
    Tree(Compare())
  {}

  template < typename Key, typename T, typename Compare >
  Tree< Key, T, Compare >::Tree(const Compare& comp):
    fakeRoot_(static_cast< node_type* >(::operator new(sizeof(node_type)))),
    cmp_(comp),
    size_(0)
  {
    fakeRoot_->parent = nullptr;
    fakeRoot_->left = nullptr;
    fakeRoot_->right = nullptr;
    fakeRoot_->height = -1;
  }

  template < typename Key, typename T, typename Compare >
  Tree< Key, T, Compare >::Tree(const Tree< Key, T, Compare >& other):
    Tree(other.cmp_)
  {
    for (const auto& elem: other) {
      insert(elem);
    }
  }

  template < typename Key, typename T, typename Compare >
  Tree< Key, T, Compare >::Tree(Tree< Key, T, Compare >&& other):
    Tree()
  {
    swap(other);
  }

  template < typename Key, typename T, typename Compare >
  template < typename InputIt >
  Tree< Key, T, Compare >::Tree(InputIt first, InputIt last, const Compare& comp):
    Tree(comp)
  {
    for (auto it = first; it != last; ++it) {
      insert(*it);
    }
  }

  template < typename Key, typename T, typename Compare >
  Tree< Key, T, Compare >::Tree(std::initializer_list< value_type > init, const Compare& comp):
    Tree(comp)
  {
    auto last = std::cend(init);
    for (auto it = std::cbegin(init); it != last; ++it) {
      insert(*it);
    }
  }

  template < typename Key, typename T, typename Compare >
  Tree< Key, T, Compare >::~Tree()
  {
    clear();
    ::operator delete(fakeRoot_);
  }

  template < typename Key, typename T, typename Compare >
  Tree< Key, T, Compare >& Tree< Key, T, Compare >::operator=(const Tree& other)
  {
    if (this != std::addressof(other)) {
      Tree< Key, T, Compare > temp(other);
      swap(temp);
    }
    return *this;
  }

  template < typename Key, typename T, typename Compare >
  Tree< Key, T, Compare >& Tree< Key, T, Compare >::operator=(Tree&& other) noexcept
  {
    if (this != std::addressof(other)) {
      clear();
      swap(other);
    }
    return *this;
  }

  template < typename Key, typename T, typename Compare >
  Tree< Key, T, Compare >& Tree< Key, T, Compare >::operator=(std::initializer_list< value_type > ilist)
  {
    Tree< Key, T, Compare > temp(cmp_);
    for (const auto& elem: ilist) {
      temp.insert(elem);
    }
    swap(temp);
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::iterator Tree< Key, T, Compare >::begin() noexcept
  {
    if (empty()) {
      return end();
    }
    node_type* cur = fakeRoot_;
    while (cur->left != nullptr) {
      cur = cur->left;
    }
    return iterator(cur);
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::const_iterator Tree< Key, T, Compare >::begin() const noexcept
  {
    return cbegin();
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::iterator Tree< Key, T, Compare >::end() noexcept
  {
    return iterator(fakeRoot_);
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::const_iterator Tree< Key, T, Compare >::end() const noexcept
  {
    return cend();
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::const_iterator Tree< Key, T, Compare >::cbegin() const noexcept
  {
    if (empty()) {
      return end();
    }
    node_type* cur = fakeRoot_;
    while (cur->left != nullptr) {
      cur = cur->left;
    }
    return const_iterator(cur);
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::const_iterator Tree< Key, T, Compare >::cend() const noexcept
  {
    return const_iterator(fakeRoot_);
  }

  template < typename Key, typename T, typename Compare >
  bool Tree< Key, T, Compare >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::size_type Tree< Key, T, Compare >::size() const noexcept
  {
    return size_;
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::mapped_type& Tree< Key, T, Compare >::operator[](const key_type& key)
  {
    return insert(std::make_pair(key, T())).first->second;
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::mapped_type& Tree< Key, T, Compare >::at(const key_type& key)
  {
    auto result = find(key);
    if (result == end()) {
      throw std::out_of_range("Key not found");
    }
    return result->second;
  }

  template < typename Key, typename T, typename Compare >
  const typename Tree< Key, T, Compare >::mapped_type& Tree< Key, T, Compare >::at(const key_type& key) const
  {
    auto result = find(key);
    if (result == cend()) {
      throw std::out_of_range("Key not found");
    }
    return result->second;
  }

  template < typename Key, typename T, typename Compare >
  template < class... Args >
  std::pair< typename Tree< Key, T, Compare >::iterator, bool > Tree< Key, T, Compare >::emplace(Args&&... args)
  {
    value_type value(std::forward< Args >(args)...);
    node_type* parent = fakeRoot_;
    node_type* cur = fakeRoot_->left;
    if (!cur) {
      auto newNode = new node_type(std::move(value));
      fakeRoot_->left = newNode;
      newNode->parent = fakeRoot_;
      ++size_;
      return {newNode, true};
    }

    auto& key = value.first;
    bool isLeft = false;
    while (cur) {
      parent = cur;
      if (cmp_(key, cur->data.first)) {
        cur = cur->left;
        isLeft = true;
      } else if (cmp_(cur->data.first, key)) {
        cur = cur->right;
        isLeft = false;
      } else {
        return {iterator(cur), false};
      }
    }

    auto newNode = new node_type(std::move(value));
    newNode->parent = parent;
    if (isLeft) {
      parent->left = newNode;
    } else {
      parent->right = newNode;
    }

    rebalanceToRoot(newNode);
    ++size_;
    return {newNode, true};
  }

  template < typename Key, typename T, typename Compare >
  template < class... Args >
  typename Tree< Key, T, Compare >::iterator
  Tree< Key, T, Compare >::emplace_hint(const_iterator position, Args&&... args)
  {
    value_type value(std::forward< Args >(args)...);
    auto& key = value.first;

    node_type* hint = position.node_;
    if (hint != nullptr && hint != fakeRoot_) {
      node_type* next = (++position).node_;
      if (cmp_(hint->data.first, key) && (position == cend() || cmp_(key, next->data.first))) {
        auto newNode = new node_type(std::move(value));
        if (hint->right == nullptr) {
          hint->right = newNode;
        } else {
          hint = hint->right;
          while (hint->left != nullptr) {
            hint = hint->left;
          }
          hint->left = newNode;
        }
        newNode->parent = hint;
        rebalanceToRoot(newNode);
        ++size_;
        return iterator(newNode);
      }
    }
    return emplace(std::move(value)).first;
  }

  template < typename Key, typename T, typename Compare >
  std::pair< typename Tree< Key, T, Compare >::iterator, bool >
  Tree< Key, T, Compare >::insert(const value_type& value)
  {
    return emplace(value);
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::iterator
  Tree< Key, T, Compare >::insert(const_iterator pos, const value_type& value)
  {
    return emplace_hint(pos, value);
  }

  template < typename Key, typename T, typename Compare >
  template < class InputIt >
  void Tree< Key, T, Compare >::insert(InputIt first, InputIt last)
  {
    for (; first != last; first++) {
      emplace(*first);
    }
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::iterator Tree< Key, T, Compare >::erase(iterator position)
  {
    erase(const_iterator(position));
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::iterator Tree< Key, T, Compare >::erase(const_iterator position)
  {
    if (position == cend()) {
      return end();
    }
    node_type* cur = position.node_;
    assert(cur != nullptr);
    ++position;
    node_type* rebalanceFrom = nullptr;

    if (cur->left == nullptr) {
      rebalanceFrom = cur->parent;
      transplant(cur, cur->right);
    } else if (cur->right == nullptr) {
      rebalanceFrom = cur->parent;
      transplant(cur, cur->left);
    } else {
      node_type* successor = cur->right;
      while (successor->left != nullptr) {
        successor = successor->left;
      }
      if (successor->parent != cur) {
        transplant(successor, successor->right);
        successor->right = cur->right;
        successor->right->parent = successor;
      }
      transplant(cur, successor);
      successor->left = cur->left;
      successor->left->parent = successor;
      rebalanceFrom = successor;
    }
    delete cur;
    --size_;
    rebalanceToRoot(rebalanceFrom);
    return iterator(position.node_);
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::size_type Tree< Key, T, Compare >::erase(const key_type& key)
  {
    const_iterator it = find(key);
    if (it != cend()) {
      erase(it);
      return 1;
    }
    return 0;
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::iterator Tree< Key, T, Compare >::erase(const_iterator first, const_iterator last)
  {
    while (first != last) {
      first = erase(first);
    }
    return iterator(last.node_);
  }

  template < typename Key, typename T, typename Compare >
  void Tree< Key, T, Compare >::swap(Tree& other) noexcept
  {
    std::swap(fakeRoot_, other.fakeRoot_);
    std::swap(cmp_, other.cmp_);
    std::swap(size_, other.size_);
  }

  template < typename Key, typename T, typename Compare >
  void Tree< Key, T, Compare >::clear() noexcept
  {
    clearTree(fakeRoot_->left);
    size_ = 0;
    fakeRoot_->left = nullptr;
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::iterator Tree< Key, T, Compare >::find(const key_type& key)
  {
    node_type* node = findByKey(key);
    return (node != nullptr) ? iterator(node) : end();
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::const_iterator Tree< Key, T, Compare >::find(const key_type& key) const
  {
    node_type* node = findByKey(key);
    return (node != nullptr) ? const_iterator(node) : cend();
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::size_type Tree< Key, T, Compare >::count(const key_type& key) const
  {
    node_type* node = findByKey(key);
    return (node != nullptr) ? 1 : 0;
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::iterator Tree< Key, T, Compare >::lower_bound(const key_type& key)
  {
    node_type* result = findLowerBound(key);
    return (result != nullptr) ? iterator(result) : end();
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::const_iterator Tree< Key, T, Compare >::lower_bound(const key_type& key) const
  {
    node_type* result = findLowerBound(key);
    return (result != nullptr) ? const_iterator(result) : cend();
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::iterator Tree< Key, T, Compare >::upper_bound(const key_type& key)
  {
    node_type* result = findUpperBound(key);
    return (result != nullptr) ? iterator(result) : end();
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::const_iterator Tree< Key, T, Compare >::upper_bound(const key_type& key) const
  {
    node_type* result = findUpperBound(key);
    return (result != nullptr) ? const_iterator(result) : cend();
  }

  template < typename Key, typename T, typename Compare >
  std::pair< typename Tree< Key, T, Compare >::iterator, typename Tree< Key, T, Compare >::iterator >
  Tree< Key, T, Compare >::equal_range(const key_type& key)
  {
    return std::make_pair(lower_bound(key), upper_bound(key));
  }

  template < typename Key, typename T, typename Compare >
  std::pair< typename Tree< Key, T, Compare >::const_iterator, typename Tree< Key, T, Compare >::const_iterator >
  Tree< Key, T, Compare >::equal_range(const key_type& key) const
  {
    return std::make_pair(lower_bound(key), upper_bound(key));
  }

  template < typename Key, typename T, typename Compare >
  template < typename F >
  F Tree< Key, T, Compare >::traverse_lnr(F f)
  {
    zholobov::Stack< node_type* > nodeStack;
    node_type* current = fakeRoot_->left;
    while ((current != nullptr) || !nodeStack.empty()) {
      while (current != nullptr) {
        nodeStack.push(current);
        current = current->left;
      }
      current = nodeStack.top();
      nodeStack.pop();
      f(current->data);
      current = current->right;
    }
    return f;
  }

  template < typename Key, typename T, typename Compare >
  template < typename F >
  F Tree< Key, T, Compare >::traverse_lnr(F f) const
  {
    zholobov::Stack< const node_type* > nodeStack;
    const node_type* current = fakeRoot_->left;
    while ((current != nullptr) || !nodeStack.empty()) {
      while (current != nullptr) {
        nodeStack.push(current);
        current = current->left;
      }
      current = nodeStack.top();
      nodeStack.pop();
      f(current->data);
      current = current->right;
    }
    return f;
  }

  template < typename Key, typename T, typename Compare >
  template < typename F >
  F Tree< Key, T, Compare >::traverse_rnl(F f)
  {
    zholobov::Stack< node_type* > nodeStack;
    node_type* current = fakeRoot_->left;
    while ((current != nullptr) || !nodeStack.empty()) {
      while (current != nullptr) {
        nodeStack.push(current);
        current = current->right;
      }
      current = nodeStack.top();
      nodeStack.pop();
      f(current->data);
      current = current->left;
    }
    return f;
  }

  template < typename Key, typename T, typename Compare >
  template < typename F >
  F Tree< Key, T, Compare >::traverse_rnl(F f) const
  {
    zholobov::Stack< const node_type* > nodeStack;
    const node_type* current = fakeRoot_->left;
    while ((current != nullptr) || !nodeStack.empty()) {
      while (current != nullptr) {
        nodeStack.push(current);
        current = current->right;
      }
      current = nodeStack.top();
      nodeStack.pop();
      f(current->data);
      current = current->left;
    }
    return f;
  }

  template < typename Key, typename T, typename Compare >
  template < typename F >
  F Tree< Key, T, Compare >::traverse_breadth(F f)
  {
    node_type* root = fakeRoot_->left;
    if (root != nullptr) {
      zholobov::Queue< node_type* > nodeQueue;
      nodeQueue.push(root);
      while (!nodeQueue.empty()) {
        node_type* current = nodeQueue.front();
        nodeQueue.pop();
        f(current->data);
        if (current->left != nullptr) {
          nodeQueue.push(current->left);
        }
        if (current->right != nullptr) {
          nodeQueue.push(current->right);
        }
      }
    }
    return f;
  }

  template < typename Key, typename T, typename Compare >
  template < typename F >
  F Tree< Key, T, Compare >::traverse_breadth(F f) const
  {
    const node_type* root = fakeRoot_->left;
    if (root != nullptr) {
      zholobov::Queue< const node_type* > nodeQueue;
      nodeQueue.push(root);
      while (!nodeQueue.empty()) {
        const node_type* current = nodeQueue.front();
        nodeQueue.pop();
        f(current->data);
        if (current->left != nullptr) {
          nodeQueue.push(current->left);
        }
        if (current->right != nullptr) {
          nodeQueue.push(current->right);
        }
      }
    }
    return f;
  }

  template < typename Key, typename T, typename Compare >
  int Tree< Key, T, Compare >::height(node_type* node) const
  {
    return node ? node->height : 0;
  }

  template < typename Key, typename T, typename Compare >
  void Tree< Key, T, Compare >::updateHeight(node_type* node)
  {
    if (node) {
      node->height = 1 + std::max(height(node->left), height(node->right));
    }
  }

  template < typename Key, typename T, typename Compare >
  int Tree< Key, T, Compare >::getBalance(node_type* node) const
  {
    return node ? height(node->left) - height(node->right) : 0;
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::node_type* Tree< Key, T, Compare >::rotateRight(node_type* node)
  {
    node_type* l = node->left;
    node->left = l->right;
    if (l->right != nullptr) {
      l->right->parent = node;
    }
    l->right = node;
    l->parent = node->parent;
    node->parent = l;
    updateHeight(node);
    updateHeight(l);
    return l;
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::node_type* Tree< Key, T, Compare >::rotateLeft(node_type* node)
  {
    node_type* r = node->right;
    node->right = r->left;
    if (r->left != nullptr) {
      r->left->parent = node;
    }
    r->left = node;
    r->parent = node->parent;
    node->parent = r;
    updateHeight(node);
    updateHeight(r);
    return r;
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::node_type* Tree< Key, T, Compare >::rebalance(node_type* node)
  {
    updateHeight(node);
    int balance = getBalance(node);
    if (balance > 1) {
      if (getBalance(node->left) < 0)
        node->left = rotateLeft(node->left);
      return rotateRight(node);
    }
    if (balance < -1) {
      if (getBalance(node->right) > 0)
        node->right = rotateRight(node->right);
      return rotateLeft(node);
    }
    return node;
  }

  template < typename Key, typename T, typename Compare >
  void Tree< Key, T, Compare >::rebalanceToRoot(node_type* fromNode)
  {
    node_type* cur = fromNode;
    while (cur != fakeRoot_) {
      node_type* p = cur->parent;
      if ((p == nullptr) || (p == fakeRoot_)) break;

      if (p->left == cur)
        p->left = rebalance(cur);
      else
        p->right = rebalance(cur);

      updateHeight(p);
      cur = p;
    }
    fakeRoot_->left = rebalance(fakeRoot_->left);
  }

  template < typename Key, typename T, typename Compare >
  void Tree< Key, T, Compare >::transplant(node_type* u, node_type* v)
  {
    if (u->parent->left == u) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    if (v != nullptr) {
      v->parent = u->parent;
    }
  }

  template < typename Key, typename T, typename Compare >
  void Tree< Key, T, Compare >::clearTree(node_type* node)
  {
    if (node != nullptr) {
      clearTree(node->left);
      clearTree(node->right);
      delete node;
    }
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::node_type* Tree< Key, T, Compare >::findByKey(key_type key) const
  {
    node_type* cur = fakeRoot_->left;
    while (cur != nullptr) {
      if (cmp_(key, cur->data.first)) {
        cur = cur->left;
      } else if (cmp_(cur->data.first, key)) {
        cur = cur->right;
      } else {
        return cur;
      }
    }
    return nullptr;
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::node_type* Tree< Key, T, Compare >::findLowerBound(key_type key) const
  {
    node_type* cur = fakeRoot_->left;
    node_type* result = nullptr;
    while (cur != nullptr) {
      if (cmp_(cur->data.first, key)) {
        cur = cur->right;
      } else {
        result = cur;
        cur = cur->left;
      }
    }
    return result;
  }

  template < typename Key, typename T, typename Compare >
  typename Tree< Key, T, Compare >::node_type* Tree< Key, T, Compare >::findUpperBound(key_type key) const
  {
    node_type* cur = fakeRoot_->left;
    node_type* result = nullptr;
    while (cur != nullptr) {
      if (cmp_(key, cur->data.first)) {
        result = cur;
        cur = cur->left;
      } else {
        cur = cur->right;
      }
    }
    return result;
  }

}

#endif
