#ifndef TREE_2_3_HPP
#define TREE_2_3_HPP
#include <cstddef>
#include <cassert>
#include <functional>
#include <exception>
#include "tree-iterators.hpp"
#include "tree-iterator-impl.hpp"
#include "tree-const-iterator-impl.hpp"
#include "tree-lnr-iterator.hpp"
#include "tree-rnl-iterator.hpp"
#include "tree-breadth-iterator.hpp"

namespace alymova
{
  template< class Key, class Value, class Comparator >
  class TwoThreeTree
  {
  public:
    using Tree = TwoThreeTree< Key, Value, Comparator >;

    using Iterator = TTTIterator< Key, Value, Comparator >;
    using ConstIterator = TTTConstIterator< Key, Value, Comparator >;
    using ConstLnrIterator = TTTConstLnrIterator< Key, Value, Comparator >;
    using ConstRnlIterator = TTTConstRnlIterator< Key, Value, Comparator >;
    using ConstBreadthIterator = TTTConstBreadthIterator< Key, Value, Comparator >;

    using Node = typename detail::TTTNode< Key, Value, Comparator >;
    using NodeType = typename Node::NodeType;
    using NodePoint = typename detail::NodePoint;
    using T = std::pair< Key, Value >;

    TwoThreeTree();
    TwoThreeTree(const Tree& other);
    TwoThreeTree(Tree&& other) noexcept;
    template< typename InputIterator>
    TwoThreeTree(InputIterator first, InputIterator last);
    TwoThreeTree(std::initializer_list< T > il);
    ~TwoThreeTree() noexcept;

    Tree& operator=(const Tree& other);
    Tree& operator=(Tree&& other) noexcept;

    Value& operator[](const Key& key);
    Value& at(const Key& key);
    const Value& at(const Key& key) const;

    template< class F >
    F traverse_lnr(F f);
    template< class F >
    F traverse_lnr(F f) const;

    template< class F >
    F traverse_rnl(F f);
    template< class F >
    F traverse_rnl(F f) const;

    template< class F >
    F traverse_breadth(F f);
    template< class F >
    F traverse_breadth(F f) const;

    Iterator begin() noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator cbegin() const noexcept;
    ConstLnrIterator lnr_cbegin() const;
    ConstRnlIterator rnl_cbegin() const;
    ConstBreadthIterator breadth_cbegin() const;

    Iterator end() noexcept;
    ConstIterator end() const noexcept;
    ConstIterator cend() const noexcept;
    ConstLnrIterator lnr_cend() const;
    ConstRnlIterator rnl_cend() const;
    ConstBreadthIterator breadth_cend() const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    std::pair< Iterator, bool > insert(const T& value);
    std::pair< Iterator, bool > insert(T&& value);
    template < class InputIterator >
    void insert(InputIterator first, InputIterator last);
    void insert(std::initializer_list< T > il);
    Iterator insert(Iterator hint, const T& value);
    Iterator insert(ConstIterator hint, const T& value);

    template< class... Args >
    std::pair< Iterator, bool > emplace(Args&&... args);
    template< class... Args >
    Iterator emplace_hint(ConstIterator hint, Args&&... args);

    size_t erase(const Key& key);
    Iterator erase(Iterator pos);
    Iterator erase(Iterator first, Iterator last);

    void swap(Tree& other) noexcept;
    void clear() noexcept;

    size_t count(const Key& key) const;
    Iterator find(const Key& key);
    ConstIterator find(const Key& key) const;
    std::pair< Iterator, Iterator > equal_range(const Key& key);
    std::pair< ConstIterator, ConstIterator > equal_range(const Key& key) const;
    Iterator lower_bound(const Key& key);
    ConstIterator lower_bound(const Key& key) const;
    Iterator upper_bound(const Key& key);
    ConstIterator upper_bound(const Key& key) const;

  private:
    size_t size_;
    Node* fake_right_;
    Node* fake_left_;
    Node* root_;
    Comparator cmp_;

    void clear(Node* node) noexcept;
    void move_fake() const noexcept;
    void split_insert(Node* node);
    Node* find_to_insert(const Key& key) const;
    Node* find_to_insert(ConstIterator hint) const noexcept;
    bool check_hint(ConstIterator hint, const Key& key) const;
    Iterator find_to_erase(Iterator pos) const noexcept;
    bool have_triple_neighbor(const Node* node) const noexcept;
    void fix(Node* node);
    void distribute_erase(Node* node);
    Node* merge_erase(Node* node);

    bool is_balanced() const noexcept;
    size_t find_height(Node* node) const noexcept;
  };

  template< class T, class Value, class Comparator >
  bool operator==(const TwoThreeTree< T, Value, Comparator >& lhs,
    const TwoThreeTree< T, Value, Comparator >& rhs) noexcept;

  template< class T, class Value, class Comparator >
  bool operator!=(const TwoThreeTree< T, Value, Comparator >& lhs,
    const TwoThreeTree< T, Value, Comparator >& rhs) noexcept;

  template< class T, class Value, class Comparator >
  bool operator==(const TwoThreeTree< T, Value, Comparator >& lhs,
    const TwoThreeTree< T, Value, Comparator >& rhs) noexcept
  {
    if (lhs.size() != rhs.size())
    {
      return false;
    }
    for (auto it_lhs = lhs.cbegin(), it_rhs = rhs.cbegin(); it_lhs != lhs.cend(); ++it_lhs, ++it_rhs)
    {
      if ((*it_lhs) != (*it_rhs))
      {
        return false;
      }
    }
    return true;
  }

  template< class T, class Value, class Comparator >
  bool operator!=(const TwoThreeTree< T, Value, Comparator >& lhs,
    const TwoThreeTree< T, Value, Comparator >& rhs) noexcept
  {
    return (!(lhs == rhs));
  }

  template< class Key, class Value, class Comparator >
  TwoThreeTree< Key, Value, Comparator >::TwoThreeTree():
    size_(0),
    fake_right_(reinterpret_cast< Node* >(new char[sizeof(Node)])),
    fake_left_(reinterpret_cast< Node* >(new char[sizeof(Node)])),
    root_(fake_right_)
  {
    fake_right_->type = NodeType::Fake;
    fake_right_->clear();
    fake_left_->type = NodeType::Fake;
    fake_left_->clear();
  }

  template< class Key, class Value, class Comparator >
  TwoThreeTree< Key, Value, Comparator >::TwoThreeTree(const Tree& other):
    Tree(other.begin(), other.end())
  {}

  template< class Key, class Value, class Comparator >
  TwoThreeTree< Key, Value, Comparator >::TwoThreeTree(Tree&& other) noexcept:
    size_(std::exchange(other.size_, 0)),
    fake_right_(std::exchange(other.fake_right_, nullptr)),
    fake_left_(std::exchange(other.fake_left_, nullptr)),
    root_(std::exchange(other.root_, nullptr))
  {}

  template< class Key, class Value, class Comparator >
  template< class InputIterator >
  TwoThreeTree< Key, Value, Comparator >::TwoThreeTree(InputIterator first, InputIterator last):
    Tree()
  {
    for (auto it = first; it != last; it++)
    {
      emplace(*it);
    }
  }

  template< class Key, class Value, class Comparator >
  TwoThreeTree< Key, Value, Comparator >::TwoThreeTree(std::initializer_list< T > il):
    Tree(il.begin(), il.end())
  {}

  template< class Key, class Value, class Comparator >
  TwoThreeTree< Key, Value, Comparator >::~TwoThreeTree() noexcept
  {
    if (size_)
    {
      clear(root_);
    }
    delete[] reinterpret_cast< char* >(fake_right_);
    delete[] reinterpret_cast< char* >(fake_left_);
  }

  template< class Key, class Value, class Comparator >
  TwoThreeTree< Key, Value, Comparator >& TwoThreeTree< Key, Value, Comparator >::operator=(const Tree& other)
  {
    assert(this != std::addressof(other) && "Assigning a map to itself");
    Tree copy(other);
    swap(copy);
    return *this;
  }

  template< class Key, class Value, class Comparator >
  TwoThreeTree< Key, Value, Comparator >& TwoThreeTree< Key, Value, Comparator >::operator=(Tree&& other) noexcept
  {
    assert(this != std::addressof(other) && "Assigning a map to itself");
    Tree moved(std::move(other));
    swap(moved);
    return *this;
  }

  template< class Key, class Value, class Comparator >
  Value& TwoThreeTree< Key, Value, Comparator >::operator[](const Key& key)
  {
    auto it = lower_bound(key);
    if (it == end() || it->first != key)
    {
      return insert(it, {key, Value()})->second;
    }
    return (it->second);
  }

  template< class Key, class Value, class Comparator >
  Value& TwoThreeTree< Key, Value, Comparator >::at(const Key& key)
  {
    return const_cast< Value& >(static_cast< const Tree& >(*this).at(key));
  }

  template< class Key, class Value, class Comparator >
  const Value& TwoThreeTree< Key, Value, Comparator >::at(const Key& key) const
  {
    auto it = find(key);
    if (it == end())
    {
      throw std::out_of_range("Container does not have an element with the such key");
    }
    return (it->second);
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::begin() noexcept
  {
    return Iterator(cbegin());
  }

  template< class Key, class Value, class Comparator >
  TTTConstIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::begin() const noexcept
  {
    return cbegin();
  }

  template< class Key, class Value, class Comparator >
  TTTConstIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::cbegin() const noexcept
  {
    if (size_ == 0)
    {
      return cend();
    }
    Node* tmp = root_;
    while (tmp->left && tmp->left != fake_left_)
    {
      tmp = tmp->left;
    }
    return ConstIterator(tmp, NodePoint::First);
  }

  template< class Key, class Value, class Comparator >
  TTTConstLnrIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::lnr_cbegin() const
  {
    NodePoint point = NodePoint::First;
    if (empty())
    {
      point = NodePoint::Fake;
    }
    return ConstLnrIterator(root_, point);
  }

  template< class Key, class Value, class Comparator >
  TTTConstRnlIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::rnl_cbegin() const
  {
    NodePoint point = NodePoint::First;
    if (empty())
    {
      point = NodePoint::Fake;
    }
    return ConstRnlIterator(root_, point);
  }

  template< class Key, class Value, class Comparator >
  TTTConstBreadthIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::breadth_cbegin() const
  {
    NodePoint point = NodePoint::First;
    if (empty())
    {
      point = NodePoint::Fake;
    }
    return ConstBreadthIterator(root_, point);
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::end() noexcept
  {
    return Iterator(cend());
  }

  template< class Key, class Value, class Comparator >
  TTTConstIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::end() const noexcept
  {
    return cend();
  }

  template< class Key, class Value, class Comparator >
  TTTConstIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::cend() const noexcept
  {
    return ConstIterator(fake_right_, NodePoint::Fake);
  }

  template< class Key, class Value, class Comparator >
  TTTConstLnrIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::lnr_cend() const
  {
    return ConstLnrIterator(fake_right_, NodePoint::Fake);
  }

  template< class Key, class Value, class Comparator >
  TTTConstRnlIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::rnl_cend() const
  {
    return ConstRnlIterator(fake_left_, NodePoint::Fake);
  }

  template< class Key, class Value, class Comparator >
  TTTConstBreadthIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::breadth_cend() const
  {
    return ConstBreadthIterator(fake_right_, NodePoint::Fake);
  }

  template< class Key, class Value, class Comparator >
  template< class F >
  F TwoThreeTree< Key, Value, Comparator >::traverse_lnr(F f)
  {
    return static_cast< const TwoThreeTree& >(*this).traverse_lnr(f);
  }

  template< class Key, class Value, class Comparator >
  template< class F >
  F TwoThreeTree< Key, Value, Comparator >::traverse_lnr(F f) const
  {
    for (auto it = lnr_cbegin(); it != lnr_cend(); ++it)
    {
      f(*it);
    }
    return f;
  }

  template< class Key, class Value, class Comparator >
  template< class F >
  F TwoThreeTree< Key, Value, Comparator >::traverse_rnl(F f)
  {
    return static_cast< const TwoThreeTree& >(*this).traverse_rnl(f);
  }

  template< class Key, class Value, class Comparator >
  template< class F >
  F TwoThreeTree< Key, Value, Comparator >::traverse_rnl(F f) const
  {
    for (auto it = rnl_cbegin(); it != rnl_cend(); ++it)
    {
      f(*it);
    }
    return f;
  }

  template< class Key, class Value, class Comparator >
  template< class F >
  F TwoThreeTree< Key, Value, Comparator >::traverse_breadth(F f)
  {
    return static_cast< const TwoThreeTree& >(*this).traverse_breadth(f);
  }

  template< class Key, class Value, class Comparator >
  template< class F >
  F TwoThreeTree< Key, Value, Comparator >::traverse_breadth(F f) const
  {
    for (auto it = breadth_cbegin(); it != breadth_cend(); ++it)
    {
      f(*it);
    }
    return f;
  }

  template< class Key, class Value, class Comparator >
  size_t TwoThreeTree< Key, Value, Comparator >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Comparator >
  bool TwoThreeTree< Key, Value, Comparator >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Comparator >
  std::pair< TTTIterator< Key, Value, Comparator >, bool >
    TwoThreeTree< Key, Value, Comparator >::insert(const T& value)
  {
    return emplace(value);
  }

  template< class Key, class Value, class Comparator >
  std::pair< TTTIterator< Key, Value, Comparator >, bool >
    TwoThreeTree< Key, Value, Comparator >::insert(T&& value)
  {
    return emplace(std::forward< T >(value));
  }

  template< class Key, class Value, class Comparator >
  template< class InputIterator >
  void TwoThreeTree< Key, Value, Comparator >::insert(InputIterator first, InputIterator last)
  {
    for (auto it = first; it != last; it++)
    {
      insert(*it);
    }
  }

  template< class Key, class Value, class Comparator >
  void TwoThreeTree< Key, Value, Comparator >::insert(std::initializer_list< T > il)
  {
    insert(il.begin(), il.end());
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator >
    TwoThreeTree< Key, Value, Comparator >::insert(Iterator hint, const T& value)
  {
    ConstIterator const_hint(hint);
    return emplace_hint(const_hint, value);
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator >
    TwoThreeTree< Key, Value, Comparator >::insert(ConstIterator hint, const T& value)
  {
    return emplace_hint(hint, value);
  }

  template< class Key, class Value, class Comparator >
  template < class... Args >
  std::pair< TTTIterator< Key, Value, Comparator >, bool >
    TwoThreeTree< Key, Value, Comparator >::emplace(Args&&... args)
  {
    std::pair< Key, Value > value(std::forward< Args >(args)...);
    size_t size_before = size_;
    ConstIterator hint = lower_bound(value.first);
    Iterator it = emplace_hint(hint, value);
    if (size_before == size())
    {
      return {it, false};
    }
    return {it, true};
  }

  template< class Key, class Value, class Comparator >
  template < class... Args >
  TTTIterator< Key, Value, Comparator >
    TwoThreeTree< Key, Value, Comparator >::emplace_hint(ConstIterator hint, Args&&... args)
  {
    std::pair< Key, Value > value(std::forward< Args >(args)...);
    Iterator it_value = find(value.first);
    if (it_value != end())
    {
      return it_value;
    }
    Node* to_insert = find_to_insert(hint);
    if (!check_hint(hint, value.first))
    {
      to_insert = find_to_insert(value.first);
    }
    if (size_ == 0)
    {
      root_ = new Node{{}, NodeType::Empty, nullptr, nullptr, nullptr, nullptr, nullptr};
      to_insert = root_;
    }
    try
    {
      to_insert->insert(value);
      split_insert(to_insert);
    }
    catch(...)
    {
      if (size_ == 0)
      {
        delete root_;
        root_ = fake_right_;
      }
      throw;
    }
    size_++;
    move_fake();
    return find(value.first);
  }

  template< class Key, class Value, class Comparator >
  size_t TwoThreeTree< Key, Value, Comparator >::erase(const Key& key)
  {
    Iterator it = find(key);
    if (it == end())
    {
      return 0;
    }
    erase(it);
    return 1;
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::erase(Iterator pos)
  {
    assert(pos != end() && "Removing from empty map");
    Key key_next;
    NodePoint point_next = NodePoint::Empty;
    Iterator pos_next = (++pos);
    if (pos_next == end())
    {
      point_next = NodePoint::Fake;
    }
    else
    {
      key_next = (pos_next)->first;
    }
    --pos;
    Iterator pos_instead = find_to_erase(pos);
    std::swap(*pos, *pos_instead);
    pos_instead.node_->remove(pos_instead.point_);
    if (pos_instead.node_->type == NodeType::Empty)
    {
      fix(pos_instead.node_);
    }
    move_fake();
    size_--;
    return (point_next == NodePoint::Fake) ? end() : find(key_next);
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::erase(Iterator first, Iterator last)
  {
    Key key_last = last->first;
    while (first != last)
    {
      first = erase(first);
      if (last != end())
      {
        last = find(key_last);
      }
    }
    return first;
  }

  template< class Key, class Value, class Comparator >
  void TwoThreeTree< Key, Value, Comparator >::swap(Tree& other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(fake_right_, other.fake_right_);
    std::swap(fake_left_, other.fake_left_);
    std::swap(root_, other.root_);
    std::swap(cmp_, other.cmp_);
  }

  template< class Key, class Value, class Comparator >
  void TwoThreeTree< Key, Value, Comparator >::clear() noexcept
  {
    clear(root_);
    size_ = 0;
  }

  template< class Key, class Value, class Comparator >
  size_t TwoThreeTree< Key, Value, Comparator >::count(const Key& key) const
  {
    if (find(key) != cend())
    {
      return 1;
    }
    return 0;
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::find(const Key& key)
  {
    ConstIterator tmp = static_cast< const Tree& >(*this).find(key);
    return Iterator(tmp);
  }

  template< class Key, class Value, class Comparator >
  TTTConstIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::find(const Key& key) const
  {
    for (ConstIterator it = cbegin(); it != cend(); it++)
    {
      if (!cmp_(key, it->first) && !cmp_(it->first, key))
      {
        return it;
      }
    }
    return cend();
  }

  template< class Key, class Value, class Comparator >
  std::pair< TTTIterator< Key, Value, Comparator >, TTTIterator< Key, Value, Comparator > >
    TwoThreeTree< Key, Value, Comparator >::equal_range(const Key& key)
  {
    return {lower_bound(key), upper_bound(key)};
  }

  template< class Key, class Value, class Comparator >
  std::pair< TTTConstIterator< Key, Value, Comparator >, TTTConstIterator< Key, Value, Comparator > >
    TwoThreeTree< Key, Value, Comparator >::equal_range(const Key& key) const
  {
    return {lower_bound(key), upper_bound(key)};
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator >
    TwoThreeTree< Key, Value, Comparator >::lower_bound(const Key& key)
  {
    ConstIterator tmp = static_cast< const Tree& >(*this).lower_bound(key);
    return Iterator(tmp);
  }

  template< class Key, class Value, class Comparator >
  TTTConstIterator< Key, Value, Comparator >
    TwoThreeTree< Key, Value, Comparator >::lower_bound(const Key& key) const
  {
    for (auto it = cbegin(); it != cend(); it++)
    {
      if (!cmp_(it->first, key))
      {
        return it;
      }
    }
    return cend();
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator >
    TwoThreeTree< Key, Value, Comparator >::upper_bound(const Key& key)
  {
    ConstIterator tmp = static_cast< const Tree& >(*this).upper_bound(key);
    return Iterator(tmp);
  }

  template< class Key, class Value, class Comparator >
  TTTConstIterator< Key, Value, Comparator >
    TwoThreeTree< Key, Value, Comparator >::upper_bound(const Key& key) const
  {
    for (auto it = cbegin(); it != cend(); it++)
    {
      if (cmp_(key, it->first))
      {
        return it;
      }
    }
    return cend();
  }

  template< class Key, class Value, class Comparator >
  void TwoThreeTree< Key, Value, Comparator >::clear(Node* root) noexcept
  {
    if (!root || root == fake_right_ || root == fake_left_)
    {
      return;
    }
    clear(root->left);
    clear(root->mid);
    clear(root->right);
    delete root;
  }

  template< class Key, class Value, class Comparator >
  void TwoThreeTree< Key, Value, Comparator >::split_insert(Node* node)
  {
    if (node->type != NodeType::Overflow)
    {
      return;
    }
    Node* left = nullptr, *right = nullptr, *parent = nullptr;
    try
    {
      parent = node->parent;
      if (!parent)
      {
        parent = new Node{{}, NodeType::Empty, nullptr, nullptr, nullptr, nullptr, nullptr};
        root_ = parent;
      }
      left = new Node{{node->data[0]}, NodeType::Double, parent, node->left, nullptr, node->mid, nullptr};
      right = new Node{{node->data[2]}, NodeType::Double, parent, node->right, nullptr, node->overflow, nullptr};
      if (left->left && left->left != fake_left_)
      {
        left->left->parent = left;
        left->right->parent = left;
        right->left->parent = right;
        right->right->parent = right;
      }
      if (right->left == fake_right_)
      {
        right->left = nullptr;
      }
      parent->insert(node->data[1]);
      {
        if (parent->type == NodeType::Double)
        {
          parent->left = left;
          parent->right = right;
        }
        else if (parent->right == node && parent->type == NodeType::Triple)
        {
          parent->mid = left;
          parent->right = right;
        }
        else if (parent->right == node && parent->type == NodeType::Overflow)
        {
          parent->right = left;
          parent->overflow = right;
        }
        else if (parent->left == node && parent->type == NodeType::Triple)
        {
          parent->left = left;
          parent->mid = right;
        }
        else if (parent->left == node && parent->type == NodeType::Overflow)
        {
          parent->overflow = parent->right;
          parent->right = parent->mid;
          parent->mid = right;
          parent->left = left;
        }
        else if (parent->mid == node)
        {
          parent->overflow = parent->right;
          parent->mid = left;
          parent->right = right;
        }
      }
    }
    catch (...)
    {
      delete left;
      delete right;
      delete parent;
      throw;
    }
    delete node;
    split_insert(parent);
  }

  template< class Key, class Value, class Comparator >
  void TwoThreeTree< Key, Value, Comparator >::move_fake() const noexcept
  {
    if (empty())
    {
      fake_right_->parent = nullptr;
      fake_left_->parent = nullptr;
      return;
    }
    Node* tmp = root_;
    while (tmp->right && tmp->right != fake_right_)
    {
      tmp = tmp->right;
    }
    tmp->right = fake_right_;
    fake_right_->parent = tmp;
    tmp = root_;
    while (tmp->left && tmp->left != fake_left_)
    {
      tmp = tmp->left;
    }
    tmp->left = fake_left_;
    fake_left_->parent = tmp;
  }

  template< class Key, class Value, class Comparator >
  detail::TTTNode< Key, Value, Comparator >*
    TwoThreeTree< Key, Value, Comparator >::find_to_insert(const Key& key) const
  {
    Node* tmp = root_;
    while (!tmp->isLeaf())
    {
      if (cmp_(key, tmp->data[0].first))
      {
        tmp = tmp->left;
      }
      else if (tmp->type == NodeType::Triple && cmp_(key, tmp->data[1].first))
      {
        tmp = tmp->mid;
      }
      else
      {
        tmp = tmp->right;
      }
    }
    return tmp;
  }

  template< class Key, class Value, class Comparator >
  detail::TTTNode< Key, Value, Comparator >*
    TwoThreeTree< Key, Value, Comparator >::find_to_insert(ConstIterator hint) const noexcept
  {
    Iterator tmp = hint;
    if ((hint == cend() && !empty()) || !tmp.node_->isLeaf())
    {
      return (--tmp).node_;
    }
    return tmp.node_;
  }

  template< class Key, class Value, class Comparator >
  bool TwoThreeTree< Key, Value, Comparator >::check_hint(ConstIterator hint, const Key& key) const
  {
    if (hint != cend())
    {
      if (!cmp_(key, hint->first))
      {
        return false;
      }
    }
    if (hint != cbegin())
    {
      return cmp_((--hint)->first, key);
    }
    return true;
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::find_to_erase(Iterator pos) const noexcept
  {
    Node* node = pos.node_;
    Node* node_instead = node;
    if (pos.point_ == NodePoint::First)
    {
      if (node_instead->left && node_instead->left != fake_left_)
      {
        node_instead = node_instead->left;
        while (node_instead->right && node_instead->right != fake_right_)
        {
          node_instead = node_instead->right;
        }
      }
    }
    else if (pos.point_ == NodePoint::Second)
    {
      if (node_instead->right && node_instead->right != fake_right_)
      {
        node_instead = node_instead->right;
        while (node_instead->left && node_instead->left != fake_left_)
        {
          node_instead = node_instead->left;
        }
      }
    }
    NodePoint point = NodePoint::First;
    if (pos.point_ == NodePoint::First && node_instead->type == NodeType::Triple)
    {
      point = NodePoint::Second;
    }
    return {node_instead, point};
  }

  template< class Key, class Value, class Comparator >
  bool TwoThreeTree< Key, Value, Comparator >::have_triple_neighbor(const Node* node) const noexcept
  {
    if (!node)
    {
      return false;
    }
    if (!node->parent)
    {
      return false;
    }
    if (node->parent->type == NodeType::Triple)
    {
      return true;
    }
    if (node->parent->left->type == NodeType::Triple)
    {
      return true;
    }
    return node->parent->right->type == NodeType::Triple;
  }

  template< class Key, class Value, class Comparator >
  void TwoThreeTree< Key, Value, Comparator >::fix(Node* node)
  {
    if (size_ == 1)
    {
      delete root_;
      root_ = fake_right_;
      return;
    }
    if (!node->parent)
    {
      return;
    }
    if (have_triple_neighbor(node))
    {
      distribute_erase(node);
      return;
    }
    Node* new_node = merge_erase(node);
    fix(new_node);
  }

  template< class Key, class Value, class Comparator >
  void TwoThreeTree< Key, Value, Comparator >::distribute_erase(Node* node)
  {
    Node* parent = node->parent;
    Node* left = parent->left;
    Node* mid = parent->mid;
    Node* right = parent->right;
    if (parent->type == NodeType::Double)
    {
      node->insert(std::move(parent->data[0]));
      parent->remove(NodePoint::First);
      if (left == node)
      {
        if (!node->left || node->left == fake_left_)
        {
          std::swap(node->left, node->right);
        }
        parent->insert(std::move(right->data[0]));
        right->remove(NodePoint::First);
        node->right = right->left;
        if (node->right)
        {
          node->right->parent = node;
        }
        right->left = right->mid;
        right->mid = nullptr;
      }
      else
      {
        if (!node->right || node->right == fake_right_)
        {
          node->right = std::exchange(node->left, nullptr);
        }
        parent->insert(std::move(left->data[1]));
        left->remove(NodePoint::Second);
        node->left = left->right;
        if (node->left)
        {
          node->left->parent = node;
        }
        left->right = left->mid;
        left->mid = nullptr;
      }
      return;
    }
    if (left == node)
    {
      node->insert(std::move(parent->data[0]));
      parent->remove(NodePoint::First);
      if (!node->left || node->left == fake_left_)
      {
        std::swap(node->left, node->right);
      }
      if (mid->type == NodeType::Triple)
      {
        parent->insert(std::move(mid->data[0]));
        mid->remove(NodePoint::First);
        node->right = mid->left;
        if (node->right)
        {
          node->right->parent = node;
        }
        mid->left = mid->mid;
        mid->mid = nullptr;
        return;
      }
      node->insert(std::move(mid->data[0]));
      node->mid = mid->left;
      node->right = mid->right;
      if (node->mid)
      {
        node->mid->parent = node;
        node->right->parent = node;
      }
      mid->clear();
      delete mid;
      parent->mid = nullptr;
      return;
    }
    if (mid == node)
    {
      node->insert(std::move(parent->data[0]));
      parent->remove(NodePoint::First);
      if (!node->right || node->right == fake_right_)
      {
        std::swap(node->left, node->right);
      }
      if (left->type == NodeType::Triple)
      {
        parent->insert(std::move(left->data[1]));
        left->remove(NodePoint::Second);
        node->left = left->right;
        if (node->left)
        {
          node->left->parent = node;
        }
        left->right = left->mid;
        left->mid = nullptr;
        return;
      }
      node->insert(std::move(left->data[0]));
      node->mid = left->right;
      node->left = left->left;
      if (node->mid)
      {
        node->mid->parent = node;
        node->left->parent = node;
      }
      left->clear();
      delete left;
      parent->left = nullptr;
      std::swap(parent->left, parent->mid);
      return;
    }
    if (right == node)
    {
      node->insert(std::move(parent->data[1]));
      parent->remove(NodePoint::Second);
      if (!node->right || node->right == fake_right_)
      {
        node->right = std::exchange(node->left, nullptr);
      }
      if (mid->type == NodeType::Triple)
      {
        parent->insert(std::move(mid->data[1]));
        mid->remove(NodePoint::Second);
        node->left = mid->right;
        if (node->left)
        {
          node->left->parent = node;
        }
        mid->right = mid->mid;
        mid->mid = nullptr;
        return;
      }
      node->insert(std::move(mid->data[0]));
      node->mid = mid->right;
      node->left = mid->left;
      if (node->mid)
      {
        node->mid->parent = node;
        node->left->parent = node;
      }
      mid->clear();
      delete mid;
      parent->mid = nullptr;
      return;
    }
  }

  template< class Key, class Value, class Comparator >
  detail::TTTNode< Key, Value, Comparator >* TwoThreeTree< Key, Value, Comparator >::merge_erase(Node* node)
  {
    Node* parent = node->parent;
    Node* left = parent->left;
    Node* right = parent->right;
    Node* node_merge;

    if (left == node)
    {
      node_merge = right;
      std::swap(node_merge->mid, node_merge->left);
      if (node->right)
      {
        node_merge->left = node->right;
        node_merge->left->parent = node_merge;
      }
      if (node->left)
      {
        node_merge->left = node->left;
        node_merge->left->parent = node_merge;
      }
      parent->left = nullptr;
    }
    else
    {
      node_merge = left;
      std::swap(node_merge->mid, node_merge->right);
      if (node->right)
      {
        node_merge->right = node->right;
        node_merge->right->parent = node_merge;
      }
      if (node->left)
      {
        node_merge->right = node->left;
        node_merge->right->parent = node_merge;
      }
      parent->right = nullptr;
    }
    node_merge->insert(std::move(parent->data[0]));
    parent->remove(NodePoint::First);
    node->clear();
    delete node;

    if (!parent->parent)
    {
      parent->clear();
      delete parent;
      root_ = node_merge;
      root_->parent = nullptr;
      return root_;
    }
    return parent;
  }

  template< class Key, class Value, class Comparator >
  bool TwoThreeTree< Key, Value, Comparator >::is_balanced() const noexcept
  {
    return find_height(root_) != -1;
  }

  template< class Key, class Value, class Comparator >
  size_t TwoThreeTree< Key, Value, Comparator >::find_height(Node* node) const noexcept
  {
    if (node == nullptr || node == fake_right_)
    {
      return 0;
    }
    size_t left_height = find_height(node->left);
    size_t right_height = find_height(node->right);
    size_t mid_height = find_height(node->mid);
    if (left_height < 0 || right_height < 0 || mid_height < 0)
    {
      return -1;
    }
    if (left_height != right_height)
    {
      return -1;
    }
    if (left_height != mid_height && node->type == NodeType::Triple)
    {
      return -1;
    }
    return left_height + 1;
  }
}

#endif
