#ifndef LIST_HPP
#define LIST_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include "iterator.hpp"
#include "node.hpp"

namespace ivanova
{
  template <typename T>
  class List
  {
  public:
    using const_reference = const T&;
    using size_type = std::size_t;
    using node_type = ListNode<T>;

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

    ~List() { clear(); }

    List& operator=(const List& other)
    {
      if (this != &other)
      {
        assign(other.begin(), other.end());
      }
      return *this;
    }

    List& operator=(List&& moved) noexcept
    {
      swap(moved);
      moved.clear();
      return *this;
    }
    void assign(size_type n, const_reference value);
    template <typename IterType>
    void assign(IterType first, IterType last);

    private:
      node_type* _head;
      node_type* _tail;
      size_type _size;
  };
  
  template <typename T>
  void List<T>::assign(size_type n, const_reference value)
  {
    clear();
    try
    {
      for (size_type i = 0; i < n; ++i)
      {
        push_back(value);
      }
    }
    catch(...)
    {
      clear();
      throw;
    }
  }

  template <typename T>
  template <typename IterType>
  void List<T>::assign(IterType first, IterType last)
  {
    bool same = std::is_same<IterType, iterator>::value
           || std::is_same<IterType, const_iterator>::value;
    if (!same || first._list != this)
    {
      clear();
      try
      {
        for (IterType x = first; x != last; ++x)
        {
          push_back(*x);
        }
      }
      catch(...)
      {
        clear();
        throw;
      }
    }
    else
    {
      if (first._node == nullptr)
      {
        clear();
        return;
      }
      while (!empty() && _head != first._node)
      {
        pop_front();
      }
      if (last._node == nullptr)
      {
        return;
      }
      while (!empty() && _tail != last._node)
      {
        pop_back();
      }
      pop_back();
    } 
  }
}
#endif
