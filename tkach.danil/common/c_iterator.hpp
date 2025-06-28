#ifndef C_ITERATOR_HPP
#define C_ITERATOR_HPP

#include <iterator>
#include "node_tree.hpp"

namespace tkach
{
  template< class Key, class Value, class Cmp>
  class AvlTree;

  template< class Key, class Value, class Cmp = std::less< Key > >
  class Citerator: public std::iterator< std::bidirectional_iterator_tag, Value >
  {
    friend class AvlTree< Key, Value, Cmp >;
  public:
    using this_t = Citerator< Key, Value, Cmp >;
    Citerator();
    Citerator(const this_t&) = default;
    ~Citerator() = default;
    this_t& operator=(const this_t&) = default;
    this_t& operator++();
    this_t operator++(int);
    this_t& operator--();
    this_t operator--(int);
    const std::pair< Key, Value > & operator*() const;
    const std::pair< Key, Value > * operator->() const;
    bool operator!=(const this_t& rhs) const;
    bool operator==(const this_t& rhs) const;
  private:
    TreeNode< Key, Value >* node_;
    explicit Citerator(TreeNode< Key, Value >* node);
  };

  template< class Key, class Value, class Cmp>
  Citerator< Key, Value, Cmp >::Citerator():
    node_(nullptr)
  {}

  template< class Key, class Value, class Cmp>
  Citerator< Key, Value, Cmp >::Citerator(TreeNode< Key, Value >* node):
    node_(node)
  {}

  template< class Key, class Value, class Cmp>
  Citerator< Key, Value, Cmp >& Citerator< Key, Value, Cmp >::operator++()
  {
    if (node_->right)
    {
      node_ = node_->right;
      while (node_->left)
      {
        node_ = node_->left;
      }
      return *this;
    }
    else
    {
      while (node_->parent && node_->parent->right == node_)
      {
        node_ = node_->parent;
      }
      node_ = node_->parent;
      return *this;
    }
  }

  template< class Key, class Value, class Cmp>
  Citerator< Key, Value, Cmp > Citerator< Key, Value, Cmp >::operator++(int)
  {
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< class Key, class Value, class Cmp>
  Citerator< Key, Value, Cmp >& Citerator< Key, Value, Cmp >::operator--()
  {
    if (node_->left)
    {
      node_ = node_->left;
      while (node_->right)
      {
        node_ = node_->right;
      }
      return *this;
    }
    else
    {
      while (node_->parent && node_->parent->left == node_)
      {
        node_ = node_->parent;
      }
      node_ = node_->parent;
      return *this;
    }
  }

  template< class Key, class Value, class Cmp>
  Citerator< Key, Value, Cmp > Citerator< Key, Value, Cmp >::operator--(int)
  {
    this_t result(*this);
    --(*this);
    return result;
  }

  template< class Key, class Value, class Cmp>
  const std::pair< Key, Value >& Citerator< Key, Value, Cmp >::operator*() const
  {
    return node_->data;
  }

  template< class Key, class Value, class Cmp>
  const std::pair< Key, Value >* Citerator< Key, Value, Cmp >::operator->() const
  {
    return std::addressof(node_->data);
  }

  template< class Key, class Value, class Cmp>
  bool Citerator< Key, Value, Cmp >::operator==(const this_t& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class Key, class Value, class Cmp>
  bool Citerator< Key, Value, Cmp >::operator!=(const this_t& rhs) const
  {
    return !(rhs == *this);
  }
}

#endif
