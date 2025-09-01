#ifndef UBST_HPP
#define UBST_HPP

#include <cstddef>
#include <stdexcept>
#include "node.hpp"
#include "constiterator.hpp"

namespace shramko
{
  template < typename Key, typename Value, typename Compare = std::less< Key > >
  class UBstTree
  {
  public:
    using const_iterator = ConstIterator< Key, Value, Compare >;
    UBstTree();
    UBstTree(const UBstTree & other);
    UBstTree(UBstTree && other) noexcept;
    ~UBstTree();
    UBstTree & operator=(const UBstTree & other);
    UBstTree & operator=(UBstTree && other) noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear() noexcept;
    void swap(UBstTree & other) noexcept;
    Value & operator[](const Key & key);
    const Value & operator[](const Key & key) const;
    Value & at(const Key & key);
    const Value & at(const Key & key) const;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_iterator find(const Key & key) const noexcept;

  private:
    Node< Key, Value > * root_;
    size_t size_;
    Compare comp_;

    void clearNode(Node< Key, Value > * node);
    Node< Key, Value > * insertNode(
      Node< Key, Value > * node,
      const Key & key,
      const Value & value,
      Node< Key, Value > * parent,
      size_t & size
    );
    Node< Key, Value > * findNode(
      Node< Key, Value > * node,
      const Key & key
    ) const;
    Node< Key, Value > * minNode(
      Node< Key, Value > * node
    ) const;
    void copyTree(
      Node< Key, Value > *& node,
      Node< Key, Value > * otherNode,
      Node< Key, Value > * parent
    );
  };

  template < typename Key, typename Value, typename Compare >
  UBstTree< Key, Value, Compare >::UBstTree():
    root_(nullptr),
    size_(0),
    comp_(Compare())
  {}

  template < typename Key, typename Value, typename Compare >
  UBstTree< Key, Value, Compare >::UBstTree(const UBstTree & other):
    root_(nullptr),
    size_(0),
    comp_(other.comp_)
  {
    copyTree(root_, other.root_, nullptr);
  }

  template < typename Key, typename Value, typename Compare >
  void UBstTree< Key, Value, Compare >::copyTree(
    Node< Key, Value > *& node,
    Node< Key, Value > * otherNode,
    Node< Key, Value > * parent
  )
  {
    if (!otherNode)
    {
      return;
    }
    node = new Node< Key, Value >(otherNode->data.first, otherNode->data.second);
    node->parent = parent;
    size_++;
    copyTree(node->left, otherNode->left, node);
    copyTree(node->right, otherNode->right, node);
  }

  template < typename Key, typename Value, typename Compare >
  UBstTree< Key, Value, Compare >::UBstTree(UBstTree && other) noexcept:
    root_(other.root_),
    size_(other.size_),
    comp_(std::move(other.comp_))
  {
    other.root_ = nullptr;
    other.size_ = 0;
  }

  template < typename Key, typename Value, typename Compare >
  UBstTree< Key, Value, Compare >::~UBstTree()
  {
    clearNode(root_);
    root_ = nullptr;
    size_ = 0;
  }

  template < typename Key, typename Value, typename Compare >
  UBstTree< Key, Value, Compare > & UBstTree< Key, Value, Compare >::operator=(
    const UBstTree & other
  )
  {
    if (this != std::addressof(other))
    {
      UBstTree temp(other);
      swap(temp);
    }
    return *this;
  }

  template < typename Key, typename Value, typename Compare >
  UBstTree< Key, Value, Compare > & UBstTree< Key, Value, Compare >::operator=(
    UBstTree && other
  ) noexcept
  {
    if (this != std::addressof(other))
    {
      clearNode(root_);
      root_ = other.root_;
      size_ = other.size_;
      comp_ = std::move(other.comp_);
      other.root_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  template < typename Key, typename Value, typename Compare >
  bool UBstTree< Key, Value, Compare >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < typename Key, typename Value, typename Compare >
  size_t UBstTree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template < typename Key, typename Value, typename Compare >
  void UBstTree< Key, Value, Compare >::clear() noexcept
  {
    clearNode(root_);
    root_ = nullptr;
    size_ = 0;
  }

  template < typename Key, typename Value, typename Compare >
  void UBstTree< Key, Value, Compare >::swap(UBstTree & other) noexcept
  {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
    std::swap(comp_, other.comp_);
  }

  template < typename Key, typename Value, typename Compare >
  Value & UBstTree< Key, Value, Compare >::operator[](const Key & key)
  {
    Node< Key, Value > * node = findNode(root_, key);
    if (!node)
    {
      root_ = insertNode(root_, key, Value(), nullptr, size_);
      node = findNode(root_, key);
    }
    return node->data.second;
  }

  template < typename Key, typename Value, typename Compare >
  const Value & UBstTree< Key, Value, Compare >::operator[](const Key & key) const
  {
    Node< Key, Value > * node = findNode(root_, key);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    return node->data.second;
  }

  template < typename Key, typename Value, typename Compare >
  Value & UBstTree< Key, Value, Compare >::at(const Key & key)
  {
    Node< Key, Value > * node = findNode(root_, key);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    return node->data.second;
  }

  template < typename Key, typename Value, typename Compare >
  const Value & UBstTree< Key, Value, Compare >::at(const Key & key) const
  {
    Node< Key, Value > * node = findNode(root_, key);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    return node->data.second;
  }

  template < typename Key, typename Value, typename Compare >
  typename UBstTree< Key, Value, Compare >::const_iterator
  UBstTree< Key, Value, Compare >::cbegin() const noexcept
  {
    return const_iterator(minNode(root_));
  }

  template < typename Key, typename Value, typename Compare >
  typename UBstTree< Key, Value, Compare >::const_iterator
  UBstTree< Key, Value, Compare >::cend() const noexcept
  {
    return const_iterator(nullptr);
  }

  template < typename Key, typename Value, typename Compare >
  typename UBstTree< Key, Value, Compare >::const_iterator
  UBstTree< Key, Value, Compare >::find(const Key & key) const noexcept
  {
    Node< Key, Value > * node = findNode(root_, key);
    if (node)
    {
      return const_iterator(node);
    }
    return cend();
  }

  template < typename Key, typename Value, typename Compare >
  void UBstTree< Key, Value, Compare >::clearNode(Node< Key, Value > * node)
  {
    if (!node)
    {
      return;
    }
    clearNode(node->left);
    clearNode(node->right);
    delete node;
  }

  template < typename Key, typename Value, typename Compare >
  Node< Key, Value > * UBstTree< Key, Value, Compare >::insertNode(
    Node< Key, Value > * node,
    const Key & key,
    const Value & value,
    Node< Key, Value > * parent,
    size_t & size
  )
  {
    if (!node)
    {
      ++size;
      Node< Key, Value > * newNode = new Node< Key, Value >(key, value);
      newNode->parent = parent;
      return newNode;
    }
    if (comp_(key, node->data.first))
    {
      node->left = insertNode(node->left, key, value, node, size);
    }
    else if (comp_(node->data.first, key))
    {
      node->right = insertNode(node->right, key, value, node, size);
    }
    else
    {
      node->data.second = value;
    }
    return node;
  }

  template < typename Key, typename Value, typename Compare >
  Node< Key, Value > * UBstTree< Key, Value, Compare >::findNode(
    Node< Key, Value > * node,
    const Key & key
  ) const
  {
    if (!node)
    {
      return nullptr;
    }
    if (comp_(key, node->data.first))
    {
      return findNode(node->left, key);
    }
    else if (comp_(node->data.first, key))
    {
      return findNode(node->right, key);
    }
    return node;
  }

  template < typename Key, typename Value, typename Compare >
  Node< Key, Value > * UBstTree< Key, Value, Compare >::minNode(
    Node< Key, Value > * node
  ) const
  {
    if (!node)
    {
      return nullptr;
    }
    while (node->left)
    {
      node = node->left;
    }
    return node;
  }
}

#endif
