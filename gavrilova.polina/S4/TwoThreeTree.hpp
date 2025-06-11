#ifndef TWO_THREE_TREE_HPP
#define TWO_THREE_TREE_HPP

#include <cassert>
#include <functional>
#include <iostream>
#include <stdexcept>
#include "NodeTTT.hpp"

namespace detail {

}
namespace gavrilova {

  template < class Key, class Value, class Cmp = std::less< Key > >
  struct IteratorTTT;

  template < class Key, class Value, class Cmp = std::less< Key > >
  struct ConstIterator;

  template < class Key, class Value, class Cmp = std::less< Key > >
  class TwoThreeTree {
  public:
    using Node = NodeTwoThreeTree< Key, Value >;
    using this_t = TwoThreeTree< Key, Value, Cmp >;
    using value_type = std::pair< Key, Value >;
    using Iterator = gavrilova::IteratorTTT< Key, Value, Cmp >;
    using ConstIterator = gavrilova::ConstIterator< Key, Value, Cmp >;

    TwoThreeTree();
    TwoThreeTree(const TwoThreeTree& other);
    TwoThreeTree(TwoThreeTree&& other) noexcept;
    // TwoThreeTree(std::initializer_list<Value> init);
    ~TwoThreeTree();

    TwoThreeTree& operator=(const TwoThreeTree& other);
    TwoThreeTree& operator=(TwoThreeTree&& other) noexcept;
    // TwoThreeTree &operator=(std::initializer_list<Value> init);

    Value& operator[](const Key& key);
    Value& at(const Key& key);
    const Value& at(const Key& key) const;

    Iterator begin();
    ConstIterator begin() const;
    ConstIterator cbegin() const noexcept;
    Iterator end();
    ConstIterator end() const;
    ConstIterator cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void clear() noexcept;
    // void push(const Key& key, const Value& value);
    std::pair< Iterator, bool > insert(const value_type& value);

    size_t erase(const Key& key);
    Iterator erase(Iterator pos);
    Iterator erase(Iterator first, Iterator last);

    void swap(TwoThreeTree& other) noexcept;
    Iterator find(const Key& key);
    ConstIterator find(const Key& key) const;

    Iterator lower_bound(const Key& key);
    ConstIterator lower_bound(const Key& key) const;
    Iterator upper_bound(const Key& key);
    ConstIterator upper_bound(const Key& key) const;

    Value get(const Key& key);
    Node* get_node();

  private:
    Node* fake_;
    size_t size_;
    Cmp cmp_;

    bool is_leaf(Node* node) const;
    void clear_recursive(Node* node) noexcept;
    Node* find_leaf(const Key& key);
    void push_to_2node(Node* node, const Key& key, const Value& value);
    Node* split_leaf(Node* leaf, const Key& key, const Value& value);
    void split_and_up(Node* node_to_promote);
    void absorb_into_2_node(Node* parent, Node* node_to_promote);
    void split_3_node_parent(Node* parent, Node* node_to_promote);
    Node* copy_subtree(Node* node, Node* parent);
  };

  template < class Key, class Value, class Cmp >
  TwoThreeTree< Key, Value, Cmp >::TwoThreeTree():
    fake_(reinterpret_cast< Node* >(new char[sizeof(Node)])),
    size_(0)
  {
    fake_->children[0] = nullptr;
    fake_->children[1] = nullptr;
    fake_->children[2] = nullptr;
    fake_->parent = nullptr;
    fake_->is_3_node = false;
    fake_->is_fake = true;
  }

  template < class Key, class Value, class Cmp >
  TwoThreeTree< Key, Value, Cmp >::TwoThreeTree(const TwoThreeTree& other):
    TwoThreeTree()
  {
    Node* new_root = nullptr;
    try {
      if (!other.empty()) {
        new_root = copy_subtree(other.fake_->children[0], fake_);
      }
      fake_->children[0] = new_root;
      size_ = other.size_;
    } catch (const std::bad_alloc&) {
      clear();
      throw;
    }
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
  Value& TwoThreeTree< Key, Value, Cmp >::operator[](const Key& key)
  {
    Iterator it = find(key);
    if (it == end()) {
      std::pair< Iterator, bool > result = insert({key, Value()});
      return result.first->second;
    }
    return it->second;
  }

  template < class Key, class Value, class Cmp >
  Value& TwoThreeTree< Key, Value, Cmp >::at(const Key& key)
  {
    Iterator it = find(key);
    if (it == end()) {
      throw std::out_of_range("Key not found in tree");
    }
    return it->second;
  }

  template < class Key, class Value, class Cmp >
  const Value& TwoThreeTree< Key, Value, Cmp >::at(const Key& key) const
  {
    ConstIterator it = find(key);
    if (it == end()) {
      throw std::out_of_range("Key not found in tree");
    }
    return it->second;
  }

  template < class Key, class Value, class Cmp >
  IteratorTTT< Key, Value, Cmp > TwoThreeTree< Key, Value, Cmp >::begin()
  {
    if (empty()) {
      return end();
    }
    Node* tmp = fake_->children[0];
    while (tmp->children[0] != fake_) {
      tmp = tmp->children[0];
    }
    return IteratorTTT< Key, Value, Cmp >(tmp, 0, fake_);
  }

  template < class Key, class Value, class Cmp >
  ConstIterator< Key, Value, Cmp > TwoThreeTree< Key, Value, Cmp >::begin() const
  {
    if (empty()) {
      return end();
    }
    const Node* tmp = fake_->children[0];
    while (tmp->children[0] != fake_) {
      tmp = tmp->children[0];
    }
    return ConstIterator{tmp, 0, fake_};
  }

  template < class Key, class Value, class Cmp >
  ConstIterator< Key, Value, Cmp > TwoThreeTree< Key, Value, Cmp >::cbegin() const noexcept
  {
    return begin();
  }

  template < class Key, class Value, class Cmp >
  IteratorTTT< Key, Value, Cmp > TwoThreeTree< Key, Value, Cmp >::end()
  {
    return Iterator(fake_, 0, fake_);
  }

  template < class Key, class Value, class Cmp >
  ConstIterator< Key, Value, Cmp > TwoThreeTree< Key, Value, Cmp >::end() const
  {
    return ConstIterator(fake_, 0, fake_);
  }

  template < class Key, class Value, class Cmp >
  ConstIterator< Key, Value, Cmp > TwoThreeTree< Key, Value, Cmp >::cend() const noexcept
  {
    return end();
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
  std::pair< typename TwoThreeTree< Key, Value, Cmp >::Iterator, bool >
  TwoThreeTree< Key, Value, Cmp >::insert(const std::pair< Key, Value >& value)
  {
    const Key& key = value.first;

    Iterator it = find(key);
    if (it != end()) {
      return std::make_pair(it, false);
    }

    if (empty()) {
      Node* new_root = new Node();
      new_root->data[0] = value;
      new_root->parent = fake_;
      new_root->children[0] = fake_;
      new_root->children[1] = fake_;
      new_root->children[2] = fake_;
      fake_->children[0] = new_root;
      ++size_;
      return std::make_pair(Iterator(new_root, 0, fake_), true);
    }

    Node* leaf = find_leaf(key);

    if (!leaf->is_3_node) {
      push_to_2node(leaf, key, value.second);
      ++size_;
      int new_pos = (!cmp_(leaf->data[0].first, key) && !cmp_(key, leaf->data[0].first)) ? 0 : 1;
      return std::make_pair(Iterator(leaf, new_pos, fake_), true);
    }

    Node* parent = leaf->parent;
    size_t ind_in_parent = 0;
    if (parent != fake_) {
      if (parent->children[0] != leaf) {
        ind_in_parent = (parent->children[1] == leaf) ? 1 : 2;
      }
    }

    Node* middle = split_leaf(leaf, key, value.second);
    parent->children[ind_in_parent] = middle;
    middle->parent = parent;

    split_and_up(middle);

    ++size_;
    it = find(key);
    return std::make_pair(it, true);
  }

  template < class Key, class Value, class Cmp >
  void TwoThreeTree< Key, Value, Cmp >::swap(TwoThreeTree& other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(cmp_, other.cmp_);
    std::swap(size_, other.size_);
  }

  template < class Key, class Value, class Cmp >
  typename TwoThreeTree< Key, Value, Cmp >::Iterator TwoThreeTree< Key, Value, Cmp >::find(const Key& key)
  {
    if (empty()) {
      return end();
    }

    Node* current = fake_->children[0];
    while (current != fake_) {
      if (!cmp_(key, current->data[0].first) && !cmp_(current->data[0].first, key)) {
        return Iterator(current, 0, fake_);
      }

      if (current->is_3_node) {
        if (!cmp_(key, current->data[1].first) && !cmp_(current->data[1].first, key)) {
          return Iterator(current, 1, fake_);
        }
      }
      if (cmp_(key, current->data[0].first)) {
        current = current->children[0];
      } else if (!current->is_3_node || cmp_(key, current->data[1].first)) {
        current = current->children[1];
      } else {
        current = current->children[2];
      }
    }
    return end();
  }

  template < class Key, class Value, class Cmp >
  typename TwoThreeTree< Key, Value, Cmp >::ConstIterator TwoThreeTree< Key, Value, Cmp >::find(const Key& key) const
  {
    if (empty()) {
      return cend();
    }

    const Node* current = fake_->children[0];
    while (current != fake_) {
      if (!cmp_(key, current->data[0].first) && !cmp_(current->data[0].first, key)) {
        return ConstIterator(current, 0, fake_);
      }

      if (current->is_3_node) {
        if (!cmp_(key, current->data[1].first) && !cmp_(current->data[1].first, key)) {
          return ConstIterator(current, 1, fake_);
        }
      }
      if (cmp_(key, current->data[0].first)) {
        current = current->children[0];
      } else if (!current->is_3_node || cmp_(key, current->data[1].first)) {
        current = current->children[1];
      } else {
        current = current->children[2];
      }
    }
    return cend();
  }

  template < class Key, class Value, class Cmp >
  IteratorTTT< Key, Value, Cmp > TwoThreeTree< Key, Value, Cmp >::lower_bound(const Key& key)
  {
    for (auto it = begin(); it != end(); it++) {
      if (!cmp_(it->first, key)) {
        return it;
      }
    }
    return end();
  }

  template < class Key, class Value, class Cmp >
  ConstIterator< Key, Value, Cmp > TwoThreeTree< Key, Value, Cmp >::lower_bound(const Key& key) const
  {
    for (auto it = cbegin(); it != cend(); it++) {
      if (!cmp_(it->first, key)) {
        return it;
      }
    }
    return cend();
  }

  template < class Key, class Value, class Cmp >
  IteratorTTT< Key, Value, Cmp > TwoThreeTree< Key, Value, Cmp >::upper_bound(const Key& key)
  {
    for (auto it = begin(); it != end(); it++) {
      if (cmp_(key, it->first)) {
        return it;
      }
    }
    return end();
  }

  template < class Key, class Value, class Cmp >
  ConstIterator< Key, Value, Cmp > TwoThreeTree< Key, Value, Cmp >::upper_bound(const Key& key) const
  {
    for (auto it = cbegin(); it != cend(); it++) {
      if (cmp_(key, it->first)) {
        return it;
      }
    }
    return cend();
  }

  template < class Key, class Value, class Cmp >
  Value TwoThreeTree< Key, Value, Cmp >::get(const Key& key)
  {
    auto it = find(key);
    if (it == end()) {
      throw std::logic_error("There is no nodes with so key");
    }
    return *it;
  }

  template < class Key, class Value, class Cmp >
  typename TwoThreeTree< Key, Value, Cmp >::Node* TwoThreeTree< Key, Value, Cmp >::get_node()
  {
    return (fake_->children[0] && !fake_->children[0]->is_fake) ? fake_->children[0] : nullptr;
  }

  template < class Key, class Value, class Cmp >
  bool TwoThreeTree< Key, Value, Cmp >::is_leaf(Node* node) const
  {
    assert(node);
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
  typename TwoThreeTree< Key, Value, Cmp >::Node* TwoThreeTree< Key, Value, Cmp >::find_leaf(const Key& key)
  {
    Node* current = fake_->children[0];
    while (true) {
      if (is_leaf(current)) {
        break;
      }

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
  }

  template < class Key, class Value, class Cmp >
  typename TwoThreeTree< Key, Value, Cmp >::Node* TwoThreeTree< Key, Value, Cmp >::split_leaf(Node* leaf, const Key& key, const Value& value)
  {
    value_type temp[3] = {leaf->data[0], leaf->data[1], {key, value}};

    if (cmp_(temp[2].first, temp[0].first)) std::swap(temp[0], temp[2]);
    if (cmp_(temp[2].first, temp[1].first)) std::swap(temp[1], temp[2]);
    if (cmp_(temp[1].first, temp[0].first)) std::swap(temp[0], temp[1]);
    leaf->data[0] = temp[0];
    leaf->data[1] = {};
    leaf->is_3_node = false;

    Node* new_node = new Node();
    new_node->data[0] = temp[2];

    Node* middle = new Node();
    middle->data[0] = temp[1];
    middle->children[0] = leaf;
    middle->children[1] = new_node;
    middle->children[2] = fake_;
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
  void TwoThreeTree< Key, Value, Cmp >::split_and_up(Node* node_to_promote)
  {
    Node* parent = node_to_promote->parent;

    if (parent == fake_) {
      fake_->children[0] = node_to_promote;
      node_to_promote->parent = fake_;
      return;
    }

    if (!parent->is_3_node) {
      absorb_into_2_node(parent, node_to_promote);
    } else {
      split_3_node_parent(parent, node_to_promote);
    }
  }

  template < class Key, class Value, class Cmp >
  void TwoThreeTree< Key, Value, Cmp >::absorb_into_2_node(Node* parent, Node* node_to_promote)
  {
    if (cmp_(node_to_promote->data[0].first, parent->data[0].first)) {
      parent->data[1] = parent->data[0];
      parent->data[0] = node_to_promote->data[0];
      parent->children[2] = parent->children[1];
      parent->children[1] = node_to_promote->children[1];
      parent->children[0] = node_to_promote->children[0];
    } else {
      parent->data[1] = node_to_promote->data[0];
      parent->children[2] = node_to_promote->children[1];
      parent->children[1] = node_to_promote->children[0];
    }

    parent->is_3_node = true;

    parent->children[0]->parent = parent;
    parent->children[1]->parent = parent;
    parent->children[2]->parent = parent;

    delete node_to_promote;
  }

  template < class Key, class Value, class Cmp >
  void TwoThreeTree< Key, Value, Cmp >::split_3_node_parent(Node* parent, Node* node_to_promote)
  {
    value_type temp_keys[3];
    Node* temp_children[4];

    if (node_to_promote == parent->children[0]) {
      temp_keys[0] = node_to_promote->data[0];
      temp_keys[1] = parent->data[0];
      temp_keys[2] = parent->data[1];
      temp_children[0] = node_to_promote->children[0];
      temp_children[1] = node_to_promote->children[1];
      temp_children[2] = parent->children[1];
      temp_children[3] = parent->children[2];
    } else if (node_to_promote == parent->children[1]) {
      temp_keys[0] = parent->data[0];
      temp_keys[1] = node_to_promote->data[0];
      temp_keys[2] = parent->data[1];
      temp_children[0] = parent->children[0];
      temp_children[1] = node_to_promote->children[0];
      temp_children[2] = node_to_promote->children[1];
      temp_children[3] = parent->children[2];
    } else {
      temp_keys[0] = parent->data[0];
      temp_keys[1] = parent->data[1];
      temp_keys[2] = node_to_promote->data[0];
      temp_children[0] = parent->children[0];
      temp_children[1] = parent->children[1];
      temp_children[2] = node_to_promote->children[0];
      temp_children[3] = node_to_promote->children[1];
    }

    parent->data[0] = temp_keys[0];
    parent->data[1] = {};
    parent->is_3_node = false;
    parent->children[0] = temp_children[0];
    parent->children[1] = temp_children[1];
    parent->children[2] = fake_;
    parent->children[0]->parent = parent;
    parent->children[1]->parent = parent;

    Node* new_right_sibling = new Node();
    new_right_sibling->data[0] = temp_keys[2];
    new_right_sibling->children[0] = temp_children[2];
    new_right_sibling->children[1] = temp_children[3];
    new_right_sibling->children[2] = fake_;
    new_right_sibling->children[0]->parent = new_right_sibling;
    new_right_sibling->children[1]->parent = new_right_sibling;

    delete node_to_promote;

    Node* new_promo_node = new Node();
    new_promo_node->data[0] = temp_keys[1];
    new_promo_node->children[0] = parent;
    new_promo_node->children[1] = new_right_sibling;
    new_promo_node->children[2] = fake_;

    Node* grand_parent = parent->parent;
    new_promo_node->parent = grand_parent;
    parent->parent = new_promo_node;
    new_right_sibling->parent = new_promo_node;

    split_and_up(new_promo_node);
  }

  template < class Key, class Value, class Cmp >
  typename TwoThreeTree< Key, Value, Cmp >::Node* TwoThreeTree< Key, Value, Cmp >::copy_subtree(Node* node, Node* parent)
  {
    if (!node || node->is_fake) {
      return fake_;
    }

    Node* new_node = new Node();
    try {
      new_node->data[0] = node->data[0];
      new_node->data[1] = node->data[1];
      new_node->is_3_node = node->is_3_node;
      new_node->is_fake = false;
      new_node->parent = parent;

      for (int i = 0; i < 3; ++i) {
        new_node->children[i] = copy_subtree(node->children[i], new_node);
      }

      return new_node;
    } catch (...) {
      delete new_node;
      throw;
    }
  }

  // template < class Key, class Value, class Cmp >
  // void TwoThreeTree< Key, Value, Cmp >::push(const Key& key, const Value& value)
  // {
  //   if (empty()) {
  //     Node* new_node = new Node();
  //     value_type new_pair{key, value};
  //     new_node->data[0] = new_pair;
  //     new_node->parent = fake_;
  //     new_node->children[0] = fake_;
  //     new_node->children[1] = fake_;
  //     new_node->children[2] = fake_;
  //     fake_->children[0] = new_node;
  //     ++size_;
  //     return;
  //   }

  //   Node* leaf = find_leaf(key);
  //   if (!leaf->is_3_node) {
  //     push_to_2node(leaf, key, value);
  //   } else {
  //     Node* parent = leaf->parent;
  //     size_t ind = parent->children[0] == leaf ? 0 : 1;
  //     ind = parent->children[ind] == leaf ? ind : 2;
  //     Node* node = split_leaf(leaf, key, value);
  //     parent->children[ind] = node;
  //     node->parent = parent;
  //     split_and_up(node);
  //   }
  //   ++size_;
  //   return;
  // }

}

#endif
