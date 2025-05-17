#ifndef TREE_DEFINITION_HPP
#define TREE_DEFINITION_HPP

#include "declaration.hpp"
#include <utility>

template< class Key, class T, class Compare >
maslevtsov::Tree< Key, T, Compare >::Tree():
  dummy_root_(new Node{value_type(), value_type(), nullptr, nullptr, nullptr, nullptr, true}),
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
T& maslevtsov::Tree< Key, T, Compare >::operator[](Key&& key)
{
  return insert(std::make_pair(std::move(key), T())).first->second;
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
  return {iterator::get_min_node(dummy_root_), true};
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::const_iterator maslevtsov::Tree< Key, T, Compare >::begin() const
{
  return {const_iterator::get_min_node(dummy_root_), true};
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::const_iterator
  maslevtsov::Tree< Key, T, Compare >::cbegin() const noexcept
{
  return {const_iterator::get_min_node(dummy_root_), true};
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::iterator maslevtsov::Tree< Key, T, Compare >::end()
{
  return {dummy_root_, true};
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::const_iterator maslevtsov::Tree< Key, T, Compare >::end() const
{
  return {dummy_root_, true};
}

template< class Key, class T, class Compare >
typename maslevtsov::Tree< Key, T, Compare >::const_iterator maslevtsov::Tree< Key, T, Compare >::cend() const noexcept
{
  return {dummy_root_, true};
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
  while (current && current != dummy_root_) {
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
    Node* new_node = new Node{value, value, dummy_root_, nullptr, nullptr, nullptr, true};
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
      current->data1 = value;
      current->is_two = false;
      ++size_;
      return {iterator(current, true), true};
    }
    current->data2 = value;
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
  Node* left_node = new Node{values_to_split[0], values_to_split[0], nullptr, nullptr, nullptr, nullptr, true};
  Node* right_node = nullptr;
  try {
    right_node = new Node{values_to_split[2], values_to_split[2], nullptr, nullptr, nullptr, nullptr, true};
  } catch (const std::bad_alloc&) {
    delete left_node;
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
    node->data1 = values_to_split[1];
    node->data2 = values_to_split[1];
    node->left = left_node;
    left_node->parent = node;
    node->right = right_node;
    right_node->parent = node;
    node->middle = nullptr;
    node->is_two = true;
    return;
  } else if (parent->is_two) {
    if (compare_(values_to_split[1].first, parent->data1.first)) {
      parent->data2 = parent->data1;
      parent->data1 = values_to_split[1];
      parent->left = left_node;
      parent->middle = right_node;
    } else {
      parent->data2 = values_to_split[1];
      parent->middle = left_node;
      parent->right = right_node;
    }
    parent->is_two = false;
    left_node->parent = parent;
    right_node->parent = parent;
    delete node;
    return;
  } else {
    return split_nodes(parent, values_to_split[1], left_node, right_node);
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
