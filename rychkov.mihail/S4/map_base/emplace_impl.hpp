#ifndef MAP_BASE_EMPLACE_IMPL_HPP
#define MAP_BASE_EMPLACE_IMPL_HPP

#include "declaration.hpp"

#include <utility>
#include <limits>
#include <type_traits>

template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
template< class... Args >
std::pair< typename rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::iterator, bool >
    rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::emplace_hint_impl(std::pair< const_iterator, bool > hint_pair,
      Args&&... args)
{
  static_assert(std::is_nothrow_move_constructible< real_value_type >::value, "move construct need to be nothrow");

  const_iterator hint = hint_pair.first;
  if (!hint_pair.second)
  {
    return {{hint.node_, hint.pointed_}, false};
  }

  if (size_ == 0)
  {
    node_type* root = new node_type;
    fake_children_[0] = root;
    root->emplace_back(std::forward< Args >(args)...);
    root->parent = fake_root();
    cached_begin_ = root;
    cached_rbegin_ = root;
    size_++;
    return {begin(), true};
  }
  if (!hint.node_->full())
  {
    hint.node_->emplace(hint.pointed_, std::forward< Args >(args)...);
    size_++;
    return {{hint.node_, hint.pointed_}, true};
  }

  node_type to_insert;
  to_insert.emplace_back(std::forward< Args >(args)...);

  constexpr size_t max_tree_depth = std::numeric_limits< size_t >::digits + 1;
  struct MemSaver
  {
    node_type* data[max_tree_depth];
    node_size_type ins_points[max_tree_depth];
    size_t size = 0;
    ~MemSaver()
    {
      while (size > 0)
      {
        delete data[--size];
      }
    }
    void push(node_size_type pointed)
    {
      data[size] = new node_type;
      ins_points[size++] = pointed;
    }
  };
  MemSaver storage;
  const_iterator temp = hint;
  for (; temp.node_->full(); temp.move_up())
  {
    storage.push(temp.pointed_);
  }
  if (temp.node_->isfake())
  {
    storage.push(0);
  }

  node_type* caret = hint.node_;
  for (size_type i = 0; caret->full(); caret = caret->parent, i++)
  {
    devide(*caret, *storage.data[i], storage.ins_points[i], to_insert);
    correct_emplace_result(*caret, *storage.data[i], storage.ins_points[i], hint);
    if (cached_rbegin_ == caret)
    {
      cached_rbegin_ = storage.data[i];
    }
    storage.data[i] = nullptr;
  }
  if (caret->isfake())
  {
    node_type* root = storage.data[storage.size - 1];
    to_insert.children[0]->parent = root;
    to_insert.children[1]->parent = root;
    caret->children[0] = root;
    root->parent = caret;
    root->emplace_back(std::move(to_insert[0]));
    root->children[0] = to_insert.children[0];
    root->children[1] = to_insert.children[1];
    to_insert.pop_back();
    correct_emplace_result(*root, *storage.data[storage.size - 1], 0, hint);
    storage.data[storage.size - 1] = nullptr;
  }
  else
  {
    caret->emplace(temp.pointed_, std::move(to_insert[0]));
    caret->children[temp.pointed_] = to_insert.children[0];
    caret->children[temp.pointed_ + 1] = to_insert.children[1];
    if (hint.pointed_ > node_middle)
    {
      hint = {caret, temp.pointed_};
    }
  }
  storage.size = 0;
  size_++;
  return {{hint.node_, hint.pointed_}, true};
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
void rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::devide(node_type& left, node_type& right,
      node_size_type ins_point, node_type& to_insert)
{
  if (ins_point <= node_middle)
  {
    for (node_size_type i = node_middle; i < node_capacity; i++)
    {
      right.emplace_back(std::move(left[i]));
      right.children[right.size()] = left.children[i + 1];
    }
    left.children[ins_point] = to_insert.children[0];
    for (node_size_type i = node_middle; i < node_capacity; i++)
    {
      left.pop_back();
    }
    if (ins_point < node_middle)
    {
      right.children[0] = left.children[node_middle];
      to_insert.emplace_back(std::move(left[node_middle - 1]));
      left.pop_back();
      left.emplace(ins_point, std::move(to_insert[0]));
      left.children[ins_point] = to_insert.children[0];
      left.children[ins_point + 1] = to_insert.children[1];
      to_insert.erase(0);
    }
    else
    {
      right.children[0] = to_insert.children[1];
      if (!to_insert.isleaf())
      {
        to_insert.children[1]->parent = &right;
      }
    }
  }
  else
  {
    to_insert.emplace_back(std::move(left[node_middle]));
    for (node_size_type i = node_middle + 1; i < ins_point; i++)
    {
      right.emplace_back(std::move(left[i]));
      right.children[right.size()] = left.children[i + 1];
    }
    right.children[0] = left.children[node_middle + 1];
    right.emplace_back(std::move(to_insert[0]));
    right.children[right.size() - 1] = to_insert.children[0];
    right.children[right.size()] = to_insert.children[1];
    for (node_size_type i = ins_point; i < node_capacity; i++)
    {
      right.emplace_back(std::move(left[i]));
      right.children[right.size()] = left.children[i + 1];
    }
    for (node_size_type i = node_middle; i < node_capacity; i++)
    {
      left.pop_back();
    }
    to_insert.erase(0);
  }
  if (!right.isleaf())
  {
    for (node_size_type i = 0; i < right.size() + 1; i++)
    {
      right.children[i]->parent = &right;
    }
  }

  right.parent = left.parent;
  to_insert.children[0] = &left;
  to_insert.children[1] = &right;
}
template< class K, class T, class C, size_t N, bool IsSet, bool IsMulti >
void rychkov::MapBase< K, T, C, N, IsSet, IsMulti >::correct_emplace_result(node_type& left, node_type& right,
    node_size_type ins_point, const_iterator& hint)
{
  if ((hint.pointed_ > node_middle) && (ins_point != node_middle))
  {
    if (ins_point < node_middle)
    {
      hint.pointed_ = ins_point;
      hint.node_ = &left;
    }
    else
    {
      hint.pointed_ = hint.pointed_ - node_middle - 1;
      hint.node_ = &right;
    }
  }
}

#endif
