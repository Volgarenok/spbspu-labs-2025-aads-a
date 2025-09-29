#ifndef BINARY_SEARCH_TREE_HPP
#define BINARY_SEARCH_TREE_HPP

#include <stack>
#include <queue>
#include <vector>
#include <algorithm>
#include <tree.hpp>

namespace guseynov {
  template < typename Key, typename Value, typename Compare = std::less< Key > >
  class BinarySearchTree : public Tree< Key, Value, Compare > {
  public:
    using Base = Tree< Key, Value, Compare >;
    using Tree< Key, Value, Compare >::Tree;

    template< typename F >
    F traverse_lnr(F f) {
      if (this->empty()) {
        return f;
      }
      std::vector< std::pair< Key, Value > > elements;
      for (auto it = this->begin(); it != this->end(); ++it) {
        elements.push_back(*it);
      }
      std::sort(elements.begin(), elements.end(), [](const std::pair< Key, Value >& a, const std::pair< Key, Value >& b) {
        return a.first < b.first;
      });
      for (size_t i = 0; i < elements.size(); ++i) {
        f(elements[i]);
      }
      return f;
    }

    template< typename F >
    F traverse_lnr(F f) const {
      if (this->empty()) {
        return f;
      }
      std::vector< std::pair< Key, Value > > elements;
      for (auto it = this->cbegin(); it != this->cend(); ++it) {
        elements.push_back(*it);
      }
      std::sort(elements.begin(), elements.end(), [](const std::pair< Key, Value >& a, const std::pair< Key, Value >& b) {
        return a.first < b.first;
      });
      for (size_t i = 0; i < elements.size(); ++i) {
        f(elements[i]);
      }
      return f;
    }

    template< typename F >
    F traverse_rnl(F f) {
      if (this->empty()) {
        return f;
      }
      std::vector< std::pair< Key, Value > > elements;
      for (auto it = this->begin(); it != this->end(); ++it) {
        elements.push_back(*it);
      }
      std::sort(elements.begin(), elements.end(), [](const std::pair< Key, Value >& a, const std::pair< Key, Value >& b) {
        return a.first > b.first;
      });
      for (size_t i = 0; i < elements.size(); ++i) {
        f(elements[i]);
      }
      return f;
    }

    template< typename F >
    F traverse_rnl(F f) const {
      if (this->empty()) {
        return f;
      }
      std::vector< std::pair< Key, Value > > elements;
      for (auto it = this->cbegin(); it != this->cend(); ++it) {
        elements.push_back(*it);
      }
      std::sort(elements.begin(), elements.end(), [](const std::pair< Key, Value >& a, const std::pair< Key, Value >& b) {
        return a.first > b.first;
      });
      for (size_t i = 0; i < elements.size(); ++i) {
        f(elements[i]);
      }
      return f;
    }

    template< typename F >
    F traverse_breadth(F f) {
      if (this->empty()) {
        return f;
      }
      std::vector< std::pair< Key, Value > > elements;
      for (auto it = this->begin(); it != this->end(); ++it) {
        elements.push_back(*it);
      }
      for (size_t i = 0; i < elements.size(); ++i) {
        f(elements[i]);
      }
      return f;
    }

    template< typename F >
    F traverse_breadth(F f) const {
      if (this->empty()) {
        return f;
      }
      std::vector< std::pair< Key, Value > > elements;
      for (auto it = this->cbegin(); it != this->cend(); ++it) {
        elements.push_back(*it);
      }
      for (size_t i = 0; i < elements.size(); ++i) {
        f(elements[i]);
      }
      return f;
    }
  };
}

#endif
