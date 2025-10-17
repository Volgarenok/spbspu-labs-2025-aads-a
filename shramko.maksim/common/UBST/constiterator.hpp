#ifndef CONSTITERATOR_HPP
#define CONSTITERATOR_HPP

#include <iterator>
#include "node.hpp"

namespace shramko
{
  template < typename Key, typename Value, typename Compare >
  class UBstTree;

  template < typename Key, typename Value, typename Compare = std::less< Key > >
  class ConstIterator: public std::iterator< std::bidirectional_iterator_tag, std::pair< const Key, Value >,
    std::ptrdiff_t, const std::pair< const Key, Value >*, const std::pair< const Key, Value >& >
  {
  public:
    using value_type = std::pair< const Key, Value >;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    ConstIterator():
      node_(nullptr),
      tree_(nullptr)
    {}

    explicit ConstIterator(const Node< Key, Value >* node, const UBstTree< Key, Value, Compare >* tree = nullptr):
      node_(node),
      tree_(tree)
    {}

    reference operator*() const
    {
      return node_->data;
    }

    pointer operator->() const
    {
      return &(node_->data);
    }

    ConstIterator& operator++()
    {
      if (node_)
      {
        if (node_->right)
        {
          node_ = minNode(node_->right);
        }
        else
        {
          const Node< Key, Value >* parent = node_->parent;
          while (parent && node_ == parent->right)
          {
            node_ = parent;
            parent = parent->parent;
          }
          node_ = parent;
        }
      }
      return *this;
    }

    ConstIterator operator++(int)
    {
      ConstIterator temp = *this;
      ++(*this);
      return temp;
    }

    ConstIterator& operator--()
    {
      if (!node_)
      {
        if (tree_)
        {
          node_ = tree_->maxNode(tree_->root_);
        }
        return *this;
      }
      if (node_->left)
      {
        node_ = maxNode(node_->left);
      }
      else
      {
        const Node< Key, Value >* parent = node_->parent;
        while (parent && node_ == parent->left)
        {
          node_ = parent;
          parent = parent->parent;
        }
        node_ = parent;
      }
      return *this;
    }

    ConstIterator operator--(int)
    {
      ConstIterator temp = *this;
      --(*this);
      return temp;
    }

    bool operator==(const ConstIterator& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const ConstIterator& other) const
    {
      return !(*this == other);
    }

  private:
    friend class UBstTree< Key, Value, Compare >;
    const Node< Key, Value >* node_;
    const UBstTree< Key, Value, Compare >* tree_;

    const Node< Key, Value >* minNode(const Node< Key, Value >* node) const
    {
      while (node && node->left)
      {
        node = node->left;
      }
      return node;
    }

    const Node< Key, Value >* maxNode(const Node< Key, Value >* node) const
    {
      while (node && node->right)
      {
        node = node->right;
      }
      return node;
    }
  };
}

#endif
