#ifndef TREE_ITERATORS_HPP
#define TREE_ITERATORS_HPP

#include <cassert>
#include <functional>

namespace zholobov {

  template < typename Node >
  class TreeConstIterator;

  template < typename Node >
  class TreeIterator {
    template < typename, typename, typename >
    friend class Tree;

    friend class TreeConstIterator< Node >;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename Node::value_type;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    TreeIterator();
    TreeIterator& operator++() noexcept;
    TreeIterator operator++(int) noexcept;
    TreeIterator& operator--() noexcept;
    TreeIterator operator--(int) noexcept;
    reference operator*() const noexcept;
    pointer operator->() const noexcept;
    bool operator==(const TreeIterator& other) const noexcept;
    bool operator!=(const TreeIterator& other) const noexcept;

  private:
    TreeIterator(Node* node);
    Node* node_;
  };

  template < typename Node >
  class TreeConstIterator {
    template < typename, typename, typename >
    friend class Tree;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename Node::value_type;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using this_t = TreeConstIterator< Node >;

    TreeConstIterator();
    TreeConstIterator(const TreeIterator< Node >& other);
    TreeConstIterator& operator++() noexcept;
    TreeConstIterator operator++(int) noexcept;
    TreeConstIterator& operator--() noexcept;
    TreeConstIterator operator--(int) noexcept;
    reference operator*() const noexcept;
    pointer operator->() const noexcept;
    bool operator==(const TreeConstIterator& other) const noexcept;
    bool operator!=(const TreeConstIterator& other) const noexcept;

  private:
    TreeConstIterator(Node* node);
    Node* node_;
  };

}

template < typename Node >
zholobov::TreeIterator< Node >::TreeIterator():
  node_(nullptr)
{}

template < typename Node >
zholobov::TreeIterator< Node >::TreeIterator(Node* node):
  node_(node)
{}

template < typename Node >
zholobov::TreeIterator< Node >& zholobov::TreeIterator< Node >::operator++() noexcept
{
  assert(node_ != nullptr);
  if (node_->right != nullptr) {
    node_ = node_->right;
    while (node_->left != nullptr) {
      node_ = node_->left;
    }
  } else {
    while ((node_->parent != nullptr) && (node_ == node_->parent->right)) {
      node_ = node_->parent;
    }
    if (node_->parent != nullptr) {
      node_ = node_->parent;
    }
  }
  return *this;
}

template < typename Node >
zholobov::TreeIterator< Node > zholobov::TreeIterator< Node >::operator++(int) noexcept
{
  assert(node_ != nullptr);
  zholobov::TreeIterator< Node > temp(*this);
  ++(*this);
  return temp;
}

template < typename Node >
zholobov::TreeIterator< Node >& zholobov::TreeIterator< Node >::operator--() noexcept
{
  assert(node_ != nullptr);
  if (node_->parent == nullptr) {
    node_ = node_->left;
    if (node_ != nullptr) {
      while (node_->right != nullptr) {
        node_ = node_->right;
      }
    }
  } else if (node_->left) {
    node_ = node_->left;
    while (node_->right != nullptr) {
      node_ = node_->right;
    }
  } else {
    while ((node_->parent != nullptr) && (node_ == node_->parent->left)) {
      node_ = node_->parent;
    }
    node_ = node_->parent;
  }
  return *this;
}

template < typename Node >
zholobov::TreeIterator< Node > zholobov::TreeIterator< Node >::operator--(int) noexcept
{
  assert(node_ != nullptr);
  zholobov::TreeIterator< Node > temp(*this);
  --(*this);
  return temp;
}

template < typename Node >
typename zholobov::TreeIterator< Node >::reference zholobov::TreeIterator< Node >::operator*() const noexcept
{
  assert(node_ != nullptr);
  return node_->data;
}

template < typename Node >
typename zholobov::TreeIterator< Node >::pointer zholobov::TreeIterator< Node >::operator->() const noexcept
{
  assert(node_ != nullptr);
  return std::addressof(node_->data);
}

template < typename Node >
bool zholobov::TreeIterator< Node >::operator==(const TreeIterator& other) const noexcept
{
  return node_ == other.node_;
}

template < typename Node >
bool zholobov::TreeIterator< Node >::operator!=(const TreeIterator& other) const noexcept
{
  return !(*this == other);
}

template < typename Node >
zholobov::TreeConstIterator< Node >::TreeConstIterator():
  node_(nullptr)
{}

template < typename Node >
zholobov::TreeConstIterator< Node >::TreeConstIterator(const zholobov::TreeIterator< Node >& other):
  node_(other.node_)
{}

template < typename Node >
zholobov::TreeConstIterator< Node >::TreeConstIterator(Node* node):
  node_(node)
{}

template < typename Node >
zholobov::TreeConstIterator< Node >& zholobov::TreeConstIterator< Node >::operator++() noexcept
{
  assert(node_ != nullptr);
  if (node_->right != nullptr) {
    node_ = node_->right;
    while (node_->left != nullptr) {
      node_ = node_->left;
    }
  } else {
    while ((node_->parent != nullptr) && (node_ == node_->parent->right)) {
      node_ = node_->parent;
    }
    if (node_->parent != nullptr) {
      node_ = node_->parent;
    }
  }
  return *this;
}

template < typename Node >
zholobov::TreeConstIterator< Node > zholobov::TreeConstIterator< Node >::operator++(int) noexcept
{
  assert(node_ != nullptr);
  zholobov::TreeConstIterator< Node > temp(*this);
  ++(*this);
  return temp;
}

template < typename Node >
zholobov::TreeConstIterator< Node >& zholobov::TreeConstIterator< Node >::operator--() noexcept
{
  assert(node_ != nullptr);
  if (node_->parent == nullptr) {
    node_ = node_->left;
    if (node_ != nullptr) {
      while (node_->right != nullptr) {
        node_ = node_->right;
      }
    }
  } else if (node_->left != nullptr) {
    node_ = node_->left;
    while (node_->right != nullptr) {
      node_ = node_->right;
    }
  } else {
    while ((node_->parent != nullptr) && (node_ == node_->parent->left)) {
      node_ = node_->parent;
    }
    node_ = node_->parent;
  }
  return *this;
}

template < typename Node >
zholobov::TreeConstIterator< Node > zholobov::TreeConstIterator< Node >::operator--(int) noexcept
{
  assert(node_ != nullptr);
  zholobov::TreeConstIterator< Node > temp(*this);
  --(*this);
  return temp;
}

template < typename Node >
typename zholobov::TreeConstIterator< Node >::reference zholobov::TreeConstIterator< Node >::operator*() const noexcept
{
  assert(node_ != nullptr);
  return node_->data;
}

template < typename Node >
typename zholobov::TreeConstIterator< Node >::pointer zholobov::TreeConstIterator< Node >::operator->() const noexcept
{
  assert(node_ != nullptr);
  return std::addressof(node_->data);
}

template < typename Node >
bool zholobov::TreeConstIterator< Node >::operator==(const TreeConstIterator& other) const noexcept
{
  return node_ == other.node_;
}

template < typename Node >
bool zholobov::TreeConstIterator< Node >::operator!=(const TreeConstIterator& other) const noexcept
{
  return !(*this == other);
}

#endif
