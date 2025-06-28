#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
#include "node_tree.hpp"

namespace tkach
{
  template< class Key, class Value, class Cmp>
  class AvlTree;

  template< class Key, class Value, class Cmp = std::less< Key > >
  class Iterator: public std::iterator< std::bidirectional_iterator_tag, Value >
  {
    friend class AvlTree< Key, Value, Cmp >;
  public:
    using this_t = Iterator< Key, Value, Cmp >;
    Iterator();
    Iterator(const this_t&) = default;
    ~Iterator() = default;
    this_t& operator=(const this_t&) = default;
    this_t& operator++();
    this_t operator++(int);
    this_t& operator--();
    this_t operator--(int);
    std::pair< Key, Value > & operator*();
    std::pair< Key, Value > * operator->();
    bool operator!=(const this_t& rhs) const;
    bool operator==(const this_t& rhs) const;
  private:
    TreeNode< Key, Value >* node_;
    explicit Iterator(TreeNode< Key, Value >* node);
  };

  template< class Key, class Value, class Cmp>
  Iterator< Key, Value, Cmp >::Iterator():
    node_(nullptr)
  {}

  template< class Key, class Value, class Cmp>
  Iterator< Key, Value, Cmp >::Iterator(TreeNode< Key, Value >* node):
    node_(node)
  {}

  template< class Key, class Value, class Cmp>
  Iterator< Key, Value, Cmp >& Iterator< Key, Value, Cmp >::operator++()
  {
    if (node_->right)
    {
      node_ = node_->right;
      while (node_->left)
      {
        node_ = node_->left;
      }
      return *this;
    }
    else
    {
      while (node_->parent && node_->parent->right == node_)
      {
        node_ = node_->parent;
      }
      node_ = node_->parent;
      return *this;
    }
  }

  template< class Key, class Value, class Cmp>
  Iterator< Key, Value, Cmp > Iterator< Key, Value, Cmp >::operator++(int)
  {
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< class Key, class Value, class Cmp>
  Iterator< Key, Value, Cmp >& Iterator< Key, Value, Cmp >::operator--()
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

  template< class Key, class Value, class Cmp>
  Iterator< Key, Value, Cmp > Iterator< Key, Value, Cmp >::operator--(int)
  {
    this_t result(*this);
    --(*this);
    return result;
  }

  template< class Key, class Value, class Cmp>
  std::pair< Key, Value >& Iterator< Key, Value, Cmp >::operator*()
  {
    return node_->data;
  }

  template< class Key, class Value, class Cmp>
  std::pair< Key, Value >* Iterator< Key, Value, Cmp >::operator->()
  {
    return std::addressof(node_->data);
  }

  template< class Key, class Value, class Cmp>
  bool Iterator< Key, Value, Cmp >::operator==(const this_t& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class Key, class Value, class Cmp>
  bool Iterator< Key, Value, Cmp >::operator!=(const this_t& rhs) const
  {
    return !(rhs == *this);
  }
}

#endif
