#ifndef MAP_HPP
#define MAP_HPP
#include <utility>
#include <stdexcept>

namespace asafov
{
  template< typename Key, typename Value >
  class map
  {
    enum Color: char
    {
      RED,
      BLACK
    };

    struct node
    {
      std::pair< Key, Value > data;
      node* left = nullptr;
      node* right = nullptr;
      node* parent = nullptr;
      Color color = RED;

      explicit node(const std::pair< Key, Value >& val):
        data(val)
      {}
    };

    void rotateLeft(node* x)
    {
      node* y = x->right;
      x->right = y->left;
      if (y->left)
      {
        y->left->parent = x;
      }
      y->parent = x->parent;
      if (!x->parent)
      {
        root_ = y;
      }
      else if (x == x->parent->left)
      {
        x->parent->left = y;
      }
      else
      {
        x->parent->right = y;
      }
      y->left = x;
      x->parent = y;
    }
    void rotateRight(node* y)
    {
      node* x = y->left;
      y->left = x->right;
      if (x->right)
      {
        x->right->parent = y;
      }
      x->parent = y->parent;
      if (!y->parent)
      {
        root_ = x;
      }
      else if (y == y->parent->right)
      {
        y->parent->right = x;
      }
      else
      {
        y->parent->left = x;
      }
      x->right = y;
      y->parent = x;
    }
    void balance(node* z)
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
              rotateLeft(z);
            }
            z->parent->color = BLACK;
            z->parent->parent->color = RED;
            rotateRight(z->parent->parent);
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
              rotateRight(z);
            }
            z->parent->color = BLACK;
            z->parent->parent->color = RED;
            rotateLeft(z->parent->parent);
          }
        }
      }
      root_->color = BLACK;
    }

    node* findNode(const Key& key) const
    {
      node* current = root_;
      while (current)
      {
        if (key == current->data.first)
        {
          return current;
        }
        if (key < current->data.first)
        {
          current = current->left;
        }
        else
        {
          current = current->right;
        }
      }
      return nullptr;
    }

  public:
    map() = default;
    map(const map& other) : root_(nullptr)
    {
      for (auto it = other.begin(); it != other.end(); ++it)
      {
        (*this)[it->first] = it->second;
      }
    }
    map& operator=(const map& other)
    {
      if (this != &other)
      {
        clear(root_);
        root_ = nullptr;
        for (auto it = other.begin(); it != other.end(); ++it)
        {
          (*this)[it->first] = it->second;
        }
      }
      return *this;
    }
    map(map&& other) noexcept : root_(other.root_)
    {
      other.root_ = nullptr;
    }
    map& operator=(map&& other) noexcept
    {
      if (this != &other)
      {
        clear(root_);
        root_ = other.root_;
        other.root_ = nullptr;
      }
      return *this;
    }
    ~map()
    {
      clear(root_);
    }

    bool insert(const Key& k, const Value& v)
    {
      node* current = root_;
      node* parent = nullptr;
      while (current)
      {
        if (k == current->data.first)
        {
          return false;
        }
        parent = current;
        if (k < current->data.first)
        {
          current = current->left;
        }
        else
        {
          current = current->right;
        }
      }
      node* temp = new node({k, v});
      temp->parent = parent;
      if (!parent)
      {
        root_ = temp;
      }
      else if (k < parent->data.first)
      {
        parent->left = temp;
      }
      else
      {
        parent->right = temp;
      }
      balancing(temp);
      ++size_;
      return true;
    }
    Value& operator[](const Key& key)
    {
      node* current = root_;
      node* parent = nullptr;
      while (current)
      {
        if (key == current->data.first)
        {
          return current->data.second;
        }
        parent = current;
        if (key < current->data.first)
        {
          current = current->left;
        }
        else
        {
          current = current->right;
        }
      }
      node* temp = new node({key, Value{}});
      temp->parent = parent;
      if (!parent)
      {
        root_ = temp;
      }
      else if (key < parent->data.first)
      {
        parent->left = temp;
      }
      else
      {
        parent->right = temp;
      }
      balance(temp);
      return temp->data.second;
    }
    Value& at(const Key& key)
    {
      node* node = findNode(key);
      if (!node)
      {
        throw std::out_of_range("key not found!");
      }
      return node->data.second;
    }
    const Value& at(const Key& key) const
    {
      node* node = findNode(key);
      if (!node)
      {
        throw std::out_of_range("key not found!");
      }
      return node->data.second;
    }

    void clear(node* node)
    {
      if (!node)
      {
        return;
      }
      clear(node->left);
      clear(node->right);
      node->left = node->right = nullptr;
      node->parent = nullptr;
      delete node;
    }

    size_t size() const
    {
      return size_;
    }
    bool empty() const
    {
      return root_ == nullptr;
    }

    void swap(map& other) noexcept
    {
      std::swap(root_, other.root_);
    }

    class iterator
    {
      void goLeftmost()
      {
        while (current && current->left) current = current->left;
      }

    public:
      iterator(node* node):
        current(node)
      {
        goLeftmost();
      }

      std::pair< Key, Value >& operator*() const
      {
        return current->data;
      }
      std::pair< Key, Value >* operator->() const
      {
        return &(current->data);
      }

      iterator& operator++()
      {
        if (!current) return *this;
        if (current->right)
        {
          current = current->right;
          while (current->left) current = current->left;
        }
        else
        {
          node* p = current->parent;
          while (p && current == p->right)
          {
            current = p;
            p = p->parent;
          }
          current = p;
        }
        return *this;
      }

      bool operator!=(const iterator& other) const
      {
        return current != other.current;
      }
      bool operator==(const iterator& other) const
      {
        return current == other.current;
      }
    private:
      node* current;
    };

    iterator begin() const
    {
      return iterator(root_);
    }
    iterator end() const
    {
      return iterator(nullptr);
    }
    iterator find(const Key& key) const
    {
      return iterator(findNode(key));
    }

  private:
    node* root_ = nullptr;
    size_t size_ = 0;
  };
}
#endif
