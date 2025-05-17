#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <stack>
#include <utility>

namespace asafov
{
  template< typename Key, typename Value >
  class map
  {
  private:
    enum Color { RED, BLACK };

    struct node
    {
      Key key;
      Value value;
      Color color;
      node* left;
      node* right;
      node* parent;

      node(const Key& k, const Value& v, Color c,
           node* l = nullptr, node* r = nullptr, node* p = nullptr)
        : key(k), value(v), color(c), left(l), right(r), parent(p)
      {
      }
    };

    node* root = nullptr;
    size_t tree_size = 0;

  public:
    map() = default;

    ~map()
    {
      destroy_tree(root);
    }

    Value& operator[](const Key& key)
    {
      node* n = root;
      node* parent = nullptr;

      while (n)
      {
        parent = n;
        if (key == n->key)
          return n->value;
        n = (key < n->key) ? n->left : n->right;
      }

      node* new_node = new node(key, Value(), RED, nullptr, nullptr, parent);
      if (!parent)
      {
        root = new_node;
      }
      else if (key < parent->key)
      {
        parent->left = new_node;
      }
      else
      {
        parent->right = new_node;
      }

      insert_fixup(new_node);
      ++tree_size;
      return new_node->value;
    }

    bool insert(const std::pair< Key, Value >& kv)
    {
      const Key& key = kv.first;
      const Value& value = kv.second;
      node* n = root;
      node* parent = nullptr;

      while (n)
      {
        if (key == n->key)
          return false;
        parent = n;
        n = (key < n->key) ? n->left : n->right;
      }

      node* new_node = new node(key, value, RED, nullptr, nullptr, parent);
      if (!parent)
      {
        root = new_node;
      }
      else if (key < parent->key)
      {
        parent->left = new_node;
      }
      else
      {
        parent->right = new_node;
      }

      insert_fixup(new_node);
      ++tree_size;
      return true;
    }

    bool contains(const Key& key) const
    {
      return find_node(key) != nullptr;
    }

    Value* find(const Key& key)
    {
      node* n = find_node(key);
      return n ? &n->value : nullptr;
    }

    size_t size() const
    {
      return tree_size;
    }

    bool empty() const
    {
      return tree_size == 0;
    }

    // In-order iterator
    class iterator
    {
    private:
      node* current;
      std::stack< node* > stack;

      void push_left(node* n)
      {
        while (n)
        {
          stack.push(n);
          n = n->left;
        }
      }

    public:
      iterator(node* root) : current(nullptr)
      {
        push_left(root);
        ++(*this);
      }

      std::pair< const Key&, Value& > operator*() const
      {
        return {current->key, current->value};
      }

      iterator& operator++()
      {
        if (!stack.empty())
        {
          current = stack.top();
          stack.pop();
          push_left(current->right);
        }
        else
        {
          current = nullptr;
        }
        return *this;
      }

      bool operator!=(const iterator& other) const
      {
        return current != other.current;
      }
    };

    iterator begin() const
    {
      return iterator(root);
    }

    iterator end() const
    {
      return iterator(nullptr);
    }

  private:
    node* find_node(const Key& key) const
    {
      node* n = root;
      while (n)
      {
        if (key == n->key)
          return n;
        n = (key < n->key) ? n->left : n->right;
      }
      return nullptr;
    }

    void left_rotate(node* x)
    {
      node* y = x->right;
      x->right = y->left;
      if (y->left) y->left->parent = x;
      y->parent = x->parent;
      if (!x->parent)
        root = y;
      else if (x == x->parent->left)
        x->parent->left = y;
      else
        x->parent->right = y;
      y->left = x;
      x->parent = y;
    }

    void right_rotate(node* y)
    {
      node* x = y->left;
      y->left = x->right;
      if (x->right) x->right->parent = y;
      x->parent = y->parent;
      if (!y->parent)
        root = x;
      else if (y == y->parent->right)
        y->parent->right = x;
      else
        y->parent->left = x;
      x->right = y;
      y->parent = x;
    }

    void insert_fixup(node* z)
    {
      while (z->parent && z->parent->color == RED)
      {
        if (z->parent == z->parent->parent->left)
        {
          node* y = z->parent->parent->right;
          if (y && y->color == RED)
          {
            z->parent->color = BLACK;
            y->color = BLACK;
            z->parent->parent->color = RED;
            z = z->parent->parent;
          }
          else
          {
            if (z == z->parent->right)
            {
              z = z->parent;
              left_rotate(z);
            }
            z->parent->color = BLACK;
            z->parent->parent->color = RED;
            right_rotate(z->parent->parent);
          }
        }
        else
        {
          node* y = z->parent->parent->left;
          if (y && y->color == RED)
          {
            z->parent->color = BLACK;
            y->color = BLACK;
            z->parent->parent->color = RED;
            z = z->parent->parent;
          }
          else
          {
            if (z == z->parent->left)
            {
              z = z->parent;
              right_rotate(z);
            }
            z->parent->color = BLACK;
            z->parent->parent->color = RED;
            left_rotate(z->parent->parent);
          }
        }
      }
      root->color = BLACK;
    }

    void destroy_tree(node* n)
    {
      if (!n) return;
      destroy_tree(n->left);
      destroy_tree(n->right);
      delete n;
    }
  };
}
#endif
