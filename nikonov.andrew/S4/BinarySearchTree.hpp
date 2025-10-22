#ifndef BINARYSEARCHTREE_HPP
#define BINARYSEARCHTREE_HPP
#include "BiTreeIterator.hpp"
namespace nikonov
{
  template< typename Key, typename Value, typename Compare >
  class BinarySearchTree
  {
    using value_type = std::pair< Key, Value >;
    detail::BiTreeNode< Key, Value >* root_;
    size_t size_;
    Compare cmp_;
  public:
    BinarySearchTree() noexcept;
    BinarySearchTree(const BinarySearchTree& rhs);
    BinarySearchTree(BinarySearchTree&& rhs) noexcept;
    ~BinarySearchTree();

    ConstBiTreeIterator< Key, Value > cbegin() const noexcept;
    ConstBiTreeIterator< Key, Value > cend() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    Value& operator[](const Key& k);
    Value& at(const Key& k);
    const Value& at(const Key& k) const;

    void swap(BinarySearchTree& rhs) noexcept;
    void clear() noexcept;

    ConstBiTreeIterator< Key, Value > find(const Key& k) const;
  };
}

template< typename Key, typename Value, typename Compare >
nikonov::BinarySearchTree< Key, Value, Compare >::BinarySearchTree() noexcept:
  root_(nullptr)
{}

template< typename Key, typename Value, typename Compare >
nikonov::BinarySearchTree< Key, Value, Compare >::BinarySearchTree(const BinarySearchTree& rhs)
{
  for (BiTreeIterator< Key, Value > it = rhs.begin(); it != rhs.end(); ++it) 
  {
    insert(it.node_->data_);
  }
}

template< typename Key, typename Value, typename Compare >
nikonov::BinarySearchTree< Key, Value, Compare >::BinarySearchTree(BinarySearchTree&& rhs) noexcept
{
  root_ = rhs.root_;
  rhs.root_ = nullptr;
}

template< typename Key, typename Value, typename Compare >
nikonov::BinarySearchTree< Key, Value, Compare >::~BinarySearchTree() noexcept
{
  clear();
}

template< typename Key, typename Value, typename Compare >
nikonov::ConstBiTreeIterator< Key, Value > nikonov::BinarySearchTree< Key, Value, Compare >::cbegin() const noexcept
{
  return { root_ };
}

template< typename Key, typename Value, typename Compare >
nikonov::ConstBiTreeIterator< Key, Value > nikonov::BinarySearchTree< Key, Value, Compare >::cend() const noexcept
{
  return { nullptr };
}

template< typename Key, typename Value, typename Compare >
size_t nikonov::BinarySearchTree< Key, Value, Compare >::size() const noexcept
{
  size_t size = 0;
  for (auto it = cbegin(); it != cend(); ++it)
  {
    ++size;
  }
  return size;
}

template< typename Key, typename Value, typename Compare >
bool nikonov::BinarySearchTree< Key, Value, Compare >::empty() const noexcept
{
  return root_ == nullptr;
}

template< typename Key, typename Value, typename Compare >
Value& nikonov::BinarySearchTree< Key, Value, Compare >::operator[](const Key& k)
{
  return operator[](std::move(k));
}

template< typename Key, typename Value, typename Compare >
Value& nikonov::BinarySearchTree< Key, Value, Compare >::at(const Key& k)
{
  return const_cast< Value& >(static_cast< const BinarySearchTree& >(*this).at(k));
}

template< typename Key, typename Value, typename Compare >
const Value& nikonov::BinarySearchTree< Key, Value, Compare >::at(const Key& k) const
{
  auto iter = find(k);
  if (iter != cend())
  {
    return (*iter).second;
  }
  throw std::logic_error("ERROR: can't find element with such key");
}

template< typename Key, typename Value, typename Compare >
void nikonov::BinarySearchTree< Key, Value, Compare >::swap(BinarySearchTree& rhs) noexcept
{
  std::swap(root_, rhs.root_);
}


template< typename Key, typename Value, typename Compare >
void nikonov::BinarySearchTree< Key, Value, Compare >::clear() noexcept
{
  auto iter = begin();
  while (iter != end())
  {
    auto curr = iter++;
    delete *curr;
  }
}


#endif
