#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <stdexcept>
#include <initializer_list>
#include "iterator.hpp"
#include "lnr-iterator.hpp"
#include "rnl-iterator.hpp"
#include "bfs-iterator.hpp"

namespace aleksandrov
{
  template< class K, class V, class C, bool isConst >
  class Iterator;

  template< class K, class V, class C = std::less< K > >
  class Tree
  {
  public:
    using Iter = Iterator< K, V, C, false >;
    using ConstIter = Iterator< K, V, C, true >;
    using LnrIter = LnrIterator< K, V, C, false >;
    using ConstLnrIter = LnrIterator< K, V, C, true >;
    using RnlIter = RnlIterator< K, V, C, false >;
    using ConstRnlIter = RnlIterator< K, V, C, true >;
    using BfsIter = BfsIterator< K, V, C, false >;
    using ConstBfsIter = BfsIterator< K, V, C, true >;
    using ValueType = std::pair< K, V >;

    Tree();
    explicit Tree(const C&);
    Tree(const Tree&);
    template< class InputIt >
    Tree(InputIt, InputIt);
    Tree(Tree&&) noexcept;
    Tree(std::initializer_list< ValueType >);
    ~Tree() noexcept;

    Tree& operator=(const Tree&);
    Tree& operator=(Tree&&) noexcept;
    Tree& operator=(std::initializer_list< ValueType >);

    V& at(const K&);
    const V& at(const K&) const;
    V& operator[](const K&);
    V& operator[](K&&);

    Iter begin() noexcept;
    ConstIter cbegin() const noexcept;
    Iter end() noexcept;
    ConstIter cend() const noexcept;

    LnrIter beginLNR() noexcept;
    ConstLnrIter cbeginLNR() const noexcept;
    LnrIter endLNR() noexcept;
    ConstLnrIter cendLNR() const noexcept;

    RnlIter beginRNL() noexcept;
    ConstRnlIter cbeginRNL() const noexcept;
    RnlIter endRNL() noexcept;
    ConstRnlIter cendRNL() const noexcept;

    BfsIter beginBFS() noexcept;
    ConstBfsIter cbeginBFS() const noexcept;
    BfsIter endBFS() noexcept;
    ConstBfsIter cendBFS() const noexcept;

    template< class F >
    F traverseLNR(F) const;
    template< class F >
    F traverseRNL(F) const;
    template< class F >
    F traverseBFS(F) const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void clear() noexcept;
    std::pair< Iter, bool > insert(const ValueType&);
    Iter insert(Iter, const ValueType&);
    template< class InputIt >
    void insert(InputIt, InputIt);
    void insert(std::initializer_list< ValueType >);
    template< class... Args >
    std::pair< Iter, bool > emplace(Args&&...);
    template< class... Args >
    Iter emplaceHint(Iter, Args&&...);
    Iter erase(Iter);
    Iter erase(ConstIter);
    Iter erase(Iter, Iter);
    Iter erase(ConstIter, ConstIter);
    size_t erase(const K&);
    void swap(Tree&) noexcept;

    size_t count(const K&) const;
    Iter find(const K&);
    ConstIter find(const K&) const;
    std::pair< Iter, Iter > equalRange(const K&);
    std::pair< ConstIter, ConstIter > equalRange(const K&) const;
    Iter lowerBound(const K&);
    ConstIter lowerBound(const K&) const;
    Iter upperBound(const K&);
    ConstIter upperBound(const K&) const;

    bool operator==(const Tree&) const;
    bool operator!=(const Tree&) const;

  private:
    template< class, class, class, bool >
    friend class Iterator;
    using Node = detail::Node< K, V >;
    using NodeType = detail::NodeType;

    Node* root_;
    size_t size_;
    C comp_;

    Node* copyRecursive(Node* root, Node* parent = nullptr);
    void clearRecursive(Node*) noexcept;
    std::pair< Node*, PointsTo > findIterative(Node*, const K&) const;

    Node* findInsertionLeaf(Iter hint, const K&);
    void insertUpper(Node*, Node* left, Node* right, const ValueType& median);

    Node* fixAfterErase(Node*);
    Node* redistributeData(Node*);
    Node* mergeData(Node*);

    void insertIntoNode(Node*, const ValueType&);
    void removeFromNode(Node*, const ValueType&);
  };

  template< class K, class V, class C >
  Tree< K, V, C >::Tree():
    root_(nullptr),
    size_(0),
    comp_(C{})
  {}

  template< class K, class V, class C >
  Tree< K, V, C >::Tree(const C& comp):
    root_(nullptr),
    size_(0),
    comp_(comp)
  {}

  template< class K, class V, class C >
  Tree< K, V, C >::Tree(const Tree& rhs):
    root_(copyRecursive(rhs.root_)),
    size_(rhs.size_),
    comp_(rhs.comp_)
  {}

  template< class K, class V, class C >
  template< class InputIt >
  Tree< K, V, C >::Tree(InputIt first, InputIt last):
    Tree()
  {
    insert(first, last);
  }

  template< class K, class V, class C >
  Tree< K, V, C >::Tree(Tree&& rhs) noexcept:
    root_(std::exchange(rhs.root_, nullptr)),
    size_(std::exchange(rhs.size_, 0)),
    comp_(std::exchange(rhs.comp_, std::less< K >{}))
  {}

  template< class K, class V, class C >
  Tree< K, V, C >::Tree(std::initializer_list< ValueType > ilist):
    Tree(ilist.begin(), ilist.end())
  {}

  template< class K, class V, class C >
  Tree< K, V, C >::~Tree() noexcept
  {
    clear();
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::operator=(const Tree& rhs) -> Tree&
  {
    Tree copy(rhs);
    swap(copy);
    return *this;
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::operator=(Tree&& rhs) noexcept -> Tree&
  {
    Tree copy(std::move(rhs));
    swap(copy);
    return *this;
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::operator=(std::initializer_list< ValueType > ilist) -> Tree&
  {
    Tree copy(ilist);
    swap(copy);
    return *this;
  }

  template< class K, class V, class C >
  V& Tree< K, V, C >::at(const K& key)
  {
    Iter it = find(key);
    if (it == end())
    {
      throw std::out_of_range("ERROR: No such element exists!");
    }
    return it->second;
  }

  template< class K, class V, class C >
  const V& Tree< K, V, C >::at(const K& key) const
  {
    ConstIter it = find(key);
    if (it == cend())
    {
      throw std::out_of_range("ERROR: No such element exists!");
    }
    return *it->second;
  }

  template< class K, class V, class C >
  V& Tree< K, V, C >::operator[](const K& key)
  {
    Iter it = find(key);
    if (it != end())
    {
      return it->second;
    }
    return insert({ key, V{} }).first->second;
  }

  template< class K, class V, class C >
  V& Tree< K, V, C >::operator[](K&& key)
  {
    Iter it = find(key);
    if (it != end())
    {
      return it->second;
    }
    return insert({ std::move(key), V{} }).first->second;
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::begin() noexcept -> Iter
  {
    return root_ ? Iter(root_).fallLeft() : Iter();
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::cbegin() const noexcept -> ConstIter
  {
    return root_ ? ConstIter(root_).fallLeft() : ConstIter();
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::end() noexcept -> Iter
  {
    return Iter();
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::cend() const noexcept -> ConstIter
  {
    return ConstIter();
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::beginLNR() noexcept -> LnrIter
  {
    return root_ ? ++LnrIter(root_) : LnrIter();
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::cbeginLNR() const noexcept -> ConstLnrIter
  {
    return root_ ? ++ConstLnrIter(root_) : ConstLnrIter();
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::endLNR() noexcept -> LnrIter
  {
    return LnrIter();
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::cendLNR() const noexcept -> ConstLnrIter
  {
    return ConstLnrIter();
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::beginRNL() noexcept -> RnlIter
  {
    return root_ ? ++RnlIter(root_) : RnlIter();
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::cbeginRNL() const noexcept -> ConstRnlIter
  {
    return root_ ? ++ConstRnlIter(root_) : ConstRnlIter();
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::endRNL() noexcept -> RnlIter
  {
    return RnlIter();
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::cendRNL() const noexcept -> ConstRnlIter
  {
    return ConstRnlIter();
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::beginBFS() noexcept -> BfsIter
  {
    return BfsIter(root_);
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::cbeginBFS() const noexcept -> ConstBfsIter
  {
    return ConstBfsIter(root_);
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::endBFS() noexcept -> BfsIter
  {
    return BfsIter();
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::cendBFS() const noexcept -> ConstBfsIter
  {
    return ConstBfsIter();
  }

  template< class K, class V, class C >
  template< class F >
  F Tree< K, V, C >::traverseLNR(F f) const
  {
    for (auto it = cbeginLNR(); it != cendLNR(); ++it)
    {
      f(*it);
    }
    return f;
  }

  template< class K, class V, class C >
  template< class F >
  F Tree< K, V, C >::traverseRNL(F f) const
  {
    for (auto it = cbeginRNL(); it != cendRNL(); ++it)
    {
      f(*it);
    }
    return f;
  }

  template< class K, class V, class C >
  template< class F >
  F Tree< K, V, C >::traverseBFS(F f) const
  {
    for (auto it = cbeginBFS(); it != cendBFS(); ++it)
    {
      f(*it);
    }
    return f;
  }

  template< class K, class V, class C >
  bool Tree< K, V, C >::empty() const noexcept
  {
    return !size_;
  }

  template< class K, class V, class C >
  size_t Tree< K, V, C >::size() const noexcept
  {
    return size_;
  }

  template< class K, class V, class C >
  void Tree< K, V, C >::clear() noexcept
  {
    if (!empty())
    {
      clearRecursive(root_);
      root_ = nullptr;
      size_ = 0;
    }
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::insert(const ValueType& p) -> std::pair< Iter, bool >
  {
    return emplace(p.first, p.second);
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::insert(Iter hint, const ValueType& p) -> Iter
  {
    return emplaceHint(hint, p);
  }

  template< class K, class V, class C >
  template< class InputIt >
  void Tree< K, V, C >::insert(InputIt first, InputIt last)
  {
    for (auto it = first; it != last; ++it)
    {
      insert(*it);
    }
  }

  template< class K, class V, class C >
  void Tree< K, V, C >::insert(std::initializer_list< ValueType > ilist)
  {
    insert(ilist.begin(), ilist.end());
  }

  template< class K, class V, class C >
  template< class... Args >
  auto Tree< K, V, C >::emplace(Args&&... args) -> std::pair< Iter, bool >
  {
    ValueType p(std::forward< Args >(args)...);
    Iter it = find(p.first);
    Iter toReturn = emplaceHint(end(), std::forward< Args >(args)...);
    return { toReturn, toReturn != it };
  }

  template< class K, class V, class C >
  template< class... Args >
  auto Tree< K, V, C >::emplaceHint(Iter hint, Args&&... args) -> Iter
  {
    ValueType p(std::forward< Args >(args)...);
    if (!root_)
    {
      root_ = new Node(p);
      ++size_;
      return Iter(root_);
    }

    Iter it = find(p.first);
    if (it != end())
    {
      return it;
    }

    Node* leaf = findInsertionLeaf(hint, p.first);
    if (leaf->isDouble())
    {
      leaf->data[1] = p;
      Iter toReturn(leaf, PointsTo::Right);
      if (comp_(p.first, leaf->data[0].first))
      {
        std::swap(leaf->data[0], leaf->data[1]);
        toReturn.dir_ = PointsTo::Left;
      }
      leaf->type = NodeType::Triple;
      ++size_;
      return toReturn;
    }
    Node* left = nullptr;
    Node* right = nullptr;
    try
    {
      const ValueType& min = comp_(p.first, leaf->data[0].first) ? p : leaf->data[0];
      left = new Node(min);
      const ValueType& max = comp_(leaf->data[1].first, p.first) ? p : leaf->data[1];
      right = new Node(max);
      ValueType& median = p;
      if (p.first == min.first)
      {
        median = leaf->data[0];
      }
      else if (p.first == max.first)
      {
        median = leaf->data[1];
      }
      insertUpper(leaf, left, right, median);
    }
    catch (...)
    {
      delete left;
      delete right;
      throw;
    }
    ++size_;
    return find(p.first);
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::erase(Iter pos) -> Iter
  {
    assert(pos != end());
    Node* node = pos.node_;
    const K key = pos->first;

    Node* min = node->right;
    if (node->isTriple() && pos.dir_ == PointsTo::Left)
    {
      min = node->middle;
    }
    while (min && min->left)
    {
      min = min->left;
    }
    if (min)
    {
      std::swap(*pos, min->data[0]);
      node = min;
    }

    if (node->isTriple())
    {
      if (min || (!min && pos.dir_ == PointsTo::Left))
      {
        std::swap(node->data[0], node->data[1]);
      }
      node->type = NodeType::Double;
      node = fixAfterErase(node);
      --size_;
      return lowerBound(key);
    }
    node->type = NodeType::Empty;
    node = fixAfterErase(node);
    --size_;
    return size_ ? lowerBound(key) : Iter();
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::erase(ConstIter pos) -> Iter
  {
    return erase(Iter(pos.node_, pos.dir_));
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::erase(Iter first, Iter last) -> Iter
  {
    if (first == last)
    {
      return last;
    }
    if (last == end())
    {
      while (first != end())
      {
        first = erase(first);
      }
      return end();
    }
    else
    {
      const K key = last->first;
      while (first != last)
      {
        first = erase(first);
        if (last != end())
        {
          last = find(key);
        }
        if (last == end())
        {
          break;
        }
      }
      return last;
    }
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::erase(ConstIter first, ConstIter last) -> Iter
  {
    return erase(Iter(first.node_, first.dir_), Iter(last.node_, last.dir_));
  }

  template< class K, class V, class C >
  size_t Tree< K, V, C >::erase(const K& key)
  {
    Iter it = find(key);
    if (it == end())
    {
      return 0;
    }
    erase(it);
    return 1;
  }

  template< class K, class V, class C >
  void Tree< K, V, C >::swap(Tree& rhs) noexcept
  {
    std::swap(root_, rhs.root_);
    std::swap(size_, rhs.size_);
    std::swap(comp_, rhs.comp_);
  }

  template< class K, class V, class C >
  size_t Tree< K, V, C >::count(const K& key) const
  {
    return find(key) != cend();
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::find(const K& key) -> Iter
  {
    const auto pair = findIterative(root_, key);
    return Iter(pair.first, pair.second);
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::find(const K& key) const -> ConstIter
  {
    const auto pair = findIterative(root_, key);
    return ConstIter(pair.first, pair.second);
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::equalRange(const K& key) -> std::pair< Iter, Iter >
  {
    return { lowerBound(key), upperBound(key) };
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::equalRange(const K& key) const -> std::pair< ConstIter, ConstIter >
  {
    return { lowerBound(key), upperBound(key) };
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::lowerBound(const K& key) -> Iter
  {
    Iter it = begin();
    for (; it != end() && comp_(it->first, key); ++it);
    return it;
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::lowerBound(const K& key) const -> ConstIter
  {
    ConstIter it = cbegin();
    for (; it != cend() && comp_(it->first, key); ++it);
    return it;
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::upperBound(const K& key) -> Iter
  {
    Iter it = begin();
    for (; it != end() && !comp_(key, it->first); ++it);
    return it;
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::upperBound(const K& key) const -> ConstIter
  {
    ConstIter it = cbegin();
    for (; it != cend() && !comp_(key, it->first); ++it);
    return it;
  }

  template< class K, class V, class C >
  bool Tree< K, V, C >::operator==(const Tree& rhs) const
  {
    if (size_ != rhs.size_)
    {
      return false;
    }
    auto first1 = cbegin();
    auto first2 = rhs.cbegin();
    auto last1 = cend();
    auto last2 = rhs.cend();
    while (first1 != last1 && first2 != last2)
    {
      if (*first1 != *first2)
      {
        return false;
      }
      ++first1;
      ++first2;
    }
    return true;
  }

  template< class K, class V, class C >
  bool Tree< K, V, C >::operator!=(const Tree& rhs) const
  {
    return !(*this == rhs);
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::copyRecursive(Node* root, Node* parent) -> Node*
  {
    if (!root)
    {
      return nullptr;
    }
    Node* newNode = new Node(*root);
    newNode->parent = parent;

    newNode->left = copyRecursive(root->left, newNode);
    if (root->isTriple())
    {
      newNode->middle = copyRecursive(root->middle, newNode);
    }
    newNode->right = copyRecursive(root->right, newNode);

    return newNode;
  }

  template< class K, class V, class C >
  void Tree< K, V, C >::clearRecursive(Node* root) noexcept
  {
    if (!root)
    {
      return;
    }

    clearRecursive(root->left);
    if (root->isTriple())
    {
      clearRecursive(root->middle);
    }
    clearRecursive(root->right);

    delete root;
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::findIterative(Node* node, const K& key) const -> std::pair< Node*, PointsTo >
  {
    while (node)
    {
      const K& k0 = node->data[0].first;
      if (!comp_(key, k0) && !comp_(k0, key))
      {
        return { node, PointsTo::Left };
      }
      if (comp_(key, k0))
      {
        node = node->left;
        continue;
      }
      if (node->isDouble())
      {
        node = node->right;
        continue;
      }
      const K& k1 = node->data[1].first;
      if (!comp_(key, k1) && !comp_(k1, key))
      {
        return { node, PointsTo::Right };
      }
      node = comp_(key, k1) ? node->middle : node->right;
    }
    return { nullptr, PointsTo::None };
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::findInsertionLeaf(Iter hint, const K& key) -> Node*
  {
    Node* node = hint == end() ? root_ : hint.node_;
    while (!node->isLeaf())
    {
      if (comp_(key, node->data[0].first))
      {
        node = node->left;
      }
      else if (node->isTriple())
      {
        node = comp_(node->data[1].first, key) ? node->right : node->middle;
      }
      else
      {
        node = node->right;
      }
    }
    return node;
  }

  template< class K, class V, class C >
  void Tree< K, V, C >::insertUpper(Node* node, Node* left, Node* right, const ValueType& median)
  {
    Node* parent = node->parent;
    if (!parent)
    {
      root_ = new Node(median);
      root_->left = left;
      left->parent = root_;
      root_->right = right;
      right->parent = root_;
      delete node;
      return;
    }
    else if (parent->isDouble())
    {
      parent->data[1] = median;
      if (comp_(median.first, parent->data[0].first))
      {
        std::swap(parent->data[0], parent->data[1]);
      }
      parent->type = NodeType::Triple;
      if (node->parent->left == node)
      {
        parent->left = left;
        left->parent = parent;
        parent->middle = right;
        right->parent = parent;
      }
      else if (node->parent->right == node)
      {
        parent->middle = left;
        left->parent = parent;
        parent->right = right;
        right->parent = parent;
      }
      delete node;
      return;
    }
    Node* currLeft = nullptr;
    Node* currRight = nullptr;
    try
    {
      const ValueType min = comp_(median.first, parent->data[0].first) ? median : parent->data[0];
      currLeft = new Node(min);
      const ValueType max = comp_(parent->data[1].first, median.first) ? median : parent->data[1];
      currRight = new Node(max);
      ValueType newMedian = median;
      if (median.first == min.first)
      {
        newMedian = parent->data[0];
      }
      else if (median.first == max.first)
      {
        newMedian = parent->data[1];
      }
      if (node->parent->left == node)
      {
        currLeft->left = left;
        currLeft->left->parent = currLeft;
        currLeft->right = right;
        currLeft->right->parent = currLeft;
        currRight->left = parent->middle;
        parent->middle->parent = currRight;
        currRight->right = parent->right;
        parent->right->parent = currRight;
      }
      else if (node->parent->middle == node)
      {
        currLeft->left = parent->left;
        parent->left->parent = currLeft;
        currLeft->right = left;
        left->parent = currLeft;
        currRight->left = right;
        right->parent = currRight;
        currRight->right = parent->right;
        parent->right->parent = currRight;
      }
      else if (node->parent->right == node)
      {
        currLeft->left = parent->left;
        parent->left->parent = currLeft;
        currLeft->right = parent->middle;
        parent->middle->parent = currLeft;
        currRight->left = left;
        currRight->left->parent = currRight;
        currRight->right = right;
        currRight->right->parent = currRight;
      }
      delete node;
      insertUpper(parent, currLeft, currRight, newMedian);
    }
    catch (...)
    {
      delete currLeft;
      delete currRight;
      throw;
    }
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::fixAfterErase(Node* leaf) -> Node*
  {
    if (!leaf->parent && (leaf->isDouble() || leaf->isEmpty()))
    {
      return nullptr;
    }
    if (!leaf->isEmpty())
    {
      if (leaf->parent)
      {
        fixAfterErase(leaf->parent);
      }
      return leaf;
    }
    Node* parent = leaf->parent;
    if (parent->isTriple() || parent->left->isTriple() || parent->right->isTriple())
    {
      leaf = redistributeData(leaf);
    }
    else if (parent->isTriple() && parent->middle->isTriple())
    {
      leaf = redistributeData(leaf);
    }
    else
    {
      leaf = mergeData(leaf);
    }
    return fixAfterErase(leaf);
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::redistributeData(Node* leaf) -> Node*
  {
    Node* parent = leaf->parent;
    Node* left = parent->left;
    Node* middle = parent->middle;
    Node* right = parent->right;

    if (parent->isTriple() && !left->isTriple() && !middle->isTriple() && !right->isTriple())
    {
      if (left == leaf)
      {
        parent->left = parent->middle;
        parent->middle = nullptr;
        insertIntoNode(parent->left, parent->data[0]);
        left->middle = left->left;
        left->left = leaf->right;
        removeFromNode(parent, parent->data[0]);
        if (leaf->left)
        {
          parent->left->left = leaf->left;
        }
        else if (leaf->middle)
        {
          parent->left->left = leaf->middle;
        }
        if (parent->left->left)
        {
          parent->left->left->parent = parent->left;
        }
        delete left;
      }
      else if (middle == leaf)
      {
        insertIntoNode(left, parent->data[0]);
        removeFromNode(parent, parent->data[0]);
        left->middle = left->right;
        if (leaf->left)
        {
          left->right = leaf->left;
        }
        else if (leaf->right)
        {
          left->right = leaf->right;
        }
        if (left->right)
        {
          left->right->parent = left;
        }
        delete middle;
      }
      else if (right == leaf)
      {
        insertIntoNode(middle, parent->data[1]);
        parent->right = parent->middle;
        parent->right->middle = parent->right->right;
        parent->middle = nullptr;
        removeFromNode(parent, parent->data[1]);
        if (leaf->left)
        {
          middle->right = leaf->left;
        }
        else if (leaf->right)
        {
          middle->right = leaf->right;
        }
        if (middle->right)
        {
          middle->right->parent = middle;
        }
        delete right;
      }
    }
    else if (parent->isTriple() && (left->isTriple() || middle->isTriple() || right->isTriple()))
    {
      if (right == leaf)
      {
        if (leaf->left)
        {
          leaf->middle = leaf->left;
          leaf->left = nullptr;
        }
        insertIntoNode(leaf, parent->data[1]);
        if (middle->isTriple())
        {
          parent->data[1] = middle->data[1];
          removeFromNode(middle, middle->data[1]);
          leaf->left = middle->right;
          middle->right = nullptr;
          if (leaf->left)
          {
            leaf->left->parent = leaf;
          }
        }
        else if (left->isTriple())
        {
          parent->data[1] = middle->data[0];
          leaf->left = middle->middle;
          middle->middle = middle->left;
          if (leaf->left)
          {
            leaf->left->parent = leaf;
          }
          middle->data[0] = parent->data[0];
          parent->data[0] = left->data[1];
          removeFromNode(left, left->data[1]);
          middle->left = left->right;
          if (middle->left)
          {
            left->right = nullptr;
          }
        }
      }
      else if (middle == leaf)
      {
        if (right->isTriple())
        {
          if (!leaf->left)
          {
            leaf->left = leaf->right;
          }
          insertIntoNode(middle, parent->data[1]);
          parent->data[1] = right->data[0];
          removeFromNode(right, right->data[0]);
          middle->right = right->left;
          if (middle->right)
          {
            middle->right->parent = middle;
          }
          right->left = right->middle;
          right->middle = nullptr;
        }
        else if (left->isTriple())
        {
          if (!leaf->right)
          {
            leaf->right = leaf->left;
          }
          insertIntoNode(middle, parent->data[0]);
          parent->data[0] = left->data[1];
          removeFromNode(left, left->data[1]);
          middle->left = left->right;
          if (middle->left)
          {
            middle->left->parent = middle;
          }
          left->right = left->middle;
          left->middle = nullptr;
        }
      }
      else if (left == leaf)
      {
        if (!leaf->left)
        {
          leaf->left = leaf->middle;
          leaf->middle = nullptr;
        }
        insertIntoNode(left, parent->data[0]);
        if (middle->isTriple())
        {
          parent->data[0] = middle->data[0];
          removeFromNode(middle, middle->data[0]);
          left->middle = middle->left;
          if (left->middle)
          {
            left->middle->parent = left;
          }
          middle->left = middle->middle;
          middle->middle = middle->right;
          middle->right = nullptr;
        }
        else if (right->isTriple())
        {
          parent->data[0] = middle->data[0];
          middle->data[0] = parent->data[1];
          parent->data[1] = right->data[0];
          removeFromNode(right, right->data[0]);
          left->middle = middle->left;
          if (left->middle)
          {
            left->middle->parent = left;
          }
          middle->left = middle->middle;
          middle->middle = right->left;
          if (middle->middle)
          {
            middle->middle->parent = middle;
          }
          right->left = right->middle;
          right->middle = right->right;
          right->right = nullptr;
        }
      }
    }
    else if (parent->isDouble())
    {
      insertIntoNode(leaf, parent->data[0]);
      if (left == leaf && right->isTriple())
      {
        parent->data[0] = right->data[0];
        removeFromNode(right, right->data[0]);
        if (!leaf->left)
        {
          leaf->left = leaf->middle;
        }
        leaf->right = right->left;
        right->left = right->middle;
        right->middle = nullptr;
        if (leaf->right)
        {
          leaf->right->parent = leaf;
        }
      }
      else if (right == leaf && left->isTriple())
      {
        parent->data[0] = left->data[1];
        removeFromNode(left, left->data[1]);
        if (!leaf->right)
        {
          leaf->right = leaf->left;
        }
        leaf->left = left->right;
        left->right = left->middle;
        left->middle = nullptr;
        if (leaf->left)
        {
          leaf->left->parent = leaf;
        }
      }
    }
    return parent;
  }

  template< class K, class V, class C >
  auto Tree< K, V, C >::mergeData(Node* leaf) -> Node*
  {
    Node* parent = leaf->parent;
    if (parent->left == leaf)
    {
      insertIntoNode(parent->right, parent->data[0]);
      parent->right->middle = parent->right->left;
      if (leaf->left)
      {
        parent->right->left = leaf->left;
      }
      else if (leaf->right)
      {
        parent->right->left = leaf->right;
      }
      if (parent->right->left)
      {
        parent->right->left->parent = parent->right;
      }
      removeFromNode(parent, parent->data[0]);
      delete parent->left;
      parent->left = nullptr;
    }
    else if (parent->right == leaf)
    {
      insertIntoNode(parent->left, parent->data[0]);
      parent->left->middle = parent->left->right;
      if (leaf->left)
      {
        parent->left->right = leaf->left;
      }
      else if (leaf->right)
      {
        parent->left->right = leaf->right;
      }
      if (parent->left->right)
      {
        parent->left->right->parent = parent->left;
      }
      removeFromNode(parent, parent->data[0]);
      delete parent->right;
      parent->right = nullptr;
    }
    if (!parent->parent)
    {
      Node* temp = parent->left ? parent->left : parent->right;
      temp->parent = nullptr;
      delete parent;
      return root_ = temp;
    }
    return parent;
  }

  template< class K, class V, class C >
  void Tree< K, V, C >::insertIntoNode(Node* node, const ValueType& value)
  {
    if (node->isEmpty())
    {
      node->data[0] = value;
      node->type = NodeType::Double;
    }
    else if (node->isDouble())
    {
      node->data[1] = value;
      node->type = NodeType::Triple;
      if (comp_(value.first, node->data[0].first))
      {
        std::swap(node->data[0], node->data[1]);
      }
    }
  }

  template< class K, class V, class C >
  void Tree< K, V, C >::removeFromNode(Node* node, const ValueType& value)
  {
    if (node->isDouble())
    {
      node->type = NodeType::Empty;
    }
    else if (node->isTriple())
    {
      if (!comp_(node->data[0].first, value.first) && !comp_(value.first, node->data[0].first))
      {
        std::swap(node->data[0], node->data[1]);
      }
      node->type = NodeType::Double;
    }
  }
}

#endif

