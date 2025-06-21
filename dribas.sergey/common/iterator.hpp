#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <utility>
#include <functional>

namespace dribas
{
  template< class Key, class T, class Compare >
  class AVLTree;

  template< class Key, class T >
  class Node;

  template< class Key, class T, class Compare >
  class ConstIterator;

  template< class Key, class T, class Compare = std::less< Key > >
  class Iterator
  {
    friend class ConstIterator< Key, T, Compare >;
    friend class AVLTree< Key, T, Compare >;
  public:
    using valueType = std::pair< Key, T >;
    using TreeType = AVLTree< Key, T, Compare >;
    using NodeType = Node< Key, T >;

    Iterator() noexcept;
    valueType& operator*() noexcept;
    valueType* operator->() noexcept;
    Iterator& operator++() noexcept;
    Iterator operator++(int) noexcept;
    Iterator& operator--() noexcept;
    Iterator operator--(int) noexcept;

    bool operator==(const Iterator&) const noexcept;
    bool operator!=(const Iterator&) const noexcept;

  private:
    NodeType* node_;
    const TreeType* tree_;
    explicit Iterator(NodeType*, const TreeType*) noexcept;
  };

  template< class Key, class T, class Compare >
  Iterator< Key, T, Compare >::Iterator() noexcept:
    node_(nullptr), tree_(nullptr)
  {}

  template< class Key, class T, class Compare >
  Iterator< Key, T, Compare >::Iterator(NodeType* node, const TreeType* tree) noexcept:
    node_(node), tree_(tree)
  {}

  template< class Key, class T, class Compare >
  std::pair< Key, T >& Iterator< Key, T, Compare >::operator*() noexcept
  {
    return node_->value;
  }

  template< class Key, class T, class Compare >
  std::pair< Key, T >* Iterator< Key, T, Compare >::operator->() noexcept
  {
    return std::addressof(node_->value);
  }

  template< class Key, class T, class Compare >
  Iterator< Key, T, Compare >& Iterator< Key, T, Compare >::operator++() noexcept
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

  template< class Key, class T, class Compare >
  Iterator< Key, T, Compare >& Iterator< Key, T, Compare >::operator--() noexcept
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


  template< class Key, class T, class Compare >
  Iterator< Key, T, Compare > Iterator< Key, T, Compare >::operator--(int) noexcept
  {
    Iterator tmp = *this;
    --(*this);
    return tmp;
  }

  template< class Key, class T, class Compare >
  Iterator< Key, T, Compare > Iterator< Key, T, Compare >::operator++(int) noexcept
  {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class T, class Compare >
  bool Iterator< Key, T, Compare >::operator==(const Iterator& other) const noexcept
  {
    return node_ == other.node_;
  }

  template< class Key, class T, class Compare >
  bool Iterator< Key, T, Compare >::operator!=(const Iterator& other) const noexcept
  {
    return !(node_ == other.node_);
  }
}

#endif
