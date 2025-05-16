#ifndef TREE_HPP
#define TREE_HPP

#include "constIterator.hpp"
#include "iterator.hpp"

namespace mozhegova
{
  template< typename Key, typename T, typename Cmp >
  class BiTree
  {
  public:
    using iter = TreeConstIterator< Key, T, Cmp >;
    using cIter = TreeIterator< Key, T, Cmp >;

    BiTree();
    BiTree(const BiTree< Key, T, Cmp > &);
    BiTree(BiTree< Key, T, Cmp > &&);
    ~BiTree();

    T & operator[](const Key &);
    T & operator[](const Key &&);

    cIter cbegin() const noexcept;
    cIter cend() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    T & at(const Key &);
    const T & at(const Key &) const;

    void clear() noexcept;
    void swap(BiTree &) noexcept;

    iter find(const Key &);
    cIter find(const Key &) const;
  private:
    detail::TreeNode< Key, T > * root_;
    size_t size_;
    Cmp cmp_;
  };

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp >::BiTree():
    root_(nullptr),
    size_(0),
    cmp_(Cmp())
  {}

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp >::BiTree(const BiTree< Key, T, Cmp > &)
  {

  }

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp >::BiTree(BiTree< Key, T, Cmp > &&)
  {

  }

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp >::~BiTree()
  {

  }

  template< typename Key, typename T, typename Cmp >
  size_t BiTree< Key, T, Cmp >::size() const noexcept
  {
    return size_;
  }

  template< typename Key, typename T, typename Cmp >
  bool BiTree< Key, T, Cmp >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::cIter BiTree< Key, T, Cmp >::cbegin() const noexcept
  {
    detail::TreeNode< Key, T > * temp = root_;
    while (temp->left)
    {
      temp = temp->left;
    }
    return cIter(temp);
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::cIter BiTree< Key, T, Cmp >::cend() const noexcept
  {
    detail::TreeNode< Key, T > * temp = root_;
    while (temp->right)
    {
      temp = temp->right;
    }
    return cIter(temp->right);
  }
}

#endif
