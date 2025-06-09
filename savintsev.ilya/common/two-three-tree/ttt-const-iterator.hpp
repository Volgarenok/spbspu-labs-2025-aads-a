#ifndef TTT_CONST_ITERATOR_H
#define TTT_CONST_ITERATOR_H
#include <iterator>
#include <cassert>
#include "ttt-node.hpp"

namespace savintsev
{
  template< typename Key, typename Value, typename Compare >
  class TwoThreeTree;

  template< typename Key, typename Value >
  class BidirectIterator;

  template< typename Key, typename Value >
  class BidirectConstIterator
  {
    template< typename K, typename V, typename C >
    friend class TwoThreeTree;
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::pair< Key, Value >;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type *;
    using reference = const value_type &;
    using node_type = node_t< value_type >;

    BidirectConstIterator() = default;
    BidirectConstIterator(const BidirectIterator< Key, Value > & it);

    reference operator*() const;
    pointer operator->() const;

    BidirectConstIterator & operator++();
    BidirectConstIterator operator++(int);

    BidirectConstIterator & operator--();
    BidirectConstIterator operator--(int);

    bool operator!=(const BidirectConstIterator & rhs) const;
    bool operator==(const BidirectConstIterator & rhs) const;

  private:
    node_type * root_ = nullptr;
    node_type * node_ = nullptr;
    size_t pos_ = 0;

    BidirectConstIterator(node_type * root, node_type * node = nullptr, size_t pos = 0);

    BidirectConstIterator & next();
    BidirectConstIterator & prev();
  };

  template< typename Key, typename Value >
  BidirectConstIterator< Key, Value >::BidirectConstIterator(const BidirectIterator< Key, Value > & it):
    root_(it.root_),
    node_(it.node_),
    pos_(it.pos_)
  {}
  template< typename Key, typename Value >
  typename BidirectConstIterator< Key, Value >::reference BidirectConstIterator< Key, Value >::operator*() const
  {
    assert(node_ != nullptr);
    assert(pos_ < node_->len);
    return node_->data[pos_];
  }
  template< typename Key, typename Value >
  typename BidirectConstIterator< Key, Value >::pointer BidirectConstIterator< Key, Value >::operator->() const
  {
    assert(node_ != nullptr);
    assert(pos_ < node_->len);
    return std::addressof(node_->data[pos_]);
  }
  template< typename Key, typename Value >
  BidirectConstIterator< Key, Value > & BidirectConstIterator< Key, Value >::operator++()
  {
    return next();
  }
  template< typename Key, typename Value >
  BidirectConstIterator< Key, Value > BidirectConstIterator< Key, Value >::operator++(int)
  {
    BidirectConstIterator< Key, Value > result(*this);
    ++(*this);
    return result;
  }
  template< typename Key, typename Value >
  BidirectConstIterator< Key, Value > & BidirectConstIterator< Key, Value >::operator--()
  {
    if (!node_)
    {
      node_ = root_;
      while (node_->kids[node_->len])
      {
        node_ = node_->kids[node_->len];
      }
      pos_ = node_->len - 1;
      return *this;
    }

    return prev();
  }
  template< typename Key, typename Value >
  BidirectConstIterator< Key, Value > BidirectConstIterator< Key, Value >::operator--(int)
  {
    BidirectConstIterator< Key, Value > result(*this);
    --(*this);
    return result;
  }
  template< typename Key, typename Value >
  bool BidirectConstIterator< Key, Value >::operator!=(const BidirectConstIterator & rhs) const
  {
    return !(*this == rhs);
  }
  template< typename Key, typename Value >
  bool BidirectConstIterator< Key, Value >::operator==(const BidirectConstIterator & rhs) const
  {
    if (node_ == nullptr && rhs.node_ == nullptr)
    {
      return true;
    }
    return node_ == rhs.node_ && pos_ == rhs.pos_;
  }
  template< typename K, typename V >
  BidirectConstIterator< K, V >::BidirectConstIterator(node_type * root, node_type * node, size_t pos):
    root_(root),
    node_(node),
    pos_(pos)
  {}
  template< typename Key, typename Value >
  BidirectConstIterator< Key, Value > & BidirectConstIterator< Key, Value >::next()
  {
    if (!node_)
    {
      return *this;
    }

    if (pos_ < node_->len && node_->kids[pos_ + 1])
    {
      node_ = node_->kids[pos_ + 1];
      while (node_->kids[0])
      {
        node_ = node_->kids[0];
      }
      pos_ = 0;
      return *this;
    }

    if (pos_ < node_->len - 1)
    {
      ++pos_;
      return *this;
    }

    while (node_->father)
    {
      size_t parent_pos = 0;
      while (parent_pos <= node_->father->len && node_->father->kids[parent_pos] != node_)
      {
        ++parent_pos;
      }
      node_ = node_->father;
      if (parent_pos < node_->len)
      {
        pos_ = parent_pos;
        return *this;
      }
    }
    node_ = nullptr;
    return *this;
  }
  template< typename Key, typename Value >
  BidirectConstIterator< Key, Value > & BidirectConstIterator< Key, Value >::prev()
  {
    assert(node_ != nullptr);

    if (node_->kids[pos_])
    {
      node_ = node_->kids[pos_];
      while (node_->kids[node_->len])
      {
        node_ = node_->kids[node_->len];
      }
      pos_ = node_->len - 1;
      return *this;
    }

    if (pos_ > 0)
    {
      --pos_;
      return *this;
    }

    while (node_->father)
    {
      auto parent = node_->father;
      size_t i = 0;
      while (i <= parent->len && parent->kids[i] != node_)
      {
        ++i;
      }
      assert(i <= parent->len);

      if (i > 0)
      {
        node_ = parent;
        pos_ = i - 1;
        return *this;
      }

      node_ = parent;
    }

    node_ = nullptr;
    return *this;
  }
}

#endif
