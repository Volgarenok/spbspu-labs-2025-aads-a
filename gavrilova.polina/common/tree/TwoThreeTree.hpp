#ifndef TWO_THREE_TREE_HPP
#define TWO_THREE_TREE_HPP

#include <Queue.hpp>
#include <Stack.hpp>
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
    TwoThreeTree(std::initializer_list< value_type > il);
    template < typename InputIterator >
    TwoThreeTree(InputIterator first, InputIterator last);

    ~TwoThreeTree();

    TwoThreeTree& operator=(const TwoThreeTree& other);
    TwoThreeTree& operator=(TwoThreeTree&& other) noexcept;

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

    std::pair< Iterator, bool > insert(const value_type& value);
    template < class InputIterator >
    void insert(InputIterator first, InputIterator last);
    void insert(std::initializer_list< value_type > il);

    size_t erase(const Key& key);
    Iterator erase(Iterator pos);
    Iterator erase(Iterator first, Iterator last);

    void swap(TwoThreeTree& other) noexcept;
    Iterator find(const Key& key);
    ConstIterator find(const Key& key) const;
    std::pair< Iterator, Iterator > equal_range(const Key& key);
    std::pair< ConstIterator, ConstIterator > equal_range(const Key& key) const;
    size_t count(const Key& key) const;

    Iterator lower_bound(const Key& key);
    ConstIterator lower_bound(const Key& key) const;
    Iterator upper_bound(const Key& key);
    ConstIterator upper_bound(const Key& key) const;

    Value get(const Key& key);
    Node* get_node();

    template < typename F >
    F traverse_lnr(F f) const;
    template < typename F >
    F traverse_rnl(F f) const;
    template < typename F >
    F traverse_breadth(F f) const;

    template < typename F >
    F traverse_lnr(F f);
    template < typename F >
    F traverse_rnl(F f);
    template < typename F >
    F traverse_breadth(F f);

  private:
    Node* fake_;
    size_t size_;
    Cmp cmp_;

    Node* copy_subtree(Node* node, Node* parent);
    bool is_leaf(Node* node) const;
    void clear_recursive(Node* node) noexcept;
    Node* find_leaf(const Key& key, size_t& counter_for_allocate);

    void push_to_2node(Node* node, const Key& key, const Value& value);

    void rebalance(Node* node);
    void rotation(Node* deficient_node, Node* sibling, Node* parent, int deficient_idx);
    void merge(Node* deficient_node, Node* sibling, Node* parent, int deficient_idx);

    int get_child_index(Node* child) const;
    Node* get_inorder_successor(Node* node, int key_idx);
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
  template < class InputIterator >
  TwoThreeTree< Key, Value, Cmp >::TwoThreeTree(InputIterator first, InputIterator last):
    TwoThreeTree()
  {
    for (auto it = first; it != last; it++) {
      insert(*it);
    }
  }

  template < class Key, class Value, class Cmp >
  TwoThreeTree< Key, Value, Cmp >::TwoThreeTree(std::initializer_list< value_type > il):
    TwoThreeTree(il.begin(), il.end())
  {}

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
  TwoThreeTree< Key, Value, Cmp >::insert(const value_type& value)
  {
    const Key& key = value.first;

    Iterator existing_it = find(key);
    if (existing_it != end()) {
      return {existing_it, false};
    }

    if (empty()) {
      Node* new_root = new Node();
      new_root->data[0] = value;
      new_root->parent = fake_;
      new_root->children[0] = new_root->children[1] = new_root->children[2] = fake_;
      fake_->children[0] = new_root;
      ++size_;
      return {Iterator(new_root, 0, fake_), true};
    }

    size_t nodes_to_alloc_count = 0;
    Node* leaf = find_leaf(key, nodes_to_alloc_count);
    if (!leaf->is_3_node) {
      push_to_2node(leaf, key, value.second);
      ++size_;
      int new_pos = (cmp_(key, leaf->data[1].first)) ? 0 : 1;
      return {Iterator(leaf, new_pos, fake_), true};
    }

    Node** preallocated_nodes = nullptr;
    if (nodes_to_alloc_count > 0) {
      preallocated_nodes = new Node* [nodes_to_alloc_count] { nullptr };
      try {
        for (size_t i = 0; i < nodes_to_alloc_count; ++i) {
          preallocated_nodes[i] = new Node();
        }
      } catch (const std::bad_alloc&) {
        for (size_t i = 0; i < nodes_to_alloc_count; ++i) {
          delete preallocated_nodes[i];
        }
        delete[] preallocated_nodes;
        throw;
      }
    }

    size_t nodes_used = 0;
    value_type temp_data[3] = {leaf->data[0], leaf->data[1], value};

    if (cmp_(temp_data[1].first, temp_data[0].first)) {
      std::swap(temp_data[0], temp_data[1]);
    }
    if (cmp_(temp_data[2].first, temp_data[1].first)) {
      std::swap(temp_data[1], temp_data[2]);
    }
    if (cmp_(temp_data[1].first, temp_data[0].first)) {
      std::swap(temp_data[0], temp_data[1]);
    }

    value_type promoted_value = temp_data[1];

    leaf->data[0] = temp_data[0];
    leaf->data[1] = {};
    leaf->is_3_node = false;

    Node* new_right_node = preallocated_nodes[nodes_used++];
    new_right_node->data[0] = temp_data[2];
    new_right_node->children[0] = new_right_node->children[1] = new_right_node->children[2] = fake_;

    Node* left_child_of_promo = leaf;
    Node* right_child_of_promo = new_right_node;
    Node* current_child = leaf;

    while (true) {
      Node* parent = current_child->parent;

      if (parent == fake_) {
        Node* new_root = preallocated_nodes[nodes_used++];
        new_root->data[0] = promoted_value;
        new_root->children[0] = left_child_of_promo;
        new_root->children[1] = right_child_of_promo;
        new_root->children[2] = fake_;
        new_root->parent = fake_;
        new_root->children[0]->parent = new_root;
        new_root->children[1]->parent = new_root;
        fake_->children[0] = new_root;
        break;
      }

      right_child_of_promo->parent = parent;

      if (!parent->is_3_node) {
        if (cmp_(promoted_value.first, parent->data[0].first)) {
          parent->data[1] = parent->data[0];
          parent->data[0] = promoted_value;
          parent->children[2] = parent->children[1];
          parent->children[1] = right_child_of_promo;
        } else {
          parent->data[1] = promoted_value;
          parent->children[2] = right_child_of_promo;
        }
        parent->is_3_node = true;
        break;
      }

      value_type parent_temp_data[3];
      Node* parent_temp_children[4];
      int child_idx = get_child_index(current_child);

      if (child_idx == 0) {
        parent_temp_data[0] = promoted_value;
        parent_temp_data[1] = parent->data[0];
        parent_temp_data[2] = parent->data[1];
        parent_temp_children[0] = left_child_of_promo;
        parent_temp_children[1] = right_child_of_promo;
        parent_temp_children[2] = parent->children[1];
        parent_temp_children[3] = parent->children[2];
      } else if (child_idx == 1) {
        parent_temp_data[0] = parent->data[0];
        parent_temp_data[1] = promoted_value;
        parent_temp_data[2] = parent->data[1];
        parent_temp_children[0] = parent->children[0];
        parent_temp_children[1] = left_child_of_promo;
        parent_temp_children[2] = right_child_of_promo;
        parent_temp_children[3] = parent->children[2];
      } else {
        parent_temp_data[0] = parent->data[0];
        parent_temp_data[1] = parent->data[1];
        parent_temp_data[2] = promoted_value;
        parent_temp_children[0] = parent->children[0];
        parent_temp_children[1] = parent->children[1];
        parent_temp_children[2] = left_child_of_promo;
        parent_temp_children[3] = right_child_of_promo;
      }

      promoted_value = parent_temp_data[1];

      parent->data[0] = parent_temp_data[0];
      parent->data[1] = {};
      parent->is_3_node = false;
      parent->children[0] = parent_temp_children[0];
      parent->children[1] = parent_temp_children[1];
      parent->children[2] = fake_;
      parent->children[0]->parent = parent;
      parent->children[1]->parent = parent;

      Node* new_parent_right_sibling = preallocated_nodes[nodes_used++];
      new_parent_right_sibling->data[0] = parent_temp_data[2];
      new_parent_right_sibling->children[0] = parent_temp_children[2];
      new_parent_right_sibling->children[1] = parent_temp_children[3];
      new_parent_right_sibling->children[2] = fake_;
      new_parent_right_sibling->children[0]->parent = new_parent_right_sibling;
      new_parent_right_sibling->children[1]->parent = new_parent_right_sibling;

      left_child_of_promo = parent;
      right_child_of_promo = new_parent_right_sibling;
      current_child = parent;
    }

    delete[] preallocated_nodes;
    ++size_;
    return {find(key), true};
  }

  template < class Key, class Value, class Cmp >
  template < class InputIterator >
  void TwoThreeTree< Key, Value, Cmp >::insert(InputIterator first, InputIterator last)
  {
    for (auto it = first; it != last; it++) {
      insert(*it);
    }
  }

  template < class Key, class Value, class Cmp >
  void TwoThreeTree< Key, Value, Cmp >::insert(std::initializer_list< value_type > il)
  {
    insert(il.begin(), il.end());
  }

  template < class Key, class Value, class Cmp >
  typename TwoThreeTree< Key, Value, Cmp >::Iterator
  TwoThreeTree< Key, Value, Cmp >::erase(Iterator pos)
  {
    if (pos == end()) {
      throw std::out_of_range("Cannot erase end() iterator");
    }

    Iterator next_it = pos;
    ++next_it;
    Key next_key = next_it->first;

    Node* node_to_process = pos.node_;
    int key_idx_to_remove = pos.key_pos_;

    if (!is_leaf(node_to_process)) {
      Node* successor = get_inorder_successor(node_to_process, key_idx_to_remove);
      std::swap(node_to_process->data[key_idx_to_remove], successor->data[0]);
      node_to_process = successor;
      key_idx_to_remove = 0;
    }

    if (node_to_process->is_3_node) {
      if (key_idx_to_remove == 0) {
        node_to_process->data[0] = node_to_process->data[1];
      }
      node_to_process->data[1] = {};
      node_to_process->is_3_node = false;
    } else {
      node_to_process->data[0] = {};
      rebalance(node_to_process);
    }

    --size_;

    if (next_it != end()) {
      return find(next_key);
    } else {
      return end();
    }
  }

  template < class Key, class Value, class Cmp >
  size_t TwoThreeTree< Key, Value, Cmp >::erase(const Key& key)
  {
    Iterator it = find(key);
    if (it == end()) {
      return 0;
    }
    erase(it);
    return 1;
  }

  template < class Key, class Value, class Cmp >
  typename TwoThreeTree< Key, Value, Cmp >::Iterator TwoThreeTree< Key, Value, Cmp >::erase(Iterator first, Iterator last)
  {
    if (first == last || first == end()) {
      return end();
    }

    Key key_last;
    if (last != end()) {
      key_last = last->first;
    }

    while (first != last) {
      first = erase(first);

      if (last != end()) {
        last = find(key_last);
      }
    }
    return first;
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
  std::pair< IteratorTTT< Key, Value, Cmp >, IteratorTTT< Key, Value, Cmp > >
  TwoThreeTree< Key, Value, Cmp >::equal_range(const Key& key)
  {
    Iterator first = lower_bound(key);
    if (first == end() || cmp_(key, first->first)) {
      return {first, first};
    }

    Iterator last = first;
    ++last;
    while (last != end() && !cmp_(key, last->first)) {
      ++last;
    }

    return {first, last};
  }

  template < class Key, class Value, class Cmp >
  std::pair< ConstIterator< Key, Value, Cmp >, ConstIterator< Key, Value, Cmp > >
  TwoThreeTree< Key, Value, Cmp >::equal_range(const Key& key) const
  {
    ConstIterator first = lower_bound(key);
    if (first == cend() || cmp_(key, first->first)) {
      return {first, first};
    }

    ConstIterator last = first;
    ++last;
    while (last != cend() && !cmp_(key, last->first)) {
      ++last;
    }

    return {first, last};
  }

  template < class Key, class Value, class Comparator >
  size_t TwoThreeTree< Key, Value, Comparator >::count(const Key& key) const
  {
    return find(key) == cend() ? 0 : 1;
  }

  template < class Key, class Value, class Cmp >
  IteratorTTT< Key, Value, Cmp > TwoThreeTree< Key, Value, Cmp >::lower_bound(const Key& key)
  {
    for (auto it = begin(); it != end(); ++it) {
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

  template < class Key, class Value, class Comparator >
  template < class F >
  F TwoThreeTree< Key, Value, Comparator >::traverse_lnr(F f)
  {
    return static_cast< const TwoThreeTree& >(*this).traverse_lnr(f);
  }

  template < class Key, class Value, class Comparator >
  template < class F >
  F TwoThreeTree< Key, Value, Comparator >::traverse_lnr(F f) const
  {
    if (empty()) {
      return f;
    }

    enum class LNRStage { GoLeft,
      VisitLeft,
      GoMiddle,
      VisitRight,
      GoRight };

    Stack< std::pair< Node*, LNRStage > > stack;
    stack.push({fake_->children[0], LNRStage::GoLeft});

    while (!stack.empty()) {
      Node* node = stack.top().first;
      LNRStage& stage = stack.top().second;

      if (node == fake_) {
        stack.pop();
        continue;
      }

      switch (stage) {
        case LNRStage::GoLeft:
          stage = LNRStage::VisitLeft;
          stack.push({node->children[0], LNRStage::GoLeft});
          break;

        case LNRStage::VisitLeft:
          stage = LNRStage::GoMiddle;
          f(node->data[0]);
          break;

        case LNRStage::GoMiddle:
          stage = LNRStage::VisitRight;
          stack.push({node->children[1], LNRStage::GoLeft});
          break;

        case LNRStage::VisitRight:
          stage = LNRStage::GoRight;
          if (node->is_3_node) {
            f(node->data[1]);
          }
          break;

        case LNRStage::GoRight:
          stack.pop();
          if (node->is_3_node && node->children[2] != fake_) {
            stack.push({node->children[2], LNRStage::GoLeft});
          }
          break;
      }
    }

    return f;
  }

  template < class Key, class Value, class Comparator >
  template < class F >
  F TwoThreeTree< Key, Value, Comparator >::traverse_rnl(F f)
  {
    return static_cast< const TwoThreeTree& >(*this).traverse_rnl(f);
  }

  template < class Key, class Value, class Comparator >
  template < class F >
  F TwoThreeTree< Key, Value, Comparator >::traverse_rnl(F f) const
  {
    if (empty()) {
      return f;
    }

    enum class RNLStage {
      GoRight,
      VisitRight,
      GoMiddle,
      VisitLeft,
      GoLeft
    };

    Stack< std::pair< Node*, RNLStage > > stack;
    if (fake_->children[0]->is_3_node) {
      stack.push({fake_->children[0], RNLStage::GoRight});
    } else {
      stack.push({fake_->children[0], RNLStage::GoMiddle});
    }

    while (!stack.empty()) {
      Node* node = stack.top().first;
      RNLStage& stage = stack.top().second;

      if (node == fake_) {
        stack.pop();
        continue;
      }

      switch (stage) {
        case RNLStage::GoRight:
          if (node->is_3_node) {
            stage = RNLStage::VisitRight;
            if (node->children[2]->is_3_node) {
              stack.push({node->children[2], RNLStage::GoRight});
            } else {
              stack.push({node->children[2], RNLStage::GoMiddle});
            }
          }
          break;

        case RNLStage::VisitRight:
          stage = RNLStage::GoMiddle;
          if (node->is_3_node) {
            f(node->data[1]);
          }
          break;

        case RNLStage::GoMiddle:
          stage = RNLStage::VisitLeft;
          if (node->children[1]->is_3_node) {
            stack.push({node->children[1], RNLStage::GoRight});
          } else {
            stack.push({node->children[1], RNLStage::GoMiddle});
          }
          break;

        case RNLStage::VisitLeft:
          stage = RNLStage::GoLeft;
          f(node->data[0]);
          break;

        case RNLStage::GoLeft:
          stack.pop();
          if (node->children[0]->is_3_node) {
            stack.push({node->children[0], RNLStage::GoRight});
          } else {
            stack.push({node->children[0], RNLStage::GoMiddle});
          }
          break;
      }
    }

    return f;
  }

  template < class Key, class Value, class Comparator >
  template < class F >
  F TwoThreeTree< Key, Value, Comparator >::traverse_breadth(F f)
  {
    return static_cast< const TwoThreeTree& >(*this).traverse_breadth(f);
  }

  template < class Key, class Value, class Comparator >
  template < class F >
  F TwoThreeTree< Key, Value, Comparator >::traverse_breadth(F f) const
  {
    if (empty()) {
      return f;
    }

    Queue< Node* > queue;
    queue.push(fake_->children[0]);

    while (!queue.empty()) {
      Node* current = queue.front();
      queue.pop();

      if (current == fake_) {
        continue;
      }

      f(current->data[0]);
      if (current->is_3_node) {
        f(current->data[1]);
      }

      if (current->children[0] != fake_) {
        queue.push(current->children[0]);
      }
      if (current->children[1] != fake_) {
        queue.push(current->children[1]);
      }
      if (current->is_3_node && current->children[2] != fake_) {
        queue.push(current->children[2]);
      }
    }

    return f;
  }

  template < class Key, class Value, class Cmp >
  typename TwoThreeTree< Key, Value, Cmp >::Node* TwoThreeTree< Key, Value, Cmp >::copy_subtree(Node* node, Node* parent)
  {
    if (!node || node->is_fake) {
      return fake_;
    }

    Node* new_node = new Node(node->data[0], node->data[1], node->is_3_node, parent, fake_, fake_, fake_);
    try {
      for (int i = 0; i < 3; ++i) {
        new_node->children[i] = copy_subtree(node->children[i], new_node);
      }
      return new_node;
    } catch (const std::bad_alloc& e) {
      clear_recursive(new_node);
      throw;
    }
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
  typename TwoThreeTree< Key, Value, Cmp >::Node* TwoThreeTree< Key, Value, Cmp >::find_leaf(const Key& key, size_t& counter_for_allocate)
  {
    counter_for_allocate = 0;
    Node* current = fake_->children[0];
    if (current->is_3_node) {
      ++counter_for_allocate;
    }
    while (current != fake_ && !is_leaf(current)) {
      if (current->is_3_node) {
        ++counter_for_allocate;
      } else {
        counter_for_allocate = 0;
      }
      if (cmp_(key, current->data[0].first)) {
        current = current->children[0];
      } else if (!current->is_3_node || cmp_(key, current->data[1].first)) {
        current = current->children[1];
      } else {
        current = current->children[2];
      }
    }
    if (current != fake_ && current->is_3_node) {
      ++counter_for_allocate;
    } else if (current != fake_ && !current->is_3_node) {
      counter_for_allocate = 0;
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
  void TwoThreeTree< Key, Value, Cmp >::rotation(Node* deficient_node, Node* sibling, Node* parent, int deficient_idx)
  {
    int sibling_idx = get_child_index(sibling);

    if (sibling_idx < deficient_idx) {
      deficient_node->data[0] = parent->data[sibling_idx];
      parent->data[sibling_idx] = sibling->data[1];
      sibling->data[1] = {};
      sibling->is_3_node = false;

      if (!is_leaf(sibling)) {
        deficient_node->children[1] = deficient_node->children[0];
        deficient_node->children[0] = sibling->children[2];
        if (deficient_node->children[0] != fake_) {
          deficient_node->children[0]->parent = deficient_node;
        }
        sibling->children[2] = fake_;
      }
    } else {
      deficient_node->data[0] = parent->data[deficient_idx];
      parent->data[deficient_idx] = sibling->data[0];
      sibling->data[0] = sibling->data[1];
      sibling->data[1] = {};
      sibling->is_3_node = false;

      if (!is_leaf(sibling)) {
        deficient_node->children[1] = sibling->children[0];
        if (deficient_node->children[1] != fake_) {
          deficient_node->children[1]->parent = deficient_node;
        }
        sibling->children[0] = sibling->children[1];
        sibling->children[1] = sibling->children[2];
        sibling->children[2] = fake_;
      }
    }
  }

  template < class Key, class Value, class Cmp >
  void TwoThreeTree< Key, Value, Cmp >::rebalance(Node* node)
  {
    if (node->parent == fake_) {
      if (node->children[0] != fake_) {
        fake_->children[0] = node->children[0];
        node->children[0]->parent = fake_;
      } else {
        fake_->children[0] = nullptr;
      }
      delete node;
      return;
    }

    Node* parent = node->parent;

    if (parent->parent != fake_) {
    } else {
    }

    int child_idx = get_child_index(node);

    Node* left_sibling = (child_idx > 0) ? parent->children[child_idx - 1] : nullptr;
    Node* right_sibling = (child_idx < 2 && parent->children[child_idx + 1] != fake_) ? parent->children[child_idx + 1] : nullptr;

    if (left_sibling && left_sibling->is_3_node) {
      rotation(node, left_sibling, parent, child_idx);
      return;
    }

    if (right_sibling && right_sibling->is_3_node) {
      rotation(node, right_sibling, parent, child_idx);
      return;
    }

    if (left_sibling) {
      merge(node, left_sibling, parent, child_idx);
    } else {
      merge(node, right_sibling, parent, child_idx);
    }
  }

  template < class Key, class Value, class Cmp >
  void TwoThreeTree< Key, Value, Cmp >::merge(Node* deficient_node, Node* sibling, Node* parent, int deficient_idx)
  {
    int sibling_idx = get_child_index(sibling);
    Node* node_to_delete = nullptr;

    if (sibling_idx < deficient_idx) {
      sibling->data[1] = parent->data[sibling_idx];
      sibling->is_3_node = true;

      sibling->children[2] = deficient_node->children[0];
      if (sibling->children[2] != fake_) {
        sibling->children[2]->parent = sibling;
      }

      node_to_delete = deficient_node;
    }

    else {
      deficient_node->data[0] = parent->data[deficient_idx];
      deficient_node->data[1] = sibling->data[0];
      deficient_node->is_3_node = true;

      deficient_node->children[1] = sibling->children[0];
      deficient_node->children[2] = sibling->children[1];
      if (deficient_node->children[1] != fake_) {
        deficient_node->children[1]->parent = deficient_node;
      }
      if (deficient_node->children[2] != fake_) {
        deficient_node->children[2]->parent = deficient_node;
      }

      node_to_delete = sibling;
    }

    int key_idx_to_remove = std::min(sibling_idx, deficient_idx);
    int child_idx_to_remove = get_child_index(node_to_delete);

    if (parent->is_3_node) {
      if (key_idx_to_remove == 0) {
        parent->data[0] = parent->data[1];
      }
    }
    parent->data[parent->is_3_node ? 1 : 0] = {};

    for (int i = child_idx_to_remove; i < 2; ++i) {
      parent->children[i] = parent->children[i + 1];
    }
    parent->children[2] = fake_;

    delete node_to_delete;

    if (parent->is_3_node) {
      parent->is_3_node = false;
    } else {
      rebalance(parent);
    }
  }

  template < class Key, class Value, class Cmp >
  int TwoThreeTree< Key, Value, Cmp >::get_child_index(Node* child) const
  {
    Node* parent = child->parent;
    if (!parent || parent == fake_) {
      return -1;
    }
    if (parent->children[0] == child) {
      return 0;
    }
    if (parent->children[1] == child) {
      return 1;
    }
    if (parent->is_3_node && parent->children[2] == child) {
      return 2;
    }
    return -1;
  }

  template < class Key, class Value, class Cmp >
  typename TwoThreeTree< Key, Value, Cmp >::Node*
  TwoThreeTree< Key, Value, Cmp >::get_inorder_successor(Node* node, int key_idx)
  {
    Node* current = node->children[key_idx + 1];
    while (current != fake_) {
      if (is_leaf(current)) {
        break;
      }
      current = current->children[0];
    }
    return current;
  }

}

#endif
