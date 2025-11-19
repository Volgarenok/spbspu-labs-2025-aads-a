#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP
#include <cstddef>
#include <stack>
#include <queue>
#include <stdexcept>
#include "treeNode.hpp"
#include "treeConstIterators.hpp"

namespace krylov
{
  template< typename Key, typename T, typename Cmp >
  class ConstIterator;

  template< typename Key, typename T, typename Cmp >
  class BiTree
  {
  public:
    BiTree();
    BiTree(const BiTree< Key, T, Cmp >& rhs);
    BiTree(BiTree< Key, T, Cmp >&& rhs) noexcept;
    ~BiTree();
    BiTree< Key, T, Cmp >& operator=(const BiTree< Key, T, Cmp >& rhs);
    BiTree< Key, T, Cmp >& operator=(BiTree< Key, T, Cmp >&& rhs) noexcept;
    ConstIterator< Key, T, Cmp > cbegin() const noexcept;
    ConstIterator< Key, T, Cmp > cend() const noexcept;
    T& at(const Key & key);
    const T& at(const Key& key) const;
    T& operator[](const Key& key);
    ConstIterator< Key, T, Cmp > find(const Key& key) const;
    void clear() noexcept;
    void swap(BiTree< Key, T, Cmp >& rhs) noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
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
    BiTreeNode< Key, T > * fakeRoot_;
    BiTreeNode< Key, T > * fakeLeaf_;
    size_t size_;
    Cmp cmp_;
    void clearTree(BiTreeNode< Key, T >* root);
  };

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp >::BiTree():
    fakeRoot_(new BiTreeNode< Key, T >{ std::pair< Key, T >{}, nullptr, nullptr, nullptr }),
    fakeLeaf_(new BiTreeNode< Key, T >{ std::pair< Key, T >{}, nullptr, nullptr, nullptr }),
    size_(0),
    cmp_(Cmp{})
  {
    fakeLeaf_->parent = fakeRoot_;
    fakeRoot_->left = fakeLeaf_;
    fakeRoot_->right = fakeLeaf_;
  }

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp >::~BiTree()
  {
    clear();
    delete fakeRoot_;
    delete fakeLeaf_;
  }

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp >::BiTree(const BiTree< Key, T, Cmp >& rhs):
    BiTree()
  {
    for (auto it = rhs.cbegin(); it != rhs.cend(); ++it)
    {
      (*this)[it->first] = it->second;
    }
  }

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp >::BiTree(BiTree< Key, T, Cmp >&& rhs) noexcept:
    fakeRoot_(rhs.fakeRoot_),
    fakeLeaf_(rhs.fakeLeaf_),
    size_(rhs.size_),
    cmp_(rhs.cmp_)
  {
    rhs.fakeRoot_ = new BiTreeNode< Key, T >{ std::pair< Key, T >{}, nullptr, nullptr, nullptr };
    rhs.fakeLeaf_ = new BiTreeNode< Key, T >{ std::pair< Key, T >{}, nullptr, nullptr, rhs.fakeRoot_ };
    rhs.fakeRoot_->left = rhs.fakeLeaf_;
    rhs.fakeRoot_->right = rhs.fakeLeaf_;
    rhs.size_ = 0;
  }

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp >& BiTree< Key, T, Cmp >::operator=(const BiTree< Key, T, Cmp >& rhs)
  {
    if (this != std::addressof(rhs))
    {
      BiTree temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp >& BiTree< Key, T, Cmp >::operator=(BiTree< Key, T, Cmp >&& rhs) noexcept
  {
    if (this != std::addressof(rhs))
    {
      clear();
      delete fakeRoot_;
      delete fakeLeaf_;
      fakeRoot_ = rhs.fakeRoot_;
      fakeLeaf_ = rhs.fakeLeaf_;
      size_ = rhs.size_;
      cmp_ = std::move(rhs.cmp_);
      rhs.fakeRoot_ = new BiTreeNode< Key, T >{std::pair< Key, T >{}, nullptr, nullptr, nullptr};
      rhs.fakeLeaf_ = new BiTreeNode< Key, T >{std::pair< Key, T >{}, nullptr, nullptr, rhs.fakeRoot_};
      rhs.fakeRoot_->left = rhs.fakeLeaf_;
      rhs.fakeRoot_->right = rhs.fakeLeaf_;
      rhs.size_ = 0;
    }
    return *this;
  }

  template< typename Key, typename T, typename Cmp >
  ConstIterator< Key, T, Cmp > BiTree< Key, T, Cmp >::cbegin() const noexcept
  {
    if (empty())
    {
      return cend();
    }
    BiTreeNode< Key, T >* current = fakeRoot_->left;
    while (current->left != fakeLeaf_)
    {
      current = current->left;
    }
    return ConstIterator< Key, T, Cmp >(current, fakeLeaf_);
  }

  template< typename Key, typename T, typename Cmp >
  ConstIterator< Key, T, Cmp > BiTree< Key, T, Cmp >::cend() const noexcept
  {
    return ConstIterator< Key, T, Cmp >(fakeLeaf_, fakeLeaf_);
  }

  template< typename Key, typename T, typename Cmp >
  size_t BiTree< Key, T, Cmp >::size() const noexcept
  {
    return size_;
  }

  template< typename Key, typename T, typename Cmp >
  bool BiTree< Key, T, Cmp >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename Key, typename T, typename Cmp >
  void BiTree< Key, T, Cmp >::swap(BiTree< Key, T, Cmp >& rhs) noexcept
  {
    std::swap(fakeRoot_, rhs.fakeRoot_);
    std::swap(fakeLeaf_, rhs.fakeLeaf_);
    std::swap(size_, rhs.size_);
  }

  template< typename Key, typename T, typename Cmp >
  ConstIterator< Key, T, Cmp > BiTree< Key, T, Cmp >::find(const Key& key) const
  {
    BiTreeNode< Key, T > * current = fakeRoot_->left;
    while (current != fakeLeaf_)
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
        return ConstIterator< Key, T, Cmp >(current, fakeLeaf_);
      }
    }
    return ConstIterator< Key, T, Cmp >(fakeLeaf_, fakeLeaf_);
  }

  template< typename Key, typename T, typename Cmp >
  T& BiTree< Key, T, Cmp >::operator[](const Key& key)
  {
    if (empty())
    {
      BiTreeNode< Key, T >* newNode = new BiTreeNode< Key, T >{ std::pair< Key, T >(key, T()), fakeLeaf_, fakeLeaf_, fakeRoot_ };
      fakeRoot_->left = newNode;
      size_++;
      return newNode->data.second;
    }
    BiTreeNode< Key, T >* current = fakeRoot_->left;
    BiTreeNode< Key, T >* parent = fakeRoot_;

    while (current != fakeLeaf_)
    {
      if (cmp_(key, current->data.first))
      {
          parent = current;
          current = current->left;
      }
      else if (cmp_(current->data.first, key))
      {
        parent = current;
        current = current->right;
      }
      else
      {
        return current->data.second;
      }
    }

    BiTreeNode< Key, T >* newNode = new BiTreeNode< Key, T >{ std::pair< Key, T >(key, T()), fakeLeaf_, fakeLeaf_, parent };
    if (cmp_(key, parent->data.first))
    {
      parent->left = newNode;
    }
    else
    {
      parent->right = newNode;
    }
    size_++;
    return newNode->data.second;
  }

  template< typename Key, typename T, typename Cmp >
  T& BiTree< Key, T, Cmp >::at(const Key& key)
  {
    BiTreeNode< Key, T >* current = fakeRoot_->left;
    while (current != fakeLeaf_)
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
        return current->data.second;
      }
    }
    throw std::out_of_range("ERROR: Key not found");
  }

  template< typename Key, typename T, typename Cmp >
  const T& BiTree< Key, T, Cmp >::at(const Key& key) const
  {
    const BiTreeNode< Key, T >* current = fakeRoot_->left;
    while (current != fakeLeaf_)
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
        return current->data.second;
      }
    }
    throw std::out_of_range("ERROR: Key not found");
  }

  template< typename Key, typename T, typename Cmp >
  void BiTree< Key, T, Cmp >::clear() noexcept
  {
    clearTree(fakeRoot_->left);
    clearTree(fakeRoot_->right);
    fakeRoot_->left = fakeLeaf_;
    fakeRoot_->right = fakeLeaf_;
    size_ = 0;
  }

  template< typename Key, typename T, typename Cmp >
  void BiTree< Key, T, Cmp >::clearTree(BiTreeNode< Key, T >* root)
  {
    if (root == fakeLeaf_)
    {
      return;
    }
    clearTree(root->left);
    clearTree(root->right);
    delete root;
  }

  template< typename Key, typename T, typename Cmp >
  template< typename F >
  F BiTree< Key, T, Cmp >::traverseLnr(F f)
  {
    std::stack< BiTreeNode< Key, T >* > stack;
    BiTreeNode< Key, T >* current = fakeRoot_->left;
    bool leftTraversed = false;
    while (current != fakeLeaf_ || !stack.empty())
    {
      if (!leftTraversed && current != fakeLeaf_)
      {
        stack.push(current);
        current = current->left;
      }
      else
      {
        current = stack.top();
        stack.pop();
        f(current->data);
        current = current->right;
        leftTraversed = (current == fakeLeaf_);
      }
    }
    return f;
  }

  template< typename Key, typename T, typename Cmp >
  template< typename F >
  F BiTree< Key, T, Cmp >::traverseLnr(F f) const
  {
    return const_cast< BiTree< Key, T, Cmp >* >(this)->traverseLnr(f);
  }

  template< typename Key, typename T, typename Cmp >
  template< typename F >
  F BiTree< Key, T, Cmp >::traverseRnl(F f)
  {
    std::stack< BiTreeNode< Key, T >* > stack;
    BiTreeNode< Key, T >* current = fakeRoot_->left;
    bool rightTraversed = false;
    while (current != fakeLeaf_ || !stack.empty())
    {
      if (!rightTraversed && current != fakeLeaf_)
      {
        stack.push(current);
        current = current->right;
      }
      else
      {
        current = stack.top();
        stack.pop();
        f(current->data);
        current = current->left;
        rightTraversed = (current == fakeLeaf_);
      }
    }
    return f;
  }

  template< typename Key, typename T, typename Cmp >
  template< typename F >
  F BiTree< Key, T, Cmp >::traverseRnl(F f) const
  {
    return const_cast< BiTree< Key, T, Cmp >* >(this)->traverseRnl(f);
  }

  template< typename Key, typename T, typename Cmp >
  template< typename F >
  F BiTree< Key, T, Cmp >::traverseBreadth(F f)
  {
    std::queue< BiTreeNode< Key, T >* > queue;
    queue.push(fakeRoot_->left);
    while (!queue.empty())
    {
      BiTreeNode< Key, T >* current = queue.front();
      queue.pop();
      f(current->data);
      if (current->left != fakeLeaf_)
      {
        queue.push(current->left);
      }
      if (current->right != fakeLeaf_)
      {
        queue.push(current->right);
      }
    }
    return f;
  }

  template< typename Key, typename T, typename Cmp >
  template< typename F >
  F BiTree< Key, T, Cmp >::traverseBreadth(F f) const
  {
    return const_cast< BiTree< Key, T, Cmp >* >(this)->traverseBreadth(f);
  }
}

#endif
