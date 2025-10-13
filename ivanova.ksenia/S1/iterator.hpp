#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include <type_traits>
#include "node.hpp"

namespace ivanova
{
  template < class List, bool IsConst >
  class ListIterator
  {
    friend List;

  public:
    using ptr_list = std::conditional_t< IsConst, const List*, List* >;
    using ptr_node = std::conditional_t< IsConst, const typename List::node_type*, typename List::node_type* >;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename List::value_type;
    using difference_type = std::ptrdiff_t;
    using pointer = std::conditional_t< IsConst, typename List::const_pointer, typename List::pointer >;
    using reference = std::conditional_t< IsConst, typename List::const_reference, typename List::reference >;

    ListIterator():
      list_(nullptr),
      node_(nullptr)
    {}

    ListIterator(const ListIterator& other):
      list_(other.list_),
      node_(other.node_)
    {}

    ListIterator& operator=(const ListIterator& other)
    {
      list_ = other.list_;
      node_ = other.node_;
      return *this;
    }

    reference operator*() const
    {
      return node_->value;
    }

    pointer operator->() const
    {
      return &(node_->value);
    }

    ListIterator& operator++()
    {
      if (list_ != nullptr && node_ != nullptr)
      {
        node_ = node_->next;
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
      if (list_ == nullptr)
      {
        return *this;
      }
      node_ = (node_ != nullptr) ? node_->prev : list_->tail_;
      return *this;
    }

    ListIterator operator--(int)
    {
      ListIterator tmp(*this);
      --(*this);
      return tmp;
    }

    bool operator==(const ListIterator& rhs) const
    {
      return list_ == rhs.list_ && node_ == rhs.node_;
    }

    bool operator!=(const ListIterator& rhs) const
    {
      return !(*this == rhs);
    }

  private:
    ListIterator(ptr_list list, ptr_node node):
      list_(list),
      node_(node)
    {}

    ptr_list list_;
    ptr_node node_;
  };
}

#endif
