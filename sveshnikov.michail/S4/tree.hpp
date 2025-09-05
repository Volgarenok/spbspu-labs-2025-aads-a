#ifndef TREE_HPP
#define TREE_HPP
#include "tree-iter.hpp"
#include "tree-citer.hpp"

namespace sveshnikov
{
  template< typename Key, typename T, typename Cmp = std::less< Key > >
  class AvlTree
  {
  public:
    explicit AvlTree();
    AvlTree(const AvlTree &x);
    AvlTree(AvlTree &&x);
    AvlTree &operator=(const AvlTree &x);
    AvlTree &operator=(AvlTree &&x);
    ~AvlTree();

    Iter< Key, T > begin() noexcept;
    ConstIter< Key, T > begin() const noexcept;
    ConstIter< Key, T > cbegin() const noexcept;
    Iter< Key, T > end() noexcept;
    ConstIter< Key, T > end() const noexcept;
    ConstIter< Key, T > cend() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    T &operator[](const Key &k);
    T &at(const Key &k);
    const T &at(const Key &k) const;

    void clear() noexcept;
    void swap(AvlTree &x) noexcept;
    std::pair< Iter< Key, T >, bool > insert(const std::pair< Key, T > &val);
    void erase(Iter< Key, T > position) noexcept;
    size_t erase(const Key &k) noexcept;

    Iter< Key, T > find(const Key &k);
    ConstIter< Key, T > find(const Key &k) const;
    std::pair< ConstIter< Key, T >, ConstIter< Key, T > > equal_range(const Key &k) const;
    std::pair< Iter< Key, T >, Iter< Key, T > > equal_range(const Key &k);
    size_t count(const Key &k) const;

  private:
    tree_node_t< Key, T > *root_;
    tree_node_t< Key, T > *fake_leaf_;
    size_t size_;

    tree_node_t< Key, T > *getMinNode(tree_node_t< Key, T > *node) const noexcept;
    void clear(tree_node_t< Key, T > *node) noexcept;
    void updateHeight(tree_node_t< Key, T > *node) noexcept;
    tree_node_t< Key, T > *rotateRight(tree_node_t< Key, T > *node) noexcept;
    tree_node_t< Key, T > *rotateLeft(tree_node_t< Key, T > *node) noexcept;
    int getBalanceFactor(tree_node_t< Key, T > *node) const noexcept;
    tree_node_t< Key, T > *balance(tree_node_t< Key, T > *node) noexcept;
    void rebalanceUpwards(tree_node_t< Key, T > *node) noexcept;
    tree_node_t< Key, T > *equal_range_impl(const Key &k) const noexcept;
  };

  template< typename Key, typename T, typename Cmp >
  AvlTree< Key, T, Cmp >::AvlTree():
    root_(nullptr),
    fake_leaf_(new tree_node_t< Key, T >{}),
    size_(0)
  {}

  template< typename Key, typename T, typename Cmp >
  AvlTree< Key, T, Cmp >::AvlTree(const AvlTree &x):
    AvlTree()
  {
    for (auto it = x.cbegin(); it != x.cend(); ++it)
    {
      insert(*it);
    }
  }

  template< typename Key, typename T, typename Cmp >
  AvlTree< Key, T, Cmp >::AvlTree(AvlTree &&x):
    root_(x.root_),
    fake_leaf_(x.fake_leaf_),
    size_(x.size_)
  {
    x.root_ = nullptr;
    x.fake_leaf_ = nullptr;
    x.size_ = 0;
  }

  template< typename Key, typename T, typename Cmp >
  AvlTree< Key, T, Cmp >::~AvlTree()
  {
    clear();
    delete fake_leaf_;
  }

  template< typename Key, typename T, typename Cmp >
  AvlTree< Key, T, Cmp > &AvlTree< Key, T, Cmp >::operator=(const AvlTree &x)
  {
    if (this != std::addressof(x))
    {
      AvlTree rhs(x);
      swap(rhs);
    }
    return *this;
  }

  template< typename Key, typename T, typename Cmp >
  AvlTree< Key, T, Cmp > &AvlTree< Key, T, Cmp >::operator=(AvlTree &&x)
  {
    if (this != std::addressof(x))
    {
      swap(x);
    }
    return *this;
  }

  template< typename Key, typename T, typename Cmp >
  tree_node_t< Key, T > *AvlTree< Key, T, Cmp >::getMinNode(
      tree_node_t< Key, T > *node) const noexcept
  {
    if (!node || node == fake_leaf_)
    {
      return nullptr;
    }
    while (node->left_ && node->left_ != fake_leaf_)
    {
      node = node->left_;
    }
    return node;
  }

  template< typename Key, typename T, typename Cmp >
  Iter< Key, T > AvlTree< Key, T, Cmp >::begin() noexcept
  {
    return root_ ? Iter< Key, T >(getMinNode(root_)) : end();
  }

  template< typename Key, typename T, typename Cmp >
  ConstIter< Key, T > AvlTree< Key, T, Cmp >::begin() const noexcept
  {
    return cbegin();
  }

  template< typename Key, typename T, typename Cmp >
  ConstIter< Key, T > AvlTree< Key, T, Cmp >::cbegin() const noexcept
  {
    return root_ ? ConstIter< Key, T >(getMinNode(root_)) : end();
  }

  template< typename Key, typename T, typename Cmp >
  Iter< Key, T > AvlTree< Key, T, Cmp >::end() noexcept
  {
    return Iter< Key, T >(fake_leaf_);
  }

  template< typename Key, typename T, typename Cmp >
  ConstIter< Key, T > AvlTree< Key, T, Cmp >::end() const noexcept
  {
    return cend();
  }

  template< typename Key, typename T, typename Cmp >
  ConstIter< Key, T > AvlTree< Key, T, Cmp >::cend() const noexcept
  {
    return ConstIter< Key, T >(fake_leaf_);
  }

  template< typename Key, typename T, typename Cmp >
  size_t AvlTree< Key, T, Cmp >::size() const noexcept
  {
    return size_;
  }

  template< typename Key, typename T, typename Cmp >
  bool AvlTree< Key, T, Cmp >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename Key, typename T, typename Cmp >
  T &AvlTree< Key, T, Cmp >::operator[](const Key &k)
  {
    Iter< Key, T > it = find(k);
    if (it == end())
    {
      it = insert(std::make_pair(k, T())).first;
    }
    return it->second;
  }

  template< typename Key, typename T, typename Cmp >
  T &AvlTree< Key, T, Cmp >::at(const Key &k)
  {
    return const_cast< T & >(static_cast< const AvlTree & >(*this).at(k));
  }

  template< typename Key, typename T, typename Cmp >
  const T &AvlTree< Key, T, Cmp >::at(const Key &k) const
  {
    auto it = find(k);
    if (it == end())
    {
      throw std::out_of_range("AvlTree::at");
    }
    return it->second;
  }

  template< typename Key, typename T, typename Cmp >
  void AvlTree< Key, T, Cmp >::clear() noexcept
  {
    clear(root_);
    if (fake_leaf_)
    {
      fake_leaf_->parent_ = nullptr;
    }
    root_ = nullptr;
    size_ = 0;
  }

  template< typename Key, typename T, typename Cmp >
  void AvlTree< Key, T, Cmp >::clear(tree_node_t< Key, T > *node) noexcept
  {
    if (node && node != fake_leaf_)
    {
      clear(node->left_);
      clear(node->right_);
      delete node;
    }
  }

  template< typename Key, typename T, typename Cmp >
  void AvlTree< Key, T, Cmp >::swap(AvlTree &x) noexcept
  {
    std::swap(root_, x.root_);
    std::swap(fake_leaf_, x.fake_leaf_);
    std::swap(size_, x.size_);
  }

  template< typename Key, typename T, typename Cmp >
  std::pair< Iter< Key, T >, bool > AvlTree< Key, T, Cmp >::insert(const std::pair< Key, T > &val)
  {
    Cmp cmp;
    tree_node_t< Key, T > *curr = root_;

    while (curr)
    {
      if (!cmp(val.first, curr->data_.first) && !cmp(curr->data_.first, val.first))
      {
        Iter< Key, T > it(curr);
        return std::make_pair(it, false);
      }
      bool go_left = cmp(val.first, curr->data_.first);
      tree_node_t< Key, T > *next = go_left ? curr->left_ : curr->right_;
      if (!next || next == fake_leaf_)
      {
        break;
      }
      curr = next;
    }

    tree_node_t< Key, T > *node = new tree_node_t< Key, T >(val);
    size_++;
    node->parent_ = curr;
    if (!root_ || (curr->right_ == fake_leaf_ && cmp(curr->data_.first, val.first)))
    {
      fake_leaf_->parent_ = node;
      node->right_ = fake_leaf_;
    }

    if (!root_)
    {
      root_ = node;
    }
    else if (cmp(val.first, curr->data_.first))
    {
      curr->left_ = node;
      rebalanceUpwards(curr);
    }
    else
    {
      curr->right_ = node;
      rebalanceUpwards(curr);
    }
    return std::make_pair(Iter< Key, T >(node), true);
  }

  template< typename Key, typename T, typename Cmp >
  void AvlTree< Key, T, Cmp >::updateHeight(tree_node_t< Key, T > *node) noexcept
  {
    size_t left_h = node->left_ ? node->left_->height_ : 0;
    size_t right_h = node->right_ ? node->right_->height_ : 0;
    node->height_ = std::max(left_h, right_h) + 1;
  }

  template< typename Key, typename T, typename Cmp >
  tree_node_t< Key, T > *AvlTree< Key, T, Cmp >::rotateRight(tree_node_t< Key, T > *node) noexcept
  {
    tree_node_t< Key, T > *parent = node->parent_;
    tree_node_t< Key, T > *left_son = node->left_;
    tree_node_t< Key, T > *left_right_grand_son = left_son->right_;

    if (parent)
    {
      if (parent->right_ == node)
      {
        parent->right_ = left_son;
      }
      else
      {
        parent->left_ = left_son;
      }
    }
    else
    {
      root_ = left_son;
    }
    node->parent_ = left_son;
    node->left_ = left_right_grand_son;
    left_son->parent_ = parent;
    left_son->right_ = node;
    if (left_right_grand_son)
    {
      left_right_grand_son->parent_ = node;
    }
    updateHeight(node);
    updateHeight(left_son);
    return left_son;
  }

  template< typename Key, typename T, typename Cmp >
  tree_node_t< Key, T > *AvlTree< Key, T, Cmp >::rotateLeft(tree_node_t< Key, T > *node) noexcept
  {
    tree_node_t< Key, T > *parent = node->parent_;
    tree_node_t< Key, T > *right_son = node->right_;
    tree_node_t< Key, T > *right_left_grand_son = right_son->left_;

    if (parent)
    {
      if (parent->right_ == node)
      {
        parent->right_ = right_son;
      }
      else
      {
        parent->left_ = right_son;
      }
    }
    else
    {
      root_ = right_son;
    }
    node->parent_ = right_son;
    node->right_ = right_left_grand_son;
    right_son->parent_ = parent;
    right_son->left_ = node;
    if (right_left_grand_son)
    {
      right_left_grand_son->parent_ = node;
    }
    updateHeight(node);
    updateHeight(right_son);
    return right_son;
  }

  template< typename Key, typename T, typename Cmp >
  int AvlTree< Key, T, Cmp >::getBalanceFactor(tree_node_t< Key, T > *node) const noexcept
  {
    int left_height = node->left_ ? node->left_->height_ : 0;
    int right_height = node->right_ ? node->right_->height_ : 0;
    return left_height - right_height;
  }

  template< typename Key, typename T, typename Cmp >
  tree_node_t< Key, T > *AvlTree< Key, T, Cmp >::balance(tree_node_t< Key, T > *node) noexcept
  {
    int balance_factor = getBalanceFactor(node);
    if (balance_factor > 1)
    {
      if (getBalanceFactor(node->left_) < 0)
      {
        node->left_ = rotateLeft(node->left_);
      }
      node = rotateRight(node);
    }
    else if (balance_factor < -1)
    {
      if (getBalanceFactor(node->right_) > 0)
      {
        node->right_ = rotateRight(node->right_);
      }
      node = rotateLeft(node);
    }
    return node;
  }

  template< typename Key, typename T, typename Cmp >
  void AvlTree< Key, T, Cmp >::rebalanceUpwards(tree_node_t< Key, T > *node) noexcept
  {
    while (node)
    {
      updateHeight(node);
      tree_node_t< Key, T > *parent = node->parent_;
      node = balance(node);
      node = parent;
    }
  }

  template< typename Key, typename T, typename Cmp >
  void AvlTree< Key, T, Cmp >::erase(Iter< Key, T > position) noexcept
  {
    if (position == end())
    {
      return;
    }
    tree_node_t< Key, T > *node = position.node_;
    tree_node_t< Key, T > *repl = nullptr;
    tree_node_t< Key, T > *balance_start = nullptr;

    if ((node->right_ && node->right_ != fake_leaf_) || node->left_)
    {
      repl = (node->right_ && node->right_ != fake_leaf_) ? getMinNode(node->right_) : node->left_;
      balance_start = (repl->parent_ == node) ? repl : repl->parent_;
    }
    else
    {
      balance_start = node->parent_;
    }

    tree_node_t< Key, T > *son = repl ? repl : node;
    tree_node_t< Key, T > *parent = son->parent_;
    if (parent)
    {
      if (parent->right_ == son)
      {
        parent->right_ = nullptr;
        if (son->right_)
        {
          parent->right_ = son->right_;
          son->right_->parent_ = parent;
        }
      }
      else
      {
        parent->left_ = nullptr;
      }
    }

    if (repl)
    {
      repl->parent_ = node->parent_;
      if (node->parent_)
      {
        if (node->parent_->right_ == node)
        {
          node->parent_->right_ = repl;
        }
        else
        {
          node->parent_->left_ = repl;
        }
      }
      repl->right_ = node->right_;
      if (repl->right_)
      {
        repl->right_->parent_ = repl;
      }
      repl->left_ = node->left_;
      if (repl->left_)
      {
        repl->left_->parent_ = repl;
      }
    }

    if (node == root_)
    {
      root_ = repl;
      if (!repl)
      {
        fake_leaf_->parent_ = nullptr;
      }
    }
    delete node;
    size_--;
    rebalanceUpwards(balance_start);
  }

  template< typename Key, typename T, typename Cmp >
  size_t AvlTree< Key, T, Cmp >::erase(const Key &k) noexcept
  {
    Iter< Key, T > it = find(k);
    if (it != end())
    {
      erase(it);
      return 1;
    }
    return 0;
  }

  template< typename Key, typename T, typename Cmp >
  tree_node_t< Key, T > *AvlTree< Key, T, Cmp >::equal_range_impl(const Key &k) const noexcept
  {
    Cmp cmp;
    tree_node_t< Key, T > *curr = root_;
    tree_node_t< Key, T > *next = nullptr;
    while (curr && curr != fake_leaf_)
    {
      if (cmp(k, curr->data_.first))
      {
        next = curr;
        curr = curr->left_;
      }
      else if (cmp(curr->data_.first, k))
      {
        curr = curr->right_;
      }
      else
      {
        next = curr;
        break;
      }
    }
    return next;
  }

  template< typename Key, typename T, typename Cmp >
  std::pair< ConstIter< Key, T >, ConstIter< Key, T > > AvlTree< Key, T, Cmp >::equal_range(
      const Key &k) const
  {
    Cmp cmp;
    const tree_node_t< Key, T > *node = equal_range_impl(k);
    if (!node)
    {
      return {cend(), cend()};
    }
    ConstIter< Key, T > it(node);
    ConstIter< Key, T > next = it;
    ++next;
    bool is_find = !cmp(k, it->first) && !cmp(it->first, k);
    return is_find ? std::make_pair(it, next) : std::make_pair(it, it);
  }

  template< typename Key, typename T, typename Cmp >
  std::pair< Iter< Key, T >, Iter< Key, T > > AvlTree< Key, T, Cmp >::equal_range(const Key &k)
  {
    Cmp cmp;
    tree_node_t< Key, T > *node = equal_range_impl(k);
    if (!node)
    {
      return {end(), end()};
    }
    Iter< Key, T > it(node);
    Iter< Key, T > next = it;
    ++next;
    bool is_find = !cmp(k, it->first) && !cmp(it->first, k);
    return is_find ? std::make_pair(it, next) : std::make_pair(it, it);
  }

  template< typename Key, typename T, typename Cmp >
  Iter< Key, T > AvlTree< Key, T, Cmp >::find(const Key &k)
  {
    std::pair< Iter< Key, T >, Iter< Key, T > > it_pair = equal_range(k);
    return (it_pair.first == it_pair.second) ? end() : it_pair.first;
  }

  template< typename Key, typename T, typename Cmp >
  ConstIter< Key, T > AvlTree< Key, T, Cmp >::find(const Key &k) const
  {
    std::pair< ConstIter< Key, T >, ConstIter< Key, T > > it_pair = equal_range(k);
    return (it_pair.first == it_pair.second) ? end() : it_pair.first;
  }

  template< typename Key, typename T, typename Cmp >
  size_t AvlTree< Key, T, Cmp >::count(const Key &k) const
  {
    std::pair< ConstIter< Key, T >, ConstIter< Key, T > > it_pair = equal_range(k);
    return (it_pair.first == it_pair.second) ? 0 : 1;
  }
}

#endif
