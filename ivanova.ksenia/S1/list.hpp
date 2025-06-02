#ifndef LIST_HPP
#define LIST_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>
#include "iterator.hpp"
#include "node.hpp"

namespace ivanova
{
  template <typename T>
  class List
  {
  public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using node_type = ListNode<T>;
    using iterator = ListIterator<List, false>;
    using const_iterator = ListIterator<List, true>;

    friend iterator;
    friend const_iterator;

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

    void push_back(const_reference value) { save_push(value, true); }
    void push_front(const_reference value) { save_push(value, false); }
    void pop_back() { pop(true); }
    void pop_front() { pop(false); }

    void swap(List& other) noexcept
    {
      std::swap(_head, other._head);
      std::swap(_tail, other._tail);
      std::swap(_size, other._size);
    }

    void clear();

    void splice(iterator position, List& other)
    {
      if (empty())
      {
        swap(other);
        return;
      }
      splice(position, other, other.begin(), other.end());
    }
 
    void splice(iterator position, List& other, iterator i)
    {
      iterator n = i;
      ++n;
      splice(position, other, i, n);
    }

    void splice(iterator position, List& other, iterator first, iterator last);

    void remove(const_reference value)
    {
      remove_if([&](auto x) { return x == value; });
    }

    template <typename Predicate>
    void remove_if(Predicate pred);
  
    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }

    reference back() { return *(--end()); }
    const_reference back() const { return *(--end()); }

    size_type size() const { return _size; }
    bool empty() const { return size() == 0; }

    iterator begin() { return iterator(this, _head); }
    const_iterator begin() const { return const_iterator(this, _head); }
    const_iterator cbegin() const { return const_iterator(this, _head); }

    iterator end() { return iterator(this, nullptr); }
    const_iterator end() const { return const_iterator(this, nullptr); }
    const_iterator cend() const { return const_iterator(this, nullptr); }

    private:
      node_type* _head;
      node_type* _tail;
      size_type _size;

      void save_push(const_reference value, bool back);
      void pop(bool back);
      void deleteNode(node_type* node);
      void cutNodes(node_type* first, node_type* last);
      void linkNodes(node_type* first, node_type* second);
      size_type getDistance(iterator first, iterator last) const;
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

  template <typename T>
  void List<T>::clear()
  {
    if (empty())
    {
      return;
    }
    node_type* node = _head;
    while (node != nullptr)
    {
      node_type* x = node;
      node = node->next;
      deleteNode(x);
    }
    _head = nullptr;
    _tail = nullptr;
    _size = 0;
  }

  template <typename T>
  template <typename Predicate>
  void List<T>::remove_if(Predicate pred)
  {
    while (!empty() && pred(front()))
    {
      pop_front();
    }
    while (!empty() && pred(back()))
    {
      pop_back();
    }
    for (node_type* x = _head; x != _tail; )
    {
      if (pred(x->next->value))
      {
        node_type* y = x->next;
        linkNodes(x, y->next);
        deleteNode(y);
        --_size;
      }
      else
      {
        x = x->next;
      }
    }
  }

  template <typename T>
  void List<T>::splice(iterator position, List& other, iterator first, iterator last)
  {
    if (other.empty())
    {
      return;
    }
    size_type diff = other.getDistance(first, last);
    if (diff == 0)
    {
      return;
    }
    iterator pre_last = last;
    --pre_last;
    other.cutNodes(first._node, pre_last._node);
    if (empty())
    {
      _head = first._node;
      _tail = pre_last._node;
    }
    else if (position == begin())
    {
      linkNodes(pre_last._node, _head);
      _head = first._node;
    }
    else if (position == end())
    {
      linkNodes(_tail, first._node);
      _tail = pre_last._node;
    }
    else
    {
      node_type* curr = position._node;
      node_type* prev = curr->prev;
      linkNodes(prev, first._node);
      linkNodes(pre_last._node, curr);
    }
    other._size -= diff;
    _size += diff;
  }

  template <typename T>
  void List<T>::linkNodes(node_type* first, node_type* second)
  {
    first->next = second;
    second->prev = first;
  }

  template <typename T>
  void List<T>::deleteNode(node_type* node)
  {
    if (node != nullptr)
    {
      node->value.~value_type();
      operator delete(node);
    }
  }

  template <typename T>
  typename List<T>::size_type List<T>::getDistance(iterator first, iterator last) const
  {
    if (first == begin() && last == end())
    {
      return _size;
    }
    size_type count = 0;
    for (auto it = first; it != last; ++it)
    {
      ++count;
    }
    return count;
  }
}
#endif
