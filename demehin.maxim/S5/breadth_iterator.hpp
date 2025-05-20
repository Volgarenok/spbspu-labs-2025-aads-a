#ifndef BREADTH_ITERATOR_HPP
#define BREADTH_ITERATOR_HPP
#include <cassert>
#include <tree/node.hpp>
#include <queue.hpp>
#include <queue>

namespace demehin
{
  template< typename Key, typename T, typename Cmp >
  class Tree;

  template< typename Key, typename T, typename Cmp, bool isConst >
  class BreadthIterator
  {
    friend class BreadthIterator< Key, T, Cmp, true >;
    friend class BreadthIterator< Key, T, Cmp, false >;
    friend class Tree< Key, T, Cmp >;
  public:
    using Node = demehin::TreeNode< Key, T >;
    using this_t = BreadthIterator< Key, T, Cmp, isConst >;
    using node_t = typename std::conditional< isConst, const Node, Node >::type;
    using queue_t = typename std::conditional< isConst, std::queue< const Node* >, std::queue< Node* > >::type;
    using data_t = typename std::conditional< isConst, const std::pair< Key, T >, std::pair< Key, T > >::type;

    BreadthIterator() noexcept;

    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;

    data_t& operator*() const noexcept;
    data_t* operator->() const noexcept;

    bool operator==(const this_t&) const noexcept;
    bool operator!=(const this_t&) const noexcept;

  private:
    node_t* node_;
    queue_t queue_;

    explicit BreadthIterator(node_t*) noexcept;
  };

  template< typename Key, typename T, typename Cmp, bool isConst >
  BreadthIterator< Key, T, Cmp, isConst >::BreadthIterator() noexcept:
    node_(nullptr)
  {}

  template< typename Key, typename T, typename Cmp, bool isConst >
  BreadthIterator< Key, T, Cmp, isConst >::BreadthIterator(node_t* node) noexcept:
    node_(node)
  {
    queue_.push(node_);
  }

  template< typename Key, typename T, typename Cmp, bool isConst >
  typename BreadthIterator< Key, T, Cmp, isConst >::this_t& BreadthIterator< Key, T, Cmp, isConst >::operator++() noexcept
  {
    if (queue_.empty())
    {
      node_ = nullptr;
    }
    else
    {
      node_ = queue_.front();
      queue_.pop();
      if (node_->left != nullptr)
      {
        queue_.push(node_->left);
      }
      if (node_->right != nullptr)
      {
        queue_.push(node_->right);
      }
      if (queue_.empty())
      {
        node_ = nullptr;
      }
      else
      {
        node_ = queue_.front();
      }
    }
    return *this;

  }

  template< typename Key, typename T, typename Cmp, bool isConst >
  typename BreadthIterator< Key, T, Cmp, isConst >::this_t BreadthIterator< Key, T, Cmp, isConst >::operator++(int) noexcept
  {
    this_t res(*this);
    ++(*this);
    return res;
  }

  template< typename Key, typename T, typename Cmp, bool isConst >
  typename BreadthIterator< Key, T, Cmp, isConst >::data_t& BreadthIterator< Key, T, Cmp, isConst >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< typename Key, typename T, typename Cmp, bool isConst >
  typename BreadthIterator< Key, T, Cmp, isConst >::data_t* BreadthIterator< Key, T, Cmp, isConst >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename Key, typename T, typename Cmp, bool isConst >
  bool BreadthIterator< Key, T, Cmp, isConst >::operator==(const this_t& rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< typename Key, typename T, typename Cmp, bool isConst >
  bool BreadthIterator< Key, T, Cmp, isConst >::operator!=(const this_t& rhs) const noexcept
  {
    return !(*this == rhs);
  }
}

#endif
