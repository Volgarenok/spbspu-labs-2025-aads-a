#ifndef TREE_DEFINITION_HPP
#define TREE_DEFINITION_HPP

#include "declaration.hpp"
#include <utility>
#include <queue.hpp>

template< class Key, class T, class Compare >
maslevtsov::Tree< Key, T, Compare >::Tree():
  dummy_root_(new Node{nullptr, nullptr, nullptr, nullptr, true, value_type(), value_type()}),
  size_(0),
  compare_(Compare())
{}

template< class Key, class T, class Compare >
maslevtsov::Tree< Key, T, Compare >::Tree(const Tree& rhs):
  Tree()
{
  for (auto it = rhs.cbegin(); it != rhs.cend(); ++it) {
    insert(std::make_pair(it->first, it->second));
  }
}

template< class Key, class T, class Compare >
maslevtsov::Tree< Key, T, Compare >::Tree(Tree&& rhs) noexcept:
  dummy_root_(std::exchange(rhs.dummy_root_, nullptr)),
  size_(std::exchange(rhs.size_, 0))
{}

template< class Key, class T, class Compare >
maslevtsov::Tree< Key, T, Compare >::~Tree()
{
  clear();
  delete dummy_root_;
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::Tree& maslevtsov::Tree< Key, T, Compare >::operator=(const Tree& rhs)
{
  if (dummy_root_ != rhs.dummy_root_) {
    Tree< Key, T, Compare > copied_rhs(rhs);
    swap(copied_rhs);
  }
  return *this;
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::Tree& maslevtsov::Tree< Key, T, Compare >::operator=(Tree&& rhs) noexcept
{
  if (dummy_root_ != rhs.dummy_root_) {
    Tree< Key, T, Compare > moved_rhs(std::move(rhs));
    swap(moved_rhs);
  }
  return *this;
}

template< class Key, class T, class Compare >
T& maslevtsov::Tree< Key, T, Compare >::operator[](const Key& key)
{
  return insert(std::make_pair(key, T())).first->second;
}

template< class Key, class T, class Compare >
const T& maslevtsov::Tree< Key, T, Compare >::operator[](const Key& key) const
{
  return at(key);
}

template< class Key, class T, class Compare >
T& maslevtsov::Tree< Key, T, Compare >::at(const Key& key)
{
  iterator it = find(key);
  if (it != end()) {
    return it->second;
  }
  throw std::out_of_range("invalid key");
}

template< class Key, class T, class Compare >
const T& maslevtsov::Tree< Key, T, Compare >::at(const Key& key) const
{
  const_iterator it = find(key);
  if (it != cend()) {
    return it->second;
  }
  throw std::out_of_range("invalid key");
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::iterator maslevtsov::Tree< Key, T, Compare >::begin()
{
  return iterator(iterator::get_min_node(dummy_root_), true);
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::const_iterator maslevtsov::Tree< Key, T, Compare >::begin() const
{
  return const_iterator(const_iterator::get_min_node(dummy_root_), true);
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::const_iterator
  maslevtsov::Tree< Key, T, Compare >::cbegin() const noexcept
{
  return const_iterator(const_iterator::get_min_node(dummy_root_), true);
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::iterator maslevtsov::Tree< Key, T, Compare >::end()
{
  return iterator(dummy_root_, true);
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::const_iterator maslevtsov::Tree< Key, T, Compare >::end() const
{
  return const_iterator(dummy_root_, true);
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::const_iterator maslevtsov::Tree< Key, T, Compare >::cend() const noexcept
{
  return const_iterator(dummy_root_, true);
}

template< class Key, class T, class Compare >
template< typename F >
F maslevtsov::Tree< Key, T, Compare >::traverse_lnr(F f)
{
  if (empty()) {
    throw std::logic_error("nothing to traverse");
  }
  for (auto it = begin(); it != end(); ++it) {
    f(*it);
  }
  return f;
}

template< class Key, class T, class Compare >
template< typename F >
F maslevtsov::Tree< Key, T, Compare >::traverse_lnr(F f) const
{
  if (empty()) {
    throw std::logic_error("nothing to traverse");
  }
  for (auto it = cbegin(); it != cend(); ++it) {
    f(*it);
  }
  return f;
}

template< class Key, class T, class Compare >
template< typename F >
F maslevtsov::Tree< Key, T, Compare >::traverse_rnl(F f)
{
  if (empty()) {
    throw std::logic_error("nothing to traverse");
  }
  auto it = --end();
  for (; it != begin(); --it) {
    f(*it);
  }
  f(*it);
  return f;
}

template< class Key, class T, class Compare >
template< typename F >
F maslevtsov::Tree< Key, T, Compare >::traverse_rnl(F f) const
{
  if (empty()) {
    throw std::logic_error("nothing to traverse");
  }
  auto it = --cend();
  for (; it != cbegin(); --it) {
    f(*it);
  }
  f(*it);
  return f;
}

template< class Key, class T, class Compare >
template< typename F >
F maslevtsov::Tree< Key, T, Compare >::traverse_breadth(F f)
{
  return const_cast< const Tree< Key, T, Compare >* >(this)->traverse_breadth(f);
}

template< class Key, class T, class Compare >
template< typename F >
F maslevtsov::Tree< Key, T, Compare >::traverse_breadth(F f) const
{
  if (empty()) {
    throw std::logic_error("nothing to traverse");
  }
  Queue< Node* > queue;
  queue.push(dummy_root_->left);
  while (!queue.empty()) {
    Node* current = queue.front();
    queue.pop();
    f(current->data1);
    if (!current->is_two) {
      f(current->data2);
    }
    if (current->left) {
      queue.push(current->left);
    }
    if (current->middle) {
      queue.push(current->middle);
    }
    if (current->right) {
      queue.push(current->right);
    }
  }
  return f;
}

template< class Key, class T, class Compare >
bool maslevtsov::Tree< Key, T, Compare >::empty() const noexcept
{
  return size_ == 0;
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::size_type maslevtsov::Tree< Key, T, Compare >::size() const noexcept
{
  return size_;
}

template< class Key, class T, class Compare >
void maslevtsov::Tree< Key, T, Compare >::clear() noexcept
{
  Node* current = iterator::get_min_node(dummy_root_);
  while (current && !empty()) {
    Node* cur_parent = current->parent;
    Node* next = nullptr;
    if (cur_parent->left == current) {
      if (cur_parent->middle) {
        next = cur_parent->middle;
      } else {
        next = cur_parent->right;
      }
      next = iterator::get_min_node(next);
    } else if (cur_parent->middle == current) {
      next = iterator::get_min_node(cur_parent->right);
    } else {
      next = cur_parent;
    }
    delete current;
    current = next;
  }
  size_ = 0;
}

template< class Key, class T, class Compare >
std::pair< typename maslevtsov::Tree< Key, T, Compare >::iterator, bool >
  maslevtsov::Tree< Key, T, Compare >::insert(const value_type& value)
{
  if (empty()) {
    Node* new_node = new Node{dummy_root_, nullptr, nullptr, nullptr, true, value, value_type()};
    dummy_root_->left = new_node;
    ++size_;
    return {iterator(new_node, true), true};
  }
  std::pair< iterator, bool > result = find_impl(value.first);
  if (result.second) {
    return result;
  }
  Node* current = dummy_root_->left;
  while (current->left || current->middle || (!current->is_two && current->right)) {
    if (current->is_two) {
      if (compare_(value.first, current->data1.first)) {
        current = current->left;
      } else {
        current = current->right;
      }
    } else {
      if (compare_(value.first, current->data1.first)) {
        current = current->left;
      } else if (compare_(value.first, current->data2.first)) {
        current = current->middle;
      } else {
        current = current->right;
      }
    }
  }
  if (current->is_two) {
    if (compare_(value.first, current->data1.first)) {
      assign_value(current->data2, current->data1);
      assign_value(current->data1, value);
      current->is_two = false;
      ++size_;
      return {iterator(current, true), true};
    }
    assign_value(current->data2, value);
    current->is_two = false;
    ++size_;
    return {iterator(current, false), true};
  }
  value_type to_insert(value);
  split_nodes(current, to_insert);
  ++size_;
  return find_impl(value.first);
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::iterator maslevtsov::Tree< Key, T, Compare >::erase(iterator pos) noexcept
{
  if (pos == end()) {
    return end();
  }
  if (size_ == 1) {
    delete dummy_root_->left;
    dummy_root_->left = nullptr;
    --size_;
    return end();
  }
  iterator after_removed = pos;
  Key next_key = (++after_removed)->first;
  if (pos.node_->left) {
    Node* with_replacement = nullptr;
    if (!pos.node_->is_two && !pos.is_first_) {
      with_replacement = pos.node_->middle;
    } else {
      with_replacement = pos.node_->left;
    }
    Node* leaf = iterator::get_max_node(with_replacement);
    iterator replacement_it(leaf, leaf->is_two ? true : false);
    std::swap(*pos, *replacement_it);
    pos = replacement_it;
  }
  erase_from_leaf(pos);
  --size_;
  return after_removed == end() ? end() : find(next_key);
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::iterator
  maslevtsov::Tree< Key, T, Compare >::erase(const_iterator pos) noexcept
{
  return erase(iterator(pos.node_, pos.is_first_));
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::size_type
  maslevtsov::Tree< Key, T, Compare >::erase(const Key& key) noexcept
{
  iterator it = find(key);
  if (it == end()) {
    return 0;
  }
  erase(it);
  return 1;
}

template< class Key, class T, class Compare >
void maslevtsov::Tree< Key, T, Compare >::swap(Tree& other) noexcept
{
  std::swap(dummy_root_, other.dummy_root_);
  std::swap(size_, other.size_);
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::size_type
  maslevtsov::Tree< Key, T, Compare >::count(const Key& key) const noexcept
{
  if (find(key) != cend()) {
    return 1;
  }
  return 0;
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::iterator
  maslevtsov::Tree< Key, T, Compare >::find(const Key& key) noexcept
{
  auto result = find_impl(key);
  if (result.second) {
    return iterator(result.first.node_, result.first.is_first_);
  }
  return end();
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::const_iterator
  maslevtsov::Tree< Key, T, Compare >::find(const Key& key) const noexcept
{
  auto result = find_impl(key);
  if (result.second) {
    return const_iterator(result.first.node_, result.first.is_first_);
  }
  return cend();
}

template< class Key, class T, class Compare >
std::pair< typename maslevtsov::Tree< Key, T, Compare >::iterator,
  typename maslevtsov::Tree< Key, T, Compare >::iterator >
  maslevtsov::Tree< Key, T, Compare >::equal_range(const Key& key)
{
  iterator first_it = find_impl(key).first;
  iterator second_it = first_it;
  if (second_it != end()) {
    ++second_it;
  }
  return std::make_pair(first_it, second_it);
}

template< class Key, class T, class Compare >
std::pair< typename maslevtsov::Tree< Key, T, Compare >::const_iterator,
  typename maslevtsov::Tree< Key, T, Compare >::const_iterator >
  maslevtsov::Tree< Key, T, Compare >::equal_range(const Key& key) const
{
  iterator it = find_impl(key).first;
  const_iterator first_it = const_iterator(it.node_, it.is_first_);
  const_iterator second_it = first_it;
  if (second_it != cend()) {
    ++second_it;
  }
  return std::make_pair(first_it, second_it);
}

template< class Key, class T, class Compare >
void maslevtsov::Tree< Key, T, Compare >::split_nodes(Node* node, value_type& to_insert, Node* left_child,
  Node* right_child)
{
  value_type values_to_split[3] = {node->data1, node->data2, to_insert};
  if (compare_(values_to_split[2].first, values_to_split[0].first)) {
    std::swap(values_to_split[2], values_to_split[0]);
  }
  if (compare_(values_to_split[1].first, values_to_split[0].first)) {
    std::swap(values_to_split[1], values_to_split[0]);
  }
  if (compare_(values_to_split[2].first, values_to_split[1].first)) {
    std::swap(values_to_split[2], values_to_split[1]);
  }
  Node* left_node = new Node{nullptr, nullptr, nullptr, nullptr, true, values_to_split[0], value_type()};
  Node* right_node = nullptr;
  try {
    right_node = new Node{nullptr, nullptr, nullptr, nullptr, true, values_to_split[2], value_type()};
  } catch (...) {
    delete left_node;
    clear_subtree(left_child);
    clear_subtree(right_child);
    throw;
  }
  if (node->left) {
    Node* kids[4] = {node->left, node->middle, node->right, nullptr};
    if (compare_(to_insert.first, node->data1.first)) {
      kids[3] = kids[2];
      kids[2] = kids[1];
      kids[0] = left_child;
      kids[1] = right_child;
    } else if (compare_(node->data2.first, to_insert.first)) {
      kids[3] = right_child;
      kids[2] = left_child;
    } else {
      kids[3] = kids[2];
      kids[2] = right_child;
      kids[1] = left_child;
    }
    left_node->left = kids[0];
    if (kids[0]) {
      kids[0]->parent = left_node;
    }
    left_node->right = kids[1];
    if (kids[1]) {
      kids[1]->parent = left_node;
    }
    right_node->left = kids[2];
    if (kids[2]) {
      kids[2]->parent = right_node;
    }
    right_node->right = kids[3];
    if (kids[3]) {
      kids[3]->parent = right_node;
    }
  }
  Node* parent = node->parent;
  if (parent == dummy_root_) {
    try {
      node->data1 = values_to_split[1];
      node->data2 = value_type();
    } catch (...) {
      delete left_node;
      delete right_node;
      clear_subtree(left_child);
      clear_subtree(right_child);
      std::terminate();
    }
    node->left = left_node;
    left_node->parent = node;
    node->right = right_node;
    right_node->parent = node;
    node->middle = nullptr;
    node->is_two = true;
    return;
  } else if (parent->is_two) {
    if (compare_(values_to_split[1].first, parent->data1.first)) {
      try {
        parent->data2 = parent->data1;
        parent->data1 = values_to_split[1];
      } catch (...) {
        delete left_node;
        delete right_node;
        clear_subtree(left_child);
        clear_subtree(right_child);
        std::terminate();
      }
      parent->left = left_node;
      parent->middle = right_node;
    } else {
      try {
        parent->data2 = values_to_split[1];
      } catch (...) {
        delete left_node;
        delete right_node;
        clear_subtree(left_child);
        clear_subtree(right_child);
        std::terminate();
      }
      parent->middle = left_node;
      parent->right = right_node;
    }
    parent->is_two = false;
    left_node->parent = parent;
    right_node->parent = parent;
    delete node;
    return;
  } else {
    split_nodes(parent, values_to_split[1], left_node, right_node);
  }
}

template< class Key, class T, class Compare >
void maslevtsov::Tree< Key, T, Compare >::clear_subtree(Node* node) noexcept
{
  if (!node) {
    return;
  }
  clear_subtree(node->left);
  clear_subtree(node->middle);
  clear_subtree(node->right);
  delete node;
}

template< class Key, class T, class Compare >
void maslevtsov::Tree< Key, T, Compare >::assign_value(value_type& dist, const value_type& src)
{
  try {
    dist = src;
  } catch (...) {
    std::terminate();
  }
}

template< class Key, class T, class Compare >
void maslevtsov::Tree< Key, T, Compare >::erase_from_leaf(iterator pos) noexcept
{
  if (!pos.node_->is_two) {
    if (pos.is_first_) {
      std::swap(pos.node_->data1, pos.node_->data2);
    }
    pos.node_->is_two = true;
    assign_value(pos.node_->data2, value_type());
  } else {
    Node* to_delete = pos.node_;
    assign_value(to_delete->data1, value_type());
    if (!to_delete->parent->is_two) {
      balance_parent_three(to_delete);
    } else {
      balance_parent_two(to_delete);
    }
  }
}

template< class Key, class T, class Compare >
void maslevtsov::Tree< Key, T, Compare >::balance_parent_three(Node* deleted) noexcept
{
  Node* parent = deleted->parent;
  if (parent->middle == deleted) {
    if (!parent->right->is_two) {
      std::swap(deleted->data1, parent->data2);
      std::swap(parent->data2, parent->right->data1);
      std::swap(parent->right->data1, parent->right->data2);
      assign_value(parent->right->data2, value_type());
      parent->right->is_two = true;
    } else if (!parent->left->is_two) {
      std::swap(deleted->data1, parent->data1);
      std::swap(parent->data1, parent->left->data2);
      assign_value(parent->left->data2, value_type());
      parent->left->is_two = true;
    } else {
      std::swap(parent->data2, parent->right->data2);
      std::swap(parent->right->data1, parent->right->data2);
      parent->right->is_two = false;
      parent->is_two = true;
      assign_value(parent->middle->data1, value_type());
      delete parent->middle;
      parent->middle = nullptr;
    }
  } else if (parent->left == deleted) {
    if (!parent->middle->is_two) {
      std::swap(deleted->data1, parent->data1);
      std::swap(parent->data1, parent->middle->data1);
      std::swap(parent->middle->data1, parent->middle->data2);
      assign_value(parent->middle->data2, value_type());
      parent->middle->is_two = true;
    } else {
      std::swap(parent->data1, parent->middle->data2);
      std::swap(parent->data1, parent->data2);
      std::swap(parent->middle->data1, parent->middle->data2);
      parent->middle->is_two = false;
      parent->is_two = true;
      assign_value(parent->left->data1, value_type());
      delete parent->left;
      parent->left = parent->middle;
      parent->middle = nullptr;
    }
  } else {
    if (!parent->middle->is_two) {
      std::swap(deleted->data1, parent->data2);
      std::swap(parent->data2, parent->middle->data2);
      assign_value(parent->middle->data2, value_type());
      parent->middle->is_two = true;
    } else {
      std::swap(parent->data2, parent->middle->data2);
      parent->middle->is_two = false;
      parent->is_two = true;
      assign_value(parent->right->data1, value_type());
      delete parent->right;
      parent->right = parent->middle;
      parent->middle = nullptr;
    }
  }
}

template< class Key, class T, class Compare >
void maslevtsov::Tree< Key, T, Compare >::balance_parent_two(Node* deleted) noexcept
{
  Node* parent = deleted->parent;
  if (parent->left == deleted && !parent->right->is_two) {
    std::swap(parent->data1, parent->left->data1);
    std::swap(parent->data1, parent->right->data1);
    std::swap(parent->right->data1, parent->right->data2);
    assign_value(parent->right->data2, value_type());
    parent->right->is_two = true;
  } else if (parent->right == deleted && !parent->left->is_two) {
    std::swap(parent->data1, parent->right->data1);
    std::swap(parent->data1, parent->left->data2);
    assign_value(parent->left->data2, value_type());
    parent->left->is_two = true;
  } else {
    balance_parent_bro_two(deleted);
  }
}

template< class Key, class T, class Compare >
void maslevtsov::Tree< Key, T, Compare >::balance_parent_bro_two(Node* deleted) noexcept
{
  Node* parent = deleted->parent;
  assign_value(deleted->data1, value_type());
  if (parent->left == deleted) {
    delete deleted;
    parent->left = nullptr;
    std::swap(parent->data1, parent->right->data2);
    std::swap(parent->right->data1, parent->right->data2);
    std::swap(parent->left, parent->right);
  } else {
    delete deleted;
    parent->right = nullptr;
    std::swap(parent->data1, parent->left->data2);
  }
  parent->left->is_two = false;
  Node* current = parent;
  Node* cur_parent = current->parent;
  bool is_balanced = false;
  while (cur_parent != dummy_root_) {
    if (cur_parent->is_two) {
      is_balanced = balance_next_parent_two(current, cur_parent);
    } else {
      balance_next_parent_three(current, cur_parent);
      is_balanced = true;
    }
    if (is_balanced) {
      break;
    }
    current = cur_parent;
    cur_parent = current->parent;
  }
  if (!is_balanced) {
    dummy_root_->left = current->left;
    current->left = nullptr;
    delete current;
    dummy_root_->left->parent = dummy_root_;
  }
}

template< class Key, class T, class Compare >
void maslevtsov::Tree< Key, T, Compare >::balance_next_parent_three(Node* deleted, Node* next_parent) noexcept
{
  if (next_parent->left == deleted) {
    if (!next_parent->middle->is_two) {
      std::swap(deleted->data1, next_parent->data1);
      deleted->right = next_parent->middle->left;
      std::swap(next_parent->data1, next_parent->middle->data1);
      std::swap(next_parent->middle->data1, next_parent->middle->data2);
      next_parent->middle->is_two = true;
      next_parent->middle->left = next_parent->middle->middle;
      next_parent->middle->middle = nullptr;
    } else {
      std::swap(next_parent->data1, next_parent->middle->data2);
      std::swap(next_parent->data1, next_parent->data2);
      std::swap(next_parent->middle->data1, next_parent->middle->data2);
      next_parent->middle->is_two = false;
      next_parent->is_two = true;
      next_parent->middle->middle = next_parent->middle->left;
      next_parent->middle->left = next_parent->left->left;
      next_parent->left->left->parent = next_parent->middle;
      next_parent->left->left = nullptr;
      delete next_parent->left;
      next_parent->left = next_parent->middle;
      next_parent->middle = nullptr;
    }
  } else if (next_parent->middle == deleted) {
    if (!next_parent->left->is_two) {
      std::swap(deleted->data1, next_parent->data1);
      deleted->right = deleted->left;
      deleted->left = next_parent->left->right;
      next_parent->left->right->parent = deleted;
      std::swap(next_parent->data1, next_parent->left->data2);
      next_parent->left->is_two = true;
      next_parent->left->right = next_parent->left->middle;
      next_parent->left->middle = nullptr;
    } else if (!next_parent->right->is_two) {
      std::swap(deleted->data1, next_parent->data2);
      deleted->right = next_parent->right->left;
      std::swap(next_parent->data2, next_parent->right->data1);
      std::swap(next_parent->right->data1, next_parent->right->data2);
      next_parent->right->is_two = true;
      next_parent->right->left = next_parent->right->middle;
      next_parent->right->middle = nullptr;
    } else {
      std::swap(next_parent->data1, next_parent->left->data2);
      std::swap(next_parent->data1, next_parent->data2);
      next_parent->left->is_two = false;
      next_parent->is_two = true;
      next_parent->left->middle = next_parent->left->right;
      next_parent->left->right = next_parent->middle->left;
      next_parent->middle->left->parent = next_parent->left;
      next_parent->middle->left = nullptr;
      delete next_parent->middle;
      next_parent->middle = nullptr;
    }
  } else {
    if (!next_parent->middle->is_two) {
      std::swap(deleted->data1, next_parent->data2);
      deleted->right = deleted->left;
      deleted->left = next_parent->middle->right;
      next_parent->middle->right->parent = deleted;
      std::swap(next_parent->data2, next_parent->middle->data2);
      next_parent->middle->is_two = true;
      next_parent->middle->right = next_parent->middle->middle;
      next_parent->middle->middle = nullptr;
    } else {
      std::swap(next_parent->data2, next_parent->middle->data2);
      next_parent->middle->is_two = false;
      next_parent->is_two = true;
      next_parent->middle->middle = next_parent->middle->right;
      next_parent->middle->right = next_parent->right->left;
      next_parent->right->left->parent = next_parent->middle;
      next_parent->right->left = nullptr;
      delete next_parent->right;
      next_parent->right = next_parent->middle;
      next_parent->middle = nullptr;
    }
  }
}

template< class Key, class T, class Compare >
bool maslevtsov::Tree< Key, T, Compare >::balance_next_parent_two(Node* deleted, Node* next_parent) noexcept
{
  if (next_parent->left == deleted) {
    if (next_parent->right->is_two) {
      std::swap(next_parent->data1, next_parent->right->data2);
      std::swap(next_parent->right->data1, next_parent->right->data2);
      next_parent->right->is_two = false;
      next_parent->right->middle = next_parent->right->left;
      next_parent->right->left = deleted->left;
      deleted->left->parent = next_parent->right;
      deleted->left = nullptr;
      delete deleted;
      next_parent->left = next_parent->right;
      next_parent->right = nullptr;
      return false;
    } else {
      std::swap(deleted->data1, next_parent->data1);
      deleted->right = next_parent->right->left;
      next_parent->right->left->parent = deleted;
      std::swap(next_parent->data1, next_parent->right->data1);
      std::swap(next_parent->right->data1, next_parent->right->data2);
      next_parent->right->is_two = true;
      next_parent->right->left = next_parent->right->middle;
      next_parent->right->middle = nullptr;
      return true;
    }
  } else {
    if (next_parent->left->is_two) {
      std::swap(next_parent->data1, next_parent->left->data2);
      next_parent->left->is_two = false;
      next_parent->left->middle = next_parent->left->right;
      next_parent->left->right = deleted->left;
      deleted->left->parent = next_parent->left;
      deleted->left = nullptr;
      delete deleted;
      next_parent->right = nullptr;
      return false;
    } else {
      std::swap(deleted->data1, next_parent->data1);
      deleted->right = deleted->left;
      deleted->left = next_parent->left->right;
      std::swap(next_parent->data1, next_parent->right->data2);
      next_parent->left->is_two = true;
      next_parent->left->right = next_parent->right->middle;
      next_parent->left->middle = nullptr;
      return true;
    }
  }
}

template< class Key, class T, class Compare >
std::pair< typename maslevtsov::Tree< Key, T, Compare >::iterator, bool >
  maslevtsov::Tree< Key, T, Compare >::find_impl(const Key& key) const noexcept
{
  if (empty()) {
    return {iterator(dummy_root_, true), false};
  }
  Node* current = dummy_root_->left;
  Node* greater = nullptr;
  bool greater_is_first = true;
  while (current) {
    if (!compare_(key, current->data1.first) && !compare_(current->data1.first, key)) {
      return std::make_pair(iterator(current, true), true);
    }
    if (!current->is_two && !compare_(key, current->data2.first) && !compare_(current->data2.first, key)) {
      return std::make_pair(iterator(current, false), true);
    }
    if (compare_(key, current->data1.first)) {
      greater = current;
      greater_is_first = true;
      current = current->left;
    } else if (!current->is_two && compare_(key, current->data2.first)) {
      greater = current;
      greater_is_first = false;
      current = current->middle;
    } else {
      current = current->right;
    }
  }
  if (greater) {
    return std::make_pair(iterator(greater, greater_is_first), false);
  } else {
    return std::make_pair(iterator(dummy_root_, true), false);
  }
}

#endif
