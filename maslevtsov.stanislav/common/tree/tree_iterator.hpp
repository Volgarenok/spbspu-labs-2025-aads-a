#ifndef TREE_ITERATOR_HPP
#define TREE_ITERATOR_HPP

#include <iterator>
#include <type_traits>
#include "tree_node.hpp"

namespace maslevtsov {
  namespace detail {
    enum class IteratorType
    {
      CONSTANT,
      NONCONSTANT,
    };
  }

  template< class T, detail::IteratorType it_type >
  class TreeIterator final: public std::iterator< std::bidirectional_iterator_tag, T >
  {
  public:
    template< class Key, class Type, class Compare >
    friend class Tree;

    TreeIterator();

    TreeIterator& operator++();
    TreeIterator& operator++(int);
    TreeIterator& operator--();
    TreeIterator& operator--(int);

    typename std::conditional< it_type == detail::IteratorType::CONSTANT, const T&, T& >::type operator*() const;
    typename std::conditional< it_type == detail::IteratorType::CONSTANT, const T*, T* >::type operator->() const;

    bool operator==(const TreeIterator& rhs) const;
    bool operator!=(const TreeIterator& rhs) const;

  private:
    TreeNode< T >* node_;
    bool is_first_;

    explicit TreeIterator(TreeNode< T >* node, bool is_first_);
    static TreeNode< T >* get_min_node(TreeNode< T >* node);
    static TreeNode< T >* get_max_node(TreeNode< T >* node);
  };

  template< class T, detail::IteratorType it_type >
  TreeIterator< T, it_type >::TreeIterator():
    node_(nullptr)
  {}

  template< class T, detail::IteratorType it_type >
  typename TreeIterator< T, it_type >::TreeIterator& TreeIterator< T, it_type >::operator++()
  {
    if (!node_) {
      return *this;
    }
    if (!node_->is_two && is_first_) {
      if (node_->middle) {
        node_ = get_min_node(node_->middle);
        is_first_ = true;
      } else {
        is_first_ = false;
      }
      return *this;
    }
    if ((node_->is_two || !is_first_) && node_->right) {
      node_ = get_min_node(node_->right);
      is_first_ = true;
      return *this;
    }
    TreeNode< T >* child = node_;
    TreeNode< T >* parent = node_->parent;
    while (parent) {
      if (parent->is_two) {
        if (parent->left == child) {
          node_ = parent;
          is_first_ = true;
          return *this;
        }
      } else {
        if (parent->left == child) {
          node_ = parent;
          is_first_ = true;
          return *this;
        } else if (parent->middle == child) {
          node_ = parent;
          is_first_ = false;
          return *this;
        }
      }
      child = parent;
      parent = parent->parent;
    }
    node_ = child;
    is_first_ = true;
    return *this;
  }

  template< class T, detail::IteratorType it_type >
  typename TreeIterator< T, it_type >::TreeIterator& TreeIterator< T, it_type >::operator++(int)
  {
    TreeIterator< T, it_type > result(*this);
    ++(*this);
    return result;
  }

  template< class T, detail::IteratorType it_type >
  typename TreeIterator< T, it_type >::TreeIterator& TreeIterator< T, it_type >::operator--()
  {
    if (!node_) {
      return *this;
    }
    if (!node_->is_two && !is_first_) {
      if (node_->middle) {
        node_ = get_max_node(node_->middle);
        is_first_ = node_->is_two ? true : false;
      } else {
        is_first_ = true;
      }
      return *this;
    }
    if ((node_->is_two || is_first_) && node_->left) {
      node_ = get_max_node(node_->left);
      is_first_ = node_->is_two ? true : false;
      return *this;
    }
    TreeNode< T >* child = node_;
    TreeNode< T >* parent = node_->parent;
    while (parent) {
      if (parent->is_two) {
        if (parent->right == child) {
          node_ = parent;
          is_first_ = node_->is_two ? true : false;
          return *this;
        }
      } else {
        if (parent->right == child) {
          node_ = parent;
          is_first_ = node_->is_two ? true : false;
          return *this;
        } else if (parent->middle == child) {
          node_ = parent;
          is_first_ = node_->is_two ? true : false;
          return *this;
        }
      }
      child = parent;
      parent = parent->parent;
    }
    node_ = child;
    is_first_ = node_->is_two ? true : false;
    return *this;
  }

  template< class T, detail::IteratorType it_type >
  typename TreeIterator< T, it_type >::TreeIterator& TreeIterator< T, it_type >::operator--(int)
  {
    TreeIterator< T, it_type > result(*this);
    --(*this);
    return result;
  }

  template< class T, detail::IteratorType it_type >
  typename std::conditional< it_type == detail::IteratorType::CONSTANT, const T&, T& >::type
    TreeIterator< T, it_type >::operator*() const
  {
    return is_first_ ? node_->data1 : node_->data2;
  }

  template< class T, detail::IteratorType it_type >
  typename std::conditional< it_type == detail::IteratorType::CONSTANT, const T*, T* >::type
    TreeIterator< T, it_type >::operator->() const
  {
    return is_first_ ? std::addressof(node_->data1) : std::addressof(node_->data2);
  }

  template< class T, detail::IteratorType it_type >
  bool TreeIterator< T, it_type >::operator==(const TreeIterator& rhs) const
  {
    return node_ == rhs.node_ && is_first_ == rhs.is_first_;
  }

  template< class T, detail::IteratorType it_type >
  bool TreeIterator< T, it_type >::operator!=(const TreeIterator& rhs) const
  {
    return !(*this == rhs);
  }

  template< class T, detail::IteratorType it_type >
  TreeIterator< T, it_type >::TreeIterator(TreeNode< T >* node, bool is_first):
    node_(node),
    is_first_(is_first)
  {}

  template< class T, detail::IteratorType it_type >
  TreeNode< T >* TreeIterator< T, it_type >::get_min_node(TreeNode< T >* node)
  {
    TreeNode< T >* result = node;
    while (result && result->left) {
      result = result->left;
    }
    return result;
  }

  template< class T, detail::IteratorType it_type >
  TreeNode< T >* TreeIterator< T, it_type >::get_max_node(TreeNode< T >* node)
  {
    TreeNode< T >* result = node;
    while (result && result->right) {
      result = result->right;
    }
    return result;
  }
}

#endif
