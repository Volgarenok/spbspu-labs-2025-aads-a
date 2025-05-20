#ifndef TREE_HPP
#define TREE_HPP
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include "iterator.hpp"
#include "lnrIterator.hpp"
#include "rnlIterator.hpp"
#include "breadthIterator.hpp"
#include "treeNode.hpp"

namespace kiselev
{
  template< typename Key, typename Value, typename Cmp = std::less< Key > >
  class RBTree
  {
  public:
    using value = std::pair< Key, Value >;
    using Iterator = detail::Iterator< Key, Value, Cmp, false >;
    using ConstIterator = detail::Iterator< Key, Value, Cmp, true >;
    using LnrIterator = detail::LnrIterator< Key, Value, Cmp, false >;
    using ConstLnrIterator = detail::LnrIterator< Key, Value, Cmp, true >;
    using RnlIterator = detail::RnlIterator< Key, Value, Cmp, false >;
    using ConstRnlIterator = detail::RnlIterator< Key, Value, Cmp, true >;
    using BreadthIterator = detail::BreadthIterator< Key, Value, Cmp, false >;
    using ConstBreadthIterator = detail::BreadthIterator< Key, Value, Cmp, true >;
    using IteratorPair = std::pair< Iterator, Iterator >;
    using ConstIteratorPair = std::pair< ConstIterator, ConstIterator >;

    RBTree();
    RBTree(const RBTree< Key, Value, Cmp >&);
    RBTree(RBTree< Key, Value, Cmp >&&) noexcept;
    RBTree(std::initializer_list< value >);
    template< typename InputIt >
    RBTree(InputIt first, InputIt last);
    ~RBTree();

    RBTree< Key, Value, Cmp >& operator=(const RBTree< Key, Value, Cmp >&);
    RBTree< Key, Value, Cmp >& operator=(RBTree< Key, Value, Cmp >&&) noexcept;
    RBTree< Key, Value, Cmp >& operator=(std::initializer_list< value >);
    Value& operator[](const Key&);
    const Value& operator[](const Key&) const;

    Value& at(const Key&);
    const Value& at(const Key&) const;

    size_t size() const noexcept;
    bool empty() const noexcept;

    Iterator begin() noexcept;
    ConstIterator cbegin() const noexcept;
    Iterator end() noexcept;
    ConstIterator cend() const noexcept;
    LnrIterator lnrBegin();
    ConstLnrIterator lnrCbegin() const;
    LnrIterator lnrEnd() noexcept;
    ConstLnrIterator lnrCend() const noexcept;

    RnlIterator rnlBegin();
    ConstRnlIterator rnlCbegin() const;
    RnlIterator rnlEnd() noexcept;
    ConstRnlIterator rnlCend() const noexcept;

    BreadthIterator breadthBegin() noexcept;
    ConstBreadthIterator breadthCbegin() const noexcept;
    BreadthIterator breadthEnd() noexcept;
    ConstBreadthIterator breadthCend() const noexcept;

    template< typename F >
    F traverse_lnr(F f) const;
    template< typename F >
    F traverse_rnl(F f) const;
    template< typename F >
    F traverse_breadth(F f) const;

    std::pair< Iterator, bool > insert(const value&);
    std::pair< Iterator, bool > insert(value&);
    std::pair< Iterator, bool > insert(value&&);
    Iterator insert(ConstIterator, const value&);
    Iterator insert(Iterator, const value&);
    template< typename InputIt >
    void insert(InputIt first, InputIt last);
    void insert(std::initializer_list< value >);

    Iterator erase(Iterator) noexcept;
    Iterator erase(ConstIterator) noexcept;
    size_t erase(const Key&) noexcept;
    Iterator erase(Iterator first, Iterator last) noexcept;
    Iterator erase(ConstIterator first, ConstIterator last) noexcept;

    template< typename... Args >
    std::pair< Iterator, bool > emplace(Args&&...);
    template< typename... Args >
    Iterator emplaceHint(ConstIterator, Args&&...);

    void swap(RBTree< Key, Value, Cmp >&) noexcept;
    void clear() noexcept;

    Iterator find(const Key&) noexcept;
    ConstIterator find(const Key&) const noexcept;
    size_t count(const Key&) const noexcept;
    Iterator lowerBound(const Key&) noexcept;
    ConstIterator lowerBound(const Key&) const noexcept;
    Iterator upperBound(const Key&) noexcept;
    ConstIterator upperBound(const Key&) const noexcept;
    std::pair< Iterator, Iterator > equalRange(const Key&) noexcept;
    std::pair< ConstIterator, ConstIterator > equalRange(const Key&) const noexcept;

    TreeNode< Key, Value >* getMax() const noexcept;

  private:
    using Node = TreeNode< Key, Value>;

    void rotateLeft(Node* node) noexcept;
    void rotateRight(Node* node) noexcept;
    void fixInsert(Node* node) noexcept;
    void fixDelete(Node* node) noexcept;
    Node* root_;
    Cmp cmp_;
    size_t size_;
  };

  template< typename Key, typename Value, typename Cmp >
  RBTree< Key, Value, Cmp >::RBTree():
    root_(nullptr),
    size_(0)
  {}

  template< typename Key, typename Value, typename Cmp >
  RBTree< Key, Value, Cmp >::RBTree(const RBTree< Key, Value, Cmp >& tree):
    RBTree()
  {
    for (ConstIterator it = tree.cbegin(); it != tree.cend(); ++it)
    {
      insert(*it);
    }
  }

  template< typename Key, typename Value, typename Cmp >
  RBTree< Key, Value, Cmp >::RBTree(RBTree< Key, Value, Cmp >&& tree) noexcept:
    root_(std::exchange(tree.root_, nullptr)),
    size_(std::exchange(tree.size_, 0))
  {}

  template< typename Key, typename Value, typename Cmp >
  template< typename InputIt >
  RBTree< Key, Value, Cmp >::RBTree(InputIt first, InputIt last):
    RBTree()
  {
    for (; first != last; first++)
    {
      insert(*first);
    }
  }
  template< typename Key, typename Value, typename Cmp >
  RBTree< Key, Value, Cmp >::RBTree(std::initializer_list< value > il):
    RBTree(il.begin(), il.end())
  {}

  template< typename Key, typename Value, typename Cmp >
  RBTree< Key, Value, Cmp >& RBTree< Key, Value, Cmp >::operator=(const RBTree< Key, Value, Cmp >& tree)
  {
    RBTree< Key, Value, Cmp > cpy(tree);
    swap(cpy);
    return *this;
  }

  template< typename Key, typename Value, typename Cmp >
  RBTree< Key, Value, Cmp >& RBTree< Key, Value, Cmp >::operator=(RBTree< Key, Value, Cmp >&& tree) noexcept
  {
    RBTree< Key, Value, Cmp > temp(std::move(tree));
    swap(temp);
    return *this;
  }

  template< typename Key, typename Value, typename Cmp >
  RBTree< Key, Value, Cmp >& RBTree< Key, Value, Cmp >::operator=(std::initializer_list< value > il)
  {
    RBTree< Key, Value, Cmp > temp(il);
    swap(temp);
    return *this;
  }

  template< typename Key, typename Value, typename Cmp >
  RBTree< Key, Value, Cmp >::~RBTree()
  {
    clear();
  }

  template< typename Key, typename Value, typename Cmp >
  void RBTree< Key, Value, Cmp >::clear() noexcept
  {
    erase(begin(), end());
  }

  template< typename Key, typename Value, typename Cmp >
  void RBTree< Key, Value, Cmp >::rotateLeft(Node* node) noexcept
  {
    Node* child = node->right;
    node->right = child->left;
    if (child->left)
    {
      child->left->parent = node;
    }
    child->parent = node->parent;
    if (!node->parent)
    {
      root_ = child;
    }
    else if (node == node->parent->left)
    {
      node->parent->left = child;
    }
    else
    {
      node->parent->right = child;
    }
    child->left = node;
    node->parent = child;
  }

  template< typename Key, typename Value, typename Cmp >
  void RBTree< Key, Value, Cmp >::rotateRight(Node* node) noexcept
  {
    Node* child = node->left;
    node->left = child->right;
    if (child->right)
    {
      child->right->parent = node;
    }
    child->parent = node->parent;
    if (!node->parent)
    {
      root_ = child;
    }
    else if (node == node->parent->right)
    {
      node->parent->right = child;
    }
    else
    {
      node->parent->left = child;
    }
    child->right = node;
    node->parent = child;
  }

  template< typename Key, typename Value, typename Cmp >
  void RBTree< Key, Value, Cmp >::fixInsert(Node* node) noexcept
  {
    Node* parent = nullptr;
    Node* grandParent = nullptr;
    while (node && node->color == Color::RED && node->parent && node->parent->color == Color::RED)
    {
      parent = node->parent;
      grandParent = parent->parent;
      if (parent == grandParent->left)
      {
        Node* uncle = grandParent->right;
        if (uncle && uncle->color == Color::RED)
        {
          grandParent->color = Color::RED;
          parent->color = Color::BLACK;
          uncle->color = Color::BLACK;
          node = grandParent;
        }
        else
        {
          if (node == parent->right)
          {
            rotateLeft(parent);
            node = parent;
            parent = node->parent;
          }
          rotateRight(grandParent);
          std::swap(parent->color, grandParent->color);
          node = parent;
        }
      }
      else
      {
        Node* uncle = grandParent->left;
        if (uncle && uncle->color == Color::RED)
        {
          grandParent->color = Color::RED;
          parent->color = Color::BLACK;
          uncle->color = Color::BLACK;
          node = grandParent;
        }
        else
        {
          if (node == parent->left)
          {
            rotateRight(parent);
            node = parent;
            parent = node->parent;
          }
          rotateLeft(grandParent);
          std::swap(parent->color, grandParent->color);
          node = parent;
        }
      }
    }
    root_->color = Color::BLACK;
  }

  template< typename Key, typename Value, typename Cmp >
  void RBTree< Key, Value, Cmp >::fixDelete(Node* node) noexcept
  {
    while (node != root_ && node->color == Color::BLACK)
    {
      if (node == node->parent->left)
      {
        Node* brother = node->parent->right;
        if (brother && brother->color == Color::RED)
        {
          brother->color = Color::BLACK;
          node->parent->color = Color::RED;
          rotateLeft(node->parent);
          brother = node->parent->right;
        }
        if ((!brother->left || brother->left->color == Color::BLACK) && (!brother->right || brother->right->color == Color::BLACK))
        {
          brother->color = Color::RED;
          node = node->parent;
        }
        else
        {
          if (!brother->right || brother->right->color == Color::BLACK)
          {
            if (brother->left)
            {
              brother->left->color = Color::BLACK;
            }
            brother->color = Color::RED;
            rotateRight(brother);
            brother = node->parent->right;
          }
          brother->color = node->parent->color;
          node->parent->color = Color::BLACK;
          if (brother->right)
          {
            brother->right->color = Color::BLACK;
          }
          rotateLeft(node->parent);
          node = root_;
        }
      }
      else
      {
        Node* brother = node->parent->left;
        if (brother && brother->color == Color::RED)
        {
          brother->color = Color::BLACK;
          node->parent->color = Color::RED;
          rotateRight(node->parent);
          brother = node->parent->left;
        }
        if ((!brother->left || brother->left->color == Color::BLACK) && (!brother->right || brother->right->color == Color::BLACK))
        {
          brother->color = Color::RED;
          node = node->parent;
        }
        else
        {
          if (!brother->left || brother->left->color == Color::BLACK)
          {
            if (brother->right)
            {
              brother->right->color = Color::BLACK;
            }
            brother->color = Color::RED;
            rotateLeft(brother);
            brother = node->parent->left;
          }
          brother->color = node->parent->color;
          node->parent->color = Color::BLACK;
          if (brother->left)
          {
            brother->left->color = Color::BLACK;
          }
          rotateRight(node->parent);
          node = root_;
        }
      }
    }
    if (node)
    {
      node->color = Color::BLACK;
    }
  }

  template< typename Key, typename Value, typename Cmp >
  size_t RBTree< Key, Value, Cmp >::size() const noexcept
  {
    return size_;
  }

  template< typename Key, typename Value, typename Cmp >
  bool RBTree< Key, Value, Cmp >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename Key, typename Value, typename Cmp >
  void RBTree< Key, Value, Cmp >::swap(RBTree< Key, Value, Cmp >& tree) noexcept
  {
    std::swap(root_, tree.root_);
    std::swap(size_, tree.size_);
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::Iterator RBTree< Key, Value, Cmp >::begin() noexcept
  {
    if (empty())
    {
      return end();
    }
    Node* temp = root_;
    while (temp->left)
    {
      temp = temp->left;
    }
    return Iterator(temp, false);
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::ConstIterator RBTree< Key, Value, Cmp >::cbegin() const noexcept
  {
    if (empty())
    {
      return cend();
    }
    Node* temp = root_;
    while (temp->left)
    {
      temp = temp->left;
    }
    return ConstIterator(temp, false);
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::Iterator RBTree< Key, Value, Cmp >::end() noexcept
  {
    if (empty())
    {
      return Iterator(root_, true);
    }
    Node* temp = root_;
    while (temp->right)
    {
      temp = temp->right;
    }
    return Iterator(temp, true);
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::ConstIterator RBTree< Key, Value, Cmp >::cend() const noexcept
  {
    if (empty())
    {
      return ConstIterator(root_, true);
    }
    Node* temp = root_;
    while (temp->right)
    {
      temp = temp->right;
    }
    return ConstIterator(temp, true);
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp>::LnrIterator RBTree< Key, Value, Cmp >::lnrBegin()
  {
    if (empty())
    {
      return lnrEnd();
    }
    LnrIterator it(root_);
    while (it.node->left)
    {
      it.stack_.push(it.node_);
      it.node_ = it.node_->left;
    }
    return it;
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::ConstLnrIterator RBTree< Key, Value, Cmp >::lnrCbegin() const
  {
    if (empty())
    {
      return lnrCend();
    }
    LnrIterator it(root_);
    while (it.node_->left)
    {
      it.stack_.push(it.node_);
      it.node_ = it.node_->left;
    }
    return it;
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::LnrIterator RBTree< Key, Value, Cmp >::lnrEnd() noexcept
  {
    return LnrIterator(nullptr);
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::ConstLnrIterator RBTree< Key, Value, Cmp >::lnrCend() const noexcept
  {
    return ConstLnrIterator(nullptr);
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::RnlIterator RBTree< Key, Value, Cmp >::rnlBegin()
  {
    if (empty())
    {
      return rnlEnd();
    }
    RnlIterator it(root_);
    while (it.node_->right)
    {
      it.stack_.push(it.node_);
      it.node_ = it.node_->right;
    }
    return it;
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::ConstRnlIterator RBTree< Key, Value, Cmp >::rnlCbegin() const
  {
    if (empty())
    {
      return rnlCend();
    }
    ConstRnlIterator it(root_);
    while(it.node_->right)
    {
      it.stack_.push(it.node_);
      it.node_ = it.node_->right;
    }
    return it;
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::RnlIterator RBTree< Key, Value, Cmp >::rnlEnd() noexcept
  {
    return RnlIterator(nullptr);
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::ConstRnlIterator RBTree< Key, Value, Cmp >::rnlCend() const noexcept
  {
    return ConstRnlIterator(nullptr);
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::BreadthIterator RBTree< Key, Value, Cmp >::breadthBegin() noexcept
  {
    return BreadthIterator(root_);
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::ConstBreadthIterator RBTree< Key, Value, Cmp >::breadthCbegin() const noexcept
  {
    return ConstBreadthIterator(root_);
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::BreadthIterator RBTree< Key, Value, Cmp >::breadthEnd() noexcept
  {
    return BreadthIterator(nullptr);
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::ConstBreadthIterator RBTree< Key, Value, Cmp >::breadthCend() const noexcept
  {
    return ConstBreadthIterator(nullptr);
  }

  template< typename Key, typename Value, typename Cmp >
  template< typename F >
  F RBTree< Key, Value, Cmp >::traverse_lnr(F f) const
  {
    for (ConstLnrIterator it = lnrCbegin(); it != lnrCend(); ++it)
    {
      f(*(it));
    }
    return f;
  }

  template< typename Key, typename Value, typename Cmp >
  template< typename F >
  F RBTree< Key, Value, Cmp >::traverse_rnl(F f) const
  {
    for (ConstRnlIterator it = rnlCbegin(); it != rnlCend(); ++it)
    {
      f(*(it));
    }
    return f;
  }

  template< typename Key, typename Value, typename Cmp >
  template< typename F >
  F RBTree< Key, Value, Cmp >::traverse_breadth(F f) const
  {
    for (ConstBreadthIterator it = breadthCbegin(); it != breadthCend(); ++it)
    {
      f(*(it));
    }
    return f;
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::Iterator RBTree< Key, Value, Cmp >::find(const Key& key) noexcept
  {
    Node* temp = root_;
    while (temp)
    {
      if (cmp_(key, temp->data.first))
      {
        temp = temp->left;
      }
      else if (cmp_(temp->data.first, key))
      {
        temp = temp->right;
      }
      else
      {
        return Iterator(temp, false);
      }
    }
    return end();
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::ConstIterator RBTree< Key, Value, Cmp >::find(const Key& key) const noexcept
  {
    Node* temp = root_;
    while (temp)
    {
      if (cmp_(key, temp->data.first))
      {
        temp = temp->left;
      }
      else if (cmp_(temp->data.first, key))
      {
        temp = temp->right;
      }
      else
      {
        return ConstIterator(temp, false);
      }
    }
    return cend();
  }

  template< typename Key, typename Value, typename Cmp >
  template< typename... Args >
  std::pair< typename RBTree< Key, Value, Cmp >::Iterator, bool > RBTree< Key, Value, Cmp >::emplace(Args &&... args)
  {
    Node* newNode = new Node{ Color::BLACK, nullptr, nullptr, nullptr, { std::forward< Args >(args)... } };
    try
    {
      if (!root_)
      {
        root_ = newNode;
        size_ = 1;
        return { Iterator(root_, false), true };
      }
      Node* temp = root_;
      Node* parent = nullptr;
      while (temp)
      {
        parent = temp;
        if (cmp_(newNode->data.first, temp->data.first))
        {
          temp = temp->left;
        }
        else if (cmp_(temp->data.first, newNode->data.first))
        {
          temp = temp->right;
        }
        else
        {
          delete newNode;
          return { Iterator(temp, false), false };
        }
      }

      newNode->parent = parent;
      newNode->color = Color::RED;
      if (cmp_(parent->data.first, newNode->data.first))
      {
        parent->right = newNode;
      }
      else
      {
        parent->left = newNode;
      }
    }
    catch (...)
    {
      delete newNode;
      throw;
    }
    fixInsert(newNode);
    size_++;
    return { Iterator(newNode, false), true };
  }

  template< typename Key, typename Value, typename Cmp >
  template< typename... Args >
  typename RBTree< Key, Value, Cmp >::Iterator RBTree< Key, Value, Cmp >::emplaceHint(ConstIterator hint, Args &&... args)
  {
    if (hint == cend() || empty())
    {
      return emplace(std::forward< Args >(args)...).first;
    }
    Node* pos = hint.node_;
    Node* newNode = new Node{ Color::RED, nullptr, nullptr, pos, { std::forward< Args >(args)... } };
    value val = newNode->data;
    try
    {
      if (cmp_(val.first, pos->data.first))
      {
        if (!pos->left)
        {
          pos->left = newNode;
          fixInsert(newNode);
          ++size_;
          return Iterator(newNode, false);
        }
      }
      else if (cmp_(pos->data.first, val.first))
      {
        if (!pos->right)
        {
          pos->right = newNode;
          fixInsert(newNode);
          ++size_;
          return Iterator(newNode, false);
        }
      }
      else
      {
        delete newNode;
        return Iterator(pos, false);
      }
      delete newNode;
      return emplace(val).first;
    }
    catch (...)
    {
      delete newNode;
      throw;
    }
  }

  template< typename Key, typename Value, typename Cmp >
  std::pair< typename RBTree< Key, Value, Cmp >::Iterator, bool > RBTree< Key, Value, Cmp >::insert(const value& val)
  {
    return emplace(val);
  }

  template< typename Key, typename Value, typename Cmp >
  std::pair< typename RBTree< Key, Value, Cmp >::Iterator, bool > RBTree< Key, Value, Cmp >::insert(value& val)
  {
    return emplace(val);
  }

  template< typename Key, typename Value, typename Cmp >
  std::pair< typename RBTree< Key, Value, Cmp >::Iterator, bool > RBTree< Key, Value, Cmp >::insert(value&& val)
  {
    return emplace(std::move(val));
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::Iterator RBTree< Key, Value, Cmp >::insert(ConstIterator pos, const value& val)
  {
    return emplaceHint(pos, val);
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::Iterator RBTree< Key, Value, Cmp >::insert(Iterator pos, const value& val)
  {
    ConstIterator it(pos);
    return emplaceHint(it, val);
  }

  template< typename Key, typename Value, typename Cmp >
  template< typename InputIt >
  void RBTree< Key, Value, Cmp >::insert(InputIt first, InputIt last)
  {
    RBTree< Key, Value, Cmp > temp(*this);
    for (; first != last; ++first)
    {
      temp.insert(*first);
    }
    swap(temp);
  }

  template< typename Key, typename Value, typename Cmp >
  void RBTree< Key, Value, Cmp >::insert(std::initializer_list< value > il)
  {
    insert(il.begin(), il.end());
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::Iterator RBTree< Key, Value, Cmp >::erase(ConstIterator pos) noexcept
  {
    if (pos == cend())
    {
      return end();
    }
    Node* toDelete = pos.node_;
    Node* replace = nullptr;
    Node* child = nullptr;
    if (size_ == 1)
    {
      delete root_;
      size_ = 0;
      return end();
    }
    if (!toDelete->left || !toDelete->right)
    {
      replace = toDelete;
    }
    else
    {
      replace = toDelete->right;
      while (replace->left)
      {
        replace = replace->left;
      }
    }
    child = replace->left ? replace->left : replace->right;
    if (child)
    {
      child->parent = replace->parent;
    }
    if (!replace->parent)
    {
      root_ = child;
    }
    else if (replace == replace->parent->left)
    {
      replace->parent->left = child;
    }
    else
    {
      replace->parent->right = child;
    }
    if (replace != toDelete)
    {
      toDelete->data = std::move(replace->data);
    }
    if (replace->color == Color::BLACK)
    {
      fixDelete(child ? child : replace->parent);
    }
    Iterator next(pos.node_, pos.isEnd_);
    ++next;
    delete replace;
    --size_;
    return next;
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::Iterator RBTree< Key, Value, Cmp >::erase(Iterator pos) noexcept
  {
    ConstIterator it(pos);
    return erase(it);
  }

  template< typename Key, typename Value, typename Cmp >
  size_t RBTree< Key, Value, Cmp >::erase(const Key& key) noexcept
  {
    Iterator it = find(key);
    if (it == end())
    {
      return 0;
    }
    erase(it);
    return 1;
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::Iterator RBTree< Key, Value, Cmp >::erase(ConstIterator first, ConstIterator last) noexcept
  {
    while (first != last)
    {
      first = erase(first);
    }
    return Iterator(last.node_, last.isEnd_);
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::Iterator RBTree< Key, Value, Cmp >::erase(Iterator first, Iterator last) noexcept
  {
    ConstIterator constFirst(first);
    ConstIterator constLast(last);
    return erase(constFirst, constLast);
  }

  template< typename Key, typename Value, typename Cmp >
  size_t RBTree< Key, Value, Cmp >::count(const Key& key) const noexcept
  {
    ConstIterator it = find(key);
    return it == cend() ? 0 : 1;
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::Iterator RBTree< Key, Value, Cmp >::lowerBound(const Key& key) noexcept
  {
    Node* temp = root_;
    Node* res = nullptr;
    while (temp)
    {
      if (!cmp_(temp->data.first, key))
      {
        res = temp;
        temp = temp->left;
      }
      else
      {
        temp = temp->right;
      }
    }
    return res ? Iterator(res, false) : end();
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::ConstIterator RBTree< Key, Value, Cmp >::lowerBound(const Key& key) const noexcept
  {
    return ConstIterator(lowerBound(key));
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::Iterator RBTree< Key, Value, Cmp >::upperBound(const Key& key) noexcept
  {
    Node* temp = root_;
    Node* res = nullptr;
    while (temp)
    {
      if (cmp_(key, temp->data.first))
      {
        res = temp;
        temp = temp->left;
      }
      else
      {
        temp = temp->right;
      }
    }
    return res ? Iterator(res, false) : end();
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::ConstIterator RBTree< Key, Value, Cmp >::upperBound(const Key& key) const noexcept
  {
    return ConstIterator(upperBound(key));
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::IteratorPair RBTree< Key, Value, Cmp >::equalRange(const Key& key) noexcept
  {
    return { Iterator(lowerBound(key)), Iterator(upperBound(key)) };
  }

  template< typename Key, typename Value, typename Cmp >
  typename RBTree< Key, Value, Cmp >::ConstIteratorPair RBTree< Key, Value, Cmp >::equalRange(const Key& key) const noexcept
  {
    return { ConstIterator(lowerBound(key)), ConstIterator(upperBound(key)) };
  }

  template< typename Key, typename Value, typename Cmp >
  const Value& RBTree< Key, Value, Cmp >::operator[](const Key& key) const
  {
    ConstIterator it = find(key);
    return it->second;
  }

  template< typename Key, typename Value, typename Cmp >
  Value& RBTree< Key, Value, Cmp >::operator[](const Key& key)
  {
    Iterator it = insert(std::make_pair(key, Value())).first;
    return it->second;
  }

  template< typename Key, typename Value, typename Cmp >
  const Value& RBTree< Key, Value, Cmp >::at(const Key& key) const
  {
    ConstIterator it = find(key);
    if (it == cend())
    {
      throw std::out_of_range("There is no such key");
    }
    return it->second;
  }

  template< typename Key, typename Value, typename Cmp >
  Value& RBTree< Key, Value, Cmp >::at(const Key& key)
  {
    return const_cast< Value& >(static_cast< const RBTree< Key, Value, Cmp >& >(*this).at(key));
  }
}
#endif
