#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <cassert>
#include "node.hpp"

namespace demehin
{
  template< typename Key, typename T, typename Cmp >
  class Tree;

  template< typename Key, typename T, typename Cmp >
  class TreeConstIterator;

  template< typename Key, typename T, typename Cmp >
  class TreeIterator
  {
    friend class Tree< Key, T, Cmp >;
    friend class TreeConstIterator< Key, T, Cmp >;
  public:
    using this_t = TreeIterator< Key, T, Cmp >;
    using Node = demehin::TreeNode< Key, T >;
    using DataPair = std::pair< Key, T >;

    TreeIterator() noexcept;
    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t& operator--() noexcept;
    this_t operator--(int) noexcept;

    DataPair& operator*() const;
    DataPair* operator->() const noexcept;

    bool operator==(const this_t& rhs) const noexcept;
    bool operator!=(const this_t& rhs) const noexcept;

  private:
    Node* getNode() const noexcept;
    Node* node_;

    explicit TreeIterator(Node*) noexcept;
  };

  template< typename Key, typename T, typename Cmp >
  TreeIterator< Key, T, Cmp >::TreeIterator() noexcept:
    node_(nullptr)
  {}

  template< typename Key, typename T, typename Cmp >
  TreeIterator< Key, T, Cmp >::TreeIterator(Node* node) noexcept:
    node_(node)
  {}

  template< typename Key, typename T, typename Cmp >
  typename TreeIterator< Key, T, Cmp >::this_t& TreeIterator< Key, T, Cmp >::operator++() noexcept
  {
    assert(node_ != nullptr);
    if (node_->right != nullptr && node_->right->height != -1)
    {
      node_ = node_->right;
      while (node_->left != nullptr && node_->left->height != -1)
      {
        node_ = node_->left;
      }
    }
    else
    {
      Node* parent = node_->parent;
      while (parent != nullptr && parent->height != -1 && node_ == parent->right)
      {
        node_ = parent;
        parent = parent->parent;
      }
      node_= parent;
    }
    return *this;
  }

  template< typename Key, typename T, typename Cmp >
  typename TreeIterator< Key, T, Cmp >::this_t TreeIterator< Key, T, Cmp >::operator++(int) noexcept
  {
    assert(node_ != nullptr);
    this_t res(*this);
    ++(*this);
    return res;
  }

  template< typename Key, typename T, typename Cmp >
  typename TreeIterator< Key, T, Cmp >::this_t& TreeIterator< Key, T, Cmp >::operator--() noexcept
  {
    assert(node_ != nullptr);
    if (node_->left != nullptr)
    {
      node_ = node_->left;
      while (node_->right != nullptr)
      {
        node_ = node_->right;
      }
    }
    else
    {
      while ((node_->parent != nullptr) && (node_->parent->left == node_))
      {
        node_ = node_->parent;
      }
      node_ = node_->parent;
    }
    return *this;
  }

  template< typename Key, typename T, typename Cmp >
  typename TreeIterator< Key, T, Cmp >::this_t TreeIterator< Key, T, Cmp >::operator--(int) noexcept
  {
    assert(node_ != nullptr);
    this_t res(*this);
    --(*this);
    return res;
  }

  template< typename Key, typename T, typename Cmp >
  typename TreeIterator< Key, T, Cmp >::DataPair& TreeIterator< Key, T, Cmp >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< typename Key, typename T, typename Cmp >
  typename TreeIterator< Key, T, Cmp >::DataPair* TreeIterator< Key, T, Cmp >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename Key, typename T, typename Cmp >
  bool TreeIterator< Key, T, Cmp >::operator==(const this_t& rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< typename Key, typename T, typename Cmp >
  bool TreeIterator< Key, T, Cmp >::operator!=(const this_t& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< typename Key, typename T, typename Cmp >
  typename TreeIterator< Key, T, Cmp >::Node* TreeIterator< Key, T, Cmp >::getNode() const noexcept
  {
    return node_;
  }
}

#endif
