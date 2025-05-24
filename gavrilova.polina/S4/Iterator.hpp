// #ifndef ITERATOR_TTT_HPP
// #define ITERATOR_TTT_HPP

// #include <cassert>
// #include <iterator>
// #include "TwoThreeTree.hpp"

// namespace gavrilova {
//   template < class Key, class Value, class Cmp >
//   struct Iterator: public std::iterator< std::bidirectional_iterator_tag, std::pair< Key, Value > > {
//     using this_t = Iterator< Key, Value, Cmp >;
//     using Node = NodeTwoThreeTree< Key, Value >;

//     Iterator():
//       node_(nullptr) {};
//     ~Iterator() = default;
//     Iterator(const this_t&) = default;
//     this_t& operator=(const this_t&) = default;

//     this_t& operator++();
//     this_t operator++(int);
//     this_t& operator--();
//     this_t operator--(int);
//     const Value& operator*() const;
//     const Value* operator->() const;
//     bool operator!=(const this_t&) const;
//     bool operator==(const this_t&) const;

//   private:
//     NodeTwoThreeTree< Key, Value >* node_;
//     friend class TwoThreeTree< Key, Value, Cmp >;
//     // friend class ConstIterator< Key, Value, Cmp >;
//     explicit Iterator(NodeTwoThreeTree< Key, Value >* node):
//       node_(node) {};
//   };

//   template < class Key, class Value, class Cmp >
//   Iterator< Key, Value, Cmp >& Iterator< Key, Value, Cmp >::operator++()
//   {
//     int ind_right = node_->is_3_node ? 2 : 1;
//     if (!node_) {
//       return *this;
//     }
//     if (!node_->is_leaf()) {
//       Node* next = node_->children[ind_right];
//       if (next) {
//         node_ = next;
//         while (node_->children[0]) {
//           node_ = node_->children[0];
//         }
//         return *this;
//       }
//     }

//     Node* parent = node_->parent;
//     Node* current = node_;
//     size_t parent_pos = 0;

//     while (parent) {
//       if (parent->children[0] == current) {
//         parent_pos = 0;
//         break;
//       } else if (parent->children[1] == current) {
//         parent_pos = 1;
//         break;
//       } else if (parent->children[2] == current) {
//         parent_pos = 2;
//         current = parent;
//         parent = parent->parent;
//         continue;
//       }
//     }

//     if (!parent) {
//       node_ = nullptr;
//       node_->is_3_node = false;
//       return *this;
//     }

//     node_ = parent;
//     node_->is_3_node = (parent_pos < node_->key_count) ? parent_pos : node_->key_count - 1;
//     return *this;
//   }

//   template < class Key, class Value, class Cmp >
//   Iterator< Key, Value, Cmp > Iterator< Key, Value, Cmp >::operator++(int)
//   {
//     Iterator< Key, Value, Cmp > result(*this);
//     ++(*this);
//     return result;
//   }

//   template < class Key, class Value, class Cmp >
//   Iterator< Key, Value, Cmp >& Iterator< Key, Value, Cmp >::operator--()
//   {
//     if (!node_) {
//       node_ = root_;
//       if (!node_) return *this;
//       while (!node_->is_leaf()) {
//         node_ = node_->children[node_->is_3_node ? 2 : 1];
//       }
//       node_->is_3_node = node_->key_count - 1;
//       return *this;
//     }

//     if (!node_->is_leaf()) {
//       Node* next = node_->children[pos_];
//       if (next) {
//         node_ = next;
//         while (!node_->is_leaf()) {
//           node_ = node_->children[node_->is_3_node ? 2 : 1];
//         }
//         node_->is_3_node = node_->key_count - 1;
//         return *this;
//       }
//     }

//     Node* parent = node_->parent;
//     Node* current = node_;
//     size_t parent_pos = 0;

//     while (parent) {
//       if (parent->children[1] == current && current == node_) {
//         if (!node_->is_3_node) {
//           parent_pos = 0;
//           break;
//         }
//       } else if (parent->children[2] == current) {
//         parent_pos = 1;
//         break;
//       }

//       current = parent;
//       parent = parent->parent;
//     }

//     if (!parent) {
//       node_ = nullptr;
//       node_->is_3_node = false;
//       return *this;
//     }

//     node_ = parent;
//     node_->is_3_node = parent_pos;
//     return *this;
//   }

//   template < class Key, class Value, class Cmp >
//   Iterator< Key, Value, Cmp >& Iterator< Key, Value, Cmp >::operator--(int)
//   {
//     Iterator< Key, Value, Cmp > result(*this);
//     --(*this);
//     return result;
//   }

//   template < class Key, class Value, class Cmp >
//   bool Iterator< Key, Value, Cmp >::operator==(const this_t& rhs) const
//   {
//     return node_ == rhs.node_;
//   }

//   template < class Key, class Value, class Cmp >
//   bool Iterator< Key, Value, Cmp >::operator!=(const this_t& rhs) const
//   {
//     return !(rhs == *this);
//   }

//   template < class Key, class Value, class Cmp >
//   T& Iterator< Key, Value, Cmp >::operator*()
//   {
//     assert(node_ != nullptr);
//     return node_->data;
//   }

//   template < class Key, class Value, class Cmp >
//   T* Iterator< Key, Value, Cmp >::operator->()
//   {
//     assert(node_ != nullptr);
//     return std::addressof(node_->data);
//   }
// }
