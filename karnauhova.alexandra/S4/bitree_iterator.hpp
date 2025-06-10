#ifndef BITREE_ITERATOR_HPP
#define BITREE_ITERATOR_HPP
#include "tree_node.hpp"
#include <iostream>
#include <cstddef>
#include <stdexcept>
namespace karnauhova
{
  template<typename Key, typename Value, typename Compare>
  class BiTree;

  template< typename Key, typename Value, typename Compare = std::less< Key >>
  struct BiTreeIterator
  {
    friend class BiTree< Key, Value, Compare >;
  public:
    BiTreeIterator(karnauhova::TreeNode< Key, Value >* node, bool position);
    using this_t = BiTreeIterator< Key, Value >;
    this_t operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t operator--() noexcept;
    this_t operator--(int) noexcept;

    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const noexcept;

    bool operator==(const this_t& rhs) const noexcept;
    bool operator!=(const this_t& rhs) const noexcept;

    const Value& value() const;
  private:
    TreeNode< Key, Value >* node_;
    bool isData2;
  };

  template< typename Key, typename Value, typename Compare>
  BiTreeIterator< Key, Value, Compare >::BiTreeIterator(karnauhova::TreeNode< Key, Value >* node, bool position):
    node_(node),
    isData2(position)
  {}
  template< typename Key, typename Value, typename Compare>
  typename BiTreeIterator< Key, Value, Compare >::this_t BiTreeIterator< Key, Value, Compare >::operator++() noexcept
  {
    if (!isData2 && node_->full)
    {
      if (node_->middle)
      {
        TreeNode< Key, Value >* current = node_->middle;
        while (current->left)
        {
          current = current->left;
        }
        BiTreeIterator< Key, Value > it(current, 0);
        return it;
      }
      BiTreeIterator< Key, Value > it(node_, 1);
      return it;
    }
    if (node_->right)
    {
      TreeNode< Key, Value >* current = node_->right;
      while (current->left)
      {
        current = current->left;
      }
      BiTreeIterator< Key, Value > it(current, 0);
      return it;
    }
    TreeNode< Key, Value >* parent = node_->parent;
    TreeNode< Key, Value >* last = node_;
    while ((parent->left != last || parent->middle != last) && parent->parent)
    {
      last = last->parent;
      parent = parent->parent;
    }
    if (parent->middle == last && parent->full)
    {
      BiTreeIterator< Key, Value > it(parent, 1);
      return it;
    }
    else
    {
      BiTreeIterator< Key, Value > it(parent, 0);
      return it;
    }
  }

  template< typename Key, typename Value, typename Compare>
  typename BiTreeIterator< Key, Value, Compare >::this_t BiTreeIterator< Key, Value, Compare >::operator--() noexcept
  {
    if (isData2 && node_->full)
    {
      if (node_->middle)
      {
        TreeNode< Key, Value >* current = node_->middle;
        while (current->right)
        {
          current = current->right;
        }
        if (current->full)
        {
          BiTreeIterator< Key, Value > it(current, 1);
          return it;
        }
        else
        {
          BiTreeIterator< Key, Value > it(current, 0);
          return it;
        }
      }
      BiTreeIterator< Key, Value > it(node_, 0);
      return it;
    }
    if (node_->left)
    {
      TreeNode< Key, Value >* current = node_->left;
      while (current->right)
      {
        current = current->right;
      }
      if (node_->full)
      {
        BiTreeIterator< Key, Value > it(current, 1);
        return it;
      }
      else
      {
        BiTreeIterator< Key, Value > it(current, 0);
        return it;
      }
    }
    TreeNode< Key, Value >* parent = node_->parent;
    TreeNode< Key, Value >* last = node_;
    while ((parent->right != last || parent->middle != last) && parent->parent)
    {
      last = last->parent;
      parent = parent->parent;
    }
    if (parent->right == last && parent->full)
    {
      BiTreeIterator< Key, Value > it(parent, 1);
      return it;
    }
    else
    {
      BiTreeIterator< Key, Value > it(parent, 0);
      return it;
    }
  }

  template< typename Key, typename Value, typename Compare>
  typename BiTreeIterator< Key, Value, Compare >::this_t BiTreeIterator< Key, Value, Compare >::operator++(int) noexcept
  {
    this_t res(*this);
    ++(*this);
    return res;
  }

  template< typename Key, typename Value, typename Compare>
  typename BiTreeIterator< Key, Value, Compare >::this_t BiTreeIterator< Key, Value, Compare >::operator--(int) noexcept
  {
    this_t res(*this);
    --(*this);
    return res;
  }

  template< typename Key, typename Value, typename Compare>
  std::pair< Key, Value >& BiTreeIterator< Key, Value, Compare >::operator*() const
  {
    if (isData2)
    {
      return node_->data2;
    }
    return node_->data1;
  }

  template< typename Key, typename Value, typename Compare>
  const Value& BiTreeIterator< Key, Value, Compare >::value() const
  {
    return *this.second;
  }

  template< typename Key, typename Value, typename Compare>
  std::pair< Key, Value >* BiTreeIterator< Key, Value, Compare >::operator->() const noexcept
  {
    if (isData2)
    {
      return std::addressof(node_->data2);
    }
    return std::addressof(node_->data1);
  }

  template< typename Key, typename Value, typename Compare>
  bool BiTreeIterator< Key, Value, Compare >::operator==(const this_t& rhs) const noexcept
  {
    if (isData2)
    {
      if (rhs.isData2)
      {
        return node_->data2 == rhs.node_->data2;
      }
      return node_->data2 == rhs.node_->data1;
    }
    if (rhs.isData2)
    {
      return node_->data1 == rhs.node_->data2;
    }
    return node_->data1 == rhs.node_->data1;
  }

  template< typename Key, typename Value, typename Compare>
  bool BiTreeIterator< Key, Value, Compare >::operator!=(const this_t& rhs) const noexcept
  {
    return !(*this == rhs);
  }
}
#endif

