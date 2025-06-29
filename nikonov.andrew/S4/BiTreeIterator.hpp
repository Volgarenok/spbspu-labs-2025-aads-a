#ifndef BITREEITERATOR_HPP
#define BITREEITERATOR_HPP
#include "BiTreeNode.hpp"
#include <iterator>
#include <cassert>
#include <memory>
namespace nikonov
{
  template< typename Key, typename Value, typename Compare >
  class BinarySearchTree;

  template< typename Key, typename Value >
  class BiTreeIterator: public std::iterator< std::forward_iterator_tag, std::pair< Key, Value > >
  {
    template< typename Key1, typename Value1, typename Compare >
    friend class BinarySearchTree;

    detail::BiTreeNode< Key, Value >* node_;
    explicit BiTreeIterator(detail::BiTreeNode< Key, Value >* ptr);
  public:
    using this_t = BiTreeIterator< Key, Value >;
    BiTreeIterator();
    ~BiTreeIterator() = default;
    BiTreeIterator(const this_t&) = default;
    this_t& operator=(const this_t&) = default;

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;

    std::pair< Key, Value >& operator*() noexcept;
    std::pair< Key, Value >* operator->() noexcept;

    bool operator!=(const this_t& rhs) const noexcept;
    bool operator==(const this_t& rhs) const noexcept;
  };

  template< typename Key, typename Value >
  BiTreeIterator< Key, Value >::BiTreeIterator(detail::BiTreeNode< Key, Value >* ptr):
    node_(ptr)
  {}

  template< typename Key, typename Value >
  BiTreeIterator< Key, Value >::BiTreeIterator():
    node_(nullptr)
  {}

  template< typename Key, typename Value >
  BiTreeIterator< Key, Value >& BiTreeIterator< Key, Value >::operator++() noexcept
  {
    assert(node_ != nullptr);
    if (node_->right_)
    {
      node_ = node_->right_;
      while (node_->left_)
      {
        node_ = node_->left_;
      }
    }
    else
    {
      auto parent = node_->parent_;
      while (parent && node_ == parent->right_)
      {
        node_ = parent;
        parent = parent->parent_;
      }
      node_ = parent;
    }
    return *this;
  }

  template< typename Key, typename Value >
  BiTreeIterator< Key, Value > BiTreeIterator< Key, Value >::operator++(int) noexcept
  {
    assert(node_ != nullptr);
    BiTreeIterator copy(*this);
    ++(*this);
    return copy;
  }

  template< typename Key, typename Value >
  std::pair< Key, Value >& BiTreeIterator< Key, Value >::operator*() noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename Key, typename Value >
  std::pair< Key, Value >* BiTreeIterator< Key, Value >::operator->() noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename Key, typename Value >
  bool BiTreeIterator< Key, Value >::operator==(const this_t& rhs) const noexcept
  {
    return (node_ == rhs->node_);
  }
  template< typename Key, typename Value >
  bool BiTreeIterator< Key, Value >::operator!=(const this_t& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< typename Key, typename Value >
  class ConstBiTreeIterator: public std::iterator< std::forward_iterator_tag, std::pair< Key, Value > >
  {
    detail::BiTreeNode< Key, Value >* node_;
    explicit ConstBiTreeIterator(detail::BiTreeNode< Key, Value >* ptr);
  public:
    using this_t = ConstBiTreeIterator< Key, Value >;
    ConstBiTreeIterator();
    ~ConstBiTreeIterator() = default;
    ConstBiTreeIterator(const this_t&) = default;
    this_t& operator=(const this_t&) = default;

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;

    const std::pair< Key, Value >& operator*() const noexcept;
    const std::pair< Key, Value >* operator->() const noexcept;

    bool operator!=(const this_t& rhs) const noexcept;
    bool operator==(const this_t& rhs) const noexcept;
  };

  template< typename Key, typename Value >
  ConstBiTreeIterator< Key, Value >::ConstBiTreeIterator(detail::BiTreeNode< Key, Value >* ptr):
    node_(ptr)
  {}

  template< typename Key, typename Value >
  ConstBiTreeIterator< Key, Value >::ConstBiTreeIterator():
    node_(nullptr)
  {}

  template< typename Key, typename Value >
  ConstBiTreeIterator< Key, Value >& ConstBiTreeIterator< Key, Value >::operator++() noexcept
  {
    assert(node_ != nullptr);
    if (node_->right_)
    {
      node_ = node_->right_;
      while (node_->left_)
      {
        node_ = node_->left_;
      }
    }
    else
    {
      auto parent = node_->parent_;
      while (parent && node_ == parent->right_)
      {
        node_ = parent;
        parent = parent->parent_;
      }
      node_ = parent;
    }
    return *this;
  }

  template< typename Key, typename Value >
  ConstBiTreeIterator< Key, Value > ConstBiTreeIterator< Key, Value >::operator++(int) noexcept
  {
    assert(node_ != nullptr);
    ConstBiTreeIterator copy(*this);
    ++(*this);
    return copy;
  }

  template< typename Key, typename Value >
  const std::pair< Key, Value >& ConstBiTreeIterator< Key, Value >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename Key, typename Value >
  const std::pair< Key, Value >* ConstBiTreeIterator< Key, Value >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename Key, typename Value >
  bool ConstBiTreeIterator< Key, Value >::operator==(const this_t& rhs) const noexcept
  {
    return (node_ == rhs->node_);
  }
  template< typename Key, typename Value >
  bool ConstBiTreeIterator< Key, Value >::operator!=(const this_t& rhs) const noexcept
  {
    return !(*this == rhs);
  }
}
#endif
