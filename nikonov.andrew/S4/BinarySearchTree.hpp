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
    Compare cmp_ = Compare();
  public:
    BinarySearchTree() noexcept;
    BinarySearchTree(const BinarySearchTree& rhs);
    BinarySearchTree(BinarySearchTree&& rhs) noexcept;
    BinarySearchTree(std::initializer_list< value_type > il);
    template< typename InputIterator >
    BinarySearchTree(InputIterator begin, InputIterator end);
    ~BinarySearchTree() noexcept;

    BiTreeIterator< Key, Value > begin() noexcept;
    ConstBiTreeIterator< Key, Value > begin() const noexcept;
    ConstBiTreeIterator< Key, Value > cbegin() const noexcept;
    BiTreeIterator< Key, Value > end() noexcept;
    ConstBiTreeIterator< Key, Value > end() const noexcept;
    ConstBiTreeIterator< Key, Value > cend() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    Value& operator[](const Key& k);
    Value& operator[](Key&& k);
    Value& at(const Key& k);
    const Value& at(const Key& k) const;

    std::pair< BiTreeIterator< Key, Value >, bool > insert(const value_type& val);
    template < class P >
    std::pair< BiTreeIterator< Key, Value >, bool > insert(P&& val);
    BiTreeIterator< Key, Value > insert(ConstBiTreeIterator< Key, Value > position, const value_type& val);
    template < class P >
    BiTreeIterator< Key, Value > insert(ConstBiTreeIterator< Key, Value > position, P&& val);	
    template < class InputIterator >
    void insert(InputIterator first, InputIterator last);

    BiTreeIterator< Key, Value > erase(ConstBiTreeIterator< Key, Value > position);
    size_t erase(const Key& k);
    BiTreeIterator< Key, Value > erase(ConstBiTreeIterator< Key, Value > first, ConstBiTreeIterator< Key, Value > last);

    void swap(BinarySearchTree& rhs) noexcept;
    void clear() noexcept;

    BiTreeIterator< Key, Value > find(const Key& k);
    ConstBiTreeIterator< Key, Value > find(const Key& k) const;

    size_t count(const Key& k) const;
 
    std::pair< BiTreeIterator< Key, Value >, BiTreeIterator< Key, Value > > equal_range(const Key& k);
    std::pair< ConstBiTreeIterator< Key, Value >, ConstBiTreeIterator< Key, Value > > equal_range(const Key& k) const;

    BiTreeIterator< Key, Value > lower_bound(const Key& k);
    ConstBiTreeIterator< Key, Value > lower_bound(const Key& k) const;
    BiTreeIterator< Key, Value > upper_bound(const Key& k);
    ConstBiTreeIterator< Key, Value > upper_bound(const Key& k) const;
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
nikonov::BiTreeIterator< Key, Value > nikonov::BinarySearchTree< Key, Value, Compare >::begin() noexcept
{
  return { root_ };
}

template< typename Key, typename Value, typename Compare >
nikonov::ConstBiTreeIterator< Key, Value > nikonov::BinarySearchTree< Key, Value, Compare >::begin() const noexcept
{
  return { root_ };
}

template< typename Key, typename Value, typename Compare >
nikonov::ConstBiTreeIterator< Key, Value > nikonov::BinarySearchTree< Key, Value, Compare >::cbegin() const noexcept
{
  return { root_ };
}

template< typename Key, typename Value, typename Compare >
nikonov::BiTreeIterator< Key, Value > nikonov::BinarySearchTree< Key, Value, Compare >::end() noexcept
{
  return { nullptr };
}

template< typename Key, typename Value, typename Compare >
nikonov::ConstBiTreeIterator< Key, Value > nikonov::BinarySearchTree< Key, Value, Compare >::end() const noexcept
{
  return { nullptr };
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
Value& nikonov::BinarySearchTree< Key, Value, Compare >::operator[](Key&& k)
{
  auto iter = find(k);
  if (iter != end())
  {
    return (*iter).second;
  }
  auto inserted = insert(std::make_pair(k, Value()));
  return inserted.first->second;
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
std::pair< nikonov::BiTreeIterator< Key, Value >, bool > nikonov::BinarySearchTree< Key, Value, Compare >::insert(const value_type& val)
{
  
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
