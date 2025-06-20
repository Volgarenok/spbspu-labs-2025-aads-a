#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include <cassert>
#include "node.hpp"

namespace aleksandrov
{
  template< typename K, typename V, typename C >
  class Tree;

  enum class PointsTo
  {
    None,
    Left,
    Right
  };

  template< typename K, typename V, typename C, bool isConst >
  class Iterator: public std::iterator< std::bidirectional_iterator_tag, V >
  {
  public:
    using Node = detail::Node< K, V >;
    using ValueType = std::pair< const K, V >;
    using NodeType = detail::NodeType;
    using reference = std::conditional_t< isConst, const ValueType&, ValueType& >;
    using pointer = std::conditional_t< isConst, const ValueType*, ValueType* >;

    Iterator();

    Iterator& operator++() noexcept;
    Iterator operator++(int) noexcept;
    Iterator& operator--() noexcept;
    Iterator operator--(int) noexcept;

    reference operator*() const noexcept;
    pointer operator->() const noexcept;

    bool operator==(const Iterator&) const noexcept;
    bool operator!=(const Iterator&) const noexcept;
  private:
    friend Tree< K, V, C >;

    Node* node_;
    PointsTo dir_;

    explicit Iterator(Node*, PointsTo);
  };

  template< typename K, typename V, typename C, bool isConst >
  Iterator< K, V, C, isConst >::Iterator():
    node_(nullptr),
    dir_(PointsTo::None)
  {}

  template< typename K, typename V, typename C, bool isConst >
  Iterator< K, V, C, isConst >::Iterator(Node* node, PointsTo dir):
    node_(node),
    dir_(dir)
  {}

  template< typename K, typename V, typename C, bool isConst >
  Iterator< K, V, C, isConst >& Iterator< K, V, C, isConst >::operator++() noexcept
  {
    assert(node_);
    if (node_->isTriple())
    {
      if (dir_ == PointsTo::Left && node_->middle)
      {
        node_ = node_->middle;
        while (node_->left)
        {
          node_ = node_->left;
        }
        return *this;
      }
      if (node_->right && dir_ == PointsTo::Right)
      {
        node_ = node_->right;
        while (node_->left)
        {
          node_ = node_->left;
        }
        dir_ = PointsTo::Left;
        return *this;
      }
      if (dir_ == PointsTo::Left)
      {
        dir_ = PointsTo::Right;
        return *this;
      }
    }
    if (node_->right)
    {
      node_ = node_->right;
      while (node_->left)
      {
        node_ = node_->left;
      }
      dir_ = PointsTo::Left;
      return *this;
    }

    while (node_->parent)
    {
      if (node_->parent->left = node_)
      {
        node_ = node_->parent;
        dir_ = PointsTo::Left;
        return *this;
      }
      if (node_->isTriple() && node_->parent->middle == node_)
      {
        node_ = node_->parent;
        dir_ = PointsTo::Right;
        return *this;
      }
      node_ = node_->parent;
    }
    node_ = nullptr;
    dir_ = PointsTo::None;
    return *this;
  }

  template< typename K, typename V, typename C, bool isConst >
  Iterator< K, V, C, isConst > Iterator< K, V, C, isConst >::operator++(int) noexcept
  {
    assert(node_);
    auto result(*this);
    ++(*this);
    return result;
  }

  template< typename K, typename V, typename C, bool isConst >
  Iterator< K, V, C, isConst >& Iterator< K, V, C, isConst >::operator--() noexcept
  {
    assert(node_);
    if (node_->isTriple())
    {
      if (dir_ == PointsTo::Right && node_->middle)
      {
        node_ = node_->middle;
        while (node_->right)
        {
          node_ = node_->right;
        }
        return *this;
      }
      if (node_->left && dir_ == PointsTo::Left)
      {
        node_ = node_->left;
        while (node_->right)
        {
          node_ = node_->right;
        }
        dir_ = PointsTo::Right;
        return *this;
      }
      if (dir_ == PointsTo::Right)
      {
        dir_ = PointsTo::Left;
        return *this;
      }
    }
    if (node_->left)
    {
      node_ = node_->left;
      while (node_->right)
      {
        node_ = node_->right;
      }
      dir_ = PointsTo::Right;
      return *this;
    }
    node_ = nullptr;
    dir_ = PointsTo::None;
    return *this;
  }

  template< typename K, typename V, typename C, bool isConst >
  Iterator< K, V, C, isConst > Iterator< K, V, C, isConst >::operator--(int) noexcept
  {
    assert(node_);
    auto result(*this);
    --(*this);
    return result;
  }

  template< typename K, typename V, typename C, bool isConst >
  typename Iterator< K, V, C, isConst >::reference Iterator< K, V, C, isConst >::operator*() const noexcept
  {
    assert(node_);
    assert(dir_ != PointsTo::None);
    return dir_ == PointsTo::Left ? node_->data[0] : node_->data[1];
  }

  template< typename K, typename V, typename C, bool isConst >
  typename Iterator< K, V, C, isConst >::pointer Iterator< K, V, C, isConst >::operator->() const noexcept
  {
    assert(node_);
    assert(dir_ != PointsTo::None);
    return std::addressof(dir_ == PointsTo::Left ? node_->data[0] : node_->data[1]);
  }

  template< typename K, typename V, typename C, bool isConst >
  bool Iterator< K, V, C, isConst >::operator==(const Iterator& rhs) const noexcept
  {
    return node_ == rhs.node_ && dir_ == rhs.dir_;
  }

  template< typename K, typename V, typename C, bool isConst >
  bool Iterator< K, V, C, isConst >::operator!=(const Iterator& rhs) const noexcept
  {
    return !(*this == rhs);
  }
}

#endif

