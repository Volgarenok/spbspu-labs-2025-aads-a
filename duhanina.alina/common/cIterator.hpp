#ifndef CITERATOR_HPP
#define CITERATOR_HPP

#include <iterator>
#include <cassert>
#include <utility>
#include "node.hpp"

namespace duhanina
{
  template < typename Key, typename Value, typename Compare >
  class Tree;

  template < typename Key, typename Value, typename Compare >
  class ConstIterator: public std::iterator< std::bidirectional_iterator_tag, std::pair< const Key, Value > >
  {
  public:
    ConstIterator();

    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;

    ConstIterator& operator++();
    ConstIterator operator++(int);
    ConstIterator& operator--();
    ConstIterator operator--(int);

    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const;

  private:
    const Node< Key, Value >* current_;
    friend class Tree< Key, Value, Compare >;
    explicit ConstIterator(const Node< Key, Value >* node);
  };

  template < typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare >::ConstIterator():
    current_(nullptr)
  {}

  template < typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare >::ConstIterator(const Node< Key, Value >* node):
    current_(node)
  {}

  template < typename Key, typename Value, typename Compare >
  const std::pair< Key, Value >& ConstIterator< Key, Value, Compare >::operator*() const
  {
    assert(current_ != nullptr);
    return current_->data;
  }

  template < typename Key, typename Value, typename Compare >
  const std::pair< Key, Value >* ConstIterator< Key, Value, Compare >::operator->() const
  {
    assert(current_ != nullptr);
    return std::addressof(current_->data);
  }

  template < typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare >& ConstIterator< Key, Value, Compare >::operator++()
  {
    assert(current_ != nullptr);
    if (current_->right)
    {
      current_ = current_->right;
      while (current_->left)
      {
        current_ = current_->left;
      }
    }
    else
    {
      const Node< Key, Value >* parent = current_->parent;
      while (parent && current_ == parent->right)
      {
        current_ = parent;
        parent = parent->parent;
      }
      current_ = parent;
    }
    return *this;
  }

  template < typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare > ConstIterator< Key, Value, Compare >::operator++(int)
  {
    assert(current_ != nullptr);
    ConstIterator temp = *this;
    ++(*this);
    return temp;
  }

  template < typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare >& ConstIterator< Key, Value, Compare >::operator--()
  {
    assert(current_ != nullptr);
    if (current_->left)
    {
      current_ = current_->left;
      while (current_->right)
      {
        current_ = current_->right;
      }
    }
    else
    {
      const Node< Key, Value >* parent = current_->parent;
      while (parent && current_ == parent->left)
      {
        current_ = parent;
        parent = parent->parent;
      }
      current_ = parent;
    }
    return *this;
  }

  template < typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare > ConstIterator< Key, Value, Compare >::operator--(int)
  {
    assert(current_ != nullptr);
    ConstIterator temp = *this;
    --(*this);
    return temp;
  }

  template < typename Key, typename Value, typename Compare >
  bool ConstIterator< Key, Value, Compare >::operator==(const ConstIterator& other) const
  {
    return current_ == other.current_;
  }

  template < typename Key, typename Value, typename Compare >
  bool ConstIterator< Key, Value, Compare >::operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
}

#endif
