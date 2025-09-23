#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "node.hpp"
#include "iterator.hpp"
#include "constIterator.hpp"
#include <utility>
#include <initializer_list>
#include <stdexcept>
#include <stack.hpp>

namespace averenkov
{

  template < class Key, class Value, class Compare = std::less< Key > >
  class Tree
  {
  public:
    using iterator = Iterator< Key, Value, Compare >;
    using const_iterator = ConstIterator< Key, Value, Compare >;

    Tree();
    explicit Tree(const Compare& cmp);
    Tree(const Tree& other);
    Tree(Tree&& other) noexcept;
    Tree(std::initializer_list< std::pair< const Key, Value > > init, const Compare& cmp = Compare{});
    template< class InputIt >
    Tree(InputIt first, InputIt last, const Compare& cmp = Compare{});
    ~Tree();

    Tree& operator=(const Tree& other);
    Tree& operator=(Tree&& other) noexcept;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    Value& operator[](const Key& key);
    const Value& operator[](const Key& key) const;

    iterator insert(const std::pair< const Key, Value >& value);
    iterator insert(iterator itnode, const Key& k, const Value& v, iterator itpar);
    void push(const Key& key, const Value& value);
    template < class InputIt >
    void insert(InputIt first, InputIt last);
    void insert(std::initializer_list< std::pair< const Key, Value > > ilist);

    iterator erase(iterator pos) noexcept;
    iterator erase(iterator first, iterator last) noexcept;
    size_t erase(const Key& key) noexcept;
    void clear() noexcept;
    void swap(Tree& other) noexcept;

    size_t count(const Key& key) const;
    iterator find(const Key& key);
    const_iterator find(const Key& key) const;
    std::pair< iterator, iterator > equal_range(const Key& key);
    std::pair< const_iterator, const_iterator > equal_range(const Key& key) const;
    iterator lower_bound(const Key& key);
    const_iterator lower_bound(const Key& key) const;
    iterator upper_bound(const Key& key);
    const_iterator upper_bound(const Key& key) const;

    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    using NodeType = detail::Node< Key, Value >;

    NodeType* fake_root_;
    Compare comp_;
    size_t size_;

    NodeType* getRoot() const noexcept;
    void setRoot(NodeType* newRoot) noexcept;
    void setRoot(iterator newRoot) noexcept;
    void update_height(NodeType* NodeType) noexcept;
    int get_balance(NodeType* node) noexcept;
    NodeType* balanceNode(NodeType* node) noexcept;
    NodeType* rotate_right(NodeType* y) noexcept;
    NodeType* rotate_left(NodeType* x) noexcept;
    NodeType* rotate_right_left(NodeType* y) noexcept;
    NodeType* rotate_left_right(NodeType* y) noexcept;
    NodeType* find(NodeType* node, const Key& k) const noexcept;
    NodeType* find_min(NodeType* node) const noexcept;
    NodeType* find_max(NodeType* node) const noexcept;
    void clear(NodeType* node) noexcept;
    NodeType* copy_tree(NodeType* node, NodeType* parent);
    template < typename IsIterator >
    IsIterator bound_impl(const Key& key, NodeType root, NodeType end, bool upp) const;
  };

  template < class Key, class Value, class Compare >
  Tree< Key, Value, Compare >::Tree():
    fake_root_(new NodeType(Key(), Value(), nullptr)),
    comp_(Compare()),
    size_(0)
  {
  }

  template < class Key, class Value, class Compare >
  Tree< Key, Value, Compare >::Tree(const Compare& cmp):
    fake_root_(new NodeType(Key(), Value(), nullptr)),
    comp_(cmp),
    size_(0)
  {
  }

  template < class Key, class Value, class Compare >
  Tree< Key, Value, Compare >::Tree(const Tree& other):
    Tree(other.begin(), other.end(), other.comp_)
  {
  }

  template < class Key, class Value, class Compare >
  Tree< Key, Value, Compare >::Tree(Tree&& other) noexcept:
    fake_root_(std::exchange(other.fake_root_, new NodeType(Key(), Value(), nullptr))),
    comp_(other.comp_),
    size_(std::exchange(other.size_, 0))
  {
  }

  template <class Key, class Value, class Compare >
  Tree< Key, Value, Compare >::Tree(std::initializer_list< std::pair< const Key, Value > > init, const Compare& cmp):
    Tree(init.begin(), init.end(), cmp)
  {
  }

  template < class Key, class Value, class Compare >
  template < class InputIt >
  Tree< Key, Value, Compare >::Tree(InputIt first, InputIt last, const Compare& cmp):
    fake_root_(new NodeType(Key(), Value(), nullptr)),
    comp_(cmp),
    size_(0)
  {
    for (; first != last; ++first)
    {
      insert(*first);
    }
  }

  template < class Key, class Value, class Compare >
  Tree< Key, Value, Compare >::~Tree()
  {
    clear();
    delete fake_root_;
  }

  template < class Key, class Value, class Compare >
  Tree< Key, Value, Compare >& Tree< Key, Value, Compare >::operator=(const Tree& other)
  {
    if (this != std::addressof(other))
    {
      auto temp(other);
      clear();
      swap(temp);
    }
    return *this;
  }

  template < class Key, class Value, class Compare >
  Tree< Key, Value, Compare >& Tree< Key, Value, Compare >::operator=(Tree&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      auto temp(std::move(other));
      clear();
      swap(temp);
    }
    return *this;
  }

  template < class Key, class Value, class Compare >
  bool Tree< Key, Value, Compare >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < class Key, class Value, class Compare >
  size_t Tree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::iterator Tree< Key, Value, Compare >::begin()
  {
    if (!getRoot() || getRoot() == fake_root_)
    {
      return end();
    }
    return iterator(find_min(getRoot()));
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::iterator Tree< Key, Value, Compare >::end()
  {
    return iterator(fake_root_);
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::const_iterator Tree< Key, Value, Compare >::begin() const
  {
    if (!getRoot() || fake_root_ == getRoot())
    {
      return end();
    }
    return const_iterator(find_min(getRoot()));
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::const_iterator Tree< Key, Value, Compare >::end() const
  {
    return const_iterator(fake_root_);
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::const_iterator Tree< Key, Value, Compare >::cbegin() const
  {
    return begin();
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::const_iterator Tree< Key, Value, Compare >::cend() const
  {
    return end();
  }

  template < class Key, class Value, class Compare >
  void Tree< Key, Value, Compare >::clear() noexcept
  {
    if (getRoot() && getRoot() != fake_root_)
    {
      clear(getRoot());
      fake_root_->left = nullptr;
    }
    size_ = 0;
  }

  template < class Key, class Value, class Compare >
  void Tree< Key, Value, Compare >::clear(NodeType* node) noexcept
  {
    if (!node || node == fake_root_)
    {
      return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
    node = nullptr;
  }

  template < class Key, class Value, class Compare >
  void Tree< Key, Value, Compare >::push(const Key& k, const Value& v)
  {
    setRoot(insert(iterator(getRoot()), k, v, iterator(fake_root_)));
    size_++;
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::iterator
    Tree< Key, Value, Compare >::insert(iterator itnode, const Key& k, const Value& v, iterator itpar)
  {
    auto node = itnode.current;
    auto par = itpar.current;
    if (!node)
    {
      return iterator(new NodeType(k, v, par));
    }
    if (comp_(k, node->data.first))
    {
      node->left = insert(iterator(node->left), k, v, iterator(node)).current;
    }
    else if (comp_(node->data.first, k))
    {
      node->right = insert(iterator(node->right), k, v, iterator(node)).current;
    }
    else
    {
      node->data.second = v;
      return iterator(node);
    }
    update_height(node);
    int balance = get_balance(node);
    if (balance > 1 && comp_(k, node->left->data.first))
    {
      return iterator(rotate_right(node));
    }
    if (balance < -1 && comp_(node->right->data.first, k))
    {
      return iterator(rotate_left(node));
    }
    if (balance > 1 && comp_(node->left->data.first, k))
    {
      node->left = rotate_left(node->left);
      return iterator(rotate_right(node));
    }
    if (balance < -1 && comp_(k, node->right->data.first))
    {
      node->right = rotate_right(node->right);
      return iterator(rotate_left(node));
    }
    return iterator(node);
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::NodeType*
    Tree< Key, Value, Compare >::find(NodeType* node, const Key& k) const noexcept
  {
    if (!node || node == fake_root_)
    {
      return nullptr;
    }
    if (comp_(k, node->data.first))
    {
      return find(node->left, k);
    }
    else if (comp_(node->data.first, k))
    {
      return find(node->right, k);
    }
    else
    {
      return node;
    }
  }

  template < class Key, class Value, class Compare >
  void Tree< Key, Value, Compare >::swap(Tree< Key, Value, Compare >& other) noexcept
  {
    std::swap(fake_root_, other.fake_root_);
    std::swap(size_, other.size_);
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::iterator
    Tree< Key, Value, Compare >::find(const Key& key)
  {
    NodeType* NodeType = find(getRoot(), key);
    if (NodeType && NodeType != fake_root_)
    {
      return iterator(NodeType);
    }
    return end();
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::const_iterator
    Tree< Key, Value, Compare >::find(const Key& key) const
  {
    const NodeType* NodeType = find(getRoot(), key);
    if (NodeType && NodeType != fake_root_)
    {
      return const_iterator(NodeType);
    }
    return end();
  }

  template < class Key, class Value, class Compare >
  size_t Tree< Key, Value, Compare >::count(const Key& key) const
  {
    return find(key) != end() ? 1 : 0;
  }

  template < class Key, class Value, class Compare >
  std::pair< typename Tree< Key, Value, Compare >::iterator, typename Tree< Key, Value, Compare >::iterator >
    Tree< Key, Value, Compare >::equal_range(const Key& key)
  {
    return { lower_bound(key), upper_bound(key) };
  }

  template < class Key, class Value, class Compare >
  std::pair< typename Tree< Key, Value, Compare >::const_iterator, typename Tree< Key, Value, Compare >::const_iterator >
    Tree< Key, Value, Compare >::equal_range(const Key& key) const
  {
    return { lower_bound(key), upper_bound(key) };
  }

  template < class Key, class Value, class Compare >
  template < class IsIterator >
  IsIterator Tree<Key, Value, Compare>::bound_impl(const Key& key, NodeType root, NodeType end, bool upp) const
  {
    NodeType current = root;
    NodeType result = end;
    while (current && current != end)
    {
      bool condition = upp ? comp(key, current->data.first) : !comp(current->data.first, key);
      if (condition)
      {
        result = current;
        current = current->left;
      }
      else
      {
        current = current->right;
      }
    }
    return IsIterator(result);
}

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::iterator
  Tree< Key, Value, Compare >::lower_bound(const Key& key)
  {
    return bound_impl(key, getRoot(), fake_root_, false);
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::const_iterator
  Tree< Key, Value, Compare >::lower_bound(const Key& key) const
  {
    return bound_impl(key, getRoot(), fake_root_, false);
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::iterator
  Tree< Key, Value, Compare >::upper_bound(const Key& key)
  {
    return bound_impl(key, getRoot(), fake_root_, true);
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::const_iterator
  Tree< Key, Value, Compare >::upper_bound(const Key& key) const
  {
    return bound_impl(key, getRoot(), fake_root_, true);
  }


  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::iterator
    Tree< Key, Value, Compare >::insert(const std::pair< const Key, Value >& value)
  {
    push(value.first, value.second);
    return iterator(find(value.first));
  }

  template < class Key, class Value, class Compare >
  template < typename InputIt >
  void Tree< Key, Value, Compare >::insert(InputIt first, InputIt last)
  {
    for (; first != last; ++first)
    {
      insert(*first);
    }
  }

  template < class Key, class Value, class Compare >
  void Tree< Key, Value, Compare >::insert(std::initializer_list< std::pair< const Key, Value > > ilist)
  {
    for (const auto& item : ilist)
    {
      insert(item);
    }
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::iterator
    Tree< Key, Value, Compare >::erase(iterator pos) noexcept
  {
    if (pos == end() || pos.current == fake_root_)
    {
      return end();
    }
    NodeType* NodeTypeo_erase = pos.current;
    NodeType* parent = NodeTypeo_erase->parent;
    Stack< NodeType* > path;
    try
    {
      if (NodeTypeo_erase->left == fake_root_ && NodeTypeo_erase->right == fake_root_)
      {
        if (parent->left == NodeTypeo_erase)
        {
          parent->left = fake_root_;
        }
        else
        {
          parent->right = fake_root_;
        }
        path.push(parent);
      }
      else if (NodeTypeo_erase->left == fake_root_ || NodeTypeo_erase->right == fake_root_)
      {
        NodeType* child = nullptr;
        if (NodeTypeo_erase->left == fake_root_)
        {
          child = NodeTypeo_erase->right;
        }
        else
        {
          child = NodeTypeo_erase->left;
        }
        if (parent->left == NodeTypeo_erase)
        {
          parent->left = child;
        }
        else
        {
          parent->right = child;
        }
        child->parent = parent;
        path.push(parent);
      }
      else
      {
        NodeType* min_right = NodeTypeo_erase->right;
        while (min_right->left != fake_root_)
        {
          min_right = min_right->left;
        }
        NodeTypeo_erase = min_right;
        parent = min_right->parent;
        if (parent->left == min_right)
        {
          parent->left = (min_right->right != fake_root_) ? min_right->right : fake_root_;
        }
        else
        {
          parent->right = (min_right->right != fake_root_) ? min_right->right : fake_root_;
        }
        if (min_right->right != fake_root_)
        {
          min_right->right->parent = parent;
        }
        path.push(parent);
        NodeTypeo_erase = min_right;
      }
      NodeType* new_root = path.top();
      if (new_root != nullptr)
      {
        fake_root_->left = new_root;
        new_root->parent = fake_root_;
      }
    }
    catch (...)
    {
      delete NodeTypeo_erase;
      return end();
    }
    delete NodeTypeo_erase;
    size_--;
    return iterator(parent);
  }

  template < class Key, class Value, class Compare >
  size_t Tree< Key, Value, Compare >::erase(const Key& key) noexcept
  {
    auto it = find(key);
    if (it != end())
    {
      erase(it);
      return 1;
    }
    return 0;
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::iterator
    Tree< Key, Value, Compare >::erase(iterator first, iterator last) noexcept
  {
    while (first != last)
    {
      first = erase(first);
    }
    return last;
  }

  template < class Key, class Value, class Compare >
  Value& Tree< Key, Value, Compare >::at(const Key& key)
  {
    NodeType* NodeType = find(key).current;
    if (!NodeType || NodeType == fake_root_)
    {
      throw std::out_of_range("not found");
    }
    return NodeType->data.second;
  }

  template < class Key, class Value, class Compare >
  const Value& Tree< Key, Value, Compare >::at(const Key& key) const
  {
    const NodeType* NodeType = find(key);
    if (!NodeType || NodeType == fake_root_)
    {
      throw std::out_of_range("not found");
    }
    return NodeType->data.second;
  }

  template < class Key, class Value, class Compare >
  Value& Tree< Key, Value, Compare >::operator[](const Key& key)
  {
    try
    {
      auto val = at(key);
    }
    catch (...)
    {
      push(key, Value());
    }
    return find(key).current->data.second;
  }

  template < class Key, class Value, class Compare >
  const Value& Tree< Key, Value, Compare >::operator[](const Key& key) const
  {
    return at(key);
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::NodeType*
    Tree< Key, Value, Compare >::copy_tree(NodeType* node, NodeType* parent)
  {
    if (!node || node == fake_root_)
    {
      return nullptr;
    }

    NodeType* new_node = new NodeType(node->data.first, node->data.second, parent);
    new_node->left = copy_tree(node->left, new_node);
    new_node->right = copy_tree(node->right, new_node);
    new_node->height = node->height;
    return new_node;
  }

  template < class Key, class Value, class Compare >
  void Tree< Key, Value, Compare >::setRoot(NodeType* newRoot) noexcept
  {
    fake_root_->left = newRoot;
    if (newRoot)
    {
      newRoot->parent = fake_root_;
    }
  }

  template < class Key, class Value, class Compare >
  void Tree< Key, Value, Compare >::setRoot(Iterator< Key, Value, Compare > newRoot) noexcept
  {
    fake_root_->left = newRoot.current;
    if (newRoot.current)
    {
      newRoot.current->parent = fake_root_;
    }
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::NodeType*
    Tree< Key, Value, Compare >::getRoot() const noexcept
  {
    return fake_root_->left;
  }

  template < class Key, class Value, class Compare >
  void Tree< Key, Value, Compare >::update_height(NodeType* NodeType) noexcept
  {
    if (!NodeType || NodeType == fake_root_)
    {
      return;
    }
    int left_height = NodeType->left && NodeType->left != fake_root_ ? NodeType->left->height : 0;
    int right_height = NodeType->right && NodeType->right != fake_root_ ? NodeType->right->height : 0;
    NodeType->height = 1 + std::max(left_height, right_height);
  }


  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::NodeType*
    Tree< Key, Value, Compare >::rotate_right(NodeType* y) noexcept
  {
    if (!y || !y->left || y->left == fake_root_)
    {
      return y;
    }
    NodeType* x = y->left;
    NodeType* T2 = x->right;
    x->right = y;
    y->left = T2;
    if (T2 && T2 != fake_root_)
    {
      T2->parent = y;
    }
    x->parent = y->parent;
    y->parent = x;
    update_height(y);
    update_height(x);
    return x;
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::NodeType*
    Tree< Key, Value, Compare >::rotate_left(NodeType* x) noexcept
  {
    if (!x || !x->right || x->right == fake_root_)
    {
      return x;
    }
    NodeType* y = x->right;
    NodeType* T2 = y->left;
    y->left = x;
    x->right = T2;
    if (T2 && T2 != fake_root_)
    {
      T2->parent = x;
    }
    y->parent = x->parent;
    x->parent = y;
    update_height(x);
    update_height(y);
    return y;
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::NodeType*
  Tree< Key, Value, Compare >::rotate_right_left(NodeType* y) noexcept
  {
    if (!y || !y->right || y->right == fake_root_)
    {
      return y;
    }
    y->right = rotate_left(y->right);
    if (y->right && y->right != fake_root_)
    {
      y->right->parent = y;
    }
    return rotate_right(y);
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::NodeType*
  Tree< Key, Value, Compare >::rotate_left_right(NodeType* y) noexcept
  {
    if (!y || !y->left || y->left == fake_root_)
    {
      return y;
    }
    y->left = rotate_right(y->left);
    if (y->left && y->left != fake_root_)
    {
      y->left->parent = y;
    }
    return rotate_left(y);
  }

  template <class Key, class Value, class Compare>
  typename Tree< Key, Value, Compare >::NodeType*
    Tree< Key, Value, Compare >::balanceNode(NodeType* node) noexcept
  {
    if (!node || node == fake_root_)
    {
      return node;
    }
    update_height(node);
    int balance = get_balance(node);
    if (balance > 1 && get_balance(node->left) >= 0)
    {
      return rotate_right(node);
    }
    if (balance > 1 && get_balance(node->left) < 0)
    {
      node->left = rotate_left(node->left);
      return rotate_right(node);
    }
    if (balance < -1 && get_balance(node->right) <= 0)
    {
      return rotate_left(node);
    }
    if (balance < -1 && get_balance(node->right) > 0)
    {
      node->right = rotate_right(node->right);
      return rotate_left(node);
    }
    return node;
  }

  template < class Key, class Value, class Compare >
  int Tree< Key, Value, Compare >::get_balance(NodeType* node) noexcept
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

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::NodeType*
    Tree< Key, Value, Compare >::find_min(NodeType* node) const noexcept
  {
    if (!node || node == fake_root_)
    {
      return nullptr;
    }
    while (node->left && node->left != fake_root_)
    {
      node = node->left;
    }
    return node;
  }

  template < class Key, class Value, class Compare >
  typename Tree< Key, Value, Compare >::NodeType*
    Tree< Key, Value, Compare >::find_max(NodeType* node) const noexcept
  {
    if (!node || node == fake_root_)
    {
      return nullptr;
    }
    while (node->right && node->right != fake_root_)
    {
      node = node->right;
    }
    return node;
  }
}
#endif
