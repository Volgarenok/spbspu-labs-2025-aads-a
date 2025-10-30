#ifndef MYMAP_HPP
#define MYMAP_HPP
#include <functional>
#include <initializer_list>
#include <utility>
#include <stdexcept>

namespace hismatova
{
  template< typename Key, typename Value >
  class MyMap
  {
  private:
    struct Node
    {
      std::pair< const Key, Value > data;
      Node* left;
      Node* right;
      Node* parent;
      int height;

      Node(const Key& k, const Value& v, Node* p = nullptr):
        data(k, v),
        left(nullptr),
        right(nullptr),
        parent(p),
        height(1)
      {}
    };

    Node* root;
    size_t map_size;

    int height(Node* node) const
    {
      return node ? node->height: 0;
    }

    int balance_factor(Node* node) const
    {
      return node ? height(node->left) - height(node->right): 0;
    }

    void update_height(Node* node)
    {
      if (node)
      {
        node->height = std::max(height(node->left), height(node->right)) + 1;
      }
    }

    Node* rotate_right(Node* y)
    {
      Node* x = y->left;
      Node* T2 = x->right;

      x->right = y;
      y->left = T2;

      if (T2) T2->parent = y;
      x->parent = y->parent;
      y->parent = x;

      update_height(y);
      update_height(x);

      return x;
    }

    Node* rotate_left(Node* x)
    {
      Node* y = x->right;
      Node* T2 = y->left;

      y->left = x;
      x->right = T2;

      if (T2) T2->parent = x;
      y->parent = x->parent;
      x->parent = y;

      update_height(x);
      update_height(y);

      return y;
    }

    Node* balance(Node* node)
    {
      if (!node)
      {
        return nullptr;
      }

      update_height(node);

      int bf = balance_factor(node);

      if (bf > 1 && balance_factor(node->left) >= 0)
      {
        return rotate_right(node);
      }

      if (bf < -1 && balance_factor(node->right) <= 0)
      {
        return rotate_left(node);
      }

      if (bf > 1 && balance_factor(node->left) < 0)
      {
        node->left = rotate_left(node->left);
        return rotate_right(node);
      }

      if (bf < -1 && balance_factor(node->right) > 0)
      {
        node->right = rotate_right(node->right);
        return rotate_left(node);
      }

      return node;
    }

    Node* insert(Node* node, Node* parent, const Key& key, const Value& value, bool& inserted)
    {
      if (!node)
      {
        inserted = true;
        return new Node(key, value, parent);
      }

      if (key < node->data.first)
      {
        node->left = insert(node->left, node, key, value, inserted);
      }
      else if (key > node->data.first)
      {
        node->right = insert(node->right, node, key, value, inserted);
      }
      else
      {
        node->data.second = value;
        inserted = false;
        return node;
      }

      return balance(node);
    }

    Node* find_node(Node* node, const Key& key) const
    {
      while (node)
      {
        if (key < node->data.first)
        {
          node = node->left;
        }
        else if (key > node->data.first)
        {
          node = node->right;
        }
        else
        {
          return node;
        }
      }
      return nullptr;
    }

    Node* min_value_node(Node* node) const
    {
      Node* current = node;
      while (current && current->left)
      {
        current = current->left;
      }
      return current;
    }

    Node* erase(Node* node, const Key& key, bool& erased)
    {
      if (!node)
      {
        erased = false;
        return nullptr;
      }

      if (key < node->data.first)
      {
        node->left = erase(node->left, key, erased);
      }
      else if (key > node->data.first)
      {
        node->right = erase(node->right, key, erased);
      }
      else
      {
        erased = true;

        if (!node->left || !node->right)
        {
          Node* temp = node->left ? node->left: node->right;

          if (temp)
          {
            temp->parent = node->parent;
          }

          if (!temp)
          {
            temp = node;
            node = nullptr;
          }
          else
          {
            Node* to_delete = node;
            node = temp;
            temp = to_delete;
          }

          delete temp;
        }
        else
        {
          Node* temp = min_value_node(node->right);
          const_cast< Key& >(node->data.first) = temp->data.first;
          node->data.second = temp->data.second;
          node->right = erase(node->right, temp->data.first, erased);
        }
      }

      if (!node)
      {
        return nullptr;
      }

      return balance(node);
    }

    Node* erase_by_node(Node* node, Node* to_erase, bool& erased)
    {
      if (!node || !to_erase)
      {
        erased = false;
        return node;
      }

      if (to_erase->data.first < node->data.first)
      {
        node->left = erase_by_node(node->left, to_erase, erased);
      }
      else if (to_erase->data.first > node->data.first)
      {
        node->right = erase_by_node(node->right, to_erase, erased);
      }
      else
      {
        erased = true;

        if (!node->left || !node->right)
        {
          Node* temp = node->left ? node->left: node->right;

          if (temp)
          {
            temp->parent = node->parent;
          }
          if (!temp)
          {
            temp = node;
            node = nullptr;
          }
          else
          {
            Node* to_delete = node;
            node = temp;
            temp = to_delete;
          }

          delete temp;
        }
        else
        {
          Node* temp = min_value_node(node->right);
          const_cast< Key& >(node->data.first) = temp->data.first;
          node->data.second = temp->data.second;
          node->right = erase(node->right, temp->data.first, erased);
        }
      }

      if (!node)
      {
        return nullptr;
      }

      return balance(node);
    }

    void clear(Node* node)
    {
      if (node)
      {
        clear(node->left);
        clear(node->right);
        delete node;
      }
    }

    Node* copy_tree(Node* node, Node* parent) const
    {
      if (!node)
      {
        return nullptr;
      }

      Node* new_node = new Node(node->data.first, node->data.second, parent);
      new_node->left = copy_tree(node->left, new_node);
      new_node->right = copy_tree(node->right, new_node);
      new_node->height = node->height;

      return new_node;
    }

  public:
    class iterator
    {
    private:
      Node* current;

      Node* find_successor(Node* node)
      {
        if (!node)
        {
          return nullptr;
        }

        if (node->right)
        {
          node = node->right;
          while (node->left)
          {
            node = node->left;
          }
          return node;
        }

        Node* parent = node->parent;
        while (parent && node == parent->right)
        {
          node = parent;
          parent = parent->parent;
        }
        return parent;
      }

      Node* find_predecessor(Node* node)
      {
        if (!node)
        {
          return nullptr;
        }

        if (node->left)
        {
          node = node->left;
          while (node->right)
          {
            node = node->right;
          }
          return node;
        }

        Node* parent = node->parent;
        while (parent && node == parent->left)
        {
          node = parent;
          parent = parent->parent;
        }
        return parent;
      }

    public:
      using iterator_category = std::forward_iterator_tag;
      using value_type = std::pair< const Key, Value >;
      using difference_type = std::ptrdiff_t;
      using pointer = value_type*;
      using reference = value_type&;

      iterator(Node* node = nullptr):
        current(node)
      {}

      reference operator*() const
      {
        return current->data;
      }
      pointer operator->() const
      {
        return &current->data;
      }

      iterator& operator++()
      {
        current = find_successor(current);
        return *this;
      }

      iterator operator++(int)
      {
        iterator temp = *this;
        ++(*this);
        return temp;
      }

      iterator& operator--()
      {
        current = find_predecessor(current);
        return *this;
      }

      iterator operator--(int)
      {
        iterator temp = *this;
        --(*this);
        return temp;
      }

      bool operator==(const iterator& other) const
      {
        return current == other.current;
      }
      bool operator!=(const iterator& other) const
      {
        return current != other.current;
      }

      Node* get_node() const
      {
        return current;
      }

      friend class MyMap;
    };

    class const_iterator
    {
    private:
      const Node* current;

      const Node* find_successor(const Node* node)
      {
        if (!node)
        {
          return nullptr;
        }

        if (node->right)
        {
          node = node->right;
          while (node->left)
          {
            node = node->left;
          }
          return node;
        }

        const Node* parent = node->parent;
        while (parent && node == parent->right)
        {
          node = parent;
          parent = parent->parent;
        }
        return parent;
      }

      const Node* find_predecessor(const Node* node) 
      {
        if (!node)
        {
          return nullptr;
        }

        if (node->left)
        {
          node = node->left;
          while (node->right)
          {
            node = node->right;
          }
          return node;
        }

        const Node* parent = node->parent;
        while (parent && node == parent->left)
        {
          node = parent;
          parent = parent->parent;
        }
        return parent;
      }

    public:
      using iterator_category = std::forward_iterator_tag;
      using value_type = std::pair< const Key, Value >;
      using difference_type = std::ptrdiff_t;
      using pointer = const value_type*;
      using reference = const value_type&;

      const_iterator(const Node* node = nullptr):
        current(node)
      {}

      reference operator*() const
      {
        return current->data;
      }
      pointer operator->() const 
      {
        return &current->data;
      }

      const_iterator& operator++()
      {
        current = find_successor(current);
        return *this;
      }

      const_iterator operator++(int)
      {
        const_iterator temp = *this;
        ++(*this);
        return temp;
      }

      const_iterator& operator--()
      {
        current = find_predecessor(current);
        return *this;
      }

      const_iterator operator--(int)
      {
        const_iterator temp = *this;
        --(*this);
        return temp;
      }

      bool operator==(const const_iterator& other) const
      {
        return current == other.current;
      }
      bool operator!=(const const_iterator& other) const
      {
        return current != other.current;
      }

      const Node* get_node() const
      {
        return current;
      }
    };

    MyMap():
      root(nullptr),
      map_size(0)
    {}

    MyMap(std::initializer_list< std::pair< const Key, Value > > init):
      root(nullptr),
      map_size(0)
    {
      for (const auto& pair: init)
      {
        insert(pair.first, pair.second);
      }
    }

    MyMap(const MyMap& other):
      root(nullptr),
      map_size(0)
    {
      root = copy_tree(other.root, nullptr);
      map_size = other.map_size;
    }

    MyMap(MyMap&& other) noexcept:
      root(other.root),
      map_size(other.map_size)
    {
      other.root = nullptr;
      other.map_size = 0;
    }

    ~MyMap()
    {
      clear();
    }

    MyMap& operator=(const MyMap& other)
    {
      if (this != &other)
      {
        clear();
        root = copy_tree(other.root, nullptr);
        map_size = other.map_size;
      }
      return *this;
    }

    MyMap& operator=(MyMap&& other) noexcept
    {
      if (this != &other)
      {
        clear();
        root = other.root;
        map_size = other.map_size;
        other.root = nullptr;
        other.map_size = 0;
      }
      return *this;
    }

    bool empty() const
    {
      return map_size == 0;
    }
    size_t size() const
    {
      return map_size;
    }

    Value& operator[](const Key& key)
    {
      bool inserted;
      root = insert(root, nullptr, key, Value(), inserted);
      if (inserted)
      {
        map_size++;
      }
      return find_node(root, key)->data.second;
    }

    Value& at(const Key& key)
    {
      Node* node = find_node(root, key);
      if (!node)
      {
        throw std::out_of_range("Key not found in MyMap");
      }
      return node->data.second;
    }

    const Value& at(const Key& key) const
    {
      const Node* node = find_node(root, key);
      if (!node)
      {
        throw std::out_of_range("Key not found in MyMap");
      }
      return node->data.second;
    }

    std::pair< iterator, bool > insert(const Key& key, const Value& value)
    {
      bool inserted;
      root = insert(root, nullptr, key, value, inserted);
      if (inserted)
      {
        map_size++;
      }
      return std::make_pair(iterator(find_node(root, key)), inserted);
    }

    std::pair< iterator, bool > insert(const std::pair< const Key, Value >& pair)
    {
      return insert(pair.first, pair.second);
    }

    bool erase(const Key& key)
    {
      bool erased;
      root = erase(root, key, erased);
      if (erased)
      {
        map_size--;
      }
      return erased;
    }

    iterator erase(iterator pos)
    {
      if (pos == end())
      {
        return end();
      }

      Node* next_node = pos.get_node();
      ++pos;

      bool erased;
      root = erase_by_node(root, next_node, erased);
      if (erased) map_size--;

      return iterator(next_node);
    }

    void clear()
    {
      clear(root);
      root = nullptr;
      map_size = 0;
    }

    iterator find(const Key& key)
    {
      return iterator(find_node(root, key));
    }

    const_iterator find(const Key& key) const
    {
      return const_iterator(find_node(root, key));
    }

    bool contains(const Key& key) const
    {
      return find_node(root, key) != nullptr;
    }

    size_t count(const Key& key) const
    {
      return find_node(root, key) ? 1: 0;
    }

    iterator begin()
    {
      return iterator(min_value_node(root));
    }

    const_iterator begin() const
    {
      return const_iterator(min_value_node(root));
    }

    const_iterator cbegin() const
    {
      return const_iterator(min_value_node(root));
    }

    iterator end()
    {
      return iterator(nullptr);
    }

    const_iterator end() const
    {
      return const_iterator(nullptr);
    }

    const_iterator cend() const
    {
      return const_iterator(nullptr);
    }
  };
}
#endif
