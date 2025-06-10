#ifndef TTT_ITERATOR_H
#define TTT_ITERATOR_H
#include <iterator>
#include "ttt-node.hpp"
#include "ttt-const-iterator.hpp"

namespace savintsev
{
  template< typename Key, typename Value, typename Compare >
  class TwoThreeTree;

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
    assert(pos_ < node_->len_);
    return node_->data_[pos_];
  }
  template< typename Key, typename Value >
  typename BidirectIterator< Key, Value >::pointer BidirectIterator< Key, Value >::operator->()
  {
    assert(node_ != nullptr);
    assert(pos_ < node_->len_);
    return std::addressof(node_->data_[pos_]);
  }
    template< typename Key, typename Value >
  typename BidirectIterator< Key, Value >::reference BidirectIterator< Key, Value >::operator*() const
  {
    assert(node_ != nullptr);
    assert(pos_ < node_->len_);
    return node_->data_[pos_];
  }
  template< typename Key, typename Value >
  typename BidirectIterator< Key, Value >::pointer BidirectIterator< Key, Value >::operator->() const
  {
    assert(node_ != nullptr);
    assert(pos_ < node_->len_);
    return std::addressof(node_->data_[pos_]);
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
      while (node_->kids_[node_->len_])
      {
        node_ = node_->kids_[node_->len_];
      }
      pos_ = node_->len_ - 1;
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
    while (node_->kids_[node_->len_])
    {
      node_ = node_->kids_[node_->len_];
    }

    pos_ = node_->len_ - 1;
    return *this;
  }
  template< typename Key, typename Value >
  BidirectIterator< Key, Value > & BidirectIterator< Key, Value >::next()
  {
    if (!node_)
    {
      return *this;
    }

    if (pos_ < node_->len_ && node_->kids_[pos_ + 1])
    {
      node_ = node_->kids_[pos_ + 1];
      while (node_->kids_[0])
      {
        node_ = node_->kids_[0];
      }
      pos_ = 0;
      return *this;
    }

    if (pos_ < node_->len_ - 1)
    {
      ++pos_;
      return *this;
    }

    while (node_->parent_)
    {
      size_t parent_pos = 0;
      while (parent_pos <= node_->parent_->len_ && node_->parent_->kids_[parent_pos] != node_)
      {
        ++parent_pos;
      }
      node_ = node_->parent_;
      if (parent_pos < node_->len_)
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

    if (node_->kids_[pos_])
    {
      node_ = node_->kids_[pos_];
      while (node_->kids_[node_->len_])
      {
        node_ = node_->kids_[node_->len_];
      }
      pos_ = node_->len_ - 1;
      return *this;
    }

    if (pos_ > 0)
    {
      --pos_;
      return *this;
    }

    while (node_->parent_)
    {
      auto parent = node_->parent_;
      size_t i = 0;
      while (i <= parent->len_ && parent->kids_[i] != node_)
      {
        ++i;
      }
      assert(i <= parent->len_);

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
