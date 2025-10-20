#ifndef FORWARD_ITERATOR_HPP
#define FORWARD_ITERATOR_HPP

#include <iterator>
#include <forward_list>
#include "node.hpp"

namespace shabalin
{
  template < typename T >
  class List;

  template < typename T >
  struct ConstListIterator;

  template < typename T >
  struct ListIterator: public std::iterator< std::forward_iterator_tag, T >
  {
    friend class List< T >;
    friend struct ConstListIterator< T >;

  public:
    using this_t = ListIterator< T >;
    ListIterator():
      node_(nullptr)
    {}
    ListIterator(Node< T > * node):
      node_(node)
    {}
    ~ListIterator() = default;

    ListIterator(const this_t &) = default;
    this_t & operator=(const this_t &) = default;
    this_t & operator++()
    {
      node_ = node_->next_;
      return *this;
    }
    this_t operator++(int)
    {
      this_t result(*this);
      ++(*this);
      return result;
    }
    T & operator*()
    {
      return node_->data_;
    }
    T * operator->()
    {
      return std::addressof(node_->data_);
    }

    bool operator==(const this_t & rhs) const
    {
      return node_ == rhs.node_;
    }
    bool operator!=(const this_t & rhs) const
    {
      return !(rhs == *this);
    }

  private:
    Node< T > * node_;
  };
}

#endif

