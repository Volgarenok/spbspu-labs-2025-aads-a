#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include <type_traits>
#include "node.hpp"

namespace ivanova
{
  template <class List, bool IsConst>
  class ListIterator
  {
    friend List;
  public:

    using ptr_list = std::conditional_t<IsConst, const List*, List*>;
    using ptr_node = std::conditional_t<IsConst, const typename List::node_type*, typename List::node_type*>;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename List::value_type;
    using difference_type = std::ptrdiff_t;
    using pointer = std::conditional_t<IsConst, typename List::const_pointer, typename List::pointer>;
    using reference = std::conditional_t<IsConst, typename List::const_reference, typename List::reference>;

    ListIterator() : _list(nullptr), _node(nullptr) {}
    ListIterator(ptr_list list, ptr_node node) : _list(list), _node(node) {}
    ListIterator(const ListIterator& other) : _list(other._list), _node(other._node) {}

    ListIterator& operator=(const ListIterator& other)
    {
      _list = other._list;
      _node = other._node;
      return *this;
    }

    reference operator*() const { return _node->value; }
    pointer operator->() const { return &(_node->value); }

    ListIterator& operator++()
    {
      if (_list != nullptr && _node != nullptr)
      {
        _node = _node->next;
      }
      return *this;
    }

    ListIterator operator++(int)
    {
      ListIterator tmp(*this);
      ++(*this);
      return tmp;
    }

    ListIterator& operator--()
    {
      if (_list == nullptr)
      {
        return *this;
      }
      _node = (_node != nullptr) ? _node->prev : _list->_tail;
      return *this;
    }

    ListIterator operator--(int)
    {
      ListIterator tmp(*this);
      --(*this);
      return tmp;
    }

    bool operator==(const ListIterator& rhs) const { return _list == rhs._list && _node == rhs._node; }
    bool operator!=(const ListIterator& rhs) const { return !(*this == rhs); }

  private:
    ptr_list _list;
    ptr_node _node;
  };
}

#endif
