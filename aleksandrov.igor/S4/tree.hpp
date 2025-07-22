#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <stdexcept>
#include "iterator.hpp"

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
    using ValueType = std::pair< K, V >;

    Tree();
    Tree(const Tree&);
    Tree(Tree&&) noexcept;
    ~Tree() noexcept;

    Tree& operator=(const Tree&);
    Tree& operator=(Tree&&) noexcept;

    Iter begin() noexcept;
    ConstIter cbegin() const noexcept;
    Iter end() noexcept;
    ConstIter cend() const noexcept;

    V& at(const K&);
    V& operator[](const K&);

    size_t size() const noexcept;
    bool empty() const noexcept;

    void insert(const ValueType&);
    void erase(Iter);

    void clear() noexcept;
    void swap(Tree&) noexcept;

    Iter find(const K&);
    ConstIter find(const K&) const;
    size_t count(const K&) const;

  private:
    template< class, class, class, bool >
    friend class Iterator;
    using Node = detail::Node< K, V >;

    Node* root_;
    size_t size_;
    C comp_;

    Node* copyRecursive(Node* root, Node* parent = nullptr);
    void clearRecursive(Node*) noexcept;
    std::pair< Node*, PointsTo > findRecursive(Node*, const K&) const;
    Node* findInsertionLeaf(const K&) const;
    void insertUpper(Node* node, Node* left, Node* right, const ValueType& median);
    Node* fixAfterErase(Node*);
    Node* redistributeData(Node*);
    Node* mergeData(Node*);
    void insertToNode(Node*, const ValueType&);
    void removeFromNode(Node*, const ValueType&);
  };

  template< class K, class V, class C >
  Tree< K, V, C >::Tree():
    root_(nullptr),
    size_(0)
  {}

  template< class K, class V, class C >
  Tree< K, V, C >::Tree(const Tree& rhs):
    root_(copyRecursive(rhs.root_)),
    size_(rhs.size_),
    comp_(rhs.comp_)
  {}

  template< class K, class V, class C >
  Tree< K, V, C >::Tree(Tree&& rhs) noexcept:
    root_(std::exchange(rhs.root_, nullptr)),
    size_(std::exchange(rhs.size_, 0)),
    comp_(std::exchange(rhs.comp_, std::less< K >{}))
  {}

  template< class K, class V, class C >
  Tree< K, V, C >::~Tree() noexcept
  {
    clear();
  }

  template< class K, class V, class C >
  Tree< K, V, C >& Tree< K, V, C >::operator=(const Tree& rhs)
  {
    Tree copy(rhs);
    swap(copy);
    return *this;
  }

  template< class K, class V, class C >
  Tree< K, V, C >& Tree< K, V, C >::operator=(Tree&& rhs) noexcept
  {
    Tree copy(std::move(rhs));
    swap(copy);
    return *this;
  }

  template< class K, class V, class C >
  typename Tree< K, V, C >::Iter Tree< K, V, C >::begin() noexcept
  {
    assert(root_ != nullptr && "ERROR: Tree is empty!");
    return Iter(root_).fallLeft();
  }

  template< class K, class V, class C >
  typename Tree< K, V, C >::ConstIter Tree< K, V, C >::cbegin() const noexcept
  {
    assert(root_ != nullptr && "ERROR: Tree is empty!");
    return ConstIter(root_).fallLeft();
  }

  template< class K, class V, class C >
  typename Tree< K, V, C >::Iter Tree< K, V, C >::end() noexcept
  {
    return Iter();
  }

  template< class K, class V, class C >
  typename Tree< K, V, C >::ConstIter Tree< K, V, C >::cend() const noexcept
  {
    return ConstIter();
  }

  template< class K, class V, class C >
  V& Tree< K, V, C >::at(const K& key)
  {
    Iter it(find(key));
    if (it == end())
    {
      throw std::out_of_range("ERROR: No such element exists!");
    }
    return (*it).second;
  }

  template< class K, class V, class C >
  V& Tree< K, V, C >::operator[](const K& key)
  {
    return (*find(key)).second;
  }

  template< class K, class V, class C >
  size_t Tree< K, V, C >::size() const noexcept
  {
    return size_;
  }

  template< class K, class V, class C >
  bool Tree< K, V, C >::empty() const noexcept
  {
    return !size_;
  }

  template< class K, class V, class C >
  void Tree< K, V, C >::insert(const ValueType& p)
  {
    if (find(p.first) != end())
    {
      return;
    }

    if (!root_)
    {
      root_ = new Node(p);
      ++size_;
      return;
    }

    Node* leaf = findInsertionLeaf(p.first);
    if (leaf->isDouble())
    {
      leaf->data[1] = p;
      if (comp_(p.first, leaf->data[0].first))
      {
        std::swap(leaf->data[0], leaf->data[1]);
      }
      leaf->type = detail::NodeType::Triple;
      ++size_;
      return;
    }
    else if (leaf->isTriple())
    {
      const ValueType& min = comp_(p.first, leaf->data[0].first) ? p : leaf->data[0];
      Node* left = new Node(min);
      const ValueType& max = comp_(leaf->data[1].first, p.first) ? p : leaf->data[1];
      Node* right = new Node(max);
      const ValueType& median = p == min ? leaf->data[0] : (p == max ? leaf->data[1] : p);
      insertUpper(leaf, left, right, median);
      ++size_;
    }
    else
    {
      throw std::logic_error("Could not insert an element for some reason...");
    }
  }

  template< class K, class V, class C >
  void Tree< K, V, C >::erase(Iter pos)
  {
    assert(pos != end());
    Node* node = pos.node_;

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
      node->type = detail::NodeType::Double;
      node = fixAfterErase(node);
      --size_;
      return;
    }
    node->type = detail::NodeType::Empty;
    node = fixAfterErase(node);
    --size_;
  }

  template< class K, class V, class C >
  void Tree< K, V, C >::clear() noexcept
  {
    clearRecursive(root_);
    root_ = nullptr;
    size_ = 0;
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
  typename Tree< K, V, C >::Iter Tree< K, V, C >::find(const K& key)
  {
    auto pair = findRecursive(root_, key);
    return Iter(pair.first, pair.second);
  }

  template< class K, class V, class C >
  typename Tree< K, V, C >::ConstIter Tree< K, V, C >::find(const K& key) const
  {
    auto pair = findRecursive(root_, key);
    return ConstIter(pair.first, pair.second);
  }

  template< class K, class V, class C >
  typename Tree< K, V, C >::Node* Tree< K, V, C >::copyRecursive(Node* root, Node* parent)
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
  std::pair< typename Tree< K, V, C >::Node*, PointsTo > Tree< K, V, C >::findRecursive(Node* node, const K& key) const
  {
    if (!node)
    {
      return { nullptr, PointsTo::None };
    }

    if (comp_(key, node->data[0].first))
    {
      return findRecursive(node->left, key);
    }
    else if (node->isDouble())
    {
      if (!comp_(node->data[0].first, key) && !comp_(key, node->data[0].first))
      {
        return { node, PointsTo::Left };
      }
      return findRecursive(node->right, key);
    }
    else if (node->isTriple() && comp_(node->data[1].first, key))
    {
      return findRecursive(node->right, key);
    }
    else if (node->isTriple() && comp_(node->data[0].first, key) && comp_(key, node->data[1].first))
    {
      return findRecursive(node->middle, key);
    }
    else
    {
      if (!comp_(node->data[0].first, key) && !comp_(key, node->data[0].first))
      {
        return { node, PointsTo::Left };
      }
      else if (node->isTriple() && !comp_(node->data[1].first, key) && !comp_(key, node->data[1].first))
      {
        return { node, PointsTo::Right };
      }
      else
      {
        return { nullptr, PointsTo::None };
      }
    }
  }

  template< class K, class V, class C >
  typename Tree< K, V, C >::Node* Tree< K, V, C >::findInsertionLeaf(const K& key) const
  {
    Node* node = root_;
    while (!node->isLeaf())
    {
      if (comp_(key, node->data[0].first))
      {
        node = node->left;
      }
      else if (node->isTriple())
      {
        if (comp_(node->data[1].first, key))
        {
          node = node->right;
        }
        else
        {
          node = node->middle;
        }
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
    }
    else if (parent->isDouble())
    {
      parent->data[1] = median;
      if (comp_(median.first, parent->data[0].first))
      {
        std::swap(parent->data[0], parent->data[1]);
      }
      parent->type = detail::NodeType::Triple;
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
    }
    else if (parent->isTriple())
    {
      const ValueType min = comp_(median.first, parent->data[0].first) ? median : parent->data[0];
      Node* currLeft = new Node(min);
      const ValueType max = comp_(parent->data[1].first, median.first) ? median : parent->data[1];
      Node* currRight = new Node(max);
      const ValueType newMedian = median == min ? parent->data[0] : (median == max ? parent->data[1] : median);
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
    else
    {
      throw std::logic_error("Could not insert upper for some reason...");
    }
  }

  template< class K, class V, class C >
  typename Tree< K, V, C >::Node* Tree< K, V, C >::fixAfterErase(Node* leaf)
  {
    using namespace detail;
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
  typename Tree< K, V, C >::Node* Tree< K, V, C >::redistributeData(Node* leaf)
  {
    using namespace detail;
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
        insertToNode(parent->left, parent->data[0]);
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
        insertToNode(left, parent->data[0]);
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
        insertToNode(middle, parent->data[1]);
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
        insertToNode(leaf, parent->data[1]);
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
          insertToNode(middle, parent->data[1]);
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
          insertToNode(middle, parent->data[0]);
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
        insertToNode(left, parent->data[0]);
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
      insertToNode(leaf, parent->data[0]);
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
  typename Tree< K, V, C >::Node* Tree< K, V, C >::mergeData(Node* leaf)
  {
    Node* parent = leaf->parent;

    if (parent->left == leaf)
    {
      insertToNode(parent->right, parent->data[0]);
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
      insertToNode(parent->left, parent->data[0]);
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
      return root_ = temp;
    }
    return parent;
  }

  template< class K, class V, class C >
  void Tree< K, V, C >::insertToNode(Node* node, const ValueType& value)
  {
    using namespace detail;
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
    else
    {
      throw std::logic_error("Cannot insert into triple node!");
    }
  }

  template< class K, class V, class C >
  void Tree< K, V, C >::removeFromNode(Node* node, const ValueType& value)
  {
    using namespace detail;
    if (node->isDouble())
    {
      node->type = NodeType::Empty;
    }
    else if (node->isTriple())
    {
      if (!comp_(node->data[0].first, value.first) && !comp_(value.first, node->data[0].first))
      {
        std::swap(node->data[0], node->data[1]);
        node->type = NodeType::Double;
      }
      else if (!comp_(node->data[1].first, value.first) && !comp_(value.first, node->data[1].first))
      {
        node->type = NodeType::Double;
      }
      else
      {
        throw std::logic_error("Cannot find a given value in a node!");
      }
    }
    else
    {
      throw std::logic_error("Cannot remove from a null-node!");
    }
  }
}

#endif

