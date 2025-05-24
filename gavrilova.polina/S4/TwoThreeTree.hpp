#ifndef TWO_THREE_TREE_HPP
#define TWO_THREE_TREE_HPP

#include <functional>
#include <iostream>
#include <stdexcept>
#include "NodeTTT.hpp"

namespace detail {

}
namespace gavrilova {
  template < class Key, class Value, class Cmp = std::less< Key > >
  class TwoThreeTree {
  public:
    using Node = NodeTwoThreeTree< Key, Value >;
    using this_t = TwoThreeTree< Key, Value, Cmp >;
    using pair = std::pair< Key, Value >;

    TwoThreeTree();
    TwoThreeTree(const TwoThreeTree& other);
    TwoThreeTree(TwoThreeTree&& other) noexcept;
    // TwoThreeTree(std::initializer_list<Value> init);
    ~TwoThreeTree();

    TwoThreeTree& operator=(const TwoThreeTree& other);
    TwoThreeTree& operator=(TwoThreeTree&& other) noexcept;
    // TwoThreeTree &operator=(std::initializer_list<Value> init);

    void push(const Key& key, const Value& value);
    Value get(const Key& key);
    Node* find(const Key& key);
    bool empty() const noexcept;
    size_t size() const noexcept;

    void swap(TwoThreeTree& other) noexcept;
    void clear() noexcept;
    Node* get_node() { return fake_->children[0]; }

  private:
    Node* fake_;
    size_t size_;
    Cmp cmp_;

    void clear_recursive(Node* node) noexcept;
    Node* find_leaf(const Key& key);
    void push_to_2node(Node* node, const Key& key, const Value& value);
    Node* split_leaf(Node* leaf, const Key& key, const Value& value);
    void split_and_up(Node* to_up);
    bool is_leaf(Node* node);
  };

  template < class Key, class Value, class Cmp >
  TwoThreeTree< Key, Value, Cmp >::TwoThreeTree():
    fake_(reinterpret_cast< Node* >(new char[sizeof(Node)])),
    size_(0)
  {
    fake_->children[0] = nullptr;
    fake_->children[1] = nullptr;
    fake_->children[2] = nullptr;
    fake_->parent = fake_;
    fake_->is_3_node = false;
    fake_->key_count = 0;
  }

  template < class Key, class Value, class Cmp >
  TwoThreeTree< Key, Value, Cmp >::TwoThreeTree(const TwoThreeTree& other):
    TwoThreeTree()
  {
  }

  template < class Key, class Value, class Cmp >
  TwoThreeTree< Key, Value, Cmp >::TwoThreeTree(TwoThreeTree&& other) noexcept:
    fake_(other.fake_),
    size_(other.size_),
    cmp_(other.cmp_)
  {
    other.fake_ = nullptr;
    other.size_ = 0;
  }

  template < class Key, class Value, class Cmp >
  TwoThreeTree< Key, Value, Cmp >::~TwoThreeTree()
  {
    clear();
    delete[] reinterpret_cast< char* >(fake_);
  }

  template < class Key, class Value, class Cmp >
  TwoThreeTree< Key, Value, Cmp >& TwoThreeTree< Key, Value, Cmp >::operator=(const TwoThreeTree& other)
  {
    if (this != std::addressof(other)) {
      TwoThreeTree< Key, Value, Cmp > cpy(other);
      swap(cpy);
    }
    return *this;
  }

  template < class Key, class Value, class Cmp >
  TwoThreeTree< Key, Value, Cmp >& TwoThreeTree< Key, Value, Cmp >::operator=(TwoThreeTree&& other) noexcept
  {
    if (this == std::addressof(other)) {
      return *this;
    }
    clear();
    delete[] reinterpret_cast< char* >(fake_);
    fake_ = other.fake_;
    size_ = other.size_;
    other.fake_ = nullptr;
    other.size_ = 0;

    return *this;
  }

  template < class Key, class Value, class Cmp >
  void TwoThreeTree< Key, Value, Cmp >::push(const Key& key, const Value& value)
  {
    // std::cout << "! in push function\n";
    if (empty()) {
      std::cout << "PUSH IN EMPTY\n";
      Node* new_node = new Node();
      pair new_pair{key, value};
      new_node->data[0] = new_pair;
      new_node->key_count = 1;
      new_node->parent = fake_;
      new_node->children[0] = fake_;
      new_node->children[1] = fake_;
      new_node->children[2] = fake_;
      fake_->children[0] = new_node;
      ++size_;
      return;
    }

    Node* leaf = find_leaf(key);
    // std::cout << "FInd leaf: " << leaf->data[0].first << " " << leaf->data[1].first << "\n";
    if (!leaf->is_3_node) {
      std::cout << "PUSH IN 2NODE \n";
      push_to_2node(leaf, key, value);
    } else {
      std::cout << "PUSH IN 3NODE \n";
      Node* parent = leaf->parent;
      size_t ind = parent->children[0] == leaf ? 0 : 1;
      ind = parent->children[ind] == leaf ? ind : 2;
      Node* node = split_leaf(leaf, key, value);
      std::cout << "after split_leaf: data[0] " << node->data[0].first << " ";
      std::cout << ", child[0] " << node->children[0]->data[0].first << ", chilld[1] " << node->children[1]->data[0].first << "\n";
      parent->children[ind] = node;
      node->parent = parent;
      std::cout << "parent of node: data[0] " << parent->data[0].first << " " << parent->data[1].first;
      std::cout << ", child[0] " << parent->children[0]->data[0].first << ", chilld[1]\n";
      split_and_up(node);
      // node_up(leaf->parent, middle_to_up, leaf, leaf->parent->children[2]);
    }
    ++size_;
    return;
  }

  template < class Key, class Value, class Cmp >
  Value TwoThreeTree< Key, Value, Cmp >::get(const Key& key)
  {
    // std::cout << "\nGET\n";
    Node* node = find(key);
    if (!node) {
      throw std::logic_error("There is no nodes with so key");
    }
    if (!node->is_3_node) {
      return node->data->second;
    } else {
      return (node->data[0].first == key ? node->data[0].second : node->data[1].second);
    }
  }

  template < class Key, class Value, class Cmp >
  typename TwoThreeTree< Key, Value, Cmp >::Node* TwoThreeTree< Key, Value, Cmp >::find(const Key& key)
  {
    if (empty()) {
      // std::cout << "empty\n";
      return nullptr;
    }

    Node* current = fake_->children[0];

    while (current != fake_) {
      std::cout << "WHILE IN FIND\n";
      std::cout << "Current data: [0] " << current->data[0].first << " [1] " << current->data[1].first << "\n";
      std::cout << "is_3_node: " << current->is_3_node << "\n";
      if (!cmp_(key, current->data[0].first) &&
          !cmp_(current->data[0].first, key)) {
        // std::cout << "cond1\n";
        return current;
      }

      if (current->is_3_node &&
          !cmp_(key, current->data[1].first) &&
          !cmp_(current->data[1].first, key)) {
        // std::cout << "cond2\n";
        return current;
      }

      if (cmp_(key, current->data[0].first)) {
        // std::cout << "cond3\n";
        current = current->children[0];
      } else if (!current->is_3_node || cmp_(key, current->data[1].first)) {
        // std::cout << "cond4\n";
        current = current->children[1];
      } else {
        // std::cout << "else\n";
        current = current->children[2];
      }
    }

    return nullptr;
  }

  template < class Key, class Value, class Cmp >
  bool TwoThreeTree< Key, Value, Cmp >::empty() const noexcept
  {
    return size() == 0;
  }

  template < class Key, class Value, class Cmp >
  size_t TwoThreeTree< Key, Value, Cmp >::size() const noexcept
  {
    return size_;
  }

  template < class Key, class Value, class Cmp >
  void TwoThreeTree< Key, Value, Cmp >::swap(TwoThreeTree& other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(cmp_, other.cmp_);
    std::swap(size_, other.size_);
  }

  template < class Key, class Value, class Cmp >
  void TwoThreeTree< Key, Value, Cmp >::clear() noexcept
  {
    if (empty()) {
      return;
    }
    clear_recursive(fake_->children[0]);
    fake_->children[0] = fake_;
    fake_->children[1] = fake_;
    fake_->children[2] = fake_;
    size_ = 0;
  }

  template < class Key, class Value, class Cmp >
  typename TwoThreeTree< Key, Value, Cmp >::Node* TwoThreeTree< Key, Value, Cmp >::find_leaf(const Key& key)
  {
    Node* current = fake_->children[0];
    while (true) {
      if (is_leaf(current)) break;

      if (cmp_(key, current->data[0].first)) {
        current = current->children[0];
      } else if (!current->is_3_node || cmp_(key, current->data[1].first)) {
        current = current->children[1];
      } else {
        current = current->children[2];
      }
    }
    return current;
  }

  // template <class Key, class Value>
  // NodeTwoThreeTree<Value, Key> *copy(NodeTwoThreeTree<Value, Key> *node, NodeTwoThreeTree<Value, Key> *parent)
  // {
  //   using Node = NodeTwoThreeTree<Value, Key>;
  //   if (!node)
  //   {
  //     return nullptr;
  //   }

  //   Node *new_node = new Node();
  //   new_node->data = node->data;
  //   new_node->is_3_node = node->is_3_node;
  //   new_node->key_count = node->key_count;
  //   new_node->parent = parent;

  //   try
  //   {
  //     for (size_t i = 0; i <= node->key_count; ++i)
  //     {
  //       new_node->children[i] = copy(node->children[i], new_node);
  //     }
  //   }
  //   catch (const std::bad_alloc &e)
  //   {
  //     new_node.clear();
  //     throw;
  //   }

  //   return new_node;
  // }

  template < class Key, class Value, class Cmp >
  void TwoThreeTree< Key, Value, Cmp >::push_to_2node(Node* node, const Key& key, const Value& value)
  {
    if (cmp_(key, node->data[0].first)) {
      node->data[1] = node->data[0];
      node->data[0] = {key, value};
    } else {
      node->data[1] = {key, value};
    }
    node->is_3_node = true;
    node->key_count = 2;
  }

  template < class Key, class Value, class Cmp >
  typename TwoThreeTree< Key, Value, Cmp >::Node* TwoThreeTree< Key, Value, Cmp >::split_leaf(Node* leaf, const Key& key, const Value& value)
  {
    pair temp[3] = {leaf->data[0], leaf->data[1], {key, value}};

    if (cmp_(temp[2].first, temp[0].first)) std::swap(temp[0], temp[2]);
    if (cmp_(temp[2].first, temp[1].first)) std::swap(temp[1], temp[2]);
    if (cmp_(temp[1].first, temp[0].first)) std::swap(temp[0], temp[1]);
    leaf->data[0] = temp[0];
    leaf->data[1] = {};
    leaf->is_3_node = false;
    leaf->key_count = 1;

    Node* new_node = new Node();
    new_node->data[0] = temp[2];
    new_node->key_count = 1;

    Node* middle = new Node();
    middle->data[0] = temp[1];
    middle->children[0] = leaf;
    middle->children[1] = new_node;
    middle->children[2] = fake_;
    middle->key_count = 1;
    middle->parent = leaf->parent;

    leaf->parent = middle;
    new_node->parent = middle;
    leaf->children[0] = fake_;
    new_node->children[0] = fake_;
    leaf->children[1] = fake_;
    new_node->children[1] = fake_;
    leaf->children[2] = fake_;
    new_node->children[2] = fake_;

    return middle;
  }

  template < class Key, class Value, class Cmp >
  bool TwoThreeTree< Key, Value, Cmp >::is_leaf(Node* node)
  {
    return node->children[0] == fake_;
  }

  template < class Key, class Value, class Cmp >
  void TwoThreeTree< Key, Value, Cmp >::clear_recursive(Node* node) noexcept
  {
    if (node == fake_) {
      return;
    }
    clear_recursive(node->children[0]);
    clear_recursive(node->children[1]);
    clear_recursive(node->children[2]);
    delete node;
  }

  template < class Key, class Value, class Cmp >
  void TwoThreeTree< Key, Value, Cmp >::split_and_up(Node* to_up)
  {
    std::cout << "SPLIT AND UP: ";
    Node* parent = to_up->parent;

    if (parent == fake_) {
      std::cout << "fake\n";
      fake_->children[0] = to_up;
      return;
    }
    if (!parent->is_3_node) {
      std::cout << "2node ";
      if (cmp_(to_up->data[0].first, parent->data[0].first)) {
        std::cout << "1\n";
        parent->data[1] = parent->data[0];
        parent->data[0] = to_up->data[0];
        parent->is_3_node = true;
        to_up->children[0]->parent = parent;
        to_up->children[1]->parent = parent;
        parent->children[2] = parent->children[1];
        parent->children[0] = to_up->children[0];
        parent->children[1] = to_up->children[1];
        delete to_up;
      } else if (cmp_(parent->data[0].first, to_up->data[0].first)) {
        std::cout << "2\n";
        parent->data[1] = to_up->data[0];
        parent->is_3_node = true;
        to_up->children[0]->parent = parent;
        to_up->children[1]->parent = parent;
        parent->children[1] = to_up->children[0];
        parent->children[2] = to_up->children[1];
        delete to_up;
      }
      return;
    } else {
      std::cout << "to_up " << to_up->data[0].first << "\n";
      std::cout << "ch0 " << parent->children[0]->data[0].first << "\n";
      std::cout << "ch1 " << parent->children[1]->data[0].first << "\n";
      std::cout << "ch2 " << parent->children[2]->data[0].first << "\n";
      std::cout << "3node";
      Node* left_child_to_up = to_up->children[0];
      Node* right_child_to_up = to_up->children[1];
      pair data_to_up = to_up->data[0];

      if (parent->children[0] == to_up) {
        std::cout << " 1\n";
        to_up->parent = parent->parent;
        to_up->data[0] = parent->data[0];

        parent->data[0] = parent->data[1];
        parent->data[1] = {};
        parent->children[0] = parent->children[1];
        parent->children[1] = parent->children[2];
        parent->children[2] = fake_;
        parent->is_3_node = false;
        parent->key_count = 1;

        Node* new_node = new Node();
        new_node->data[0] = data_to_up;
        new_node->children[0] = left_child_to_up;
        new_node->children[1] = right_child_to_up;
        new_node->children[2] = fake_;

        parent->parent = to_up;
        new_node->parent = to_up;
        to_up->children[0] = new_node;
        to_up->children[1] = parent;
      } else if (parent->children[2] == to_up) {
        std::cout << " 2\n";
        to_up->parent = parent->parent;
        to_up->data[0] = parent->data[1];

        parent->data[1] = {};
        parent->children[2] = fake_;
        parent->is_3_node = false;
        parent->key_count = 1;

        Node* new_node = new Node();
        new_node->data[0] = data_to_up;
        new_node->children[0] = left_child_to_up;
        new_node->children[1] = right_child_to_up;
        new_node->children[2] = fake_;

        parent->parent = to_up;
        new_node->parent = to_up;
        to_up->children[0] = parent;
        to_up->children[1] = new_node;
      } else {
        std::cout << " 3\n";
        to_up->parent = parent->parent;

        Node* new_node = new Node();
        new_node->data[0] = parent->data[1];
        new_node->children[0] = right_child_to_up;
        new_node->children[1] = parent->children[2];
        new_node->children[2] = fake_;

        parent->data[1] = {};
        parent->children[1] = left_child_to_up;
        parent->children[2] = fake_;
        parent->is_3_node = false;
        parent->key_count = 1;

        parent->parent = to_up;
        new_node->parent = to_up;
        to_up->children[0] = parent;
        to_up->children[1] = new_node;
      }
      split_and_up(to_up);
    }
  }

}

#endif
