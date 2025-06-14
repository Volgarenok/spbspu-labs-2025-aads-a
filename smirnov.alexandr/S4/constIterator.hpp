#ifndef CONST_ITERATOR_HPP
#define CONST_ITERATOR_HPP
#include <functional>
#include "node.hpp"

namespace smirnov
{
  template < typename Key, typename Value, typename Compare >
  class AvlTree;
  template < typename Key, typename Value, typename Compare = std::less< Key > >
  class ConstIterator
  {
    friend class AvlTree< Key, Value, Compare >;
  public:
    ConstIterator();
    const std::pair< const Key, Value > & operator*() const;
    const std::pair< const Key, Value > * operator->() const;
    ConstIterator & operator++();
    bool operator==(const ConstIterator & other) const;
    bool operator!=(const ConstIterator & other) const;
  private:
    explicit ConstIterator(Node< Key, Value > * node);
    Node< Key, Value > * current_;
  };

  template < typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare >::ConstIterator():
    current_(nullptr)
  {}

  template < typename Key, typename Value, typename Compare >
  ConstIterator<Key, Value, Compare>::ConstIterator(Node< Key, Value > * node):
    current_(node)
  {}

  template < typename Key, typename Value, typename Compare >
  const std::pair< const Key, Value > & ConstIterator< Key, Value, Compare >::operator*() const
  {
    return current_->data;
  }

  template < typename Key, typename Value, typename Compare >
  const std::pair< const Key, Value > * ConstIterator< Key, Value, Compare >::operator->() const
  {
    return std::adressof(current_->data);
  }

  template < typename Key, typename Value, typename Compare >
  ConstIterator< Key, Value, Compare > & ConstIterator< Key, Value, Compare >::operator++()
  {
    current_ = nullptr;
    return *this;
  }

  template < typename Key, typename Value, typename Compare >
  bool ConstIterator< Key, Value, Compare >::operator==(const ConstIterator & other) const
  {
    return current_ == other.current_;
  }

  template < typename Key, typename Value, typename Compare >
  bool ConstIterator< Key, Value, Compare >::operator!=(const ConstIterator & other) const
  {
    return !(*this == other);
  }
}
#endif

