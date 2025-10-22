#ifndef BITREEITERATOR_HPP
#define BITREEITERATOR_HPP
#include <iterator>
#include <utility>
#include "BiTreeNode.hpp"

namespace nikonov
{
  template< typename Key, typename Value >
  class ConstBiTreeIterator : public std::iterator< std::bidirectional_iterator_tag, const std::pair< Key, Value > >
  {
  public:
    using Node = detail::BiTreeNode< Key, Value >;

    ConstBiTreeIterator() noexcept;
    ConstBiTreeIterator(const Node* node) noexcept;
    ConstBiTreeIterator(const ConstBiTreeIterator&) = default;
    ~ConstBiTreeIterator() = default;

    ConstBiTreeIterator& operator=(const ConstBiTreeIterator&) = default;

    ConstBiTreeIterator& operator++();
    ConstBiTreeIterator operator++(int);
    ConstBiTreeIterator& operator--();
    ConstBiTreeIterator operator--(int);

    bool operator==(const ConstBiTreeIterator& other) const noexcept;
    bool operator!=(const ConstBiTreeIterator& other) const noexcept;

    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;

  private:
    const Node* current_;

    const Node* findMin(const Node* node) const;
    const Node* findMax(const Node* node) const;
  };

  template< typename Key, typename Value >
  ConstBiTreeIterator< Key, Value >::ConstBiTreeIterator() noexcept:
    current_(nullptr)
  {}

  template< typename Key, typename Value >
  ConstBiTreeIterator< Key, Value >::ConstBiTreeIterator(const Node* node) noexcept:
    current_(node)
  {}

  template< typename Key, typename Value >
  ConstBiTreeIterator< Key, Value >& ConstBiTreeIterator< Key, Value >::operator++()
  {
    if (current_ == nullptr)
    {
      return *this;
    }
    if (current_->right != nullptr)
    {
      current_ = findMin(current_->right);
    }
    else
    {
      const Node* parent = current_->parent;
      const Node* node = current_;
      while (parent != nullptr && node == parent->right)
      {
        node = parent;
        parent = parent->parent;
      }
      current_ = parent;
    }
    return *this;
  }

  template< typename Key, typename Value >
  ConstBiTreeIterator< Key, Value > ConstBiTreeIterator< Key, Value >::operator++(int)
  {
    ConstBiTreeIterator temp = *this;
    ++(*this);
    return temp;
  }

  template< typename Key, typename Value >
  ConstBiTreeIterator< Key, Value >& ConstBiTreeIterator< Key, Value >::operator--()
  {
    if (current_ == nullptr)
    {
      return *this;
    }
    if (current_->left != nullptr)
    {
      current_ = findMax(current_->left);
    }
    else
    {
      const Node* parent = current_->parent;
      const Node* node = current_;
      
      while (parent != nullptr && node == parent->left)
      {
        node = parent;
        parent = parent->parent;
      }
      current_ = parent;
    }
    return *this;
  }

  template< typename Key, typename Value >
  ConstBiTreeIterator< Key, Value > ConstBiTreeIterator< Key, Value >::operator--(int)
  {
    ConstBiTreeIterator temp = *this;
    --(*this);
    return temp;
  }

  template< typename Key, typename Value >
  bool ConstBiTreeIterator< Key, Value >::operator==(const ConstBiTreeIterator& other) const noexcept
  {
    return current_ == other.current_;
  }

  template< typename Key, typename Value >
  bool ConstBiTreeIterator< Key, Value >::operator!=(const ConstBiTreeIterator& other) const noexcept
  {
    return !(*this == other);
  }

  template< typename Key, typename Value >
  const std::pair< Key, Value >& ConstBiTreeIterator< Key, Value >::operator*() const
  {
    return current_->data;
  }

  template< typename Key, typename Value >
  const std::pair< Key, Value >* ConstBiTreeIterator< Key, Value >::operator->() const
  {
    return &(current_->data);
  }

  template< typename Key, typename Value >
  const typename ConstBiTreeIterator< Key, Value >::Node* ConstBiTreeIterator< Key, Value >::findMin(const Node* node) const
  {
    if (node == nullptr)
    {
      return nullptr;
    }
    while (node->left != nullptr)
    {
      node = node->left;
    }
    return node;
  }

  template< typename Key, typename Value >
  const typename ConstBiTreeIterator< Key, Value >::Node* ConstBiTreeIterator< Key, Value >::findMax(const Node* node) const
  {
    if (node == nullptr)
    {
      return nullptr;
    }
    while (node->right != nullptr)
    {
      node = node->right;
    }
    return node;
  }
}
#endif
