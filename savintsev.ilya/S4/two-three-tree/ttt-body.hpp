#ifndef TTT_BODY_HPP
#define TTT_BODY_HPP
#include <functional>
#include <iostream>
#include <cassert>
#include "ttt-node.hpp"
#include "ttt-iterator.hpp"
#include "ttt-const-iterator.hpp"

namespace savintsev
{
  template< typename Key, typename Value, typename Compare = std::less< Key > >
  class TwoThreeTree
  {
  public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair< Key, Value >;
    using node_type = node_t< value_type >;

    using iterator = BidirectIterator< Key, Value >;
    using const_iterator = BidirectConstIterator< Key, Value >;

    using size_type = size_t;

    ~TwoThreeTree();
    TwoThreeTree() = default;
    TwoThreeTree(const TwoThreeTree & other);
    TwoThreeTree(TwoThreeTree && other) noexcept;
    TwoThreeTree & operator=(const TwoThreeTree & other);
    TwoThreeTree & operator=(TwoThreeTree && other);

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    void clear() noexcept;
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type count(const key_type & k) const;

    void swap(TwoThreeTree & other) noexcept;

    iterator find(const key_type & k);
    const_iterator find (const key_type & k) const;

    mapped_type & at(const key_type & k);
    const mapped_type & at(const key_type & k) const;

    mapped_type & operator[](const key_type & k);
    mapped_type & operator[](key_type && k);

    std::pair< iterator, bool > insert(const value_type & val);

    iterator erase(const_iterator position);
    size_type erase(const key_type & k);

    std::pair< const_iterator, const_iterator > equal_range(const key_type & k) const;
    std::pair< iterator, iterator> equal_range(const key_type & k);
  private:
    node_type * root_ = nullptr;
    size_t size_ = 0;

    std::pair< iterator, bool > lazy_find(const key_type & k) const;
    std::pair< iterator, bool > insert_node(node_type * target, const value_type & val);

    node_type * split_node(node_type * node);
    void insert_data_in_node(node_type * node, const value_type & val);
    void remove_data_from_node(node_type * node, const value_type & val);
    void remove_data_from_node(node_type * node, const key_type & k);
    node_type * clear_nodes(node_type * node);
    node_type * clone_nodes(node_type * other);
    node_type * search_min(node_type * root) const;
    node_type * fix_nodes_properties(node_type * leaf);
    node_type * redistribute_nodes(node_type * leaf);
    node_type * merge_nodes(node_type * leaf);
  };

  template< typename Key, typename Value, typename Compare >
  TwoThreeTree< Key, Value, Compare >::~TwoThreeTree()
  {
    clear();
  }

  template< typename Key, typename Value, typename Compare >
  TwoThreeTree< Key, Value, Compare >::TwoThreeTree(const TwoThreeTree & other):
    root_(nullptr),
    size_(0)
  {
    if (other.root_)
    {
      try
      {
        root_ = clone_nodes(other.root_);
      }
      catch (const std::bad_alloc & e)
      {
        root_ = clear_nodes(root_);
        throw;
      }
      size_ = other.size_;
    }
  }

  template< typename K, typename V, typename C >
  TwoThreeTree< K, V, C > & TwoThreeTree< K, V, C >::operator=(const TwoThreeTree & other)
  {
    if (this != std::addressof(other))
    {
      TwoThreeTree temp(other);
      swap(temp);
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  TwoThreeTree< Key, Value, Compare >::TwoThreeTree(TwoThreeTree && other) noexcept:
    root_(other.root_),
    size_(other.size_)
  {
    other.root_ = nullptr;
    other.size_ = 0;
  }

  template< typename K, typename V, typename C >
  TwoThreeTree< K, V, C > & TwoThreeTree< K, V, C >::operator=(TwoThreeTree && other)
  {
    if (this != std::addressof(other))
    {
      root_ = clear_nodes(root_);
      root_ = other.root_;
      size_ = other.size_;
      other.root_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }
  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::iterator TwoThreeTree< K, V, C >::begin() noexcept
  {
    if (!root_ || root_->len == 0)
    {
      return end();
    }

    node_type * min_node = search_min(root_);
    return iterator(root_, min_node, 0);
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::iterator TwoThreeTree< K, V, C >::end() noexcept
  {
    return iterator(root_, nullptr, 0);
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::const_iterator TwoThreeTree< K, V, C >::begin() const noexcept
  {
    if (!root_ || root_->len == 0)
    {
      return end();
    }

    node_type * min_node = search_min(root_);
    return const_iterator(root_, min_node, 0);
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::const_iterator TwoThreeTree< K, V, C >::end() const noexcept
  {
    return const_iterator(root_, nullptr, 0);
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::const_iterator TwoThreeTree< K, V, C >::cbegin() const noexcept
  {
    if (!root_ || root_->len == 0)
    {
      return end();
    }

    node_type * min_node = search_min(root_);
    return const_iterator(root_, min_node, 0);
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::const_iterator TwoThreeTree< K, V, C >::cend() const noexcept
  {
    return const_iterator(root_, nullptr, 0);
  }

  template< typename Key, typename Value, typename Compare >
  void TwoThreeTree< Key, Value, Compare >::clear() noexcept
  {
    root_ = clear_nodes(root_);
    size_ = 0;
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::size_type TwoThreeTree< K, V, C >::size() const noexcept
  {
    return size_;
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::size_type TwoThreeTree< K, V, C >::count(const key_type & k) const
  {
    auto result = lazy_find(k);
    if (result.second)
    {
      return 1;
    }
    return 0;
  }

  template< typename Key, typename Value, typename Compare >
  void TwoThreeTree< Key, Value, Compare >::swap(TwoThreeTree & other) noexcept
  {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
  }

  template< typename K, typename V, typename C >
  bool TwoThreeTree< K, V, C >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::iterator TwoThreeTree< K, V, C >::find(const key_type & k)
  {
    auto result = lazy_find(k);
    return result.second ? result.first : end();
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::const_iterator TwoThreeTree< K, V, C >::find(const key_type & k) const
  {
    auto result = lazy_find(k);
    return result.second ? result.first : end();
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::mapped_type & TwoThreeTree< K, V, C >::operator[](const key_type & k)
  {
    auto res = lazy_find(k);
    if (res.second)
    {
      return res.first->second;
    }
    return insert_node(res.first.node_, {k, mapped_type{}}).first->second;
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::mapped_type & TwoThreeTree< K, V, C >::operator[](key_type && k)
  {
    auto res = lazy_find(k);
    if (res.second)
    {
      return res.first->second;
    }
    return insert_node(res.first.node_, {std::move(k), mapped_type{}}).first->second;
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::mapped_type & TwoThreeTree< K, V, C >::at(const key_type & k)
  {
    auto it_pair = lazy_find(k);
    if (it_pair.first == end() || !it_pair.second)
    {
      throw std::out_of_range("ERROR: Key not found");
    }
    return it_pair.first->second;
  }

  template< typename K, typename V, typename C >
  const typename TwoThreeTree< K, V, C >::mapped_type & TwoThreeTree< K, V, C >::at(const key_type & k) const
  {
    auto it_pair = lazy_find(k);
    if (!it_pair.second)
    {
      throw std::out_of_range("ERROR: Key not found");
    }
    return it_pair.first->second;
  }

  template< typename K, typename V, typename C >
  std::pair
  <
    typename TwoThreeTree< K, V, C >::iterator,
    bool
  >
  TwoThreeTree< K, V, C >::lazy_find(const key_type & k) const
  {
    if (!root_)
    {
      return {iterator(root_), false};
    }
    node_type * node = root_;
    while (node)
    {
      assert(node->len <= 3);
      for (size_t i = 0; i < node->len; ++i)
      {
        if (node->data[i].first == k)
        {
          return {iterator(root_, node, i), true};
        }
      }

      if (!node->kids[0])
      {
        return std::make_pair(iterator(root_, node, 0), false);
      }

      if (C{}(k, node->data[0].first))
      {
        node = node->kids[0];
      }
      else if (node->len == 1 || C{}(k, node->data[1].first))
      {
        node = node->kids[1];
      }
      else
      {
        node = node->kids[2];
      }
    }
    return {iterator(root_), false};
  }

  template< typename K, typename V, typename C >
  std::pair
  <
    typename TwoThreeTree< K, V, C >::iterator,
    bool
  >
  TwoThreeTree< K, V, C >::insert_node(node_type * target, const value_type & val)
  {
    node_type * old_root = root_;
    size_t old_size = size_;

    try
    {
      if (!target)
      {
        node_type * new_node = new node_type{};
        try
        {
          insert_data_in_node(new_node, val);
        }
        catch (...)
        {
          delete new_node;
          throw;
        }
        root_ = new_node;
        size_++;
        return {iterator(root_, new_node, 0), true};
      }

      node_type * current = target;
      bool data_inserted = false;

      try
      {
        insert_data_in_node(current, val);
        data_inserted = true;

        while (current && current->len == 3)
        {
          node_type * new_current = split_node(current);
          if (new_current == current)
          {
            break;
          }
          current = new_current;
        }

        while (current && current->father)
        {
          current = current->father;
        }
        root_ = current;
        size_++;

        return {lazy_find(val.first).first, true};
      }
      catch (...)
      {
        if (data_inserted)
        {
          remove_data_from_node(target, val.first);
          while (current && current != target)
          {
            current = merge_nodes(current);
          }
        }
        throw;
      }
    }
    catch (...)
    {
      root_ = old_root;
      size_ = old_size;
      throw;
    }
  }

  template< typename Key, typename Value, typename Compare >
  void TwoThreeTree< Key, Value, Compare >::insert_data_in_node(node_type * node, const value_type & val)
  {
    assert(node->len < 3);

    value_type tmp_data[3];
    size_t tmp_len = node->len;

    for (size_t i = 0; i < tmp_len; ++i)
    {
      tmp_data[i] = node->data[i];
    }

    tmp_data[tmp_len++] = val;

    if (tmp_len == 2)
    {
      if (!Compare{}(tmp_data[0].first, tmp_data[1].first))
      {
        std::swap(tmp_data[0], tmp_data[1]);
      }
    }
    else if (tmp_len == 3)
    {
      if (!Compare{}(tmp_data[0].first, tmp_data[1].first))
      {
        std::swap(tmp_data[0], tmp_data[1]);
      }
      if (!Compare{}(tmp_data[1].first, tmp_data[2].first))
      {
        std::swap(tmp_data[1], tmp_data[2]);
      }
      if (!Compare{}(tmp_data[0].first, tmp_data[1].first))
      {
        std::swap(tmp_data[0], tmp_data[1]);
      }
    }

    for (size_t i = 0; i < tmp_len; ++i)
    {
      node->data[i] = std::move(tmp_data[i]);
    }
    node->len = tmp_len;
  }

  template< typename Key, typename Value, typename Compare >
  void TwoThreeTree< Key, Value, Compare >::remove_data_from_node(node_type * node, const value_type & val)
  {
    remove_data_from_node(node, val.first);
  }

  template< typename Key, typename Value, typename Compare >
  void TwoThreeTree< Key, Value, Compare >::remove_data_from_node(node_type * node, const key_type & k)
  {
    if (node->len == 0)
    {
      return;
    }
    if (node->len >= 1 && node->data[0].first == k)
    {
      node->data[0] = node->data[1];
      node->data[1] = node->data[2];
      node->len--;
    }
    else if (node->len == 2 && node->data[1].first == k)
    {
      node->data[1] = node->data[2];
      node->len--;
    }
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::node_type * TwoThreeTree< K, V, C >::clear_nodes(node_type * node)
  {
    if (!node)
    {
      return nullptr;
    }
    for (size_t i = 0; i < 4; ++i)
    {
      node->kids[i] = clear_nodes(node->kids[i]);
    }
    delete node;
    return nullptr;
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::node_type * TwoThreeTree< K, V, C >::clone_nodes(node_type * other)
  {
    if (!other)
    {
      return nullptr;
    }
    node_type * root = nullptr;
    try
    {
      root = new node_type{};
      for (size_t i = 0; i < other->len; ++i)
      {
        root->data[i] = other->data[i];
      }
      root->len = other->len;
      for (size_t i = 0; i < 3; ++i)
      {
        if (other->kids[i])
        {
          root->kids[i] = clone_nodes(other->kids[i]);
          root->kids[i]->father = root;
        }
      }
    }
    catch (...)
    {
      root = clear_nodes(root);
      throw;
    }
    return root;
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::node_type * TwoThreeTree< K, V, C >::search_min(node_type * root) const
  {
    node_type * temp = root;
    if (!temp)
    {
      return nullptr;
    }

    while (temp->kids[0])
    {
      temp = temp->kids[0];
    }

    return (temp->len == 0 ? nullptr : temp);
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::node_type * TwoThreeTree< K, V, C >::fix_nodes_properties(node_type * leaf)
  {
    if (!leaf)
    {
      return nullptr;
    }

    if (!leaf->father && leaf->len == 0)
    {
      for (size_t i = 0; i < 4; ++i)
      {
        if (leaf->kids[i])
        {
          leaf->kids[i]->father = nullptr;
          node_type* new_root = leaf->kids[i];
          delete leaf;
          return new_root;
        }
      }
      delete leaf;
      return nullptr;
    }
    if (leaf->len != 0)
    {
      if (leaf->father)
      {
        return fix_nodes_properties(leaf->father);
      }
      return leaf;
    }
    else
    {
      node_type * parent = leaf->father;
      if (parent->kids[0]->len == 2 || parent->kids[1]->len == 2 || parent->len == 2)
      {
        leaf = redistribute_nodes(leaf);
      }
      else if (parent->len == 2 && parent->kids[2]->len == 2)
      {
        leaf = redistribute_nodes(leaf);
      }
      else
      {
        leaf = merge_nodes(leaf);
      }
      return fix_nodes_properties(leaf);
    }
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::node_type * TwoThreeTree< K, V, C >::redistribute_nodes(node_type * leaf)
  {
    node_type * parent = leaf->father;
    node_type * first = parent->kids[0];
    node_type * second = parent->kids[1];
    node_type * third = parent->kids[2];
    if ((parent->len == 2) && (first->len < 2) && (second->len < 2) && (third->len < 2))
    {
      if (first == leaf)
      {
        parent->kids[0] = parent->kids[1];
        parent->kids[1] = parent->kids[2];
        parent->kids[2] = nullptr;
        insert_data_in_node(parent->kids[0], parent->data[0]);
        parent->kids[0]->kids[2] = parent->kids[0]->kids[1];
        parent->kids[0]->kids[1] = parent->kids[0]->kids[0];

        if (leaf->kids[0])
        {
          parent->kids[0]->kids[0] = leaf->kids[0];
        }
        else if (leaf->kids[1])
        {
          parent->kids[0]->kids[0] = leaf->kids[1];
        }
        if (parent->kids[0]->kids[0])
        {
          parent->kids[0]->kids[0]->father = parent->kids[0];
        }
        remove_data_from_node(parent, parent->data[0]);
        delete first;
      }
      else if (second == leaf)
      {
        insert_data_in_node(first, parent->data[0]);
        remove_data_from_node(parent, parent->data[0]);
        if (leaf->kids[0])
        {
          first->kids[2] = leaf->kids[0];
        }
        else if (leaf->kids[1])
        {
          first->kids[2] = leaf->kids[1];
        }
        if (first->kids[2])
        {
          first->kids[2]->father = first;
        }
        parent->kids[1] = parent->kids[2];
        parent->kids[2] = nullptr;

        delete second;
      }
      else if (third == leaf)
      {
        insert_data_in_node(second, parent->data[1]);
        parent->kids[2] = nullptr;
        remove_data_from_node(parent, parent->data[1]);
        if (leaf->kids[0])
        {
          second->kids[2] = leaf->kids[0];
        }
        else if (leaf->kids[1])
        {
          second->kids[2] = leaf->kids[1];
        }
        if (second->kids[2])
        {
          second->kids[2]->father = second;
        }
        delete third;
      }
    }
    else if ((parent->len == 2) && ((first->len == 2) || (second->len == 2) || (third->len == 2)))
    {
      if (third == leaf)
      {
        if (leaf->kids[0])
        {
          leaf->kids[1] = leaf->kids[0];
          leaf->kids[0] = nullptr;
        }

        insert_data_in_node(leaf, parent->data[1]);
        if (second->len == 2)
        {
          parent->data[1] = second->data[1];
          remove_data_from_node(second, second->data[1]);
          leaf->kids[0] = second->kids[2];
          second->kids[2] = nullptr;
          if (leaf->kids[0])
          {
            leaf->kids[0]->father = leaf;
          }
        }
        else if (first->len == 2)
        {
          parent->data[1] = second->data[0];
          leaf->kids[0] = second->kids[1];
          second->kids[1] = second->kids[0];
          if (leaf->kids[0])
          {
            leaf->kids[0]->father = leaf;
          }
          second->data[0] = parent->data[0];
          parent->data[0] = first->data[1];
          remove_data_from_node(first, first->data[1]);
          second->kids[0] = first->kids[2];
          if (second->kids[0])
          {
            second->kids[0]->father = second;
          }
          first->kids[2] = nullptr;
        }
      }
      else if (second == leaf)
      {
        if (third->len == 2)
        {
          if (leaf->kids[0] == nullptr)
          {
            leaf->kids[0] = leaf->kids[1];
            leaf->kids[1] = nullptr;
          }
          insert_data_in_node(second, parent->data[1]);
          parent->data[1] = third->data[0];
          remove_data_from_node(third, third->data[0]);
          second->kids[1] = third->kids[0];
          if (second->kids[1])
          {
            second->kids[1]->father = second;
          }
          third->kids[0] = third->kids[1];
          third->kids[1] = third->kids[2];
          third->kids[2] = nullptr;
        }
        else if (first->len == 2)
        {
          if (leaf->kids[1] == nullptr)
          {
            leaf->kids[1] = leaf->kids[0];
            leaf->kids[0] = nullptr;
          }
          insert_data_in_node(second, parent->data[0]);
          parent->data[0] = first->data[1];
          remove_data_from_node(first, first->data[1]);
          second->kids[0] = first->kids[2];
          if (second->kids[0])
          {
            second->kids[0]->father = second;
          }
          first->kids[2] = nullptr;
        }
      }
      else if (first == leaf)
      {
        if (leaf->kids[0] == nullptr)
        {
          leaf->kids[0] = leaf->kids[1];
          leaf->kids[1] = nullptr;
        }
        insert_data_in_node(first, parent->data[0]);
        if (second->len == 2)
        {
          parent->data[0] = second->data[0];
          remove_data_from_node(second, second->data[0]);
          first->kids[1] = second->kids[0];
          if (first->kids[1])
          {
            first->kids[1]->father = first;
          }
          second->kids[0] = second->kids[1];
          second->kids[1] = second->kids[2];
          second->kids[2] = nullptr;
        }
        else if (third->len == 2)
        {
          parent->data[0] = second->data[0];
          second->data[0] = parent->data[1];
          parent->data[1] = third->data[0];
          remove_data_from_node(third, third->data[0]);
          first->kids[1] = second->kids[0];
          if (first->kids[1])
          {
            first->kids[1]->father = first;
          }
          second->kids[0] = second->kids[1];
          second->kids[1] = third->kids[0];
          if (second->kids[1])
          {
            second->kids[1]->father = second;
          }
          third->kids[0] = third->kids[1];
          third->kids[1] = third->kids[2];
          third->kids[2] = nullptr;
        }
      }
    }
    else if (parent->len == 1)
    {
      insert_data_in_node(leaf, parent->data[0]);

      if (first == leaf && second->len == 2)
      {
        parent->data[0] = second->data[0];
        remove_data_from_node(second, second->data[0]);

        if (leaf->kids[0] == nullptr)
        {
          leaf->kids[0] = leaf->kids[1];
        }
        leaf->kids[1] = second->kids[0];
        second->kids[0] = second->kids[1];
        second->kids[1] = second->kids[2];
        second->kids[2] = nullptr;
        if (leaf->kids[1])
        {
          leaf->kids[1]->father = leaf;
        }
      }
      else if (second == leaf && first->len == 2)
      {
        parent->data[0] = first->data[1];
        remove_data_from_node(first, first->data[1]);

        if (leaf->kids[1] == nullptr)
        {
          leaf->kids[1] = leaf->kids[0];
        }
        leaf->kids[0] = first->kids[2];
        first->kids[2] = nullptr;
        if (leaf->kids[0])
        {
          leaf->kids[0]->father = leaf;
        }
      }
    }
    return parent;
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::node_type * TwoThreeTree< K, V, C >::merge_nodes(node_type * leaf)
  {
    node_type * parent = leaf->father;

    if (parent->kids[0] == leaf)
    {
      insert_data_in_node(parent->kids[1], parent->data[0]);
      parent->kids[1]->kids[2] = parent->kids[1]->kids[1];
      parent->kids[1]->kids[1] = parent->kids[1]->kids[0];

      if (leaf->kids[0])
      {
        parent->kids[1]->kids[0] = leaf->kids[0];
      }
      else if (leaf->kids[1])
      {
        parent->kids[1]->kids[0] = leaf->kids[1];
      }
      if (parent->kids[1]->kids[0])
      {
        parent->kids[1]->kids[0]->father = parent->kids[1];
      }
      remove_data_from_node(parent, parent->data[0]);
      delete parent->kids[0];
      parent->kids[0] = nullptr;
    }
    else if (parent->kids[1] == leaf)
    {
      insert_data_in_node(parent->kids[0], parent->data[0]);

      if (leaf->kids[0])
      {
        parent->kids[0]->kids[2] = leaf->kids[0];
      }
      else if (leaf->kids[1])
      {
        parent->kids[0]->kids[2] = leaf->kids[1];
      }

      if (parent->kids[0]->kids[2])
      {
        parent->kids[0]->kids[2]->father = parent->kids[0];
      }
      remove_data_from_node(parent, parent->data[0]);
      delete parent->kids[1];
      parent->kids[1] = nullptr;
    }

    if (!parent->father)
    {
      node_type * temp = nullptr;
      if (parent->kids[0])
      {
        temp = parent->kids[0];
      }
      else
      {
        temp = parent->kids[1];
      }
      temp->father = nullptr;
      delete parent;
      return temp;
    }
    return parent;
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::iterator TwoThreeTree< K, V, C >::erase(const_iterator position)
  {
    if (!position.node_)
    {
      return end();
    }
    node_type * target = position.node_;
    node_type * closest = nullptr;
    if (position.pos_ + 1 < 3 && target->kids[position.pos_ + 1])
    {
      closest = search_min(target->kids[position.pos_ + 1]);
    }

    const key_type k = position->first;
    const key_type k_next = (++position)->first;
    if (closest)
    {
      std::swap(target->data[position.pos_], closest->data[0]);
      target = closest;
    }
    remove_data_from_node(target, k);
    root_ = fix_nodes_properties(target);
    size_--;
    return find(k_next);
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::size_type TwoThreeTree< K, V, C >::erase(const key_type & k)
  {
    std::pair< iterator, bool > result = lazy_find(k);
    if (!result.second)
    {
      return 0ull;
    }
    node_type * target = result.first.node_;
    node_type * closest = search_min(target->kids[result.first.pos_ + 1]);
    if (closest)
    {
      std::swap(target->data[result.first.pos_], closest->data[0]);
      target = closest;
    }
    remove_data_from_node(target, k);
    root_ = fix_nodes_properties(target);
    size_--;
    return 1ull;
  }

  template< typename K, typename V, typename C >
  std::pair
  <
    typename TwoThreeTree< K, V, C >::iterator,
    typename TwoThreeTree< K, V, C >::iterator
  >
  TwoThreeTree< K, V, C >::equal_range(const key_type & k)
  {
    auto found = lazy_find(k);

    if (found.second)
    {
      iterator next = found.first;
      ++next;
      return {found.first, next};
    }
    else
    {
      return {found.first, found.first};
    }
  }

  template< typename K, typename V, typename C >
  std::pair
  <
    typename TwoThreeTree< K, V, C >::const_iterator,
    typename TwoThreeTree< K, V, C >::const_iterator
  >
  TwoThreeTree< K, V, C >::equal_range(const key_type & k) const
  {
    auto found = lazy_find(k);

    if (found.second)
    {
      const_iterator next = found.first;
      ++next;
      return {found.first, next};
    }
    else
    {
      return {found.first, found.first};
    }
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::node_type * TwoThreeTree< K, V, C >::split_node(node_type * node)
  {
    if (node->len < 3)
    {
      return node;
    }
    node_type * left = new node_type{};
    node_type * right = nullptr;
    try
    {
      right = new node_type{};
    }
    catch (const std::bad_alloc & e)
    {
      delete left;
      throw;
    }
    left->father = node->father;
    right->father = node->father;
    for (size_t i = 0; i < 2; ++i)
    {
      left->kids[i] = node->kids[i];
      if (left->kids[i])
      {
        left->kids[i]->father = left;
      }
      right->kids[i] = node->kids[i + 2];
      if (right->kids[i])
      {
        right->kids[i]->father = right;
      }
    }

    insert_data_in_node(left, node->data[0]);
    try
    {
      insert_data_in_node(right, node->data[2]);
    }
    catch (...)
    {
      remove_data_from_node(left, node->data[0]);
      throw;
    }

    try
    {
      if (node->father)
      {
        node_type * parent = node->father;

        insert_data_in_node(parent, node->data[1]);

        if (parent->kids[0] == node)
        {
          parent->kids[3] = parent->kids[2];
          parent->kids[2] = parent->kids[1];
          parent->kids[1] = right;
          parent->kids[0] = left;
        }
        else if (parent->kids[1] == node)
        {
          parent->kids[3] = parent->kids[2];
          parent->kids[2] = right;
          parent->kids[1] = left;
        }
        else
        {
          parent->kids[3] = right;
          parent->kids[2] = left;
        }

        delete node;
        return parent;
      }
      else
      {
        left->father = node;
        right->father = node;
        node->data[0] = node->data[1];
        node->kids[0] = left;
        node->kids[1] = right;
        node->kids[2] = nullptr;
        node->kids[3] = nullptr;
        node->len = 1;
        return node;
      }
    }
    catch (...)
    {
      remove_data_from_node(left, node->data[0]);
      remove_data_from_node(right, node->data[2]);
      throw;
    }
  }

  template< typename K, typename V, typename C >
  std::pair
  <
    typename TwoThreeTree< K, V, C >::iterator,
    bool
  >
  TwoThreeTree< K, V, C >::insert(const value_type & val)
  {
    auto result = lazy_find(val.first);
    if (result.second)
    {
      return {result.first, false};
    }
    return insert_node(result.first.node_, val);
  }
}

#endif
