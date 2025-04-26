#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include "node.hpp"

namespace aleksandrov
{
  template< typename Key, typename Value, typename Compare >
  class ConstIterator;

  template< typename Key, typename Value, typename Compare >
  class Tree
  {
  public:
    Tree();
    ~Tree();

    size_t size() const noexcept;
    bool empty() const noexcept;

    ConstIterator< Key, Value, Compare > begin() const noexcept;
    ConstIterator< Key, Value, Compare > end() const noexcept;

    void clear() noexcept;
  private:
    using Node = detail::Node< Key, Value >;
    Node* root_;
    size_t size_;
  };


  template< typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >::Tree():
    root_(nullptr),
    size_(0)
  {}

  template< typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >::~Tree()
  {
    clear();
    delete root_;
  }

  template< typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare > Tree< Key, Value, Compare >::begin() const noexcept
  {
    assert(root_);
    if (root_->left)
    {
      return ConstIterator< Key, Value, Compare >(root_->left).fallLeft();
    }
    else if (root_->middle)
    {
      return ConstIterator< Key, Value, Compare >(root_->middle).fallLeft();
    }
    return ConstIterator< Key, Value, Compare >(root_);
  }

  template< typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare > Tree< Key, Value, Compare >::end() const noexcept
  {
    assert(root_);
    if (root_->right)
    {
      return ConstIterator< Key, Value, Compare >(root_->right).fallRight();
    }
    else if (root_->middle)
    {
      return ConstIterator< Key, Value, Compare >(root_->middle).fallRight();
    }
    return ConstIterator< Key, Value, Compare >(root_);
  }

  template< typename Key, typename Value, typename Compare >
  size_t Tree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template< typename Key, typename Value, typename Compare >
  bool Tree< Key, Value, Compare >::empty() const noexcept
  {
    return !size_;
  }

  template< typename Key, typename Value, typename Compare >
  void Tree< Key, Value, Compare >::clear() noexcept
  {}
}

#endif

