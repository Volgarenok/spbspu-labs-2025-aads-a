#ifndef CONSTITERATOR_HPP
#define CONSTITERATOR_HPP

#include <utility>
#include <functional>

namespace dribas
{
  template < class Key, class T, class Compare >
  class AVLTree;

  template < class Key, class T >
  class Node;

  template < class Key, class T, class Compare >
  class Iterator;

  template < class Key, class T, class Compare = std::less< Key > >
  class ConstIterator
  {
    friend class AVLTree< Key, T, Compare >;
  public:
    using valueType = std::pair< Key, T >;
    using TreeType = AVLTree< Key, T, Compare >;
    using NodeType = Node< Key, T >;

    ConstIterator() noexcept;
    ConstIterator(const Iterator< Key, T, Compare >&) noexcept;
    const valueType& operator*() const noexcept;
    const valueType* operator->() const noexcept;
    ConstIterator& operator++() noexcept;
    ConstIterator operator++(int) noexcept;
    ConstIterator& operator--() noexcept;
    ConstIterator operator--(int) noexcept;

    bool operator==(const ConstIterator&) const noexcept;
    bool operator!=(const ConstIterator&) const noexcept;

  private:
    const NodeType* node_;
    const TreeType* tree_;
    explicit ConstIterator(const Node< Key, T >*, const AVLTree< Key, T, Compare >*) noexcept;
  };

  template < class Key, class T, class Compare >
  ConstIterator< Key, T, Compare >::ConstIterator(const Iterator< Key, T, Compare >& other) noexcept:
    node_(other.node_), tree_(other.tree_)
  {}

  template < class Key, class T, class Compare >
  ConstIterator< Key, T, Compare >::ConstIterator() noexcept:
    node_(nullptr), tree_(nullptr)
  {}

  template < class Key, class T, class Compare >
  ConstIterator< Key, T, Compare >::ConstIterator(const Node< Key, T >* node, const AVLTree< Key, T, Compare >* tree) noexcept:
    node_(node), tree_(tree)
  {}

  template < class Key, class T, class Compare >
  const std::pair< Key, T >& ConstIterator< Key, T, Compare >::operator*() const noexcept
  {
    return node_->value;
  }

  template < class Key, class T, class Compare >
  const std::pair< Key, T >* ConstIterator< Key, T, Compare >::operator->() const noexcept
  {
    return std::addressof(node_->value);
  }

  template < class Key, class T, class Compare >
  ConstIterator< Key, T, Compare >& ConstIterator< Key, T, Compare >::operator++() noexcept
  {
    if (!node_->right->isFake) {
      node_ = node_->right;
      while (!node_->left->isFake) {
        node_ = node_->left;
      }
    } else {
      auto* parent = node_->parent;
      while (parent != nullptr && node_ == parent->right) {
        node_ = parent;
        parent = parent->parent;
      }
      node_= parent;
    }
    if (!node_) {
      node_ = tree_->fakeleaf_;
    }
    return *this;
  }

  template < class Key, class T, class Compare >
  ConstIterator< Key, T, Compare >& ConstIterator< Key, T, Compare >::operator--() noexcept
  {
    if (node_ == tree_->fakeleaf_) {
      node_ = tree_->root_;
      while (node_ != tree_->fakeleaf_ && !node_->right->isFake) {
        node_ = node_->right;
      }
      return *this;
    }
    if (!node_->left->isFake) {
      node_ = node_->left;
      while (!node_->right->isFake) {
        node_ = node_->right;
      }
    } else {
      while ((node_->parent != nullptr) && (node_->parent->left == node_)) {
        node_ = node_->parent;
      }
      node_ = node_->parent;
      if (!node_) {
        node_ = tree_->fakeleaf_;
      }
    }
    return *this;
  }

  template < class Key, class T, class Compare >
  ConstIterator< Key, T, Compare >ConstIterator< Key, T, Compare >::operator--(int) noexcept
  {
    ConstIterator tmp = *this;
    --(*this);
    return tmp;
  }

  template < class Key, class T, class Compare >
  ConstIterator< Key, T, Compare >ConstIterator< Key, T, Compare >::operator++(int) noexcept
  {
    ConstIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template < class Key, class T, class Compare >
  bool ConstIterator< Key, T, Compare >::operator==(const ConstIterator& other) const noexcept
  {
    return node_ == other.node_;
  }

  template < class Key, class T, class Compare >
  bool ConstIterator< Key, T, Compare >::operator!=(const ConstIterator& other) const noexcept
  {
    return !(node_ == other.node_);
  }
}

#endif
