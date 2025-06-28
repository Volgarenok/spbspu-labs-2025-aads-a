#ifndef TTT_ITERATOR_H
#define TTT_ITERATOR_H
#include <iterator>
#include <cassert>
#include "ttt-node.hpp"

namespace savintsev
{
  template< typename Key, typename Value, typename Compare >
  class TwoThreeTree;

  template< typename Key, typename Value >
  class BidirectConstIterator;

  template< typename Key, typename Value >
  class BidirectIterator
  {
    template< typename K, typename V, typename C >
    friend class TwoThreeTree;
    friend class BidirectConstIterator< Key, Value >;
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::pair< Key, Value >;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;
    using node_type = node_t< value_type >;

    BidirectIterator() = default;
    reference operator*();
    pointer operator->();
    reference operator*() const;
    pointer operator->() const;

    BidirectIterator & operator++();
    BidirectIterator operator++(int);

    BidirectIterator & operator--();
    BidirectIterator operator--(int);
    bool operator!=(const BidirectIterator & rhs) const;
    bool operator==(const BidirectIterator & rhs) const;

  private:
    node_type * root_ = nullptr;
    node_type * node_ = nullptr;
    size_t pos_ = 0;

    BidirectIterator(node_type * root, node_type * node = nullptr, size_t pos = 0):
      root_(root),
      node_(node),
      pos_(pos)
    {}

    BidirectIterator & rbegin();
    BidirectIterator & next();
    BidirectIterator & prev();
  };

  template< typename Key, typename Value >
  typename BidirectIterator< Key, Value >::reference BidirectIterator< Key, Value >::operator*()
  {
    assert(node_ != nullptr);
    assert(pos_ < node_->len);
    return node_->data[pos_];
  }
  template< typename Key, typename Value >
  typename BidirectIterator< Key, Value >::pointer BidirectIterator< Key, Value >::operator->()
  {
    assert(node_ != nullptr);
    assert(pos_ < node_->len);
    return std::addressof(node_->data[pos_]);
  }
    template< typename Key, typename Value >
  typename BidirectIterator< Key, Value >::reference BidirectIterator< Key, Value >::operator*() const
  {
    assert(node_ != nullptr);
    assert(pos_ < node_->len);
    return node_->data[pos_];
  }
  template< typename Key, typename Value >
  typename BidirectIterator< Key, Value >::pointer BidirectIterator< Key, Value >::operator->() const
  {
    assert(node_ != nullptr);
    assert(pos_ < node_->len);
    return std::addressof(node_->data[pos_]);
  }
  template< typename Key, typename Value >
  BidirectIterator< Key, Value > & BidirectIterator< Key, Value >::operator++()
  {
    return next();
  }
  template< typename Key, typename Value >
  BidirectIterator< Key, Value > BidirectIterator< Key, Value >::operator++(int)
  {
    BidirectIterator< Key, Value > result(*this);
    ++(*this);
    return result;
  }
  template< typename Key, typename Value >
  BidirectIterator< Key, Value > & BidirectIterator< Key, Value >::operator--()
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
  BidirectIterator< Key, Value > BidirectIterator< Key, Value >::operator--(int)
  {
    BidirectIterator< Key, Value > result(*this);
    --(*this);
    return result;
  }
  template< typename Key, typename Value >
  bool BidirectIterator< Key, Value >::operator!=(const BidirectIterator & rhs) const
  {
    return !(*this == rhs);
  }
  template< typename Key, typename Value >
  bool BidirectIterator< Key, Value >::operator==(const BidirectIterator & rhs) const
  {
    if (node_ == nullptr && rhs.node_ == nullptr)
    {
      return true;
    }
    return node_ == rhs.node_ && pos_ == rhs.pos_;
  }
  template< typename Key, typename Value >
  BidirectIterator< Key, Value > & BidirectIterator< Key, Value >::rbegin()
  {
    if (!root_)
    {
      node_ = nullptr;
      return *this;
    }

    node_ = root_;
    while (node_->kids[node_->len])
    {
      node_ = node_->kids[node_->len];
    }

    pos_ = node_->len - 1;
    return *this;
  }
  template< typename Key, typename Value >
  BidirectIterator< Key, Value > & BidirectIterator< Key, Value >::next()
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
  BidirectIterator< Key, Value > & BidirectIterator< Key, Value >::prev()
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
