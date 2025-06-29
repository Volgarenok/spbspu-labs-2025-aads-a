#ifndef TREE_ITERATOR_HPP
#define TREE_ITERATOR_HPP

#include <iterator>
#include "tree_node.hpp"
#include "tree.hpp"

namespace lanovenko
{
  template< typename Key, typename Value, typename Comparator >
  class Tree;

  template< typename Key, typename Value, typename Comparator >
  class TreeIterator final: public std::iterator< std::bidirectional_iterator_tag, Value >
  {
  public:
    using this_t = TreeIterator< Key, Value, Comparator >;

    ~TreeIterator() = default;
    TreeIterator() noexcept;
    TreeIterator(const this_t&) = default;
    this_t& operator=(const this_t&) = default;

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t& operator--() noexcept;
    this_t operator--(int) noexcept;
    std::pair< Key, Value >& operator*() noexcept;
    std::pair< Key, Value >* operator->() noexcept;
    bool operator!=(const this_t& rhs) const noexcept;
    bool operator==(const this_t& rhs) const noexcept;
  private:
    TreeNode< Key, Value >* node_;
    explicit TreeIterator(TreeNode< Key, Value >* node) noexcept;
    friend class Tree< Key, Value, Comparator >;
  };

  template< typename Key, typename Value, typename Comparator >
  TreeIterator< Key, Value, Comparator >::TreeIterator() noexcept:
    node_(nullptr)
  {}

  template< typename Key, typename Value, typename Comparator >
  TreeIterator< Key, Value, Comparator >& TreeIterator< Key, Value, Comparator >::operator++() noexcept
  {
    assert(node_);
    if (node_->right_)
    {
      node_ = Tree< Key, Value, Comparator >::minValueNode(node_->right_);
      return *this;
    }
    else
    {
      while (node_->parent_ && node_->parent_->left_ != node_)
      {
        node_ = node_->parent_;
      }
      node_ = node_->parent_;
      return *this;
    }
  }

  template< typename Key, typename Value, typename Comparator >
  TreeIterator< Key, Value, Comparator > TreeIterator< Key, Value, Comparator >::operator++(int) noexcept
  {
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename Key, typename Value, typename Comparator >
  TreeIterator< Key, Value, Comparator >& TreeIterator< Key, Value, Comparator >::operator--() noexcept
  {
    if (node_->left)
    {
      node_ = node_->left;
      while (node_->right)
      {
        node_ = node_->right;
      }
      return *this;
    }
    else
    {
      while (node_->parent && node_->parent->left == node_)
      {
        node_ = node_->parent;
      }
      node_ = node_->parent;
      return *this;
    }
  }

  template< typename Key, typename Value, typename Comparator >
  TreeIterator< Key, Value, Comparator > TreeIterator< Key, Value, Comparator >::operator--(int) noexcept
  {
    this_t result(*this);
    --(*this);
    return result;
  }

  template< class Key, class Value, class Comparator >
  std::pair< Key, Value >& TreeIterator< Key, Value, Comparator >::operator*() noexcept
  {
    assert(node_);
    return node_->data_;
  }

  template< class Key, class Value, class Comparator >
  std::pair< Key, Value >* TreeIterator< Key, Value, Comparator >::operator->() noexcept
  {
    assert(node_);
    return std::addressof(node_->data_);
  }

  template< class Key, class Value, class Cmp >
  bool TreeIterator< Key, Value, Cmp >::operator==(const this_t& rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< class Key, class Value, class Cmp>
  bool TreeIterator< Key, Value, Cmp >::operator!=(const this_t& rhs) const noexcept
  {
    return !(rhs == *this);
  }

  template< typename Key, typename Value, typename Comparator >
  TreeIterator< Key, Value, Comparator >::TreeIterator(TreeNode< Key, Value >* node) noexcept:
    node_(node)
  {}
}

#endif
