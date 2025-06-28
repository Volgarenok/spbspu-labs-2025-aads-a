#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <iostream>
#include <functional>
#include <cstddef>
#include <iterator>
#include <cassert>
#include "stack.hpp"
#include "queue.hpp"

namespace petrov
{
  template< typename K, typename T >
  struct AVLTreeNode
  {
    std::pair< const K, T > data;
    AVLTreeNode< K, T > * left;
    AVLTreeNode< K, T > * right;
    AVLTreeNode< K, T > * parent;
    int height;
    void setHeight();
  };

  template< typename K, typename T, typename Cmp = std::less< K > >
  struct ConstAVLTreeIterator: std::iterator< std::bidirectional_iterator_tag, T >
  {
    template< typename A, typename B, typename C >
    friend struct AVLTree;
  public:
    using val_t = std::pair< const K, T >;
    using node_t = AVLTreeNode< K, T >;
    using this_t = ConstAVLTreeIterator< K, T, Cmp >;
    ConstAVLTreeIterator():
      node_(nullptr)
    {}
    ConstAVLTreeIterator(const this_t & rhs) = default;
    ~ConstAVLTreeIterator() = default;
    this_t & operator=(const this_t & rhs) = default;
    this_t & operator++();
    this_t operator++(int);
    this_t & operator--();
    this_t operator--(int);
    const val_t & operator*() const;
    const val_t * operator->() const;
    bool operator==(const this_t & rhs) const;
    bool operator!=(const this_t & rhs) const;
  private:
    const node_t * node_;
    explicit ConstAVLTreeIterator(const node_t * node):
      node_(node)
    {}
  };

  template< typename K, typename T, typename Cmp >
  struct AVLTreeIterator: std::iterator< std::bidirectional_iterator_tag, T >
  {
    template< typename A, typename B, typename C >
    friend struct AVLTree;
  public:
    using val_t = std::pair< const K, T >;
    using node_t = AVLTreeNode< K, T >;
    using this_t = AVLTreeIterator< K, T, Cmp >;
    AVLTreeIterator():
      node_(nullptr)
    {}
    AVLTreeIterator(const this_t & rhs) = default;
    ~AVLTreeIterator() = default;
    this_t & operator=(const this_t & rhs) = default;
    this_t & operator++();
    this_t operator++(int);
    this_t & operator--();
    this_t operator--(int);
    val_t & operator*();
    val_t * operator->();
    bool operator==(const this_t & rhs) const;
    bool operator!=(const this_t & rhs) const;
  private:
    node_t * node_;
    explicit AVLTreeIterator(node_t * node):
      node_(node)
    {}
  };

  template< typename K, typename T, typename Cmp = std::less< K > >
  struct AVLTree
  {
  public:
    using this_t = AVLTree< K, T, Cmp >;
    using node_t = AVLTreeNode< K, T >;
    using const_it_t = ConstAVLTreeIterator< K, T, Cmp >;
    using it_t = AVLTreeIterator< K, T, Cmp >;
    AVLTree();
    AVLTree(const this_t & rhs);
    AVLTree(this_t && rhs);
    ~AVLTree();
    this_t & operator=(const this_t & rhs);
    this_t & operator=(this_t && rhs);
    T & operator[](const K & key);
    const T & operator[](const K & key) const;
    const_it_t cbegin() const;
    const_it_t cend() const;
    it_t begin();
    it_t end();
    bool empty() const noexcept;
    size_t size() const noexcept;
    std::pair< const_it_t, bool > insert(const std::pair< K, T > & val);
    std::pair< const_it_t, bool > insert(std::pair< K, T > && val);
    it_t erase(it_t position);
    size_t erase(const K & key);
    it_t find(const K & key);
    const_it_t find(const K & key) const;
    T & at(const K & key);
    const T & at(const K & key) const;
    void clear() noexcept;
    void swap(this_t & rhs) noexcept;
    template< typename F >
    F traverse_lnr(F f);
    template< typename F >
    F traverse_rnl(F f);
    template< typename F >
    F traverse_breadth(F f);
    template< typename F >
    F traverse_lnr(F f) const;
    template< typename F >
    F traverse_rnl(F f) const;
    template< typename F >
    F traverse_breadth(F f) const;
  private:
    node_t * root_;
    size_t size_;
    size_t erased_;
    void balance(node_t * node);
    void leftRotate(node_t * node);
    void rightRotate(node_t * node);
    void clearBiTree(node_t * node) noexcept;
    template< typename Pair >
    std::pair< const_it_t, bool > insertImpl(Pair && val);
  };

  template< typename K, typename T >
  void AVLTreeNode< K, T >::setHeight()
  {
    if (left && right)
    {
      height = std::max(left->height, right->height) + 1;
    }
    else if (!left && !right)
    {
      height = 1;
    }
    else
    {
      if (!left)
      {
        height = right->height + 1;
      }
      else
      {
        height = left->height + 1;
      }
    }
  }

  template< typename K, typename T, typename Cmp >
  typename ConstAVLTreeIterator< K, T, Cmp >::this_t & ConstAVLTreeIterator< K, T, Cmp >::operator++()
  {
    assert(node_ != nullptr);
    if (!node_->parent || Cmp{}(node_->data.first, node_->parent->data.first))
    {
      if (node_->right)
      {
        node_ = node_->right;
        while (node_->left)
        {
          node_ = node_->left;
        }
      }
      else
      {
        node_ = node_->parent;
      }
    }
    else
    {
      if (node_->right)
      {
        node_ = node_->right;
        while (node_->left)
        {
          node_ = node_->left;
        }
      }
      else
      {
        while (node_->parent && Cmp{}(node_->parent->data.first, node_->data.first))
        {
          node_ = node_->parent;
        }
        node_ = node_->parent;
      }
    }
    return *this;
  }

  template< typename K, typename T, typename Cmp >
  typename ConstAVLTreeIterator< K, T, Cmp >::this_t ConstAVLTreeIterator< K, T, Cmp >::operator++(int)
  {
    assert(node_ != nullptr);
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename K, typename T, typename Cmp >
  const typename ConstAVLTreeIterator< K, T, Cmp >::val_t & ConstAVLTreeIterator< K, T, Cmp >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< typename K, typename T, typename Cmp >
  const typename ConstAVLTreeIterator< K, T, Cmp >::val_t * ConstAVLTreeIterator< K, T, Cmp >::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename K, typename T, typename Cmp >
  bool ConstAVLTreeIterator< K, T, Cmp >::operator==(const this_t & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename K, typename T, typename Cmp >
  bool ConstAVLTreeIterator< K, T, Cmp >::operator!=(const this_t & rhs) const
  {
    return !(*this == rhs);
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTreeIterator< K, T, Cmp >::this_t & AVLTreeIterator< K, T, Cmp >::operator++()
  {
    assert(node_ != nullptr);
    if (!node_->parent || Cmp{}(node_->data.first, node_->parent->data.first))
    {
      if (node_->right)
      {
        node_ = node_->right;
        while (node_->left)
        {
          node_ = node_->left;
        }
      }
      else
      {
        node_ = node_->parent;
      }
    }
    else
    {
      if (node_->right)
      {
        node_ = node_->right;
        while (node_->left)
        {
          node_ = node_->left;
        }
      }
      else
      {
        while (node_->parent && Cmp{}(node_->parent->data.first, node_->data.first))
        {
          node_ = node_->parent;
        }
        node_ = node_->parent;
      }
    }
    return *this;
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTreeIterator< K, T, Cmp >::this_t AVLTreeIterator< K, T, Cmp >::operator++(int)
  {
    assert(node_ != nullptr);
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTreeIterator< K, T, Cmp >::val_t & AVLTreeIterator< K, T, Cmp >::operator*()
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTreeIterator< K, T, Cmp >::val_t * AVLTreeIterator< K, T, Cmp >::operator->()
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename K, typename T, typename Cmp >
  bool AVLTreeIterator< K, T, Cmp >::operator==(const this_t & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename K, typename T, typename Cmp >
  bool AVLTreeIterator< K, T, Cmp >::operator!=(const this_t & rhs) const
  {
    return !(*this == rhs);
  }

  template< typename K, typename T, typename Cmp >
  AVLTree< K, T, Cmp >::AVLTree():
    root_(nullptr),
    size_(0),
    erased_(0)
  {}

  template< typename K, typename T, typename Cmp >
  AVLTree< K, T, Cmp >::AVLTree(const this_t & rhs):
    root_(nullptr),
    size_(0),
    erased_(0)
  {
    for (auto it = rhs.cbegin(); it != rhs.cend(); ++it)
    {
      insert({ it->first, it->second });
    }
  }

  template< typename K, typename T, typename Cmp >
  AVLTree< K, T, Cmp >::AVLTree(this_t && rhs):
    root_(rhs.root_),
    size_(rhs.size_),
    erased_(rhs.erased_)
  {
    rhs.root_ = nullptr;
  }

  template< typename K, typename T, typename Cmp >
  AVLTree< K, T, Cmp >::~AVLTree()
  {
    clear();
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::this_t & AVLTree< K, T, Cmp >::operator=(const this_t & rhs)
  {
    this_t cpy(rhs);
    swap(cpy);
    return *this;
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::this_t & AVLTree< K, T, Cmp >::operator=(this_t && rhs)
  {
    clear();
    root_ = rhs.root_;
    size_ = rhs.size_;
    erased_ = rhs.erased_;
    rhs.root_ = nullptr;
    return *this;
  }

  template< typename K, typename T, typename Cmp >
  T & AVLTree< K, T, Cmp >::operator[](const K & key)
  {
    return const_cast< T & >(static_cast< const AVLTree * >(this)->operator[](key));
  }

  template< typename K, typename T, typename Cmp >
  const T & AVLTree< K, T, Cmp >::operator[](const K & key) const
  {
    auto temp = root_;
    while (temp)
    {
      if (Cmp{}(key, temp->data.first))
      {
        temp = temp->left;
      }
      else if (Cmp{}(temp->data.first, key))
      {
        temp = temp->right;
      }
      else if (!(Cmp{}(key, temp->data.first) && Cmp{}(temp->data.first, key)))
      {
        break;
      }
    }
    return temp->data.second;
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::const_it_t AVLTree< K, T, Cmp >::cbegin() const
  {
    auto temp = root_;
    if (empty())
    {
      return const_it_t();
    }
    else
    {
      while (temp->left)
      {
        temp = temp->left;
      }
    }
    return const_it_t(temp);
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::const_it_t AVLTree< K, T, Cmp >::cend() const
  {
    return const_it_t();
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::it_t AVLTree< K, T, Cmp >::begin()
  {
    auto temp = root_;
    if (empty())
    {
      return it_t();
    }
    else
    {
      while (temp->left)
      {
        temp = temp->left;
      }
    }
    return it_t(temp);
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::it_t AVLTree< K, T, Cmp >::end()
  {
    return it_t();
  }

  template< typename K, typename T, typename Cmp >
  bool AVLTree< K, T, Cmp >::empty() const noexcept
  {
    if (!size_)
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  template< typename K, typename T, typename Cmp >
  size_t AVLTree< K, T, Cmp >::size() const noexcept
  {
    return size_;
  }

  template< typename K, typename T, typename Cmp >
  std::pair< ConstAVLTreeIterator< K, T, Cmp >, bool > AVLTree< K, T, Cmp >::insert(const std::pair< K, T > & val)
  {
    return insertImpl(val);
  }

  template< typename K, typename T, typename Cmp >
  std::pair< ConstAVLTreeIterator< K, T, Cmp >, bool > AVLTree< K, T, Cmp >::insert(std::pair< K, T > && val)
  {
    return insertImpl(std::move(val));
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::it_t AVLTree< K, T, Cmp >::erase(it_t position)
  {
    auto return_it = position;
    ++return_it;
    node_t * balance_node_ptr = nullptr;
    if (position.node_->left && position.node_->right)
    {
      auto temp = position.node_;
      temp = temp->right;
      if (temp->left)
      {
        while (temp->left)
        {
          temp = temp->left;
        }
        balance_node_ptr = temp->parent;
        temp->parent->left = nullptr;
        temp->parent = position.node_->parent;
        temp->left = position.node_->left;
        temp->right = position.node_->right;
        temp->left->parent = temp;
        temp->right->parent = temp;
        if (position.node_->parent && Cmp{}(position.node_->data.first, position.node_->parent->data.first))
        {
          position.node_->parent->left = temp;
        }
        else if (position.node_->parent)
        {
          position.node_->parent->right = temp;
        }
        else
        {
          root_ = temp;
        }
      }
      else
      {
        balance_node_ptr = temp->parent;
        temp->parent = position.node_->parent;
        temp->left = position.node_->left;
        temp->left->parent = temp;
        if (position.node_->parent && Cmp{}(position.node_->data.first, position.node_->parent->data.first))
        {
          position.node_->parent->left = temp;
        }
        else if (position.node_->parent)
        {
          position.node_->parent->right = temp;
        }
        else
        {
          root_ = temp;
        }
      }
    }
    else if (position.node_->left)
    {
      auto temp = position.node_;
      temp = temp->left;
      if (temp->right)
      {
        while (temp->right)
        {
          temp = temp->right;
        }
        balance_node_ptr = temp->parent;
        temp->parent->right = nullptr;
        temp->parent = position.node_->parent;
        temp->left = position.node_->left;
        temp->right = position.node_->right;
        if (position.node_->parent && Cmp{}(position.node_->data.first, position.node_->parent->data.first))
        {
          position.node_->parent->left = temp;
        }
        else if (position.node_->parent)
        {
          position.node_->parent->right = temp;
        }
        else
        {
          root_ = temp;
        }
      }
      else
      {
        balance_node_ptr = temp->parent;
        temp->parent = position.node_->parent;
        temp->right = position.node_->right;
        if (position.node_->parent && Cmp{}(position.node_->data.first, position.node_->parent->data.first))
        {
          position.node_->parent->left = temp;
        }
        else if (position.node_->parent)
        {
          position.node_->parent->right = temp;
        }
        else
        {
          root_ = temp;
        }
      }
    }
    else if (position.node_->right)
    {
      auto temp = position.node_;
      temp = temp->right;
      if (temp->left)
      {
        while (temp->left)
        {
          temp = temp->left;
        }
        balance_node_ptr = temp->parent;
        temp->parent->left = nullptr;
        temp->parent = position.node_->parent;
        temp->left = position.node_->left;
        temp->right = position.node_->right;
        if (position.node_->parent && Cmp{}(position.node_->data.first, position.node_->parent->data.first))
        {
          position.node_->parent->left = temp;
        }
        else if (position.node_->parent)
        {
          position.node_->parent->right = temp;
        }
        else
        {
          root_ = temp;
        }
      }
      else
      {
        balance_node_ptr = temp->parent;
        temp->parent = position.node_->parent;
        temp->left = position.node_->left;
        if (position.node_->parent && Cmp{}(position.node_->data.first, position.node_->parent->data.first))
        {
          position.node_->parent->left = temp;
        }
        else if (position.node_->parent)
        {
          position.node_->parent->right = temp;
        }
        else
        {
          root_ = temp;
        }
      }
    }
    else
    {
      if (position.node_->parent && Cmp{}(position.node_->data.first, position.node_->parent->data.first))
      {
        position.node_->parent->left = nullptr;
      }
      else if (position.node_->parent)
      {
        position.node_->parent->right = nullptr;
      }
      else
      {
        root_ = nullptr;
      }
    }
    while (balance_node_ptr)
    {
      balance_node_ptr->setHeight();
      balance_node_ptr = balance_node_ptr->parent;
    }
    delete position.node_;
    size_--;
    erased_++;
    return return_it;
  }

  template< typename K, typename T, typename Cmp >
  size_t AVLTree< K, T, Cmp >::erase(const K & key)
  {
    it_t position = find(key);
    erase(position);
    return erased_;
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::it_t AVLTree< K, T, Cmp >::find(const K & key)
  {
    auto temp = root_;
    while (temp)
    {
      if (Cmp{}(key, temp->data.first))
      {
        temp = temp->left;
      }
      else if (Cmp{}(temp->data.first, key))
      {
        temp = temp->right;
      }
      else if (!(Cmp{}(key, temp->data.first), Cmp{}(temp->data.first, key)))
      {
        return it_t(temp);
      }
    }
    return end();
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp  >::const_it_t AVLTree< K, T, Cmp >::find(const K & key) const
  {
    auto temp = root_;
    while (temp)
    {
      if (Cmp{}(key, temp->data.first))
      {
        temp = temp->left;
      }
      else if (Cmp{}(temp->data.first, key))
      {
        temp = temp->right;
      }
      else if (!(Cmp{}(key, temp->data.first), Cmp{}(temp->data.first, key)))
      {
        return const_it_t(temp);
      }
    }
    return cend();
  }

  template< typename K, typename T, typename Cmp >
  T & AVLTree< K, T, Cmp >::at(const K & key)
  {
    return const_cast< T & >(static_cast< const AVLTree * >(this)->at(key));
  }

  template< typename K, typename T, typename Cmp >
  const T & AVLTree< K, T, Cmp >::at(const K & key) const
  {
    auto temp = root_;
    while (temp)
    {
      if (Cmp{}(key, temp->data.first))
      {
        temp = temp->left;
      }
      else if (Cmp{}(temp->data.first, key))
      {
        temp = temp->right;
      }
      else if (!(Cmp{}(key, temp->data.first), Cmp{}(temp->data.first, key)))
      {
        break;
      }
    }
    if (temp)
    {
      return temp->data->second;
    }
    else
    {
      throw std::out_of_range();
    }
  }

  template< typename K, typename T, typename Cmp >
  void AVLTree< K, T, Cmp >::clear() noexcept
  {
    clearBiTree(root_);
    root_ = nullptr;
  }

  template< typename K, typename T, typename Cmp >
  void AVLTree< K, T, Cmp >::clearBiTree(node_t * root) noexcept
  {
    if (root)
    {
      clearBiTree(root->left);
      clearBiTree(root->right);
      delete root;
      root = nullptr;
      size_--;
    }
  }

  template< typename K, typename T, typename Cmp >
  void AVLTree< K, T, Cmp >::balance(node_t * node)
  {
    if (node->left && node->right)
    {
      if (std::less< int >{}(node->left->height, node->right->height))
      {
        node = node->right;
        if (node->left && node->right && std::less< int >{}(node->left->height, node->right->height))
        {
          leftRotate(node);
        }
        else if (node->left && node->right)
        {
          rightRotate(node->left);
          leftRotate(node->parent);
        }
        else if (!node->left)
        {
          if (!node->right->left)
          {
            leftRotate(node->right);
          }
          else
          {
            rightRotate(node->right->left);
            leftRotate(node->right);
          }
        }
        else if (!node->right)
        {
          if (!node->left->right)
          {
            rightRotate(node->left);
          }
          else
          {
            leftRotate(node->left->right);
            rightRotate(node->left);
          }
        }
      }
      else
      {
        node = node->left;
        if (node->left && node->right && std::less< int >{}(node->right->height, node->left->height))
        {
          rightRotate(node);
        }
        else if (node->left && node->right)
        {
          leftRotate(node->right);
          rightRotate(node->parent);
        }
        else if (!node->left)
        {
          if (!node->right->left)
          {
            leftRotate(node->right);
          }
          else
          {
            rightRotate(node->right->left);
            leftRotate(node->right);
          }
        }
        else if (!node->right)
        {
          if (!node->left->right)
          {
            rightRotate(node->left);
          }
          else
          {
            leftRotate(node->left->right);
            rightRotate(node->left);
          }
        }
      }
    }
    else if (!node->left)
    {
      if (!node->right->left)
      {
        leftRotate(node->right);
      }
      else
      {
        rightRotate(node->right->left);
        leftRotate(node->right);
      }
    }
    else if (!node->right)
    {
      if (!node->left->right)
      {
        rightRotate(node->left);
      }
      else
      {
        leftRotate(node->left->right);
        rightRotate(node->left);
      }
    }
  }

  template< typename K, typename T, typename Cmp >
  void AVLTree< K, T, Cmp >::leftRotate(node_t * node)
  {
    auto grandpa = node->parent;
    auto son = node->left;
    if (grandpa->parent)
    {
      if (grandpa->parent->right == grandpa)
      {
        grandpa->parent->right = node;
      }
      else
      {
        grandpa->parent->left = node;
      }
      node->parent = grandpa->parent;
      grandpa->right = son;
      grandpa->parent = node;
      node->left = grandpa;
      if (son)
      {
        son->parent = grandpa;
      }
    }
    else
    {
      root_ = node;
      node->parent = nullptr;
      grandpa->right = son;
      grandpa->parent = node;
      node->left = grandpa;
      if (son)
      {
        son->parent = grandpa;
      }
    }
  }

  template< typename K, typename T, typename Cmp >
  void AVLTree< K, T, Cmp >::rightRotate(node_t * node)
  {
    auto grandpa = node->parent;
    auto son = node->right;
    if (grandpa->parent)
    {
      if (grandpa->parent->right == grandpa)
      {
        grandpa->parent->right = node;
      }
      else
      {
        grandpa->parent->left = node;
      }
      node->parent = grandpa->parent;
      grandpa->left = son;
      grandpa->parent = node;
      node->right = grandpa;
      if (son)
      {
        son->parent = grandpa;
      }
    }
    else
    {
      root_ = node;
      node->parent = nullptr;
      grandpa->left = son;
      grandpa->parent = node;
      node->right = grandpa;
      if (son)
      {
        son->parent = grandpa;
      }
    }
  }

  template< typename K, typename T, typename Cmp >
  void AVLTree< K, T, Cmp >::swap(this_t & rhs) noexcept
  {
    std::swap(root_, rhs.root_);
    std::swap(size_, rhs.size_);
    std::swap(erased_, rhs.erased_);
  }

  template< typename K, typename T, typename Cmp >
  template< typename Pair >
  typename std::pair< ConstAVLTreeIterator< K, T, Cmp >, bool > AVLTree< K, T, Cmp >::insertImpl(Pair && val)
  {
    if (empty())
    {
      root_ = new node_t{ std::forward< Pair >(val), nullptr, nullptr, nullptr, 1 };
      size_++;
    }
    else
    {
      auto temp = root_;
      while (temp->left || temp->right)
      {
        if (temp->left && Cmp{}(val.first, temp->data.first))
        {
          temp = temp->left;
        }
        else if (temp->right && Cmp{}(temp->data.first, val.first))
        {
          temp = temp->right;
        }
        else if (!Cmp{}(val.first, temp->data.first) && !Cmp{}(temp->data.first, val.first))
        {
          return std::pair< const_it_t, bool >{ const_it_t(temp), false };
        }
        else
        {
          break;
        }
      }
      if (Cmp{}(val.first, temp->data.first))
      {
        temp->left = new node_t{ std::forward< Pair >(val), nullptr, nullptr, temp, 1 };
        temp = temp->left;
      }
      else if (Cmp{}(temp->data.first, val.first))
      {
        temp->right = new node_t{ std::forward< Pair >(val), nullptr, nullptr, temp, 1 };
        temp = temp->right;
      }
      else if (!Cmp{}(val.first, temp->data.first) && !Cmp{}(temp->data.first, val.first))
      {
        return std::pair< const_it_t, bool >{ const_it_t(temp), false };
      }
      size_++;
      while (temp->parent)
      {
        temp = temp->parent;
        temp->setHeight();
        if (temp->left && temp->right && std::abs(temp->left->height - temp->right->height) > 1)
        {
          balance(temp);
          temp->setHeight();
        }
        else if (!temp->left && temp->right && temp->right->height > 1)
        {
          balance(temp);
          temp->setHeight();
        }
        else if (!temp->right && temp->left && temp->left->height > 1)
        {
          balance(temp);
          temp->setHeight();
        }
      }
      return std::pair< const_it_t, bool >{ const_it_t(temp), true };
    }
    return std::pair< const_it_t, bool >{ const_it_t(root_), true };
  }

  template< typename K, typename T, typename Cmp >
  template< typename F >
  F AVLTree< K, T, Cmp >::traverse_lnr(F f)
  {
    petrov::Stack< node_t * > stack;
    node_t * node = root_;
    while (!stack.empty() || node)
    {
      if (node)
      {
        stack.push(node);
        node = node->left;
      }
      else
      {
        node = stack.top();
        stack.pop();
        f(node->data);
        node = node->right;
      }
    }
    return f;
  }

  template< typename K, typename T, typename Cmp >
  template< typename F >
  F AVLTree< K, T, Cmp >::traverse_lnr(F f) const
  {
    petrov::Stack< node_t * > stack;
    node_t * node = root_;
    while (!stack.empty() || node)
    {
      if (node)
      {
        stack.push(node);
        node = node->left;
      }
      else
      {
        node = stack.top();
        stack.pop();
        f(node->data);
        node = node->right;
      }
    }
    return f;
  }

  template< typename K, typename T, typename Cmp >
  template< typename F >
  F AVLTree< K, T, Cmp >::traverse_rnl(F f)
  {
    petrov::Stack< node_t * > stack;
    node_t * node = root_;
    while (!stack.empty() || node)
    {
      if (node)
      {
        stack.push(node);
        node = node->right;
      }
      else
      {
        node = stack.top();
        stack.pop();
        f(node->data);
        node = node->left;
      }
    }
    return f;
  }

  template< typename K, typename T, typename Cmp >
  template< typename F >
  F AVLTree< K, T, Cmp >::traverse_rnl(F f) const
  {
    petrov::Stack< node_t * > stack;
    node_t * node = root_;
    while (!stack.empty() || node)
    {
      if (node)
      {
        stack.push(node);
        node = node->right;
      }
      else
      {
        node = stack.top();
        stack.pop();
        f(node->data);
        node = node->left;
      }
    }
    return f;
  }

  template< typename K, typename T, typename Cmp >
  template< typename F >
  F AVLTree< K, T, Cmp >::traverse_breadth(F f)
  {
    petrov::Queue< node_t * > queue;
    node_t * node = root_;
    if (node)
    {
      queue.push(node);
    }
    while (!queue.empty())
    {
      node = queue.front();
      f(node->data);
      queue.pop();
      if (node->left)
      {
        queue.push(node->left);
      }
      if (node->right)
      {
        queue.push(node->right);
      }
    }
    return f;
  }

  template< typename K, typename T, typename Cmp >
  template< typename F >
  F AVLTree< K, T, Cmp >::traverse_breadth(F f) const
  {
    petrov::Queue< node_t * > queue;
    node_t * node = root_;
    if (node)
    {
      queue.push(node);
    }
    while (!queue.empty())
    {
      node = queue.front();
      f(node->data);
      queue.pop();
      if (node->left)
      {
        queue.push(node->left);
      }
      if (node->right)
      {
        queue.push(node->right);
      }
    }
    return f;
  }
}

#endif
