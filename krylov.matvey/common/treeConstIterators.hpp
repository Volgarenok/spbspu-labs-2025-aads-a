#ifndef ConstIteratorS_HPP
#define ConstIteratorS_HPP
#include "treeNode.hpp"
#include "binaryTree.hpp"

namespace krylov
{
  template< typename Key, typename T, typename Cmp >
  class BiTree;

  template< typename Key, typename T, typename Cmp >
  class ConstIterator
  {
  public:
    ConstIterator();
    ConstIterator(const ConstIterator< Key, T, Cmp >&) = default;
    ~ConstIterator() = default;
    ConstIterator< Key, T, Cmp >& operator=(const ConstIterator< Key, T, Cmp >&) = default;
    ConstIterator< Key, T, Cmp >& operator++();
    ConstIterator< Key, T, Cmp > operator++(int);
    ConstIterator< Key, T, Cmp >& operator--();
    ConstIterator< Key, T, Cmp > operator--(int);
    const std::pair< Key, T >& operator*() const;
    const std::pair< Key, T >* operator->() const;
    bool operator==(const ConstIterator< Key, T, Cmp >& rhs) const;
    bool operator!=(const ConstIterator< Key, T, Cmp >& rhs) const;
  private:
    BiTreeNode< Key, T >* node_;
    BiTreeNode< Key, T >* fakeLeaf_;
    friend class BiTree< Key, T, Cmp >;
    explicit ConstIterator(BiTreeNode< Key, T >* node, BiTreeNode< Key, T >* fakeLeaf);
  };

  template< typename Key, typename T, typename Cmp >
  ConstIterator< Key, T, Cmp >::ConstIterator():
    node_(nullptr),
    fakeLeaf_(nullptr)
  {}

  template< typename Key, typename T, typename Cmp >
  ConstIterator< Key, T, Cmp >::ConstIterator(BiTreeNode< Key, T >* node, BiTreeNode< Key, T >* fakeLeaf):
    node_(node),
    fakeLeaf_(fakeLeaf)
  {}

  template< typename Key, typename T, typename Cmp >
  const std::pair< Key, T >& ConstIterator< Key, T, Cmp >::operator*() const
  {
    return node_->data;
  }

  template< typename Key, typename T, typename Cmp >
  const std::pair< Key, T >* ConstIterator< Key, T, Cmp >::operator->() const
  {
    return std::addressof(node_->data);
  }

  template< typename Key, typename T, typename Cmp >
  bool ConstIterator< Key, T, Cmp >::operator!=(const ConstIterator< Key, T, Cmp >& rhs) const
  {
    return node_ != rhs.node_;
  }

  template< typename Key, typename T, typename Cmp >
  bool ConstIterator< Key, T, Cmp >::operator==(const ConstIterator< Key, T, Cmp >& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename Key, typename T, typename Cmp >
  typename ConstIterator< Key, T, Cmp >::ConstIterator< Key, T, Cmp >& ConstIterator< Key, T, Cmp >::operator++()
  {
    if (node_ == fakeLeaf_)
    {
      return *this;
    }
    if (node_->right != fakeLeaf_)
    {
      node_ = node_->right;
      while (node_->left != fakeLeaf_)
      {
        node_ = node_->left;
      }
    }
    else
    {
      BiTreeNode< Key, T >* parent = node_->parent;
      while ((parent != fakeLeaf_->parent) && (node_ == parent->right))
      {
        node_ = parent;
        parent = parent->parent;
      }
      if (parent == fakeLeaf_->parent)
      {
        node_ = fakeLeaf_;
      }
      else
      {
        node_ = parent;
      }
    }
    return *this;
  }

  template< typename Key, typename T, typename Cmp >
  typename ConstIterator< Key, T, Cmp >::ConstIterator< Key, T, Cmp > ConstIterator< Key, T, Cmp >::operator++(int)
  {
    ConstIterator< Key, T, Cmp > temp(*this);
    ++(*this);
    return temp;
  }

  template< typename Key, typename T, typename Cmp >
  typename ConstIterator< Key, T, Cmp >::ConstIterator< Key, T, Cmp >& ConstIterator< Key, T, Cmp >::operator--()
  {
    if (node_ == fakeLeaf_)
    {
      node_ = fakeLeaf_->parent->left;
      while (node_->right != fakeLeaf_)
      {
        node_ = node_->right;
      }
    }
    else
    {
      if (node_->left == fakeLeaf_)
      {
        BiTreeNode< Key, T >* parent = node_->parent;
        while ((parent != fakeLeaf_->parent) && (node_ == parent->left))
        {
          node_ = parent;
          parent = parent->parent;
        }
        if (parent == fakeLeaf_->parent)
        {
          node_ = fakeLeaf_;
        }
        else
        {
          node_ = parent;
        }
      }
      else
      {
        node_ = node_->left;
        while (node_->right != fakeLeaf_)
        {
          node_ = node_->right;
        }
      }
    }
    return *this;
  }

  template< typename Key, typename T, typename Cmp >
  typename ConstIterator< Key, T, Cmp >::ConstIterator< Key, T, Cmp > ConstIterator< Key, T, Cmp >::operator--(int)
  {
    ConstIterator< Key, T, Cmp > temp(*this);
    --(*this);
    return temp;
  }
}

#endif
