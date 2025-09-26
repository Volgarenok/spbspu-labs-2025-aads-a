#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <iostream>
#include <functional>
#include <cstddef>
#include <iterator>
#include <cassert>
#include "stack.hpp"
#include "queue.hpp"

namespace petrov
{
  template< typename K, typename T >
  struct AVLTreeNode
  {
    template< typename A, typename B, typename C >
    friend struct AVLTree;
  public:
    using val_t = std::pair< const K, T >;
    using this_t = AVLTreeNode< K, T >;
    val_t data;
    this_t * left;
    this_t * right;
    this_t * parent;
    template< class... Args >
    AVLTreeNode(this_t * l, this_t * r, this_t * p, Args && ... args);
  private:
    int height_;
    void setHeight();
  };

  template< typename K, typename T, typename Cmp = std::less< K > >
  struct ConstAVLTreeIterator: std::iterator< std::bidirectional_iterator_tag, T >
  {
    template< typename A, typename B, typename C >
    friend struct AVLTree;
  public:
    using val_t = std::pair< const K, T >;
    using node_t = AVLTreeNode< K, T >;
    using this_t = ConstAVLTreeIterator< K, T, Cmp >;
    ConstAVLTreeIterator();
    ConstAVLTreeIterator(const this_t & rhs) = default;
    ~ConstAVLTreeIterator() = default;
    this_t & operator=(const this_t & rhs) = default;
    this_t & operator++();
    this_t operator++(int);
    this_t & operator--();
    this_t operator--(int);
    const T & operator*() const;
    const val_t * operator->() const;
    bool operator==(const this_t & rhs) const;
    bool operator!=(const this_t & rhs) const;
  private:
    node_t * node_;
    explicit ConstAVLTreeIterator(node_t * node);
  };

  template< typename K, typename T, typename Cmp >
  struct AVLTreeIterator: std::iterator< std::bidirectional_iterator_tag, T >
  {
    template< typename A, typename B, typename C >
    friend struct AVLTree;
  public:
    using val_t = std::pair< const K, T >;
    using node_t = AVLTreeNode< K, T >;
    using this_t = AVLTreeIterator< K, T, Cmp >;
    AVLTreeIterator();
    AVLTreeIterator(const this_t & rhs) = default;
    ~AVLTreeIterator() = default;
    this_t & operator=(const this_t & rhs) = default;
    this_t & operator++();
    this_t operator++(int);
    this_t & operator--();
    this_t operator--(int);
    T & operator*();
    val_t * operator->();
    bool operator==(const this_t & rhs) const;
    bool operator!=(const this_t & rhs) const;
  private:
    node_t * node_;
    explicit AVLTreeIterator(node_t * node);
  };

  template< typename K, typename T, typename Cmp = std::less< K > >
  struct AVLTree
  {
    using val_t = std::pair< const K, T >;
    using this_t = AVLTree< K, T, Cmp >;
    using node_t = AVLTreeNode< K, T >;

    using it_t = AVLTreeIterator< K, T, Cmp >;
    using const_it_t = ConstAVLTreeIterator< K, T, Cmp >;

    using pair_it_t = std::pair< it_t, it_t >;
    using pair_const_it_t = std::pair< const_it_t, const_it_t >;

    AVLTree();
    AVLTree(const this_t & rhs);
    AVLTree(this_t && rhs);
    template< class InputIterator >
    AVLTree(InputIterator first, InputIterator last);
    AVLTree(std::initializer_list< val_t > il);
    ~AVLTree();
    this_t & operator=(const this_t & rhs);
    this_t & operator=(this_t && rhs);
    T & operator[](const K & key);
    T & operator[](K && key);

    const_it_t cbegin() const;
    const_it_t cend() const;
    it_t begin();
    it_t end();

    const_it_t crbegin() const;
    const_it_t crend() const;
    it_t rbegin();
    it_t rend();

    bool empty() const noexcept;
    size_t size() const noexcept;

    std::pair< it_t, bool > insert(const val_t & val);
    std::pair< it_t, bool > insert(val_t && val);
    it_t insert(const_it_t hint, const val_t & val);
    template< class InputIterator >
    void insert(InputIterator first, InputIterator last);
    template< class... Args >
    std::pair< it_t, bool > emplace(Args && ... args);
    template< class... Args >
    it_t emplace_hint(const_it_t hint, Args && ... args);

    it_t erase(it_t position);
    it_t erase(const_it_t position);
    size_t erase(const K & key);
    it_t erase(const_it_t first, const_it_t last);

    it_t find(const K & key);
    const_it_t find(const K & key) const;
    T & at(const K & key);
    const T & at(const K & key) const;
    pair_it_t equal_range(const K & key);
    pair_const_it_t equal_range(const K & key) const;
    it_t lower_bound(const K & key);
    const_it_t lower_bound(const K & key) const;
    it_t upper_bound(const K & key);
    const_it_t upper_bound(const K & key) const;
    size_t count(const K & key) const;

    void clear() noexcept;
    void swap(this_t & rhs) noexcept;

    template< typename F >
    F traverse_lnr(F f);
    template< typename F >
    F traverse_rnl(F f);
    template< typename F >
    F traverse_breadth(F f);
    template< typename F >
    F traverse_lnr(F f) const;
    template< typename F >
    F traverse_rnl(F f) const;
    template< typename F >
    F traverse_breadth(F f) const;
  private:
    node_t * root_;
    size_t size_;
    void balance(node_t * node);
    void leftRotate(node_t * node);
    void rightRotate(node_t * node);
    bool isBalanced(node_t * node);
    node_t * lazyFind(node_t * temp, const K & key) const;
    template< class InputIterator, typename UnPred >
    InputIterator findIf(InputIterator first, InputIterator last, const K & key, UnPred p) const;
    void deepRotation(node_t * node);
    void upwardBalancing(node_t * node);
    void eraseImpl(node_t * node);
    template< class... Args >
    std::pair< it_t, bool > emplaceImpl(node_t * subroot, Args && ... args);
  };

  template< typename K, typename T >
  template< class... Args >
  AVLTreeNode< K, T >::AVLTreeNode(this_t * l, this_t * r, this_t * p, Args && ... args):
    data{val_t(std::forward< Args >(args)...)},
    left(l),
    right(r),
    parent(p)
  {
    setHeight();
  }

  template< typename K, typename T >
  void AVLTreeNode< K, T >::setHeight()
  {
    if (left && right)
    {
      height_ = std::max(left->height_, right->height_) + 1;
    }
    else if (!left && !right)
    {
      height_ = 1;
    }
    else
    {
      if (!left)
      {
        height_ = right->height_ + 1;
      }
      else
      {
        height_ = left->height_ + 1;
      }
    }
  }

  template< typename K, typename T, typename Cmp >
  ConstAVLTreeIterator< K, T, Cmp >::ConstAVLTreeIterator():
    node_(nullptr)
  {}

  template< typename K, typename T, typename Cmp >
  typename ConstAVLTreeIterator< K, T, Cmp >::this_t & ConstAVLTreeIterator< K, T, Cmp >::operator++()
  {
    assert(node_ != nullptr);
    if (node_->right)
    {
      node_ = node_->right;
      while (node_->left)
      {
        node_ = node_->left;
      }
    }
    else if (!node_->parent || Cmp{}(node_->data.first, node_->parent->data.first))
    {
      node_ = node_->parent;
    }
    else
    {
      while (node_->parent && Cmp{}(node_->parent->data.first, node_->data.first))
      {
        node_ = node_->parent;
      }
      node_ = node_->parent;
    }
    return *this;
  }

  template< typename K, typename T, typename Cmp >
  typename ConstAVLTreeIterator< K, T, Cmp >::this_t ConstAVLTreeIterator< K, T, Cmp >::operator++(int)
  {
    assert(node_ != nullptr);
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename K, typename T, typename Cmp >
  typename ConstAVLTreeIterator< K, T, Cmp >::this_t & ConstAVLTreeIterator< K, T, Cmp >::operator--()
  {
    assert(node_ != nullptr);
    if (node_->left)
    {
      node_ = node_->left;
      while (node_->right)
      {
        node_ = node_->right;
      }
    }
    else if (!node_->parent || Cmp{}(node_->parent->data.first, node_->data.first))
    {
      node_ = node_->parent;
    }
    else
    {
      while (node_->parent && Cmp{}(node_->data.first, node_->parent->data.first))
      {
        node_ = node_->parent;
      }
      node_ = node_->parent;
    }
    return *this;
  }

  template< typename K, typename T, typename Cmp >
  typename ConstAVLTreeIterator< K, T, Cmp >::this_t ConstAVLTreeIterator< K, T, Cmp >::operator--(int)
  {
    assert(node_ != nullptr);
    this_t result(*this);
    --(*this);
    return result;
  }

  template< typename K, typename T, typename Cmp >
  const T & ConstAVLTreeIterator< K, T, Cmp >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->data.second;
  }

  template< typename K, typename T, typename Cmp >
  const typename ConstAVLTreeIterator< K, T, Cmp >::val_t * ConstAVLTreeIterator< K, T, Cmp >::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename K, typename T, typename Cmp >
  bool ConstAVLTreeIterator< K, T, Cmp >::operator==(const this_t & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename K, typename T, typename Cmp >
  bool ConstAVLTreeIterator< K, T, Cmp >::operator!=(const this_t & rhs) const
  {
    return !(*this == rhs);
  }

  template< typename K, typename T, typename Cmp >
  ConstAVLTreeIterator< K, T, Cmp >::ConstAVLTreeIterator(node_t * node):
    node_(node)
  {}

  template< typename K, typename T, typename Cmp >
  AVLTreeIterator< K, T, Cmp >::AVLTreeIterator():
    node_(nullptr)
  {}

  template< typename K, typename T, typename Cmp >
  typename AVLTreeIterator< K, T, Cmp >::this_t & AVLTreeIterator< K, T, Cmp >::operator++()
  {
    assert(node_ != nullptr);
    if (node_->right)
    {
      node_ = node_->right;
      while (node_->left)
      {
        node_ = node_->left;
      }
    }
    else if (!node_->parent || Cmp{}(node_->data.first, node_->parent->data.first))
    {
      node_ = node_->parent;
    }
    else
    {
      while (node_->parent && Cmp{}(node_->parent->data.first, node_->data.first))
      {
        node_ = node_->parent;
      }
      node_ = node_->parent;
    }
    return *this;
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTreeIterator< K, T, Cmp >::this_t AVLTreeIterator< K, T, Cmp >::operator++(int)
  {
    assert(node_ != nullptr);
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTreeIterator< K, T, Cmp >::this_t & AVLTreeIterator< K, T, Cmp >::operator--()
  {
    assert(node_ != nullptr);
    if (node_->left)
    {
      node_ = node_->left;
      while (node_->right)
      {
        node_ = node_->right;
      }
    }
    else if (!node_->parent || Cmp{}(node_->parent->data.first, node_->data.first))
    {
      node_ = node_->parent;
    }
    else
    {
      while (node_->parent && Cmp{}(node_->data.first, node_->parent->data.first))
      {
        node_ = node_->parent;
      }
      node_ = node_->parent;
    }
    return *this;
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTreeIterator< K, T, Cmp >::this_t AVLTreeIterator< K, T, Cmp >::operator--(int)
  {
    assert(node_ != nullptr);
    this_t result(*this);
    --(*this);
    return result;
  }

  template< typename K, typename T, typename Cmp >
  T & AVLTreeIterator< K, T, Cmp >::operator*()
  {
    assert(node_ != nullptr);
    return node_->data.second;
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTreeIterator< K, T, Cmp >::val_t * AVLTreeIterator< K, T, Cmp >::operator->()
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename K, typename T, typename Cmp >
  bool AVLTreeIterator< K, T, Cmp >::operator==(const this_t & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename K, typename T, typename Cmp >
  bool AVLTreeIterator< K, T, Cmp >::operator!=(const this_t & rhs) const
  {
    return !(*this == rhs);
  }

  template< typename K, typename T, typename Cmp >
  AVLTreeIterator< K, T, Cmp >::AVLTreeIterator(node_t * node):
    node_(node)
  {}

  template< typename K, typename T, typename Cmp >
  AVLTree< K, T, Cmp >::AVLTree():
    root_(nullptr),
    size_(0)
  {}

  template< typename K, typename T, typename Cmp >
  AVLTree< K, T, Cmp >::AVLTree(const this_t & rhs):
    root_(nullptr),
    size_(0)
  {
    try
    {
      insert(rhs.cbegin(), rhs.cend());
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template< typename K, typename T, typename Cmp >
  AVLTree< K, T, Cmp >::AVLTree(this_t && rhs):
    root_(rhs.root_),
    size_(rhs.size_)
  {
    rhs.root_ = nullptr;
  }

  template< typename K, typename T, typename Cmp >
  template< class InputIterator >
  AVLTree< K, T, Cmp >::AVLTree(InputIterator first, InputIterator last):
    root_(nullptr),
    size_(0)
  {
    try
    {
      insert(first, last);
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template< typename K, typename T, typename Cmp >
  AVLTree< K, T, Cmp >::AVLTree(std::initializer_list< val_t > il):
    AVLTree(il.begin(), il.end())
  {}

  template< typename K, typename T, typename Cmp >
  AVLTree< K, T, Cmp >::~AVLTree()
  {
    clear();
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::this_t & AVLTree< K, T, Cmp >::operator=(const this_t & rhs)
  {
    this_t cpy(rhs);
    swap(cpy);
    return *this;
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::this_t & AVLTree< K, T, Cmp >::operator=(this_t && rhs)
  {
    clear();
    root_ = rhs.root_;
    size_ = rhs.size_;
    rhs.root_ = nullptr;
    return *this;
  }

  template< typename K, typename T, typename Cmp >
  T & AVLTree< K, T, Cmp >::operator[](const K & key)
  {
    return (*((this->insert(std::make_pair(key, T()))).first));
  }

  template< typename K, typename T, typename Cmp >
  T & AVLTree< K, T, Cmp >::operator[](K && key)
  {
    return (*((this->insert(std::make_pair(std::move(key), T()))).first));
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::const_it_t AVLTree< K, T, Cmp >::cbegin() const
  {
    auto temp = root_;
    if (empty())
    {
      return const_it_t();
    }
    else
    {
      while (temp->left)
      {
        temp = temp->left;
      }
    }
    return const_it_t(temp);
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::const_it_t AVLTree< K, T, Cmp >::cend() const
  {
    return const_it_t();
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::it_t AVLTree< K, T, Cmp >::begin()
  {
    auto temp = root_;
    if (empty())
    {
      return it_t();
    }
    else
    {
      while (temp->left)
      {
        temp = temp->left;
      }
    }
    return it_t(temp);
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::it_t AVLTree< K, T, Cmp >::end()
  {
    return it_t();
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::const_it_t AVLTree< K, T, Cmp >::crbegin() const
  {
    auto temp = root_;
    if (empty())
    {
      return const_it_t();
    }
    else
    {
      while (temp->right)
      {
        temp = temp->right;
      }
    }
    return const_it_t(temp);
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::const_it_t AVLTree< K, T, Cmp >::crend() const
  {
    return const_it_t();
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::it_t AVLTree< K, T, Cmp >::rbegin()
  {
    auto temp = root_;
    if (empty())
    {
      return it_t();
    }
    else
    {
      while (temp->right)
      {
        temp = temp->right;
      }
    }
    return it_t(temp);
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::it_t AVLTree< K, T, Cmp >::rend()
  {
    return it_t();
  }

  template< typename K, typename T, typename Cmp >
  bool AVLTree< K, T, Cmp >::empty() const noexcept
  {
    return !size_;
  }

  template< typename K, typename T, typename Cmp >
  size_t AVLTree< K, T, Cmp >::size() const noexcept
  {
    return size_;
  }

  template< typename K, typename T, typename Cmp >
  std::pair< AVLTreeIterator< K, T, Cmp >, bool > AVLTree< K, T, Cmp >::insert(const val_t & val)
  {
    return emplace(val);
  }

  template< typename K, typename T, typename Cmp >
  std::pair< AVLTreeIterator< K, T, Cmp >, bool > AVLTree< K, T, Cmp >::insert(val_t && val)
  {
    return emplace(std::move(val));
  }

  template< typename K, typename T, typename Cmp >
  template< class InputIterator >
  void AVLTree< K, T, Cmp >::insert(InputIterator first, InputIterator last)
  {
    for (auto it = first; it != last; ++it)
    {
      insert({ it->first, it->second });
    }
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::it_t AVLTree< K, T, Cmp >::insert(const_it_t hint, const val_t & val)
  {
    return emplace_hint(hint, val);
  }

  template< typename K, typename T, typename Cmp >
  template< class... Args >
  std::pair< AVLTreeIterator< K, T, Cmp >, bool > AVLTree< K, T, Cmp >::emplace(Args && ... args)
  {
    return emplaceImpl(root_, std::forward< Args >(args)...);
  }

  template< typename K, typename T, typename Cmp >
  template< class... Args >
  typename AVLTree< K, T, Cmp >::it_t AVLTree< K, T, Cmp >::emplace_hint(const_it_t hint, Args && ... args)
  {
    if (empty())
    {
      root_ = new node_t{ nullptr, nullptr, nullptr, std::forward< Args >(args)... };
      size_++;
      return it_t(root_);
    }
    std::pair< it_t, bool > ret_val;
    node_t * added = nullptr;
    try
    {
      added = new node_t{ nullptr, nullptr, nullptr, std::forward< Args >(args)... };
      auto begin = cbegin();
      if (hint == begin)
      {
        hint = begin;
        if (Cmp{}(added->data.first, hint.node_->data.first))
        {
          hint.node_->left = added;
          added->parent = hint.node_;
          size_++;
          upwardBalancing(added->parent);
          return it_t(added);
        }
      }
      else if (hint == cend())
      {
        hint = crbegin();
        if (Cmp{}(hint.node_->data.first, added->data.first))
        {
          hint.node_->right = added;
          added->parent = hint.node_;
          size_++;
          upwardBalancing(added->parent);
          return it_t(added);
        }
      }
      if (!Cmp{}(added->data.first, hint.node_->data.first))
      {
        delete added;
        ret_val = emplace(std::forward< Args >(args)...);
      }
      else
      {
        for (auto it = hint; it != crend(); --it)
        {
          if (!Cmp{}(added->data.first, it.node_->data.first))
          {
            delete added;
            ret_val = emplaceImpl(it.node_, std::forward< Args >(args)...);
            break;
          }
        }
      }
    }
    catch (...)
    {
      delete added;
      throw;
    }
    return ret_val.first;
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::it_t AVLTree< K, T, Cmp >::erase(it_t position)
  {
    auto return_it = position;
    ++return_it;
    eraseImpl(position.node_);
    return return_it;
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::it_t AVLTree< K, T, Cmp >::erase(const_it_t position)
  {
    auto return_it = position;
    ++return_it;
    eraseImpl(position.node_);
    return it_t(return_it.node_);
  }

  template< typename K, typename T, typename Cmp >
  size_t AVLTree< K, T, Cmp >::erase(const K & key)
  {
    it_t position = find(key);
    if (position != end())
    {
      erase(position);
      return 1;
    }
    else
    {
      return 0;
    }
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::it_t AVLTree< K, T, Cmp >::erase(const_it_t first, const_it_t last)
  {
    auto it = first;
    while (it != last)
    {
      erase(it++);
    }
    return it_t(last.node_);
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::it_t AVLTree< K, T, Cmp >::find(const K & key)
  {
    node_t * ret_val = lazyFind(root_, key);
    if (ret_val)
    {
      return it_t(ret_val);
    }
    return end();
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp  >::const_it_t AVLTree< K, T, Cmp >::find(const K & key) const
  {
    node_t * ret_val = lazyFind(root_, key);
    if (ret_val)
    {
      return const_it_t(ret_val);
    }
    return cend();
  }

  template< typename K, typename T, typename Cmp >
  T & AVLTree< K, T, Cmp >::at(const K & key)
  {
    return const_cast< T & >(static_cast< const AVLTree * >(this)->at(key));
  }

  template< typename K, typename T, typename Cmp >
  const T & AVLTree< K, T, Cmp >::at(const K & key) const
  {
    node_t * ret_val = lazyFind(root_, key);
    if (ret_val)
    {
      return ret_val->data.second;
    }
    else
    {
      throw std::out_of_range("<INVALID COMMAND>");
    }
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::pair_it_t AVLTree< K, T, Cmp >::equal_range(const K & key)
  {
    return pair_it_t{ lower_bound(key), upper_bound(key) };
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::pair_const_it_t AVLTree< K, T, Cmp >::equal_range(const K & key) const
  {
    return pair_const_it_t{ lower_bound(key), upper_bound(key) };
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::it_t AVLTree< K, T, Cmp >::lower_bound(const K & key)
  {
    using namespace std::placeholders;
    return findIf(begin(), end(), key, std::bind(std::greater_equal< K >{}, _1, std::cref(key)));
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::const_it_t AVLTree< K, T, Cmp >::lower_bound(const K & key) const
  {
    using namespace std::placeholders;
    return findIf(cbegin(), cend(), key, std::bind(std::greater_equal< K >{}, _1, std::cref(key)));
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::it_t AVLTree< K, T, Cmp >::upper_bound(const K & key)
  {
    using namespace std::placeholders;
    return findIf(begin(), end(), key, std::bind(std::greater< K >{}, _1, std::cref(key)));
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::const_it_t AVLTree< K, T, Cmp >::upper_bound(const K & key) const
  {
    using namespace std::placeholders;
    return findIf(cbegin(), cend(), key, std::bind(std::greater< K >{}, _1, std::cref(key)));
  }

  template< typename K, typename T, typename Cmp >
  size_t AVLTree< K, T, Cmp >::count(const K & key) const
  {
    if (lazyFind(root_, key))
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }

  template< typename K, typename T, typename Cmp >
  void AVLTree< K, T, Cmp >::clear() noexcept
  {
    if (root_)
    {
      auto temp = root_;
      while (size_)
      {
        while (temp->left)
        {
          temp = temp->left;
        }
        if (temp->right)
        {
          temp = temp->right;
          continue;
        }
        auto todelete = temp;
        if (todelete->parent)
        {
          temp = todelete->parent;
          if (temp->left == todelete)
          {
            temp->left = nullptr;
          }
          else
          {
            temp->right = nullptr;
          }
        }
        delete todelete;
        size_--;
      }
      root_ = nullptr;
    }
  }

  template< typename K, typename T, typename Cmp >
  void AVLTree< K, T, Cmp >::swap(this_t & rhs) noexcept
  {
    std::swap(root_, rhs.root_);
    std::swap(size_, rhs.size_);
  }

  template< typename K, typename T, typename Cmp >
  template< typename F >
  F AVLTree< K, T, Cmp >::traverse_lnr(F f)
  {
    petrov::Stack< node_t * > stack;
    node_t * node = root_;
    while (!stack.empty() || node)
    {
      if (node)
      {
        stack.push(node);
        node = node->left;
      }
      else
      {
        node = stack.top();
        stack.pop();
        f(node->data);
        node = node->right;
      }
    }
    return f;
  }

  template< typename K, typename T, typename Cmp >
  template< typename F >
  F AVLTree< K, T, Cmp >::traverse_lnr(F f) const
  {
    petrov::Stack< node_t * > stack;
    node_t * node = root_;
    while (!stack.empty() || node)
    {
      if (node)
      {
        stack.push(node);
        node = node->left;
      }
      else
      {
        node = stack.top();
        stack.pop();
        f(node->data);
        node = node->right;
      }
    }
    return f;
  }

  template< typename K, typename T, typename Cmp >
  template< typename F >
  F AVLTree< K, T, Cmp >::traverse_rnl(F f)
  {
    petrov::Stack< node_t * > stack;
    node_t * node = root_;
    while (!stack.empty() || node)
    {
      if (node)
      {
        stack.push(node);
        node = node->right;
      }
      else
      {
        node = stack.top();
        stack.pop();
        f(node->data);
        node = node->left;
      }
    }
    return f;
  }

  template< typename K, typename T, typename Cmp >
  template< typename F >
  F AVLTree< K, T, Cmp >::traverse_rnl(F f) const
  {
    petrov::Stack< node_t * > stack;
    node_t * node = root_;
    while (!stack.empty() || node)
    {
      if (node)
      {
        stack.push(node);
        node = node->right;
      }
      else
      {
        node = stack.top();
        stack.pop();
        f(node->data);
        node = node->left;
      }
    }
    return f;
  }

  template< typename K, typename T, typename Cmp >
  template< typename F >
  F AVLTree< K, T, Cmp >::traverse_breadth(F f)
  {
    petrov::Queue< node_t * > queue;
    node_t * node = root_;
    if (node)
    {
      queue.push(node);
    }
    while (!queue.empty())
    {
      node = queue.front();
      f(node->data);
      queue.pop();
      if (node->left)
      {
        queue.push(node->left);
      }
      if (node->right)
      {
        queue.push(node->right);
      }
    }
    return f;
  }

  template< typename K, typename T, typename Cmp >
  template< typename F >
  F AVLTree< K, T, Cmp >::traverse_breadth(F f) const
  {
    petrov::Queue< node_t * > queue;
    node_t * node = root_;
    if (node)
    {
      queue.push(node);
    }
    while (!queue.empty())
    {
      node = queue.front();
      f(node->data);
      queue.pop();
      if (node->left)
      {
        queue.push(node->left);
      }
      if (node->right)
      {
        queue.push(node->right);
      }
    }
    return f;
  }

  template< typename K, typename T, typename Cmp >
  void AVLTree< K, T, Cmp >::balance(node_t * node)
  {
    if (node->left && node->right)
    {
      if (node->left->height_ < node->right->height_)
      {
        node = node->right;
        if (node->left && node->right && node->left->height_ < node->right->height_)
        {
          leftRotate(node);
        }
        else if (node->left && node->right)
        {
          rightRotate(node->left);
          leftRotate(node->parent);
        }
        else
        {
          deepRotation(node);
        }
      }
      else
      {
        node = node->left;
        if (node->left && node->right && node->right->height_ < node->left->height_)
        {
          rightRotate(node);
        }
        else if (node->left && node->right)
        {
          leftRotate(node->right);
          rightRotate(node->parent);
        }
        else
        {
          deepRotation(node);
        }
      }
    }
    else
    {
      deepRotation(node);
    }
  }

  template< typename K, typename T, typename Cmp >
  void AVLTree< K, T, Cmp >::leftRotate(node_t * node)
  {
    auto grandpa = node->parent;
    auto son = node->left;
    if (grandpa->parent)
    {
      if (grandpa->parent->right == grandpa)
      {
        grandpa->parent->right = node;
      }
      else
      {
        grandpa->parent->left = node;
      }
      node->parent = grandpa->parent;
    }
    else
    {
      root_ = node;
      node->parent = nullptr;
    }
    grandpa->right = son;
    grandpa->parent = node;
    node->left = grandpa;
    if (son)
    {
      son->parent = grandpa;
    }
  }

  template< typename K, typename T, typename Cmp >
  void AVLTree< K, T, Cmp >::rightRotate(node_t * node)
  {
    auto grandpa = node->parent;
    auto son = node->right;
    if (grandpa->parent)
    {
      if (grandpa->parent->right == grandpa)
      {
        grandpa->parent->right = node;
      }
      else
      {
        grandpa->parent->left = node;
      }
      node->parent = grandpa->parent;
    }
    else
    {
      root_ = node;
      node->parent = nullptr;
    }
    grandpa->left = son;
    grandpa->parent = node;
    node->right = grandpa;
    if (son)
    {
      son->parent = grandpa;
    }
  }

  template< typename K, typename T, typename Cmp >
  bool AVLTree< K, T, Cmp >::isBalanced(node_t * node)
  {
    if (node->left && node->right && std::abs(node->left->height_ - node->right->height_) > 1)
    {
      return false;
    }
    else if (!node->left && node->right && node->right->height_ > 1)
    {
      return false;
    }
    else if (!node->right && node->left && node->left->height_ > 1)
    {
      return false;
    }
    else
    {
      return true;
    }
  }

  template< typename K, typename T, typename Cmp >
  typename AVLTree< K, T, Cmp >::node_t * AVLTree< K, T, Cmp >::lazyFind(node_t * temp, const K & key) const
  {
    while (temp)
    {
      if (Cmp{}(key, temp->data.first))
      {
        temp = temp->left;
      }
      else if (Cmp{}(temp->data.first, key))
      {
        temp = temp->right;
      }
      else if (!Cmp{}(key, temp->data.first) && !Cmp{}(temp->data.first, key))
      {
        return temp;
      }
    }
    return nullptr;
  }

  template< typename K, typename T, typename Cmp >
  template< class InputIterator, typename UnPred >
  InputIterator AVLTree< K, T, Cmp >::findIf(InputIterator first, InputIterator last, const K & key, UnPred p) const
  {
    for (auto it = first; it != last; ++it)
    {
      if (p(it->first))
      {
        return it;
      }
    }
    return last;
  }


  template< typename K, typename T, typename Cmp >
  void AVLTree< K, T, Cmp >::deepRotation(node_t * node)
  {
    if (!node->left)
    {
      if (!node->right->left)
      {
        leftRotate(node->right);
      }
      else
      {
        rightRotate(node->right->left);
        leftRotate(node->right);
      }
    }
    else if (!node->right)
    {
      if (!node->left->right)
      {
        rightRotate(node->left);
      }
      else
      {
        leftRotate(node->left->right);
        rightRotate(node->left);
      }
    }
  }

  template< typename K, typename T, typename Cmp >
  void AVLTree< K, T, Cmp >::upwardBalancing(node_t * node)
  {
    while (node)
    {
      node->setHeight();
      if (!isBalanced(node))
      {
        balance(node);
        node->setHeight();
      }
      node = node->parent;
    }
  }

  template< typename K, typename T, typename Cmp >
  void AVLTree< K, T, Cmp >::eraseImpl(node_t * node)
  {
    node_t * balance_node_ptr = nullptr;
    auto temp = node;
    if (node->left)
    {
      temp = temp->left;
      if (temp->right)
      {
        while (temp->right)
        {
          temp = temp->right;
        }
        balance_node_ptr = temp->parent;
        temp->parent->right = nullptr;
        temp->parent = node->parent;
        temp->left = node->left;
        temp->left->parent = temp;
      }
      else
      {
        balance_node_ptr = temp;
        temp->parent = node->parent;
      }
      if (node->right)
      {
        temp->right = node->right;
        temp->right->parent = temp;
      }
    }
    else if (node->right)
    {
      temp = temp->right;
      if (temp->left)
      {
        while (temp->left)
        {
          temp = temp->left;
        }
        balance_node_ptr = temp->parent;
        temp->parent->left = nullptr;
        temp->parent = node->parent;
        temp->right = node->right;
        temp->right->parent = temp;
      }
      else
      {
        balance_node_ptr = temp;
        temp->parent = node->parent;
      }
    }
    else
    {
      balance_node_ptr = node->parent;
      temp = nullptr;
    }
    if (node->parent && node->parent->left == node)
    {
      node->parent->left = temp;
    }
    else if (node->parent)
    {
      node->parent->right = temp;
    }
    else
    {
      root_ = temp;
    }
    upwardBalancing(balance_node_ptr);
    delete node;
    size_--;
  }

  template< typename K, typename T, typename Cmp >
  template< class... Args >
  std::pair< AVLTreeIterator< K, T, Cmp >, bool > AVLTree< K, T, Cmp >::emplaceImpl(node_t * subroot, Args && ... args)
  {
    if (empty())
    {
      root_ = new node_t{ nullptr, nullptr, nullptr, std::forward< Args >(args)... };
      size_++;
      return std::make_pair< it_t, bool >(it_t(root_), true);
    }
    node_t * added = nullptr;
    try
    {
      added = new node_t{ nullptr, nullptr, nullptr, std::forward< Args >(args)... };
      auto temp = subroot;
      while (temp->left || temp->right)
      {
        if (temp->left && Cmp{}(added->data.first, temp->data.first))
        {
          temp = temp->left;
        }
        else if (temp->right && Cmp{}(temp->data.first, added->data.first))
        {
          temp = temp->right;
        }
        else if (!Cmp{}(added->data.first, temp->data.first) && !Cmp{}(temp->data.first, added->data.first))
        {
          delete added;
          return std::make_pair< it_t, bool >(it_t(temp), false);
        }
        else
        {
          break;
        }
      }
      if (Cmp{}(added->data.first, temp->data.first))
      {
        temp->left = added;
      }
      else if (Cmp{}(temp->data.first, added->data.first))
      {
        temp->right = added;
      }
      else if (!Cmp{}(added->data.first, temp->data.first) && !Cmp{}(temp->data.first, added->data.first))
      {
        delete added;
        return std::make_pair< it_t, bool >(it_t(temp), false);
      }
      added->parent = temp;
      size_++;
      upwardBalancing(temp);
    }
    catch (...)
    {
      delete added;
      throw;
    }
    return std::make_pair< it_t, bool >(it_t(added), true);
  }
}

#endif
