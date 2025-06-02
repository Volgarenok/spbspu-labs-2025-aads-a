#ifndef LIST_HPP
#define LIST_HPP

#include "iterator.hpp"
#include "node.hpp"

namespace ivanova
{
  template <typename T>
  class List
  {
  public:
    List() noexcept : _head(nullptr), _tail(nullptr), _size(0) {}

    List(const List& other) : List()
    {
      assign(other.begin(), other.end());
    }

    List(size_type n, const_reference value) : List()
    {
      assign(n, value);
    }
  
    List(List&& moved) noexcept : _head(moved._head), _tail(moved._tail), _size(moved._size)
    {
      moved._head = nullptr;
      moved._tail = nullptr;
      moved._size = 0;
    }
  };
}
#endif
