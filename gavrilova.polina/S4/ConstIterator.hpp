// #ifndef CONST_ITERATOR_ TTT_HPP
// #define CONST_ITERATOR_TTT_HPP

// #include <iterator>
// #include <cassert>
// #include "TwoThreeTree.hpp"

// namespace gavrilova {
//   template< class Key, class Value, class Cmp >
//   struct ConstIterator: public std::iterator< std::bidirectional_iterator_tag, std::pair< Key, Value > >
//   {
//     using this_t = ConstIterator< Key, Value, Cmp >;

//     ConstIterator();
//     ~ConstIterator() = default;
//     ConstIterator(const this_t&) = default;
//     // ConstIteratorFwd(IteratorFwd< T >& other);
//     this_t& operator=(const this_t&) = default;

//     this_t& operator++();
//     this_t operator++(int);
//     this_t& operator--();
//     this_t operator--(int);
//     const T& operator*() const;
//     const T* operator->() const;
//     bool operator!=(const this_t&) const;
//     bool operator==(const this_t&) const;

//   private:
//     TwoThreeTree< Key, Value, Cmp >* node_;
//     friend class TwoThreeTree< Key, Value, Cmp >;
//     friend class Iterator< Key, Value, Cmp >;
//     explicit ConstIterator(TwoThreeTree< Key, Value, Cmp >* node): node_(node) {};
//   // };
// }
