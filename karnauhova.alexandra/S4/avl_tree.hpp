#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include "avltree_node.hpp"
#include "avltree_iterator.hpp"
#include "avltree_citerator.hpp"

namespace karnauhova
{
  template< typename Key, typename Value, typename Compare = std::less< Key > >
  class AvlTree
  {
  public:
    using Iter = AvlTreeIterator< Key, Value, Compare >;
    using CIter = AvlTreeCIterator< Key, Value, Compare >;
    using pairIter = std::pair< Iter, Iter >;
    using pairCIter = std::pair< CIter, CIter >;

    explicit AvlTree(Compare cmp = Compare{});
    AvlTree(const AvlTree< Key, Value, Compare >&, Compare cmp = Compare{});
    AvlTree(AvlTree< Key, Value, Compare >&&) noexcept;
    template< typename InputIt >
    AvlTree(InputIt, InputIt, Compare cmp = Compare{});
    AvlTree(std::initializer_list< std::pair< Iter, bool > >, Compare cmp = Compare{});
    ~AvlTree();

    AvlTree< Key, Value, Compare >& operator=(const AvlTree< Key, Value, Compare >&);
    AvlTree< Key, Value, Compare >& operator=(AvlTree< Key, Value, Compare >&&) noexcept;

    Value& operator[](const Key&);
    Value& at(const Key&);
    const Value& at(const Key&) const;

    std::pair< Iter, bool > insert(const std::pair< Key, Value >&);
    std::pair< Iter, bool > insert(std::pair< Key, Value >&&);
    template< typename InputIt >
    void insert(InputIt, InputIt);
    void insert(std::initializer_list< std::pair< Key, Value > >);

    Iter erase(Iter) noexcept;
    Iter erase(CIter) noexcept;
    size_t erase(const Key&) noexcept;
    Iter erase(CIter, CIter) noexcept;

    Iter begin() const noexcept;
    Iter end() const noexcept;
    CIter cbegin() const noexcept;
    CIter cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t count(const Key& key) const;

    Iter lower_bound(const Key&) noexcept;
    CIter lower_bound(const Key&) const noexcept;
    Iter upper_bound(const Key&) noexcept;
    CIter upper_bound(const Key&) const noexcept;
    pairIter equal_range(const Key&) noexcept;
    pairCIter equal_range(const Key&) const noexcept;

    void clear() noexcept;
    void swap(AvlTree< Key, Value, Compare >& oth) noexcept;

    Iter find(const Key& key) noexcept;
    CIter find(const Key& key) const noexcept;

  private:
    using Node = detail::AvlTreeNode< Key, Value >;
    Node* fake_;
    size_t size_;
    Compare comp_;

    void balance(Node*) noexcept;
    Node* balanceNode(Node*) noexcept;
    int bfactor(Node*) const noexcept;
    Node* rotateRight(Node*) noexcept;
    Node* rotateLeft(Node*) noexcept;
    int height(Node*) const noexcept;
    void updateHeight(Node*) noexcept;
  };

  template< typename Key, typename Value, typename Compare >
  AvlTree< Key, Value, Compare >::AvlTree(Compare cmp):
    fake_(reinterpret_cast< Node* >(new char[sizeof(Node)])),
    size_(0),
    comp_(std::move(cmp))
  {
    fake_->left = fake_->right = fake_;
    fake_->height = -1;
  }

  template< typename Key, typename Value, typename Compare >
  AvlTree< Key, Value, Compare >::AvlTree(const AvlTree< Key, Value, Compare >& oth, Compare cmp):
    AvlTree(oth.begin(), oth.end(), std::move(cmp))
  {}

  template< typename Key, typename Value, typename Compare >
  AvlTree< Key, Value, Compare >::AvlTree(AvlTree< Key, Value, Compare >&& oth) noexcept:
    fake_(std::exchange(oth.fake_, nullptr)),
    size_(std::exchange(oth.size_, 0)),
    comp_(std::move(oth.comp_))
  {}

  template< typename Key, typename Value, typename Compare >
  template< typename InputIt >
  AvlTree< Key, Value, Compare >::AvlTree(InputIt first, InputIt last, Compare cmp):
    AvlTree(std::move(cmp))
  {
    for (auto it = first; it != last; it++)
    {
      insert(*it);
    }
  }

  template< typename Key, typename Value, typename Compare >
  AvlTree< Key, Value, Compare >::AvlTree(std::initializer_list< std::pair< Iter, bool > > list, Compare cmp):
    AvlTree(list.begin(), list.end(), std::move(cmp))
  {}

  template< typename Key, typename Value, typename Compare >
  AvlTree< Key, Value, Compare >& AvlTree< Key, Value, Compare >::operator=(const AvlTree< Key, Value, Compare >& rhs)
  {
    if (this != std::addressof(rhs))
    {
      AvlTree< Key, Value, Compare > temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  AvlTree< Key, Value, Compare >& AvlTree< Key, Value, Compare >::operator=(AvlTree< Key, Value, Compare >&& rhs) noexcept
  {
    if (this != std::addressof(rhs))
    {
      AvlTree< Key, Value, Compare > temp(std::move(rhs));
      swap(temp);
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  AvlTree< Key, Value, Compare >::~AvlTree()
  {
    clear();
    delete[] reinterpret_cast< char* >(fake_);
  }

  template< typename Key, typename Value, typename Compare >
  Value& AvlTree< Key, Value, Compare >::operator[](const Key& key)
  {
    auto it = insert(std::make_pair(key, Value()));
    return it.first->second;
  }

  template< typename Key, typename Value, typename Compare >
  Value& AvlTree< Key, Value, Compare >::at(const Key& key)
  {
    return const_cast< Value& >(static_cast< const AvlTree< Key, Value, Compare >& >(*this).at(key));
  }

  template< typename Key, typename Value, typename Compare >
  const Value& AvlTree< Key, Value, Compare >::at(const Key& key) const
  {
    CIter it = find(key);
    if (it == cend())
    {
      throw std::out_of_range("No such key");
    }
    return it->second;
  }

  template< typename Key, typename Value, typename Compare >
  std::pair< AvlTreeIterator< Key, Value, Compare >, bool > AvlTree< Key, Value, Compare >::insert(const std::pair< Key, Value >& val)
  {
    Node* newNode = nullptr;
    newNode = new Node{val, fake_, fake_, nullptr, 0};
    if (empty())
    {
      fake_->left = newNode;
      newNode->parent = newNode->left = newNode->right = fake_;
      size_ = 1;
      return {Iter(newNode, fake_), true};
    }
    Node* tmp = fake_->left;
    Node* parent = fake_;
    while (tmp != fake_)
    {
      parent = tmp;
      if (comp_(newNode->data.first, tmp->data.first))
      {
        tmp = tmp->left;
      }
      else if (comp_(tmp->data.first, newNode->data.first))
      {
        tmp = tmp->right;
      }
      else
      {
        delete newNode;
        return {Iter(tmp, fake_), false};
      }
    }
    newNode->parent = parent;
    if (comp_(parent->data.first, newNode->data.first))
    {
      parent->right = newNode;
    }
    else
    {
      parent->left = newNode;
    }
    balance(fake_->left);
    size_++;
    return {Iter(newNode, fake_), true};
  }

  template< typename Key, typename Value, typename Compare >
  std::pair< AvlTreeIterator< Key, Value, Compare >, bool > AvlTree< Key, Value, Compare >::insert(std::pair< Key, Value >&& val)
  {
    return insert(val);
  }

  template< typename Key, typename Value, typename Compare >
  template< typename InputIt >
  void AvlTree< Key, Value, Compare >::insert(InputIt first, InputIt last)
  {
    for (auto it = first; it != last; ++it)
    {
      insert(*it);
    }
  }

  template< typename Key, typename Value, typename Compare >
  void AvlTree< Key, Value, Compare >::insert(std::initializer_list< std::pair< Key, Value > > list)
  {
    insert(list.begin(), list.end());
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::Iter AvlTree< Key, Value, Compare >::erase(CIter it) noexcept
  {
    if (it == cend())
    {
      return end();
    }
    Node* delet = it.node_;
    Iter res(delet, fake_);
    ++res;
    Node* new_node = nullptr;

    if (delet->left == fake_ && delet->right == fake_)
    {
      new_node = delet->parent;
      if (new_node != fake_)
      {
        if (new_node->left == delet)
        {
          new_node->left = fake_;
        }
        else
        {
          new_node->right = fake_;
        }
      }
      else
      {
        fake_->left = fake_;
      }
      delete delet;
      size_--;
    }
    else if (delet->left == fake_ || delet->right == fake_)
    {
      Node* child = delet->left != fake_ ? delet->left : delet->right;
      new_node = delet->parent;
      if (new_node != fake_)
      {
        if (new_node->left == delet)
        {
          new_node->left = child;
        }
        else
        {
          new_node->right = child;
        }
      }
      else
      {
        fake_->left = child;
      }
      child->parent = new_node;
      delete delet;
      size_--;
    }
    else
    {
      Node* tmp = delet->right;
      while (tmp->left != fake_)
      {
        tmp = tmp->left;
      }
      std::swap(delet->data, tmp->data);
      new_node = tmp->parent;
      if (new_node->left == tmp)
      {
        new_node->left = tmp->right;
      }
      else
      {
        new_node->right = tmp->right;
      }
      if (tmp->right != fake_)
      {
        tmp->right->parent = new_node;
      }
      delete tmp;
      size_--;
    }
    if (new_node != fake_)
    {
      balance(new_node);
    }
    return res;
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::Iter AvlTree< Key, Value, Compare >::erase(Iter it) noexcept
  {
    CIter i(it);
    return erase(i);
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::Iter AvlTree< Key, Value, Compare >::erase(CIter first, CIter last) noexcept
  {
    CIter current = first;
    while (current != last)
    {
      current = erase(current);
    }
    return Iter(last.node_, last.fake_);;
  }

  template< typename Key, typename Value, typename Compare >
  size_t AvlTree< Key, Value, Compare >::erase(const Key& key) noexcept
  {
    Iter it = find(key);
    if (it == end())
    {
      return 0;
    }
    erase(it);
    return 1;
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::Iter AvlTree< Key, Value, Compare >::end() const noexcept
  {
    return Iter(fake_, fake_);
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::CIter AvlTree< Key, Value, Compare >::cend() const noexcept
  {
    return CIter(fake_, fake_);
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::Iter AvlTree< Key, Value, Compare >::begin() const noexcept
  {
    if (empty())
    {
      return end();
    }
    Node* tmp = fake_->left;
    while (tmp->left != fake_)
    {
      tmp = tmp->left;
    }
    return Iter(tmp, fake_);
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::CIter AvlTree< Key, Value, Compare >::cbegin() const noexcept
  {
    if (empty())
    {
      return cend();
    }
    Node* tmp = fake_->left;
    while (tmp->left != fake_)
    {
      tmp = tmp->left;
    }
    return CIter(tmp, fake_);
  }

  template< typename Key, typename Value, typename Compare >
  bool AvlTree< Key, Value, Compare >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename Key, typename Value, typename Compare >
  size_t AvlTree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template< typename Key, typename Value, typename Compare >
  size_t AvlTree< Key, Value, Compare >::count(const Key& key) const
  {
    return find(key) != cend();
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::Iter AvlTree< Key, Value, Compare >::lower_bound(const Key& key) noexcept
  {
    Node* current = fake_->left;
    Node* res = fake_;
    while (current != fake_ && current != nullptr)
    {
      if (!comp_(current->data.first, key))
      {
        res = current;
        current = current->left;
      }
      else
      {
        current = current->right;
      }
    }
    return Iter(res, fake_);
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::CIter AvlTree< Key, Value, Compare >::lower_bound(const Key& key) const noexcept
  {
    return CIter(lower_bound(key));
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::Iter AvlTree< Key, Value, Compare >::upper_bound(const Key& key) noexcept
  {
    Node* current = fake_->left;
    Node* res = fake_;
    while (current != fake_ && current != nullptr)
    {
      if (comp_(key, current->data.first))
      {
        res = current;
        current = current->left;
      }
      else
      {
        current = current->right;
      }
    }
    return Iter(res, fake_);
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::CIter AvlTree< Key, Value, Compare >::upper_bound(const Key& key) const noexcept
  {
    return CIter(upper_bound(key));
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::pairIter AvlTree< Key, Value, Compare >::equal_range(const Key& key) noexcept
  {
    return std::make_pair(lower_bound(key), upper_bound(key));
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::pairCIter AvlTree< Key, Value, Compare >::equal_range(const Key& key) const noexcept
  {
    return std::make_pair(CIter(lower_bound(key)), CIter(upper_bound(key)));
  }

  template< typename Key, typename Value, typename Compare >
  void AvlTree< Key, Value, Compare >::clear() noexcept
  {
    if (empty())
    {
      return;
    }
    erase(begin(), end());
  }

  template< typename Key, typename Value, typename Compare >
  void AvlTree< Key, Value, Compare >::swap(AvlTree< Key, Value, Compare >& oth) noexcept
  {
    std::swap(fake_, oth.fake_);
    std::swap(size_, oth.size_);
    std::swap(comp_, oth.comp_);
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::Iter AvlTree< Key, Value, Compare >::find(const Key& key) noexcept
  {
    Node* current = fake_->left;
    while (current != fake_)
    {
      if (comp_(key, current->data.first))
      {
        current = current->left;
      }
      else if (comp_(current->data.first, key))
      {
        current = current->right;
      }
      else
      {
        return Iter(current, fake_);
      }
    }
    return end();
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::CIter AvlTree< Key, Value, Compare >::find(const Key& key) const noexcept
  {
    Node* current = fake_->left;
    while (current != fake_)
    {
      if (comp_(key, current->data.first))
      {
        current = current->left;
      }
      else if (comp_(current->data.first, key))
      {
        current = current->right;
      }
      else
      {
        return CIter(current, fake_);
      }
    }
    return cend();
  }

  template< typename Key, typename Value, typename Compare >
  int AvlTree< Key, Value, Compare >::bfactor(Node* node) const noexcept
  {
    if (node == fake_ || node == nullptr)
    {
      return 0;
    }
    return height(node->left) - height(node->right);
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::Node* AvlTree< Key, Value, Compare >::balanceNode(Node* node) noexcept
  {
    updateHeight(node);
    int factor = bfactor(node);
    if (factor > 1)
    {
      if (bfactor(node->left) < 0)
      {
        node->left = rotateLeft(node->left);
      }
      return rotateRight(node);
    }
    if (factor < -1)
    {
      if (bfactor(node->right) > 0)
      {
        node->right = rotateRight(node->right);
      }
      return rotateLeft(node);
    }
    return node;
  }

  template< typename Key, typename Value, typename Compare >
  void AvlTree< Key, Value, Compare >::balance(Node* node) noexcept
  {
    while (node != fake_)
    {
      Node* parent = node->parent;
      bool m = (node == parent->left);
      node = balanceNode(node);
      if (m)
      {
        parent->left = node;
      }
      else
      {
        parent->right = node;
      }
      node = parent;
    }
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::Node* AvlTree< Key, Value, Compare >::rotateRight(Node* node) noexcept
  {
    if (!node || node->left == fake_ || node == fake_)
    {
      return node;
    }
    Node* it = node->left;
    node->left = it->right;
    if (it->right != fake_)
    {
      it->right->parent = node;
    }

    it->parent = node->parent;
    node->parent = it;
    it->right = node;
    if (it->parent != fake_)
    {
      if (it->parent->left == node)
      {
        it->parent->left = it;
      }
      else
      {
        it->parent->right = it;
      }
    }
    else
    {
      fake_->left = it;
    }

    updateHeight(node);
    updateHeight(it);

    return it;
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::Node* AvlTree< Key, Value, Compare >::rotateLeft(Node* node) noexcept
  {
    if (!node || node->right == fake_ || node == fake_)
    {
      return node;
    }
    Node* it = node->right;
    node->right = it->left;
    if (it->left != fake_)
    {
      it->left->parent = node;
    }

    it->parent = node->parent;
    node->parent = it;
    it->left = node;
    if (it->parent != fake_)
    {
      if (it->parent->left == node)
      {
        it->parent->left = it;
      }
      else
      {
        it->parent->right = it;
      }
    }
    else
    {
      fake_->left = it;
    }

    updateHeight(node);
    updateHeight(it);

    return it;
  }

  template< typename Key, typename Value, typename Compare >
  int AvlTree< Key, Value, Compare >::height(Node* node) const noexcept
  {
    if (node == fake_ || node == nullptr)
    {
      return -1;
    }
    return node->height;
  }

  template< typename Key, typename Value, typename Compare >
  void AvlTree< Key, Value, Compare >::updateHeight(Node* node) noexcept
  {
    if (node != fake_ && node != nullptr)
    {
      node->height = std::max(height(node->left), height(node->right)) + 1;
    }
  }
}

#endif
