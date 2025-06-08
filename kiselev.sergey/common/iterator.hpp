#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
#include <cassert>
#include "treeNode.hpp"

namespace kiselev
{
  template< typename Key, typename Value, typename Cmp >
  class RBTree;

  namespace detail
  {
    template< typename Key, typename Value, typename Cmp, bool IsConst >
    class Iterator: public std::iterator< std::bidirectional_iterator_tag, std::pair< Key, Value > >
    {
    public:
      using value = std::pair< Key, Value >;
      using reference = std::conditional_t< IsConst, const value&, value& >;
      using pointer = std::conditional_t< IsConst, const value*, value* >;
      using iterator = Iterator< Key, Value, Cmp, IsConst >;

      Iterator();
      template < bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
      Iterator(const Iterator< Key, Value, Cmp, OtherIsConst >&) noexcept;
      template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
      iterator& operator=(const Iterator< Key, Value, Cmp, OtherIsConst >&) noexcept;

      iterator& operator++() noexcept;
      iterator operator++(int) noexcept;

      iterator& operator--() noexcept;
      iterator operator--(int) noexcept;

      reference operator*() const noexcept;
      pointer operator->() const noexcept;

      bool operator==(const iterator&) const noexcept;
      bool operator!=(const iterator&) const noexcept;
    private:
      TreeNode< Key, Value >* node_;
      bool isEnd_;
      Iterator(TreeNode< Key, Value >*, bool);
      friend class Iterator< Key, Value, Cmp, !IsConst >;
      friend class RBTree< Key, Value, Cmp >;
    };

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    Iterator< Key, Value, Cmp, IsConst >::Iterator():
      node_(nullptr),
      isEnd_(false)
    {}

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > >
    Iterator< Key, Value, Cmp, IsConst >::Iterator(const Iterator< Key, Value, Cmp, OtherIsConst >& other) noexcept:
      node_(other.node_),
      isEnd_(other.isEnd_)
    {}

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    Iterator< Key, Value, Cmp, IsConst >::Iterator(TreeNode< Key, Value >* node, bool isEnd):
      node_(node),
      isEnd_(isEnd)
    {}

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > >
    Iterator< Key, Value, Cmp, IsConst >& Iterator< Key, Value, Cmp, IsConst >::operator=(
      const Iterator< Key, Value, Cmp, OtherIsConst >& other) noexcept
    {
      node_ = other.node_;
      isEnd_ = other.isEnd_;
      return *this;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    Iterator< Key, Value, Cmp, IsConst >& Iterator< Key, Value, Cmp, IsConst >::operator++() noexcept
    {
      assert(node_ != nullptr);
      TreeNode< Key, Value >* temp = node_;
      if (node_->right)
      {
        node_ = node_->right;
        while (node_->left)
        {
          node_ = node_->left;
        }
      }
      else
      {
        while (node_->parent && node_ == node_->parent->right)
        {
          node_ = node_->parent;
        }
        node_ = node_->parent;
      }
      if (!node_)
      {
        node_ = temp;
        isEnd_ = true;
      }
      return *this;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    Iterator< Key, Value, Cmp, IsConst > Iterator< Key, Value, Cmp, IsConst >::operator++(int) noexcept
    {
      assert(node_ != nullptr);
      iterator result(*this);
      ++(*this);
      return result;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    Iterator< Key, Value, Cmp, IsConst >& Iterator< Key, Value, Cmp, IsConst >::operator--() noexcept
    {
      assert(node_ != nullptr);
      if (isEnd_)
      {
        isEnd_ = false;
      }
      else
      {
        if (node_->left)
        {
          node_ = node_->left;
          while (node_->right)
          {
            node_ = node_->right;
          }
          return iterator(node_);
        }
        while (node_->parent && node_ == node_->parent->left)
        {
          node_ = node_->parent;
        }
        node_ = node_->parent;
      }
      return *this;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    Iterator< Key, Value, Cmp, IsConst > Iterator< Key, Value, Cmp, IsConst >::operator--(int) noexcept
    {
      iterator result(*this);
      --(*this);
      return result;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    typename Iterator< Key, Value, Cmp, IsConst >::reference Iterator< Key, Value, Cmp, IsConst >::operator*() const noexcept
    {
      assert(node_ != nullptr);
      return node_->data;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    typename Iterator< Key, Value, Cmp, IsConst >::pointer Iterator< Key, Value, Cmp, IsConst >::operator->() const noexcept
    {
      assert(node_ != nullptr);
      return std::addressof(node_->data);
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    bool Iterator< Key, Value, Cmp, IsConst >::operator==(const iterator& other) const noexcept
    {
      return node_ == other.node_ && isEnd_ == other.isEnd_;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    bool Iterator< Key, Value, Cmp, IsConst >::operator!=(const iterator& other) const noexcept
    {
      return !(*this == other);
    }
  }
}
#endif
