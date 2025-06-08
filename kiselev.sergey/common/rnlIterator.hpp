#ifndef RNLITERATOR_HPP
#define RNLITERATOR_HPP

#include <cassert>
#include <iterator>
#include <utility>
#include "iterator.hpp"
#include "stack.hpp"
#include "treeNode.hpp"

namespace kiselev
{
  template< typename Key, typename Value, typename Cmp >
  class RBTree;

  namespace detail
  {
    template< typename Key, typename Value, typename Cmp, bool IsConst >
    class RnlIterator: public std::iterator< std::forward_iterator_tag, std::pair< Key, Value > >
    {
    public:
      using value = std::pair< Key, Value >;
      using reference = std::conditional_t< IsConst, const value&, value& >;
      using pointer = std::conditional_t< IsConst, const value*, value* >;
      using Node = TreeNode< Key, Value >;

      RnlIterator() noexcept;
      template < bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
      RnlIterator(const RnlIterator< Key, Value, Cmp, OtherIsConst >&) noexcept;
      template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
      RnlIterator< Key, Value, Cmp, IsConst >& operator=(const RnlIterator< Key, Value, Cmp, OtherIsConst >&) noexcept;

      RnlIterator< Key, Value, Cmp, IsConst > operator++();
      RnlIterator< Key, Value, Cmp, IsConst > operator++(int);

      reference operator*() const noexcept;
      pointer operator->() const noexcept;

      bool operator==(const RnlIterator< Key, Value, Cmp, IsConst >&) const noexcept;
      bool operator!=(const RnlIterator< Key, Value, Cmp, IsConst >&) const noexcept;

    private:
      Node* node_;
      Stack< Node* > stack_;
      explicit RnlIterator(Node*) noexcept;
      friend class RnlIterator< Key, Value, Cmp, !IsConst >;
      friend class RBTree< Key, Value, Cmp >;
    };

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    RnlIterator< Key, Value, Cmp, IsConst >::RnlIterator() noexcept:
      node_(nullptr),
      stack_()
    {}

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > >
    RnlIterator< Key, Value, Cmp, IsConst >::RnlIterator(const RnlIterator< Key, Value, Cmp, OtherIsConst >& oth) noexcept:
      node_(oth.node_),
      stack_(oth.stack_)
    {}

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > >
    RnlIterator< Key, Value, Cmp, IsConst >& RnlIterator< Key, Value, Cmp, IsConst >::operator=(
      const RnlIterator< Key, Value, Cmp, OtherIsConst >& oth) noexcept
    {
      node_ = oth.node_;
      stack_ = oth.stack_;
      return *this;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    RnlIterator< Key, Value, Cmp, IsConst >::RnlIterator(Node* node) noexcept:
      node_(node),
      stack_()
    {}

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    RnlIterator< Key, Value, Cmp, IsConst > RnlIterator< Key, Value, Cmp, IsConst >::operator++()
    {
      if (!node_)
      {
        return *this;
      }

      if (node_->left)
      {
        stack_.push(node_);
        node_ = node_->left;
        while (node_->right)
        {
          stack_.push(node_);
          node_ = node_->right;
        }
      }
      else
      {
        while (!stack_.empty() && stack_.top()->left == node_)
        {
          node_ = stack_.top();
          stack_.pop();
        }

        if (stack_.empty())
        {
          node_ = nullptr;
        }
        else
        {
          node_ = stack_.top();
          stack_.pop();
        }
      }
      return *this;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    RnlIterator< Key, Value, Cmp, IsConst > RnlIterator< Key, Value, Cmp, IsConst >::operator++(int)
    {
      RnlIterator< Key, Value, Cmp, IsConst > result(*this);
      ++(*this);
      return result;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    typename RnlIterator< Key, Value, Cmp, IsConst >::reference RnlIterator< Key, Value, Cmp, IsConst >::operator*() const noexcept
    {
      assert(node_ != nullptr);
      return node_->data;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    typename RnlIterator< Key, Value, Cmp, IsConst >::pointer RnlIterator< Key, Value, Cmp, IsConst >::operator->() const noexcept
    {
      assert(node_ != nullptr);
      return std::addressof(node_->data);
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    bool RnlIterator< Key, Value, Cmp, IsConst >::operator==(const RnlIterator< Key, Value, Cmp, IsConst >& oth) const noexcept
    {
      return node_ == oth.node_;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    bool RnlIterator< Key, Value, Cmp, IsConst >::operator!=(const RnlIterator< Key, Value, Cmp, IsConst >& oth) const noexcept
    {
      return !(*this == oth);
    }
  }
}
#endif
