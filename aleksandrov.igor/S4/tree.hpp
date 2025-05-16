#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <stdexcept>
#include "const-iterator.hpp"

namespace aleksandrov
{
  template< typename Key, typename Value, typename Compare >
  class ConstIterator;

  template< typename Key, typename Value, typename Compare = std::less< Key > >
  class Tree
  {
  public:
    Tree();
    Tree(const Tree< Key, Value, Compare >&);
    Tree(Tree< Key, Value, Compare >&&) noexcept;
    ~Tree();

    size_t size() const noexcept;
    bool empty() const noexcept;

    ConstIterator< Key, Value, Compare > begin() const noexcept;
    ConstIterator< Key, Value, Compare > end() const noexcept;

    void clear(detail::Node< Key, Value >*) noexcept;
    void swap(Tree< Key, Value, Compare >&) noexcept;

    const Value& at(const Key&) const;
    Value& operator[](const Key&);

    ConstIterator< Key, Value, Compare > find(const Key& key) const;
  private:
    friend ConstIterator< Key, Value, Compare >;
    using Node = detail::Node< Key, Value >;
    Node* root_;
    size_t size_;
    Compare cmp_;

    Node* copy(Node* node);
  };

  template< typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >::Tree():
    root_(nullptr),
    size_(0)
  {}

  template< typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >::Tree(const Tree< Key, Value, Compare >& rhs):
    root_(nullptr),
    size_(rhs.size_),
    cmp_(rhs.cmp_)
  {
    root_ = copy(rhs.root_);
  }

  template< typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node* Tree< Key, Value, Compare >::copy(Node* node)
  {
    if (!node)
    {
      return nullptr;
    }
    Node* newNode = new Node(node->data[0].first, node->data[0].second);

    ConstIterator< Key, Value, Compare > it(node);
    if (it.isTriple())
    {
      newNode->data[1] = node->data[1];
    }
    newNode->left = copy(node->left);
    newNode->middle = copy(node->middle);
    newNode->right = copy(node->right);

    if (newNode->left)
    {
      newNode->left->parent = newNode;
    }
    if (newNode->middle)
    {
      newNode->middle->parent = newNode;
    }
    if (newNode->right)
    {
      newNode->right->parent = newNode;
    }
    return newNode;
  }

  template< typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >::Tree(Tree< Key, Value, Compare >&& rhs) noexcept:
    root_(std::move(rhs.root_)),
    size_(std::move(rhs.size_)),
    cmp_(std::move(rhs.cmp_))
  {
    rhs.root_ = nullptr;
    rhs.size_ = 0;
  }

  template< typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >::~Tree()
  {
    clear(root_);
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
  void Tree< Key, Value, Compare >::clear(detail::Node< Key, Value >* root) noexcept
  {
    if (!root)
    {
      return;
    }
    clear(root->left);
    ConstIterator< Key, Value, Compare > it(root);
    if (it.isTriple())
    {
      clear(root->middle);
    }
    clear(root->right);
    delete root;
  }

  template< typename Key, typename Value, typename Compare >
  void Tree< Key, Value, Compare >::swap(Tree< Key, Value, Compare >& rhs) noexcept
  {
    std::swap(root_, rhs.root_);
    std::swap(size_, rhs.size_);
  }

  template< typename Key, typename Value, typename Compare >
  const Value& Tree< Key, Value, Compare >::at(const Key& key) const
  {
    ConstIterator< Key, Value, Compare > pos = find(key);
    if (pos == end())
    {
      throw std::out_of_range("There is no such key!");
    }
  }

  template< typename Key, typename Value, typename Compare >
  Value& Tree< Key, Value, Compare >::operator[](const Key& key)
  {
    ConstIterator< Key, Value, Compare > pos = find(key);
    if (pos == end())
    {
      pos = insert(std::make_pair(key, Value())).first;
    }
    return pos->second;
  }

  template< typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare > Tree< Key, Value, Compare >::find(const Key& key) const
  {
    Node* node = root_;
    while (!node)
    {
      if (cmp_(key, node->data.first))
      {
        node = node->left;
      }
      else if (!cmp_(node->data[0].first, key) && !cmp_(key, node->data[0].first))
      {
        return ConstIterator< Key, Value, Compare >(node);
      }
      else if (node->isTriple())
      {
        if (cmp_(key, node->data[1].first))
        {
          node = node->middle;
        }
        else if (!cmp_(node->data[1].first, key) && !cmp_(key, node->data[1].first))
        {
          Node* tmp = node;
          tmp->pos_ = ConstIterator< Key, Value, Compare >::PointsTo::Right;
          return ConstIterator< Key, Value, Compare >(tmp);
        }
        else
        {
          node = node->right;
        }
      }
      else
      {
        node = node->right;
      }
    }
    return end();
  }
}

#endif

