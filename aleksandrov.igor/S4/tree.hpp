#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <stdexcept>
#include <stack>
#include <queue>
#include "iterator.hpp"

namespace aleksandrov
{
  template< typename K, typename V, typename C, bool isConst >
  class Iterator;

  template< typename K, typename V, typename C = std::less< K > >
  class Tree
  {
  public:
    Tree();
    Tree(const Tree&);
    Tree(Tree&&) noexcept;
    ~Tree();

    size_t size() const noexcept;
    bool empty() const noexcept;

    template< bool isConst >
    Iterator< K, V, C, false > begin() const noexcept;
    template< bool isConst >
    Iterator< K, V, C, false > end() const noexcept;

    void clear() noexcept;
    void swap(Tree&) noexcept;

    const V& at(const K&) const;
    V& operator[](const K&);

    bool insert(const K&, const V&);
  private:
    template< bool isConst >
    friend class Iterator;
    using Node = detail::Node< K, V >;
    Node* root_;
    size_t size_;
    C comp_;

    Node* copy(Node*);
  };

  template< typename K, typename V, typename C >
  Tree< K, V, C >::Tree():
    root_(nullptr),
    size_(0)
  {}

  template< typename K, typename V, typename C >
  Tree< K, V, C >::Tree(const Tree& rhs):
    root_(nullptr),
    size_(rhs.size_),
    comp_(rhs.comp_)
  {
    root_ = copy(rhs.root_);
  }

  template< typename K, typename V, typename C >
  typename Tree< K, V, C >::Node* Tree< K, V, C >::copy(Node* root)
  {
    if (!root)
    {
      return nullptr;
    }
    Tree* copyRoot = new Tree(root->data[0], root->data[1]);
    std::stack< std::pair< Node*, Node* > > stack;
    stack.push(std::make_pair(root, copyRoot));
    try
    {
      while (!stack.empty())
      {
        std::pair< Node*, Node* > pair = stack.top();
        Node* node = pair.first;
        Node* copyNode = pair.second;
        stack.pop();
        if (node->left)
        {
          copyNode->left = new Node(node->left->data[0], node->left->data[1]);
          stack.push(std::make_pair(node->left, copyNode->left));
        }
        if (node->right)
        {
          copyNode->right = new Node(node->right->data[0], node->right->data[1]);
          stack.push(std::make_pair(node->right, copyNode->right));
        }
      }
    }
    catch (const std::bad_alloc&)
    {
      copyRoot->clear();
      throw;
    }
    return copyRoot;
  }

  template< typename K, typename V, typename C >
  Tree< K, V, C >::Tree(Tree&& rhs) noexcept:
    root_(std::move(rhs.root_)),
    size_(std::move(rhs.size_)),
    comp_(std::move(rhs.comp_))
  {
    rhs.root_ = nullptr;
    rhs.size_ = 0;
    rhs.comp_ = std::less< K >();
  }

  template< typename K, typename V, typename C >
  Tree< K, V, C >::~Tree()
  {
    clear();
  }

  template< typename K, typename V, typename C >
  template< bool isConst >
  Iterator< K, V, C, false > Tree< K, V, C >::begin() const noexcept
  {
    assert(root_);
    if (root_->left)
    {
      return Iterator< K, V, C, false >(root_->left).fallLeft();
    }
    else if (root_->middle)
    {
      return Iterator< K, V, C, false >(root_->middle).fallLeft();
    }
    return Iterator< K, V, C, false >(root_);
  }

  template< typename K, typename V, typename C >
  template< bool isConst >
  Iterator< K, V, C, false > Tree< K, V, C >::end() const noexcept
  {
    assert(root_);
    if (root_->right)
    {
      return Iterator< K, V, C, false >(root_->right).fallRight();
    }
    else if (root_->middle)
    {
      return Iterator< K, V, C, false >(root_->middle).fallRight();
    }
    return Iterator< K, V, C, false >(root_);
  }

  template< typename K, typename V, typename C >
  size_t Tree< K, V, C >::size() const noexcept
  {
    return size_;
  }

  template< typename K, typename V, typename C >
  bool Tree< K, V, C >::empty() const noexcept
  {
    return !size_;
  }

  template< typename K, typename V, typename C >
  void Tree< K, V, C >::clear() noexcept
  {
    if (!root_)
    {
      return;
    }
    std::queue< Node* > nodes;
    nodes.push(root_);
    while (!nodes.empty())
    {
      Node* curr = nodes.front();
      nodes.pop();

      if (curr->left)
      {
        nodes.push(curr->left);
      }
      if (curr->right)
      {
        nodes.push(curr->right);
      }
      delete curr;
    }
    root_ = nullptr;
  }

  template< typename K, typename V, typename C >
  void Tree< K, V, C >::swap(Tree& rhs) noexcept
  {
    std::swap(root_, rhs.root_);
    std::swap(size_, rhs.size_);
    std::swap(comp_, rhs.comp_);
  }
}

#endif

