#ifndef BINARYSEACHTREE_HPP
#define BINARYSEACHTREE_HPP
#include <iostream>
#include <initializer_list>
#include <utility>
#include <iterator>
#include <algorithm>
#include <stack>
#include <functional>
#include <string>
#include <cstddef>
#include <memory>

namespace hismatova
{

  template< typename Key, typename Value, typename Compare = std::less< Key > >
  class BinarySearchTree
  {
  private:
    struct Node
    {
      Key key;
      Value value;
      Node* left = nullptr;
      Node* right = nullptr;
      int height = 1;

      Node(const Key& k, const Value& v):
        key(k),
        value(v)
      {}
    };

    Node* root = nullptr;
    size_t node_count = 0;
    Compare comp;

  public:
    BinarySearchTree() = default;

    BinarySearchTree(std::initializer_list< std::pair< const Key, Value > > init)
    {
      for (const auto& kv: init)
      {
        insert(kv);
      }
    }

    template< typename InputIt >
    BinarySearchTree(InputIt first, InputIt last)
    {
      for (auto it = first; it != last; ++it)
      {
        insert(*it);
      }
    }

    ~BinarySearchTree()
    {
      clear();
    }

    BinarySearchTree(const BinarySearchTree& other)
    {
      for (const auto& kv: other)
      {
        insert(kv);
      }
    }

    BinarySearchTree& operator=(const BinarySearchTree& other)
    {
      if (this != &other)
      {
        clear();
        for (const auto& kv: other)
        {
          insert(kv);
        }
      }
      return *this;
    }

    BinarySearchTree(BinarySearchTree&& other) noexcept:
      root(other.root),
      node_count(other.node_count)
    {
      other.root = nullptr;
      other.node_count = 0;
    }

    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept
    {
      if (this != &other)
      {
        clear();
        root = other.root;
        node_count = other.node_count;
        other.root = nullptr;
        other.node_count = 0;
      }
      return *this;
    }

    size_t size() const
    {
      return node_count;
    }

    bool empty() const
    {
      return node_count == 0;
    }

    void clear()
    {
      clear(root);
      root = nullptr;
      node_count = 0;
    }

    void swap(BinarySearchTree& other) noexcept
    {
      std::swap(root, other.root);
      std::swap(node_count, other.node_count);
    }

    void insert(const std::pair< const Key, Value >& kv)
    {
      root = insert(root, kv.first, kv.second);
    }

    void erase(const Key& key)
    {
      root = erase(root, key);
    }

    size_t count(const Key& key) const
    {
      return find(key) != end() ? 1 : 0;
    }

    auto lower_bound(const Key& key)
    {
      return LowerUpperBound(key, true);
    }

    auto upper_bound(const Key& key)
    {
      return LowerUpperBound(key, false);
    }

    auto equal_range(const Key& key)
    {
      return std::make_pair(lower_bound(key), upper_bound(key));
    }

    class iterator
    {
    public:
      using iterator_category = std::forward_iterator_tag;
      using value_type = std::pair< const Key, Value >;
      using difference_type = std::ptrdiff_t;
      using pointer = value_type*;
      using reference = value_type&;

      iterator() = default;

      iterator(Node* root)
      {
        push_left(root);
        if (!stack.empty())
        {
          current = std::make_unique< value_type >(stack.top()->key, stack.top()->value);
        }
      }

      reference operator*() const
      {
        return *current;
      }

      pointer operator->() const
      {
        return current.get();
      }

      iterator& operator++()
      {
        advance();
        return *this;
      }

      iterator operator++(int)
      {
        iterator tmp = *this;
        ++(*this);
        return tmp;
      }

      bool operator==(const iterator& other) const
      {
        return stack.empty() && other.stack.empty();
      }

      bool operator!=(const iterator& other) const
      {
        return !(*this == other);
      }

    private:
      std::stack< Node* > stack;
      std::unique_ptr< value_type > current;

      void push_left(Node* node)
      {
        while (node)
        {
          stack.push(node);
          node = node->left;
        }
      }

      void advance()
      {
        if (stack.empty())
        {
          current.reset();
          return;
        }
        Node* node = stack.top(); stack.pop();
        push_left(node->right);
        if (stack.empty())
        {
          current.reset();
        }
        else
        {
          current = std::make_unique< value_type >(stack.top()->key, stack.top()->value);
        }
      }
    };

    class const_iterator
    {
    public:
      using iterator_category = std::forward_iterator_tag;
      using value_type = std::pair< const Key, Value >;
      using difference_type = std::ptrdiff_t;
      using pointer = const value_type*;
      using reference = const value_type&;

      const_iterator() = default;

      const_iterator(Node* root)
      {
        push_left(root);
        if (!stack.empty())
        {
          current = std::make_unique< value_type >(stack.top()->key, stack.top()->value);
        }
      }

      reference operator*() const
      {
        return *current;
      }

      pointer operator->() const
      {
        return current.get();
      }

      const_iterator& operator++()
      {
        advance();
        return *this;
      }

      const_iterator operator++(int)
      {
        const_iterator tmp = *this;
        ++(*this);
        return tmp;
      }

      bool operator==(const const_iterator& other) const
      {
        return stack.empty() && other.stack.empty();
      }

      bool operator!=(const const_iterator& other) const
      {
        return !(*this == other);
      }

    private:
      std::stack< Node* > stack;
      std::unique_ptr< value_type > current;

      void push_left(Node* node)
      {
        while (node)
        {
          stack.push(node);
          node = node->left;
        }
      }

      void advance()
      {
        if (stack.empty())
        {
          current.reset();
          return;
        }
        Node* node = stack.top(); stack.pop();
        push_left(node->right);
        if (stack.empty())
        {
          current.reset();
        }
        else
        {
          current = std::make_unique< value_type >(stack.top()->key, stack.top()->value);
        }
      }
    };

    iterator begin()
    {
      return iterator(root);
    }

    iterator end()
    {
      return iterator();
    }

    const_iterator begin() const
    {
      return const_iterator(root);
    }

    const_iterator end() const
    {
      return const_iterator();
    }

    iterator find(const Key& key)
    {
      Node* node = root;
      while (node)
      {
        if (comp(key, node->key))
        {
          node = node->left;
        }
        else if (comp(node->key, key))
        {
          node = node->right;
        }
        else
        {
          iterator it;
          it.push_left(node);
          if (!it.stack.empty())
          {
            it.current = std::make_unique< std::pair< const Key, Value > >(it.stack.top()->key, it.stack.top()->value);
          }
          return it;
        }
      }
      return end();
    }

  private:
    int height(Node* node) const
    {
      return node ? node->height : 0;
    }

    int balance_factor(Node* node) const
    {
      return height(node->right) - height(node->left);
    }

    void update_height(Node* node)
    {
      node->height = 1 + std::max(height(node->left), height(node->right));
    }

    Node* rotate_right(Node* y)
    {
      Node* x = y->left;
      y->left = x->right;
      x->right = y;
      update_height(y);
      update_height(x);
      return x;
    }

    Node* rotate_left(Node* x)
    {
      Node* y = x->right;
      x->right = y->left;
      y->left = x;
      update_height(x);
      update_height(y);
      return y;
    }

    Node* balance(Node* node)
    {
      update_height(node);
      int bf = balance_factor(node);
      if (bf == 2)
      {
        if (balance_factor(node->right) < 0)
        {
          node->right = rotate_right(node->right);
        }
        return rotate_left(node);
      }
      if (bf == -2)
      {
        if (balance_factor(node->left) > 0)
        {
          node->left = rotate_left(node->left);
        }
        return rotate_right(node);
      }
      return node;
    }

    Node* insert(Node* node, const Key& key, const Value& value)
    {
      if (!node)
      {
        ++node_count;
        return new Node(key, value);
      }
      if (comp(key, node->key))
      {
        node->left = insert(node->left, key, value);
      }
      else if (comp(node->key, key))
      {
        node->right = insert(node->right, key, value);
      }
      else
      {
        node->value = value;
      }
      return balance(node);
    }

    Node* find_min(Node* node) const
    {
      return node->left ? find_min(node->left) : node;
    }

    Node* remove_min(Node* node)
    {
      if (!node->left)
      {
        return node->right;
      }
      node->left = remove_min(node->left);
      return balance(node);
    }

    Node* erase(Node* node, const Key& key)\
    {
      if (!node)
      {
        return nullptr;
      }
      if (comp(key, node->key))
      {
        node->left = erase(node->left, key);
      }
      else if (comp(node->key, key))
      {
        node->right = erase(node->right, key);
      }
      else
      {
        Node* l = node->left;
        Node* r = node->right;
        delete node;
        --node_count;
        if (!r)
        {
          return l;
        }
        Node* min = find_min(r);
        min->right = remove_min(r);
        min->left = l;
        return balance(min);
      }
      return balance(node);
    }

    void clear(Node* node)
    {
      if (!node)
      {
        return;
      }
      clear(node->left);
      clear(node->right);
      delete node;
    }

    iterator LowerUpperBound(const Key& key, bool is_lower)
    {
      Node* node = root;
      Node* candidate = nullptr;
      while (node)
      {
        if ((is_lower && !comp(node->key, key)) || (!is_lower && comp(key, node->key)))
        {
          candidate = node;
          node = node->left;
        }
        else
        {
          node = node->right;
        }
      }
      if (!candidate)
      {
        return end();
      }
      iterator it;
      it.push_left(candidate);
      if (!it.stack.empty())
      {
        it.current = std::make_unique< std::pair< const Key, Value > >(it.stack.top()->key, it.stack.top()->value);
      }
      return it;
    }
  };

}

#endif
