#ifndef TREE_HPP
#define TREE_HPP

#include <functional>
#include <stdexcept>
#include "constIterator.hpp"
#include "iterator.hpp"

namespace mozhegova
{
  template< typename Key, typename T, typename Cmp = std::less< Key > >
  class BiTree
  {
  public:
    using cIter = TreeConstIterator< Key, T, Cmp >;
    using iter = TreeIterator< Key, T, Cmp >;
    using iterPair = std::pair< iter, iter >;
    using cIterPair = std::pair< cIter, cIter >;

    BiTree();
    BiTree(const BiTree< Key, T, Cmp > &);
    BiTree(BiTree< Key, T, Cmp > &&);

    explicit BiTree(std::initializer_list<  std::pair< Key, T > >);
    template< typename InputIt >
    BiTree(InputIt first, InputIt last);

    ~BiTree();

    BiTree< Key, T, Cmp > & operator=(const BiTree< Key, T, Cmp > &);
    BiTree< Key, T, Cmp > & operator=(BiTree< Key, T, Cmp > &&) noexcept;
    T & operator[](const Key &);

    cIter cbegin() const noexcept;
    cIter cend() const noexcept;
    iter begin() const noexcept;
    iter end() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    T & at(const Key &);
    const T & at(const Key &) const;

    void clear() noexcept;
    void swap(BiTree< Key, T, Cmp > &) noexcept;

    iter find(const Key &);
    cIter find(const Key &) const;

    void insert(const std::pair< Key, T > &);
    template< typename InputIt >
    void insert(InputIt first, InputIt last);
    iter erase(iter) noexcept;
    size_t erase(const Key &) noexcept;
    void erase(iter first, iter last);

    cIterPair equal_range(const Key &) const noexcept;
    iterPair equal_range(const Key &) noexcept;
    iter lower_bound(const Key &) noexcept;
    cIter lower_bound(const Key &) const noexcept;
    iter upper_bound(const Key &) noexcept;
    cIter upper_bound(const Key &) const noexcept;
    size_t count(const Key &) const noexcept;
  private:
    using node = detail::TreeNode< Key, T >;

    node * root_;
    node * fakeLeaf_;
    size_t size_;
    Cmp cmp_;

    void clearTree(node *) noexcept;
    node * insertTree(node *, const std::pair< Key, T > &);
    node * eraseTree(node *, const Key &) noexcept;
    node * rotateRight(node *) noexcept;
    node * rotateLeft(node *) noexcept;
    node * balance(node *) noexcept;
    node * findMax(node *) const noexcept;
    node * findMin(node *) const noexcept;
    int getBalanceFactor(node *) noexcept;
    void updateHeight(node *) noexcept;
  };

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp >::BiTree():
    root_(nullptr),
    fakeLeaf_(new node{}),
    size_(0)
  {
    try
    {
      cmp_ = Cmp{};
    }
    catch (...)
    {
      delete fakeLeaf_;
      throw;
    }
  }

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp >::BiTree(const BiTree< Key, T, Cmp > & other):
    BiTree()
  {
    cmp_ = other.cmp_;
    for (auto it = other.cbegin(); it != other.cend(); ++it)
    {
      insert(*it);
    }
  }

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp >::BiTree(BiTree< Key, T, Cmp > && other):
    root_(other.root_),
    fakeLeaf_(other.fakeLeaf_),
    size_(other.size_),
    cmp_(other.cmp_)
  {
    other.root_ = nullptr;
    other.fakeLeaf_ = nullptr;
    size_ = 0;
  }

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp >::BiTree(std::initializer_list< std::pair< Key, T > > il):
    BiTree()
  {
    for (auto it = il.begin(); it != il.end(); ++it)
    {
      insert(*it);
    }
  }

  template< typename Key, typename T, typename Cmp >
  template< typename InputIt >
  BiTree< Key, T, Cmp >::BiTree(InputIt first, InputIt last):
    BiTree()
  {
    for (auto it = first; it != last; ++it)
    {
      insert(*it);
    }
  }

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp >::~BiTree()
  {
    clear();
    delete fakeLeaf_;
  }

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp > & BiTree< Key, T, Cmp >::operator=(const BiTree< Key, T, Cmp > & other)
  {
    if (this != std::addressof(other))
    {
      BiTree< Key, T, Cmp > temp(other);
      swap(temp);
    }
    return *this;
  }

  template< typename Key, typename T, typename Cmp >
  BiTree< Key, T, Cmp > & BiTree< Key, T, Cmp >::operator=(BiTree< Key, T, Cmp > && other) noexcept
  {
    if (this != std::addressof(other))
    {
      BiTree< Key, T, Cmp > temp(std::move(other));
      swap(temp);
    }
    return *this;
  }

  template< typename Key, typename T, typename Cmp >
  T & BiTree< Key, T, Cmp >::operator[](const Key & key)
  {
    iter it = find(key);
    if (it == end())
    {
      insert(std::make_pair(key, T()));
      it = find(key);
    }
    return const_cast< T & >(it->second);
  }

  template< typename Key, typename T, typename Cmp >
  T & BiTree< Key, T, Cmp >::at(const Key & key)
  {
    iter it = find(key);
    if (it == end())
    {
      throw std::out_of_range("<INVALID COMMAND>");
    }
    return const_cast< T & >(it->second);
  }

  template< typename Key, typename T, typename Cmp >
  const T & BiTree< Key, T, Cmp >::at(const Key & key) const
  {
    cIter it = find(key);
    if (it == cend())
    {
      throw std::out_of_range("<INVALID COMMAND>");
    }
    return it->second;
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
    node * temp = root_;
    return cIter(findMin(temp));
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::cIter BiTree< Key, T, Cmp >::cend() const noexcept
  {
    return root_ ? cIter(root_->parent) : cIter(root_);
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::iter BiTree< Key, T, Cmp >::begin() const noexcept
  {
    node * temp = root_;
    return iter(findMin(temp));
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::iter BiTree< Key, T, Cmp >::end() const noexcept
  {
    return root_ ? iter(root_->parent) : iter(root_);
  }

  template< typename Key, typename T, typename Cmp >
  void BiTree< Key, T, Cmp >::clear() noexcept
  {
    clearTree(root_);
    size_ = 0;
  }

  template< typename Key, typename T, typename Cmp >
  void BiTree< Key, T, Cmp >::clearTree(node * root) noexcept
  {
    if (!root || root == fakeLeaf_)
    {
      return;
    }
    clearTree(root->left);
    clearTree(root->right);
    delete root;
  }

  template< typename Key, typename T, typename Cmp >
  void BiTree< Key, T, Cmp >::swap(BiTree< Key, T, Cmp > & rhs) noexcept
  {
    std::swap(root_, rhs.root_);
    std::swap(fakeLeaf_, rhs.fakeLeaf_);
    std::swap(size_, rhs.size_);
    std::swap(cmp_, rhs.cmp_);
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::iter BiTree< Key, T, Cmp >::find(const Key & key)
  {
    node * temp = root_;
    while (temp && temp != fakeLeaf_)
    {
      if (cmp_(temp->data.first, key))
      {
        temp = temp->right;
      }
      else if (cmp_(key, temp->data.first))
      {
        temp = temp->left;
      }
      else
      {
        return iter(temp);
      }
    }
    return end();
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::cIter BiTree< Key, T, Cmp >::find(const Key & key) const
  {
    node * temp = root_;
    while (temp && temp != fakeLeaf_)
    {
      if (cmp_(temp->data.first, key))
      {
        temp = temp->right;
      }
      else if (cmp_(key, temp->data.first))
      {
        temp = temp->left;
      }
      else
      {
        return cIter(temp);
      }
    }
    return cend();
  }

  template< typename Key, typename T, typename Cmp >
  void BiTree< Key, T, Cmp >::insert(const std::pair< Key, T > & val)
  {
    root_ = insertTree(root_, val);
    size_++;
  }

  template< typename Key, typename T, typename Cmp >
  template< typename InputIt >
  void BiTree< Key, T, Cmp >::insert(InputIt first, InputIt last)
  {
    BiTree< Key, T, Cmp > temp(*this);
    for (auto it = first; it != last; ++it)
    {
      temp.insert(*it);
    }
    swap(temp);
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::node * BiTree< Key, T, Cmp >::insertTree(node * root, const std::pair< Key, T > & val)
  {
    if (!root || root == fakeLeaf_)
    {
      return new node(val, fakeLeaf_);
    }
    else if (cmp_(root->data.first, val.first))
    {
      root->right = insertTree(root->right, val);
      root->right->parent = root;
    }
    else if (cmp_(val.first, root->data.first))
    {
      root->left = insertTree(root->left, val);
      root->left->parent = root;
    }
    else
    {
      return root;
    }
    updateHeight(root);
    return balance(root);
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::iter BiTree< Key, T, Cmp >::erase(iter it) noexcept
  {
    iter result = it;
    ++result;
    root_ = eraseTree(root_, it->first);
    size_--;
    return result;
  }

  template< typename Key, typename T, typename Cmp >
  size_t BiTree< Key, T, Cmp >::erase(const Key & key) noexcept
  {
    size_t k = size_;
    erase(find(key));
    return size_ - k;
  }

  template< typename Key, typename T, typename Cmp >
  void BiTree< Key, T, Cmp >::erase(iter first, iter last)
  {
    for (auto it = first; it != last;)
    {
      it = erase(it);
    }
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::node * BiTree< Key, T, Cmp >::eraseTree(node * root, const Key & key) noexcept
  {
    if (!root)
    {
      return nullptr;
    }
    if (root == fakeLeaf_)
    {
      return fakeLeaf_;
    }
    if (cmp_(root->data.first, key))
    {
      root->right = eraseTree(root->right, key);
    }
    else if (cmp_(key, root->data.first))
    {
      root->left = eraseTree(root->left, key);
    }
    else
    {
      if (root->left != fakeLeaf_)
      {
        node * temp = findMax(root->left);
        root->data = temp->data;
        root->left = eraseTree(root->left, temp->data.first);
      }
      else if (root->right != fakeLeaf_)
      {
        node * temp = findMin(root->right);
        root->data = temp->data;
        root->right = eraseTree(root->right, temp->data.first);
      }
      else
      {
        delete root;
        return fakeLeaf_;
      }
    }
    updateHeight(root);
    return balance(root);
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::iter BiTree< Key, T, Cmp >::lower_bound(const Key & key) noexcept
  {
    node * temp = root_;
    node * result = fakeLeaf_;
    while (temp && temp != fakeLeaf_)
    {
      if (cmp_(temp->data.first, key))
      {
        temp = temp->right;
      }
      else
      {
        result = temp;
        temp = temp->left;
      }
    }
    return iter(result);
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::cIter BiTree< Key, T, Cmp >::lower_bound(const Key & key) const noexcept
  {
    node * temp = root_;
    node * result = fakeLeaf_;
    while (temp && temp != fakeLeaf_)
    {
      if (cmp_(temp->data.first, key))
      {
        temp = temp->right;
      }
      else
      {
        result = temp;
        temp = temp->left;
      }
    }
    return cIter(result);
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::iter BiTree< Key, T, Cmp >::upper_bound(const Key & key) noexcept
  {
    node * temp = root_;
    node * result = fakeLeaf_;
    while (temp && temp != fakeLeaf_)
    {
      if (cmp_(key, temp->data.first))
      {
        result = temp;
        temp = temp->left;
      }
      else
      {
        temp = temp->right;
      }
    }
    return iter(result);
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::cIter BiTree< Key, T, Cmp >::upper_bound(const Key & key) const noexcept
  {
    node * temp = root_;
    node * result = fakeLeaf_;
    while (temp && temp != fakeLeaf_)
    {
      if (cmp_(key, temp->data.first))
      {
        result = temp;
        temp = temp->left;
      }
      else
      {
        temp = temp->right;
      }
    }
    return cIter(result);
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::cIterPair BiTree< Key, T, Cmp >::equal_range(const Key & key) const noexcept
  {
    node * temp = root_;
    node * lowBound = fakeLeaf_;
    node * uppBound = fakeLeaf_;
    while (temp && temp != fakeLeaf_)
    {
      if (cmp_(key, temp->data.first))
      {
        uppBound = temp;
        temp = temp->left;
      }
      else if (cmp_(temp->data.first, key))
      {
        lowBound = temp;
        temp = temp->right;
      }
      else
      {
        lowBound = temp;
        uppBound = temp->right;
        break;
      }
    }
    return {cIter(lowBound), cIter(uppBound)};
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::iterPair BiTree< Key, T, Cmp >::equal_range(const Key & key) noexcept
  {
    node * temp = root_;
    node * lowBound = fakeLeaf_;
    node * uppBound = fakeLeaf_;
    while (temp && temp != fakeLeaf_)
    {
      if (cmp_(key, temp->data.first))
      {
        uppBound = temp;
        temp = temp->left;
      }
      else if (cmp_(temp->data.first, key))
      {
        lowBound = temp;
        temp = temp->right;
      }
      else
      {
        lowBound = temp;
        uppBound = temp->right;
        break;
      }
    }
    return {iter(lowBound), iter(uppBound)};
  }

  template< typename Key, typename T, typename Cmp >
  size_t BiTree< Key, T, Cmp >::count(const Key & key) const noexcept
  {
    cIter it = find(key);
    if (it == cend())
    {
      return 0;
    }
    return 1;
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::node * BiTree< Key, T, Cmp >::findMax(node * root) const noexcept
  {
    while (root && root->right != fakeLeaf_)
    {
      root = root->right;
    }
    return root;
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::node * BiTree< Key, T, Cmp >::findMin(node * root) const noexcept
  {
    while (root && root->left != fakeLeaf_)
    {
      root = root->left;
    }
    return root;
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::node * BiTree< Key, T, Cmp >::rotateRight(node * root) noexcept
  {
    if (!root || root->left == fakeLeaf_)
    {
      return root;
    }
    node * newRoot = root->left;
    node * temp = newRoot->right;
    root->left = temp;
    temp->parent = root;
    newRoot->right = root;
    newRoot->parent = root->parent;
    root->parent = newRoot;
    if (newRoot->parent)
    {
      if (newRoot->parent->left == root)
      {
        newRoot->parent->left = newRoot;
      }
      else
      {
        newRoot->parent->right = newRoot;
      }
    }
    return newRoot;
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::node * BiTree< Key, T, Cmp >::rotateLeft(node * root) noexcept
  {
    if (!root || root->right == fakeLeaf_)
    {
      return root;
    }
    node * newRoot = root->right;
    node * temp = newRoot->left;
    root->right = temp;
    temp->parent = root;
    newRoot->left = root;
    newRoot->parent = root->parent;
    root->parent = newRoot;
    if (newRoot->parent)
    {
      if (newRoot->parent->left == root)
      {
        newRoot->parent->left = newRoot;
      }
      else
      {
        newRoot->parent->right = newRoot;
      }
    }
    return newRoot;
  }

  template< typename Key, typename T, typename Cmp >
  int BiTree< Key, T, Cmp >::getBalanceFactor(node * root) noexcept
  {
    return root->left->h - root->right->h;
  }

  template< typename Key, typename T, typename Cmp >
  typename BiTree< Key, T, Cmp >::node * BiTree< Key, T, Cmp >::balance(node * root) noexcept
  {
    int k = getBalanceFactor(root);
    if (k > 1)
    {
      if (getBalanceFactor(root->left) < 0)
      {
        root->left = rotateLeft(root->left);
      }
      root = rotateRight(root);
    }
    if (k < -1)
    {
      if (getBalanceFactor(root->right) > 0)
      {
        root->right = rotateRight(root->right);
      }
      root = rotateLeft(root);
    }
    return root;
  }

  template< typename Key, typename T, typename Cmp >
  void BiTree< Key, T, Cmp >::updateHeight(node * root) noexcept
  {
    root->h = 1 + std::max(root->left->h, root->right->h);
  }
}

#endif
