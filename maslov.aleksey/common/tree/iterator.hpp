#ifndef TREE_ITERATOR_HPP
#define TREE_ITERATOR_HPP

#include <cassert>
#include <iterator>
#include "node.hpp"

namespace maslov
{
  template< typename Key, typename T, typename Cmp >
  struct BiTree;

  template< typename Key, typename T, typename Cmp >
  struct TreeIterator
  {
    friend struct BiTree< Key, T, Cmp >;
   public:
    using thisT = TreeIterator< Key, T, Cmp >;

    TreeIterator();
    TreeIterator(const thisT &) = default;
    ~TreeIterator() = default;
    thisT & operator=(const thisT &) = default;
    thisT & operator++();
    thisT operator++(int);
    thisT & operator--();
    thisT operator--(int);
    std::pair< Key, T > & operator*();
    std::pair< Key, T > * operator->();
    bool operator!=(const thisT &) const;
    bool operator==(const thisT &) const;
   private:
    BiTreeNode< Key, T > * node_;
    BiTreeNode< Key, T > * fakeLeaf_;
    explicit TreeIterator(BiTreeNode< Key, T > * node, BiTreeNode< Key, T > * fakeLeaf);
  };

  template< typename Key, typename T, typename Cmp >
  TreeIterator< Key, T, Cmp >::TreeIterator():
    node_(nullptr),
    fakeLeaf_(nullptr)
  {}

  template< typename Key, typename T, typename Cmp >
  TreeIterator< Key, T, Cmp >::TreeIterator(BiTreeNode< Key, T > * node, BiTreeNode< Key, T > * fakeLeaf):
    node_(node),
    fakeLeaf_(fakeLeaf)
  {}

  template< typename Key, typename T, typename Cmp >
  typename TreeIterator< Key, T, Cmp >::thisT & TreeIterator< Key, T, Cmp >::operator++()
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
      BiTreeNode< Key, T > * parent = node_->parent;
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
  typename TreeIterator< Key, T, Cmp >::thisT TreeIterator< Key, T, Cmp >::operator++(int)
  {
    thisT tmp(*this);
    ++(*this);
    return tmp;
  }

  template< typename Key, typename T, typename Cmp >
  typename TreeIterator< Key, T, Cmp >::thisT & TreeIterator< Key, T, Cmp >::operator--()
  {
    assert(fakeLeaf_->parent->left != fakeLeaf_);
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
        BiTreeNode< Key, T > * parent = node_->parent;
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
  typename TreeIterator< Key, T, Cmp >::thisT TreeIterator< Key, T, Cmp >::operator--(int)
  {
    thisT tmp(*this);
    --(*this);
    return tmp;
  }

  template< typename Key, typename T, typename Cmp >
  std::pair< Key, T > & TreeIterator< Key, T, Cmp >::operator*()
  {
    assert(node_ != nullptr && node_ != fakeLeaf_);
    return node_->data;
  }

  template< typename Key, typename T, typename Cmp >
  std::pair< Key, T > * TreeIterator< Key, T, Cmp >::operator->()
  {
    assert(node_ != nullptr && node_ != fakeLeaf_);
    return std::addressof(node_->data);
  }

  template< typename Key, typename T, typename Cmp >
  bool TreeIterator< Key, T, Cmp >::operator!=(const thisT & rhs) const
  {
    return node_ != rhs.node_;
  }

  template< typename Key, typename T, typename Cmp >
  bool TreeIterator< Key, T, Cmp >::operator==(const thisT & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename Key, typename T, typename Cmp >
  struct TreeConstIterator
  {
    friend struct BiTree< Key, T, Cmp >;
   public:
    using thisT = TreeConstIterator< Key, T, Cmp >;

    TreeConstIterator();
    TreeConstIterator(const thisT &) = default;
    ~TreeConstIterator() = default;
    thisT & operator=(const thisT &) = default;
    thisT & operator++();
    thisT operator++(int);
    thisT & operator--();
    thisT operator--(int);
    const std::pair< Key, T > & operator*() const;
    const std::pair< Key, T > * operator->() const;
    bool operator==(const thisT & rhs) const;
    bool operator!=(const thisT & rhs) const;
   private:
    BiTreeNode< Key, T > * node_;
    BiTreeNode< Key, T > * fakeLeaf_;
    explicit TreeConstIterator(BiTreeNode< Key, T > * node, BiTreeNode< Key, T > * fakeLeaf);
  };

  template< typename Key, typename T, typename Cmp >
  TreeConstIterator< Key, T, Cmp >::TreeConstIterator():
    node_(nullptr),
    fakeLeaf_(nullptr)
  {}

  template< typename Key, typename T, typename Cmp >
  TreeConstIterator< Key, T, Cmp >::TreeConstIterator(BiTreeNode< Key, T > * node, BiTreeNode< Key, T > * fakeLeaf):
    node_(node),
    fakeLeaf_(fakeLeaf)
  {}

  template< typename Key, typename T, typename Cmp >
  typename TreeConstIterator< Key, T, Cmp >::thisT & TreeConstIterator< Key, T, Cmp >::operator++()
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
      BiTreeNode< Key, T > * parent = node_->parent;
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
  typename TreeConstIterator< Key, T, Cmp >::thisT TreeConstIterator< Key, T, Cmp >::operator++(int)
  {
    thisT tmp(*this);
    ++(*this);
    return tmp;
  }

  template< typename Key, typename T, typename Cmp >
  typename TreeConstIterator< Key, T, Cmp >::thisT & TreeConstIterator< Key, T, Cmp >::operator--()
  {
    assert(fakeLeaf_->parent->left != fakeLeaf_);
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
        BiTreeNode< Key, T > * parent = node_->parent;
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
  typename TreeConstIterator< Key, T, Cmp >::thisT TreeConstIterator< Key, T, Cmp >::operator--(int)
  {
    thisT tmp(*this);
    --(*this);
    return tmp;
  }

  template< typename Key, typename T, typename Cmp >
  const std::pair< Key, T > & TreeConstIterator< Key, T, Cmp >::operator*() const
  {
    assert(node_ != nullptr && node_ != fakeLeaf_);
    return node_->data;
  }

  template< typename Key, typename T, typename Cmp >
  const std::pair< Key, T > * TreeConstIterator< Key, T, Cmp >::operator->() const
  {
    assert(node_ != nullptr && node_ != fakeLeaf_);
    return std::addressof(node_->data);
  }

  template< typename Key, typename T, typename Cmp >
  bool TreeConstIterator< Key, T, Cmp >::operator!=(const thisT & rhs) const
  {
    return node_ != rhs.node_;
  }

  template< typename Key, typename T, typename Cmp >
  bool TreeConstIterator< Key, T, Cmp >::operator==(const thisT & rhs) const
  {
    return node_ == rhs.node_;
  }
}

#endif
