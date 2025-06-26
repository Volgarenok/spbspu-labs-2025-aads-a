#ifndef TREE_DECLARATION_HPP
#define TREE_DECLARATION_HPP

#include <utility>
#include "tree_iterator.hpp"

namespace maslevtsov {
  template< class Key, class T, class Compare = std::less< Key > >
  class Tree
  {
  public:
    using value_type = std::pair< Key, T >;
    using size_type = std::size_t;
    using iterator = TreeIterator< value_type, detail::IteratorType::NONCONSTANT >;
    using const_iterator = TreeIterator< value_type, detail::IteratorType::CONSTANT >;

    Tree();
    Tree(const Tree& rhs);
    Tree(Tree&& rhs) noexcept;
    ~Tree();

    Tree& operator=(const Tree& rhs);
    Tree& operator=(Tree&& rhs) noexcept;

    T& operator[](const Key& key);
    const T& operator[](const Key& key) const;
    T& at(const Key& key);
    const T& at(const Key& key) const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const noexcept;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const noexcept;

    template< typename F >
    F traverse_lnr(F f);
    template< typename F >
    F traverse_lnr(F f) const;
    template< typename F >
    F traverse_rnl(F f);
    template< typename F >
    F traverse_rnl(F f) const;
    template< typename F >
    F traverse_breadth(F f);
    template< typename F >
    F traverse_breadth(F f) const;

    bool empty() const noexcept;
    size_type size() const noexcept;

    void clear() noexcept;
    std::pair< iterator, bool > insert(const value_type& value);
    iterator erase(iterator pos) noexcept;
    iterator erase(const_iterator pos) noexcept;
    size_type erase(const Key& key) noexcept;
    void swap(Tree& other) noexcept;

    size_type count(const Key& key) const noexcept;
    iterator find(const Key& key) noexcept;
    const_iterator find(const Key& key) const noexcept;
    std::pair< iterator, iterator > equal_range(const Key& key);
    std::pair< const_iterator, const_iterator > equal_range(const Key& key) const;

  private:
    using Node = TreeNode< value_type >;

    Node* dummy_root_;
    size_type size_;
    Compare compare_;

    void split_nodes(Node* node, value_type& to_insert, Node* left_child = nullptr, Node* right_child = nullptr);
    void clear_subtree(Node* node) noexcept;
    void assign_value(value_type& dist, const value_type& src);

    void erase_from_leaf(iterator pos) noexcept;
    void balance_parent_three(Node* deleted) noexcept;
    void balance_parent_two(Node* deleted) noexcept;
    void balance_parent_bro_two(Node* deleted) noexcept;
    void balance_next_parent_three(Node* deleted, Node* next_parent) noexcept;
    bool balance_next_parent_two(Node* deleted, Node* next_parent) noexcept;

    std::pair< iterator, bool > find_impl(const Key& key) const noexcept;
  };
}

#endif
