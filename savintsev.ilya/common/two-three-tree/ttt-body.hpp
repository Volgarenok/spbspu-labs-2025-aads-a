#ifndef TTT_BODY_HPP
#define TTT_BODY_HPP
#include <functional>
#include <iostream>
#include <cassert>
#include <queue>
#include "ttt-node.hpp"
#include "ttt-iterator.hpp"
#include "ttt-const-iterator.hpp"
#include "queue.hpp"

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
    std::pair< iterator, iterator > equal_range(const key_type & k);

    template < typename F >
    F traverse_lnr(F f) const;
    template < typename F >
    F traverse_lnr(F f);
    template < typename F >
    F traverse_rnl(F f) const;
    template < typename F >
    F traverse_rnl(F f);
    template < typename F >
    F traverse_breadth(F f) const;
    template < typename F >
    F traverse_breadth(F f);
  private:
    node_type * root_ = nullptr;
    size_t size_ = 0;

    std::pair< iterator, bool > lazy_find(const key_type & k) const;
    node_type * split_node(node_type * node);
    void sort_node(node_type * node);
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
    if (this != &other)
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
    if (this != &other)
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
    if (!root_ || root_->len_ == 0)
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
    if (!root_ || root_->len_ == 0)
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
    if (!root_ || root_->len_ == 0)
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
    if (result.second)
    {
      return result.first;
    }
    return end();
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::const_iterator TwoThreeTree< K, V, C >::find(const key_type & k) const
  {
    auto result = lazy_find(k);
    if (result.second)
    {
      return result.first;
    }
    return end();
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::mapped_type & TwoThreeTree< K, V, C >::operator[](const key_type & k)
  {
    return insert({k, mapped_type{}}).first->second;
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::mapped_type & TwoThreeTree< K, V, C >::operator[](key_type && k)
  {
    std::pair< K, V > inserted = {k, mapped_type{}};
    return insert(inserted).first->second;
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
  std::pair<
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
      assert(node->len_ <= 3);
      for (size_t i = 0; i < node->len_; ++i)
      {
        if (node->data_[i].first == k)
        {
          return {iterator(root_, node, i), true};
        }
      }

      if (!node->kids_[0])
      {
        return std::make_pair(iterator(root_, node, 0), false);
      }

      if (C{}(k, node->data_[0].first))
      {
        node = node->kids_[0];
      }
      else if (node->len_ == 1 || C{}(k, node->data_[1].first))
      {
        node = node->kids_[1];
      }
      else
      {
        node = node->kids_[2];
      }
    }
    return {iterator(root_), false};
  }

  template< typename Key, typename Value, typename Compare >
  void TwoThreeTree< Key, Value, Compare >::insert_data_in_node(node_type * node, const value_type & val)
  {
    node->data_[node->len_] = val;
    assert(node->len_ < 3);
    node->len_++;
    sort_node(node);
  }

  template< typename Key, typename Value, typename Compare >
  void TwoThreeTree< Key, Value, Compare >::remove_data_from_node(node_type * node, const value_type & val)
  {
    remove_data_from_node(node, val.first);
  }

  template< typename Key, typename Value, typename Compare >
  void TwoThreeTree< Key, Value, Compare >::remove_data_from_node(node_type * node, const key_type & k)
  {
    if (node->len_ == 0) return;
    if (node->len_ >= 1 && node->data_[0].first == k)
    {
      node->data_[0] = node->data_[1];
      node->data_[1] = node->data_[2];
      node->len_--;
    }
    else if (node->len_ == 2 && node->data_[1].first == k)
    {
      node->data_[1] = node->data_[2];
      node->len_--;
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
      node->kids_[i] = clear_nodes(node->kids_[i]);
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
    node_type * root = new node_type{};
    for (size_t i = 0; i < other->len_; ++i)
    {
      root->data_[i] = other->data_[i];
    }
    root->len_ = other->len_;
    for (size_t i = 0; i < 3; ++i)
    {
      if (other->kids_[i])
      {
        root->kids_[i] = clone_nodes(other->kids_[i]);
        root->kids_[i]->parent_ = root;
      }
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

    while (temp->kids_[0])
    {
      temp = temp->kids_[0];
    }

    return (temp->len_ == 0 ? nullptr : temp);
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::node_type * TwoThreeTree< K, V, C >::fix_nodes_properties(node_type * leaf)
  {
    if (!leaf)
    {
      return nullptr;
    }

    if (!leaf->parent_ && leaf->len_ == 0)
    {
      for (size_t i = 0; i < 4; ++i)
      {
        if (leaf->kids_[i])
        {
          leaf->kids_[i]->parent_ = nullptr;
          node_type* new_root = leaf->kids_[i];
          delete leaf;
          return new_root;
        }
      }
      delete leaf;
      return nullptr;
    }
    if (leaf->len_ != 0)
    {
      if (leaf->parent_)
      {
        return fix_nodes_properties(leaf->parent_);
      }
      return leaf;
    }
    else
    {
      node_type * parent = leaf->parent_;
      if (parent->kids_[0]->len_ == 2 || parent->kids_[1]->len_ == 2 || parent->len_ == 2)
      {
        leaf = redistribute_nodes(leaf);
      }
      else if (parent->len_ == 2 && parent->kids_[2]->len_ == 2)
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
    node_type * parent = leaf->parent_;
    node_type * first = parent->kids_[0];
    node_type * second = parent->kids_[1];
    node_type * third = parent->kids_[2];
    if ((parent->len_ == 2) && (first->len_ < 2) && (second->len_ < 2) && (third->len_ < 2))
    {
      if (first == leaf)
      {
        parent->kids_[0] = parent->kids_[1];
        parent->kids_[1] = parent->kids_[2];
        parent->kids_[2] = nullptr;
        insert_data_in_node(parent->kids_[0], parent->data_[0]);
        parent->kids_[0]->kids_[2] = parent->kids_[0]->kids_[1];
        parent->kids_[0]->kids_[1] = parent->kids_[0]->kids_[0];

        if (leaf->kids_[0])
        {
          parent->kids_[0]->kids_[0] = leaf->kids_[0];
        }
        else if (leaf->kids_[1])
        {
          parent->kids_[0]->kids_[0] = leaf->kids_[1];
        }
        if (parent->kids_[0]->kids_[0])
        {
          parent->kids_[0]->kids_[0]->parent_ = parent->kids_[0];
        }
        remove_data_from_node(parent, parent->data_[0]);
        delete first;
      }
      else if (second == leaf)
      {
        insert_data_in_node(first, parent->data_[0]);
        remove_data_from_node(parent, parent->data_[0]);
        if (leaf->kids_[0])
        {
          first->kids_[2] = leaf->kids_[0];
        }
        else if (leaf->kids_[1])
        {
          first->kids_[2] = leaf->kids_[1];
        }
        if (first->kids_[2])
        {
          first->kids_[2]->parent_ = first;
        }
        parent->kids_[1] = parent->kids_[2];
        parent->kids_[2] = nullptr;

        delete second;
      }
      else if (third == leaf)
      {
        insert_data_in_node(second, parent->data_[1]);
        parent->kids_[2] = nullptr;
        remove_data_from_node(parent, parent->data_[1]);
        if (leaf->kids_[0])
        {
          second->kids_[2] = leaf->kids_[0];
        }
        else if (leaf->kids_[1])
        {
          second->kids_[2] = leaf->kids_[1];
        }
        if (second->kids_[2])
        {
          second->kids_[2]->parent_ = second;
        }
        delete third;
      }
    }
    else if ((parent->len_ == 2) && ((first->len_ == 2) || (second->len_ == 2) || (third->len_ == 2)))
    {
      if (third == leaf)
      {
        if (leaf->kids_[0])
        {
          leaf->kids_[1] = leaf->kids_[0];
          leaf->kids_[0] = nullptr;
        }

        insert_data_in_node(leaf, parent->data_[1]);
        if (second->len_ == 2)
        {
          parent->data_[1] = second->data_[1];
          remove_data_from_node(second, second->data_[1]);
          leaf->kids_[0] = second->kids_[2];
          second->kids_[2] = nullptr;
          if (leaf->kids_[0])
          {
            leaf->kids_[0]->parent_ = leaf;
          }
        }
        else if (first->len_ == 2)
        {
          parent->data_[1] = second->data_[0];
          leaf->kids_[0] = second->kids_[1];
          second->kids_[1] = second->kids_[0];
          if (leaf->kids_[0])
          {
            leaf->kids_[0]->parent_ = leaf;
          }
          second->data_[0] = parent->data_[0];
          parent->data_[0] = first->data_[1];
          remove_data_from_node(first, first->data_[1]);
          second->kids_[0] = first->kids_[2];
          if (second->kids_[0])
          {
            second->kids_[0]->parent_ = second;
          }
          first->kids_[2] = nullptr;
        }
      }
      else if (second == leaf)
      {
        if (third->len_ == 2)
        {
          if (leaf->kids_[0] == nullptr)
          {
            leaf->kids_[0] = leaf->kids_[1];
            leaf->kids_[1] = nullptr;
          }
          insert_data_in_node(second, parent->data_[1]);
          parent->data_[1] = third->data_[0];
          remove_data_from_node(third, third->data_[0]);
          second->kids_[1] = third->kids_[0];
          if (second->kids_[1])
          {
            second->kids_[1]->parent_ = second;
          }
          third->kids_[0] = third->kids_[1];
          third->kids_[1] = third->kids_[2];
          third->kids_[2] = nullptr;
        }
        else if (first->len_ == 2)
        {
          if (leaf->kids_[1] == nullptr)
          {
            leaf->kids_[1] = leaf->kids_[0];
            leaf->kids_[0] = nullptr;
          }
          insert_data_in_node(second, parent->data_[0]);
          parent->data_[0] = first->data_[1];
          remove_data_from_node(first, first->data_[1]);
          second->kids_[0] = first->kids_[2];
          if (second->kids_[0])
          {
            second->kids_[0]->parent_ = second;
          }
          first->kids_[2] = nullptr;
        }
      }
      else if (first == leaf)
      {
        if (leaf->kids_[0] == nullptr)
        {
          leaf->kids_[0] = leaf->kids_[1];
          leaf->kids_[1] = nullptr;
        }
        insert_data_in_node(first, parent->data_[0]);
        if (second->len_ == 2)
        {
          parent->data_[0] = second->data_[0];
          remove_data_from_node(second, second->data_[0]);
          first->kids_[1] = second->kids_[0];
          if (first->kids_[1])
          {
            first->kids_[1]->parent_ = first;
          }
          second->kids_[0] = second->kids_[1];
          second->kids_[1] = second->kids_[2];
          second->kids_[2] = nullptr;
        }
        else if (third->len_ == 2)
        {
          parent->data_[0] = second->data_[0];
          second->data_[0] = parent->data_[1];
          parent->data_[1] = third->data_[0];
          remove_data_from_node(third, third->data_[0]);
          first->kids_[1] = second->kids_[0];
          if (first->kids_[1])
          {
            first->kids_[1]->parent_ = first;
          }
          second->kids_[0] = second->kids_[1];
          second->kids_[1] = third->kids_[0];
          if (second->kids_[1])
          {
            second->kids_[1]->parent_ = second;
          }
          third->kids_[0] = third->kids_[1];
          third->kids_[1] = third->kids_[2];
          third->kids_[2] = nullptr;
        }
      }
    }
    else if (parent->len_ == 1)
    {
      insert_data_in_node(leaf, parent->data_[0]);

      if (first == leaf && second->len_ == 2)
      {
        parent->data_[0] = second->data_[0];
        remove_data_from_node(second, second->data_[0]);

        if (leaf->kids_[0] == nullptr)
        {
          leaf->kids_[0] = leaf->kids_[1];
        }
        leaf->kids_[1] = second->kids_[0];
        second->kids_[0] = second->kids_[1];
        second->kids_[1] = second->kids_[2];
        second->kids_[2] = nullptr;
        if (leaf->kids_[1])
        {
          leaf->kids_[1]->parent_ = leaf;
        }
      }
      else if (second == leaf && first->len_ == 2)
      {
        parent->data_[0] = first->data_[1];
        remove_data_from_node(first, first->data_[1]);

        if (leaf->kids_[1] == nullptr)
        {
          leaf->kids_[1] = leaf->kids_[0];
        }
        leaf->kids_[0] = first->kids_[2];
        first->kids_[2] = nullptr;
        if (leaf->kids_[0])
        {
          leaf->kids_[0]->parent_ = leaf;
        }
      }
    }
    return parent;
  }

  template< typename K, typename V, typename C >
  typename TwoThreeTree< K, V, C >::node_type * TwoThreeTree< K, V, C >::merge_nodes(node_type * leaf)
  {
    node_type * parent = leaf->parent_;

    if (parent->kids_[0] == leaf)
    {
      insert_data_in_node(parent->kids_[1], parent->data_[0]);
      parent->kids_[1]->kids_[2] = parent->kids_[1]->kids_[1];
      parent->kids_[1]->kids_[1] = parent->kids_[1]->kids_[0];

      if (leaf->kids_[0])
      {
        parent->kids_[1]->kids_[0] = leaf->kids_[0];
      }
      else if (leaf->kids_[1])
      {
        parent->kids_[1]->kids_[0] = leaf->kids_[1];
      }
      if (parent->kids_[1]->kids_[0])
      {
        parent->kids_[1]->kids_[0]->parent_ = parent->kids_[1];
      }
      remove_data_from_node(parent, parent->data_[0]);
      delete parent->kids_[0];
      parent->kids_[0] = nullptr;
    }
    else if (parent->kids_[1] == leaf)
    {
      insert_data_in_node(parent->kids_[0], parent->data_[0]);

      if (leaf->kids_[0])
      {
        parent->kids_[0]->kids_[2] = leaf->kids_[0];
      }
      else if (leaf->kids_[1])
      {
        parent->kids_[0]->kids_[2] = leaf->kids_[1];
      }

      if (parent->kids_[0]->kids_[2])
      {
        parent->kids_[0]->kids_[2]->parent_ = parent->kids_[0];
      }
      remove_data_from_node(parent, parent->data_[0]);
      delete parent->kids_[1];
      parent->kids_[1] = nullptr;
    }

    if (!parent->parent_)
    {
      node_type * temp = nullptr;
      if (parent->kids_[0])
      {
        temp = parent->kids_[0];
      }
      else
      {
        temp = parent->kids_[1];
      }
      temp->parent_ = nullptr;
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
    if (position.pos_ + 1 < 3 && target->kids_[position.pos_ + 1])
    {
      closest = search_min(target->kids_[position.pos_ + 1]);
    }

    const key_type k = position->first;
    const key_type k_next = (++position)->first;
    if (closest)
    {
      std::swap(target->data_[position.pos_], closest->data_[0]);
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
    node_type * closest = search_min(target->kids_[result.first.pos_ + 1]);
    if (closest)
    {
      std::swap(target->data_[result.first.pos_], closest->data_[0]);
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
    if (node->len_ < 3)
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
    left->parent_ = node->parent_;
    right->parent_ = node->parent_;
    for (size_t i = 0; i < 2; ++i)
    {
      left->kids_[i] = node->kids_[i];
      if (left->kids_[i])
      {
        left->kids_[i]->parent_ = left;
      }
      right->kids_[i] = node->kids_[i + 2];
      if (right->kids_[i])
      {
        right->kids_[i]->parent_ = right;
      }
    }
    insert_data_in_node(left, node->data_[0]);
    insert_data_in_node(right, node->data_[2]);

    if (node->parent_)
    {
      node_type * parent = node->parent_;

      insert_data_in_node(parent, node->data_[1]);

      if (parent->kids_[0] == node)
      {
        parent->kids_[3] = parent->kids_[2];
        parent->kids_[2] = parent->kids_[1];
        parent->kids_[1] = right;
        parent->kids_[0] = left;
      }
      else if (parent->kids_[1] == node)
      {
        parent->kids_[3] = parent->kids_[2];
        parent->kids_[2] = right;
        parent->kids_[1] = left;
      }
      else
      {
        parent->kids_[3] = right;
        parent->kids_[2] = left;
      }

      delete node;
      return parent;
    }
    else
    {
      left->parent_ = node;
      right->parent_ = node;
      node->data_[0] = node->data_[1];
      node->kids_[0] = left;
      node->kids_[1] = right;
      node->kids_[2] = nullptr;
      node->kids_[3] = nullptr;
      node->len_ = 1;
      return node;
    }
  }

  template< typename Key, typename Value, typename Compare >
  void TwoThreeTree< Key, Value, Compare >::sort_node(node_type * node)
  {
    if (node->len_ == 2)
    {
      if (!Compare{}(node->data_[0].first, node->data_[1].first))
      {
        std::swap(node->data_[0], node->data_[1]);
      }
    }
    if (node->len_ == 3)
    {
      if (!Compare{}(node->data_[0].first, node->data_[1].first))
      {
        std::swap(node->data_[0], node->data_[1]);
      }
      if (!Compare{}(node->data_[1].first, node->data_[2].first))
      {
        std::swap(node->data_[1], node->data_[2]);
      }
      if (!Compare{}(node->data_[0].first, node->data_[1].first))
      {
        std::swap(node->data_[0], node->data_[1]);
      }
    }
  }

  template< typename K, typename V, typename C >
  std::pair< typename TwoThreeTree< K, V, C >::iterator, bool > TwoThreeTree< K, V, C >::insert(const value_type & val)
  {
    auto result = lazy_find(val.first);
    if (result.second)
    {
      return {result.first, false};
    }
    node_type * current = result.first.node_;

    if (!current)
    {
      node_type * root = new node_type{};
      root_ = root;
      insert_data_in_node(root_, val);
      size_++;
      return {iterator(root_, root_), true};
    }

    insert_data_in_node(current, val);

    while (current && current->len_ == 3)
    {
      current = split_node(current);
    }
    while (current->parent_)
    {
      current = current->parent_;
    }

    root_ = current;
    size_++;

    return {lazy_find(val.first).first, true};
  }

  template< typename K, typename V, typename C >
  template< typename F >
  F TwoThreeTree< K, V, C >::traverse_lnr(F f) const
  {
    if (empty())
    {
      throw std::logic_error("Tree is empty");
    }
    for (auto it = begin(); it != end(); ++it)
    {
      f(*it);
    }
    return f;
  }

  template< typename K, typename V, typename C >
  template< typename F >
  F TwoThreeTree< K, V, C >::traverse_lnr(F f)
  {
    const TwoThreeTree< K, V, C > copy = const_cast< TwoThreeTree< K, V, C > & >(*this);
    return copy.traverse_lnr(f);
  }

  template< typename K, typename V, typename C >
  template< typename F >
  F TwoThreeTree< K, V, C >::traverse_rnl(F f) const
  {
    if (empty())
    {
      throw std::logic_error("Tree is empty");
    }

    for (auto it = end(); it != begin();)
    {
      --it;
      assert(it != end());
      f(*it);
    }

    return f;
  }

  template< typename K, typename V, typename C >
  template< typename F >
  F TwoThreeTree< K, V, C >::traverse_rnl(F f)
  {
    const TwoThreeTree< K, V, C > copy = const_cast< TwoThreeTree< K, V, C > & >(*this);
    return copy.traverse_rnl(f);
  }

  template< typename K, typename V, typename C >
  template< typename F >
  F TwoThreeTree< K, V, C >::traverse_breadth(F f) const
  {
    if (empty())
    {
      throw std::logic_error("Tree is empty");
    }

    Queue< node_type * > queue;
    queue.push(root_);

    while (!queue.empty())
    {
      node_type * current = queue.front();
      queue.pop();

      for (size_t i = 0; i < current->len_; ++i)
      {
        f(current->data_[i]);
      }

      for (size_t i = 0; i <= current->len_; ++i)
      {
        if (current->kids_[i])
        {
          queue.push(current->kids_[i]);
        }
      }
    }

    return f;
  }

  template< typename K, typename V, typename C >
  template< typename F >
  F TwoThreeTree< K, V, C >::traverse_breadth(F f)
  {
    const TwoThreeTree< K, V, C > copy = const_cast< TwoThreeTree< K, V, C > & >(*this);
    return copy.traverse_rnl(f);
  }
}

#endif
