#ifndef BREADTHITERATOR_HPP
#define BREADTHITERATOR_HPP
#include <cassert>
#include <iterator>
#include <utility>
#include "queue.hpp"
#include "treeNode.hpp"

namespace kiselev
{
  template< typename Key, typename Value, typename Cmp >
  class RBTree;

  namespace detail
  {
    template< typename Key, typename Value, typename Cmp, bool IsConst >
    class BreadthIterator: public std::iterator< std::forward_iterator_tag, std::pair< Key, Value > >
    {
    public:
      using value = std::pair< Key, Value >;
      using reference = std::conditional_t< IsConst, const value&, value& >;
      using pointer = std::conditional_t< IsConst, const value*, value* >;
      using Node = TreeNode< Key, Value >;

      BreadthIterator() noexcept;
      template < bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
      BreadthIterator(const BreadthIterator< Key, Value, Cmp, OtherIsConst >&);
      template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
      BreadthIterator< Key, Value, Cmp, IsConst > operator=(const BreadthIterator< Key, Value, Cmp, OtherIsConst >&);

      BreadthIterator< Key, Value, Cmp, IsConst > operator++();
      BreadthIterator< Key, Value, Cmp, IsConst > operator++(int);


      reference operator*() const noexcept;
      pointer operator->() const noexcept;

      bool operator==(const BreadthIterator< Key, Value, Cmp, IsConst >&) const noexcept;
      bool operator!=(const BreadthIterator< Key, Value, Cmp, IsConst >&) const noexcept;
    private:
      Node* node_;
      Queue< Node* > queue_;
      explicit BreadthIterator(Node*) noexcept;
      friend class BreadthIterator< Key, Value, Cmp, !IsConst >;
      friend class RBTree< Key, Value, Cmp >;
    };

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > >
    BreadthIterator< Key, Value, Cmp, IsConst >::BreadthIterator(const BreadthIterator< Key, Value, Cmp, OtherIsConst >& oth):
      node_(oth.node_),
      queue_(oth.queue_)
    {}

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    BreadthIterator< Key, Value, Cmp, IsConst >::BreadthIterator() noexcept:
      node_(nullptr),
      queue_()
    {}

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > >
    BreadthIterator< Key, Value, Cmp, IsConst > BreadthIterator< Key, Value, Cmp, IsConst >::operator=(
      const BreadthIterator< Key, Value, Cmp, OtherIsConst >& oth)
    {
      node_ = oth.node_;
      queue_ = oth.queue_;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    BreadthIterator< Key, Value, Cmp, IsConst >::BreadthIterator(Node* node) noexcept:
      node_(node),
      queue_()
    {
      if (node)
      {
        if (node_->left)
        {
          queue_.push(node_->left);
        }
        if (node_->right)
        {
          queue_.push(node_->right);
        }
      }
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    BreadthIterator< Key, Value, Cmp, IsConst > BreadthIterator< Key, Value, Cmp, IsConst >::operator++()
    {
      if (queue_.empty())
      {
        node_ = nullptr;
        return *this;
      }
      node_ = queue_.front();
      queue_.pop();
      if (node_->left)
      {
        queue_.push(node_->left);
      }
      if (node_->right)
      {
        queue_.push(node_->right);
      }
      return *this;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    BreadthIterator< Key, Value, Cmp, IsConst > BreadthIterator< Key, Value, Cmp, IsConst >::operator++(int)
    {
      BreadthIterator< Key, Value, Cmp, IsConst > result(*this);
      ++(*this);
      return result;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    typename BreadthIterator< Key, Value, Cmp, IsConst >::reference BreadthIterator< Key, Value, Cmp, IsConst >::operator*() const noexcept
    {
      assert(node_ != nullptr);
      return node_->data;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    typename BreadthIterator< Key, Value, Cmp, IsConst >::pointer BreadthIterator< Key, Value, Cmp, IsConst >::operator->() const noexcept
    {
      assert(node_ != nullptr);
      return std::addressof(node_->data);
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    bool BreadthIterator< Key, Value, Cmp, IsConst >::operator==(const BreadthIterator<Key, Value, Cmp, IsConst>& oth) const noexcept
    {
      return node_ == oth.node_;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    bool BreadthIterator< Key, Value, Cmp, IsConst >::operator!=(const BreadthIterator< Key, Value, Cmp, IsConst >& oth) const noexcept
    {
      return !(*this == oth);
    }
  }
}
#endif
