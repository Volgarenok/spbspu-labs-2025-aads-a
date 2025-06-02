#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include "node.hpp"

namespace ivanova
{
  template <class List, bool IsConst>
  class ListIterator
  {
    friend List;
  public:
    ListIterator() : _list(nullptr), _node(nullptr) {}
    ListIterator(ptr_list list, ptr_node node) : _list(list), _node(node) {}
    ListIterator(const ListIterator& other) : _list(other._list), _node(other._node) {}

    ListIterator& operator=(const ListIterator& other)
    {
    }

    reference operator*() const { return _node->value; }
    pointer operator->() const { return &(_node->value); }

    ListIterator& operator++()
    {
    }

    ListIterator operator++(int)
    {
    }

    ListIterator& operator--()
    {
    }

    ListIterator operator--(int)
    {
    }

    bool operator==(const ListIterator& rhs) const { return _list == rhs._list && _node == rhs._node; }
    bool operator!=(const ListIterator& rhs) const { return !(*this == rhs); }
  
  private:
    ptr_list _list;
    ptr_node _node;
  };
}

#endif
