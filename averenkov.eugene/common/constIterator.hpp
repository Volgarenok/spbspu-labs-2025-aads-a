#ifndef TREE_CONST_ITERATOR_HPP
#define TREE_CONST_ITERATOR_HPP

#include <iterator>
#include <cassert>
#include "node.hpp"
#include "iterator.hpp"

namespace averenkov
{
  template< class Key, class Value, class Compare >
  class Tree;

  template < class Key, class Value, class Compare >
  class ConstIterator : public std::iterator < std::bidirectional_iterator_tag, const std::pair< const Key, Value > >
  {
    friend class Tree< Key, Value, Compare >;
  public:
    ConstIterator();
    ConstIterator(const Iterator< Key, Value, Compare >& other);

    const std::pair< const Key, Value >& operator*() const;
    const std::pair< const Key, Value >* operator->() const;

    ConstIterator& operator++();
    ConstIterator operator++(int);

    ConstIterator& operator--();
    ConstIterator operator--(int);

    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const;

  private:
    const averenkov::detail::Node< Key, Value >* current;
    explicit ConstIterator(const averenkov::detail::Node< Key, Value >* node);

  };

  template < class Key, class Value, class Compare >
  ConstIterator< Key, Value, Compare >::ConstIterator():
    current(nullptr)
  {
  }

  template < class Key, class Value, class Compare >
  ConstIterator< Key, Value, Compare >::ConstIterator(const averenkov::detail::Node< Key, Value >* node):
    current(node)
  {
  }

  template < class Key, class Value, class Compare >
  ConstIterator< Key, Value, Compare >::ConstIterator(const Iterator< Key, Value, Compare >& other):
    current(other.current)
  {
  }

  template < class Key, class Value, class Compare >
  const std::pair< const Key, Value >& ConstIterator< Key, Value, Compare >::operator*() const
  {
    assert(current != nullptr);
    return current->data;
  }

  template < class Key, class Value, class Compare >
  const std::pair< const Key, Value >* ConstIterator< Key, Value, Compare >::operator->() const
  {
    assert(current != nullptr);
    return std::addressof(current->data);
  }

  template < class Key, class Value, class Compare >
  ConstIterator< Key, Value, Compare >& ConstIterator< Key, Value, Compare >::operator++()
  {
    assert(current != nullptr);
    if (current->right)
    {
      current = current->right;
      while (current->left)
      {
        current = current->left;
      }
    }
    else
    {
      auto* parent = current->parent;
      while (parent && current == parent->right)
      {
        current = parent;
        parent = parent->parent;
      }
      current = parent;
    }
    return *this;
  }

  template < class Key, class Value, class Compare >
  ConstIterator< Key, Value, Compare > ConstIterator< Key, Value, Compare >::operator++(int)
  {
    assert(current != nullptr);
    ConstIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template < class Key, class Value, class Compare >
  ConstIterator< Key, Value, Compare >& ConstIterator< Key, Value, Compare >::operator--()
  {
    assert(current != nullptr);
    if (current->left)
    {
      current = current->left;
      while (current->right)
      {
        current = current->right;
      }
    }
    else
    {
      auto* parent = current->parent;
      while (parent && current == parent->left)
      {
        current = parent;
        parent = parent->parent;
      }
      current = parent;
    }
    return *this;
  }

  template < class Key, class Value, class Compare >
  ConstIterator< Key, Value, Compare > ConstIterator< Key, Value, Compare >::operator--(int)
  {
    ConstIterator tmp = *this;
    --(*this);
    return tmp;
  }

  template < class Key, class Value, class Compare >
  bool ConstIterator< Key, Value, Compare >::operator==(const ConstIterator& other) const
  {
    return current == other.current;
  }

  template < class Key, class Value, class Compare >
  bool ConstIterator< Key, Value, Compare >::operator!=(const ConstIterator& other) const
  {
    return current != other.current;
  }

}
#endif
