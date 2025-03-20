#ifndef LIST_HPP
#define LIST_HPP
#include <cassert>
#include <initializer_list>
#include "list-node.hpp"
#include "list-iterators.hpp"

namespace savintsev
{
  template< class T >
  class List
  {
  public:
    using iterator = Iterator< T >;
    using const_iterator = ConstIterator< T >;

    List();
    ~List();
    List(const List & rhs);
    List(List && rhs);
    List(size_t n, const T & value);
    template< class InputIterator >
    List(InputIterator first, InputIterator last);
    List(std::initializer_list< T > il);

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    T & front();
    const T & front() const;
    T & back();
    const T & back() const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    List & operator=(std::initializer_list< T > il);

    void push_front(const T & value);
    void push_front(T && value);
    void push_back(const T & value);
    void push_back(T && value);
    void pop_front();
    void pop_back();
    void swap(List & rhs) noexcept;
    void clear() noexcept;

    void remove(const T & value) noexcept;
    template< class Predicate >
    void remove_if(Predicate pred) noexcept;

    void splice(const_iterator pos, List & rhs) noexcept;
    void splice(const_iterator pos, List && rhs) noexcept;
    void splice(const_iterator pos, List & rhs, const_iterator i) noexcept;
    void splice(const_iterator pos, List && rhs, const_iterator i) noexcept;
    void splice(const_iterator pos, List & rhs, const_iterator first, const_iterator last) noexcept;
    void splice(const_iterator pos, List && rhs, const_iterator first, const_iterator last) noexcept;

    void assign(const_iterator first, const_iterator last);
    void assign(size_t n, const T & value);
    void assign(std::initializer_list< T > il);

    iterator erase(const_iterator pos) noexcept;
    iterator erase(const_iterator first, const_iterator last) noexcept;

    iterator insert(const_iterator pos, const T & value);
    iterator insert(const_iterator pos, size_t n, const T & value);
    template< class InputIterator >
    iterator insert(const_iterator pos, InputIterator first, InputIterator last);
    iterator insert(const_iterator pos, T && value);
    iterator insert(const_iterator pos, std::initializer_list< T > il);

    void reverse() noexcept;

    template< class... Args >
    iterator emplace(const_iterator pos, Args &&... args);

    template< class... Args >
    iterator emplace_back(Args &&... args);

    template< class... Args >
    iterator emplace_front(Args &&... args);

    void sort();
    template< class Compare >
    void sort(Compare comp);

    void merge(List & x);
    void merge(List && x);
    template< class Compare >
    void merge(List & x, Compare comp);
    template< class Compare >
    void merge(List && x, Compare comp);

    void unique();
    template< class BinaryPredicate >
    void unique(BinaryPredicate binary_pred);
  private:
    ListNode< T > * dummy;
    size_t list_size;

    template< class Compare >
    static bool compare_lists(const List & lhs, const List & rhs, Compare comp);
  };

  template< class T >
  bool operator==(const List< T > & lhs, const List< T > & rhs) noexcept;

  template< class T >
  bool operator!=(const List< T > & lhs, const List< T > & rhs) noexcept;

  template< class T >
  bool operator<(const List< T > & lhs, const List< T > & rhs) noexcept;
  template< class T >
  bool operator<=(const List< T > & lhs, const List< T > & rhs) noexcept;

  template< class T >
  bool operator>(const List< T > & lhs, const List< T > & rhs) noexcept;
  template< class T >
  bool operator>=(const List< T > & lhs, const List< T > & rhs) noexcept;


  template< class T >
  bool operator==(const List< T > & lhs, const List< T > & rhs) noexcept
  {
    if (lhs.size() != rhs.size())
    {
      return false;
    }
    auto jt = rhs.cbegin();
    for (auto it = lhs.cbegin(); it != lhs.cend(); ++it)
    {
      if (!(*it == *jt))
      {
        return false;
      }
      ++jt;
    }
    return true;
  }

  template< class T >
  bool operator!=(const List< T > & lhs, const List< T > & rhs) noexcept
  {
    return (!(lhs == rhs));
  }

  template< class T >
  bool operator<(const List< T > & lhs, const List< T > & rhs) noexcept
  {
    auto jt = rhs.cbegin();
    for (auto it = lhs.cbegin(); it != lhs.cend(); ++it)
    {
      if (jt == rhs.end() || *jt < *it)
      {
        return false;
      }
      else if (*it < *jt)
      {
        return true;
      }
      ++jt;
    }
    return (jt != rhs.end());
  }

  template< class T >
  bool operator<=(const List< T > & lhs, const List< T > & rhs) noexcept
  {
    return (!(rhs < lhs));
  }

  template< class T >
  bool operator>(const List< T > & lhs, const List< T > & rhs) noexcept
  {
    return (rhs < lhs);
  }

  template< class T >
  bool operator>=(const List< T > & lhs, const List< T > & rhs) noexcept
  {
    return (!(lhs < rhs));
  }

  template< class T >
  List< T >::List():
    dummy(new ListNode< T >()),
    list_size(0)
  {}

  template< class T >
  List< T >::~List()
  {
    clear();
    delete dummy;
  }

  template< class T >
  List< T >::List(const List & rhs):
    dummy(new ListNode< T >()),
    list_size(0)
  {
    try
    {
      for (auto it = rhs.begin(); it != rhs.end(); ++it)
      {
        push_back(*it);
      }
    }
    catch (const std::bad_alloc & e)
    {
      clear();
      delete dummy;
      throw;
    }
  }

  template< class T >
  List< T >::List(List && rhs):
    dummy(rhs.dummy),
    list_size(rhs.list_size)
  {
    ListNode< T > * new_dummy = new ListNode< T >();
    rhs.dummy = new_dummy;
    rhs.dummy->next = rhs.dummy;
    rhs.dummy->prev = rhs.dummy;
    rhs.list_size = 0;
  }

  template< class T >
  List< T >::List(size_t n, const T & value):
    dummy(new ListNode< T >()),
    list_size(0)
  {
    try
    {
      for (size_t i = 0; i < n; ++i)
      {
        push_back(value);
      }
    }
    catch (const std::bad_alloc & e)
    {
      clear();
      delete dummy;
      throw;
    }
  }

  template< class T >
  List< T >::List(std::initializer_list< T > il):
    dummy(new ListNode< T >()),
    list_size(0)
  {
    try
    {
      for (auto it = il.begin(); it != il.end(); ++it)
      {
        push_back(*it);
      }
    }
    catch (const std::bad_alloc & e)
    {
      clear();
      delete dummy;
      throw;
    }
  }

  template< class T >
  template< class InputIterator >
  List< T >::List(InputIterator first, InputIterator last):
    dummy(new ListNode< T >()),
    list_size(0)
  {
    try
    {
      for (auto it = first; it != last; ++it)
      {
        push_back(*it);
      }
    }
    catch (const std::bad_alloc & e)
    {
      clear();
      delete dummy;
      throw;
    }
  }

  template< class T >
  typename List< T >::iterator List< T >::begin() noexcept
  {
    return iterator(dummy->next);
  }

  template< class T >
  typename List< T >::const_iterator List< T >::begin() const noexcept
  {
    return const_iterator(dummy->next);
  }

  template< class T >
  typename List< T >::iterator List< T >::end() noexcept
  {
    return iterator(dummy);
  }

  template< class T >
  typename List< T >::const_iterator List< T >::end() const noexcept
  {
    return const_iterator(dummy);
  }

  template< class T >
  typename List< T >::const_iterator List< T >::cbegin() const noexcept
  {
    return const_iterator(dummy->next);
  }

  template< class T >
  typename List< T >::const_iterator List< T >::cend() const noexcept
  {
    return const_iterator(dummy);
  }

  template< class T >
  T & List< T >::front()
  {
    return dummy->next->data;
  }

  template< class T >
  const T & List< T >::front() const
  {
    return dummy->next->data;
  }

  template< class T >
  T & List< T >::back()
  {
    return dummy->prev->data;
  }

  template< class T >
  const T & List< T >::back() const
  {
    return dummy->prev->data;
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return list_size == 0;
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return list_size;
  }

  template< class T >
  List< T > & List< T >::operator=(std::initializer_list< T > il)
  {
    clear();
    for (auto it = il.begin(); it != il.end(); ++it)
    {
      push_back(*it);
    }
    return *this;
  }

  template< class T >
  void List< T >::push_front(const T & value)
  {
    ListNode< T > * new_node = new ListNode< T >(value, dummy->next, dummy);
    dummy->next->prev = new_node;
    dummy->next = new_node;
    ++list_size;
  }

  template< class T >
  void List< T >::push_front(T && value)
  {
    ListNode< T > * new_node = new ListNode< T >(std::move(value), dummy->next, dummy);
    dummy->next->prev = new_node;
    dummy->next = new_node;
    ++list_size;
  }

  template< class T >
  void List< T >::push_back(const T & value)
  {
    ListNode< T > * new_node = new ListNode< T >(value, dummy, dummy->prev);
    dummy->prev->next = new_node;
    dummy->prev = new_node;
    ++list_size;
  }

  template< class T >
  void List< T >::push_back(T && value)
  {
    ListNode< T > * new_node = new ListNode< T >(std::move(value), dummy, dummy->prev);
    dummy->prev->next = new_node;
    dummy->prev = new_node;
    ++list_size;
  }

  template< class T >
  void List< T >::pop_front()
  {
    assert(!empty());
    ListNode< T > * temp_front = dummy->next;
    dummy->next = temp_front->next;
    dummy->next->prev = dummy;
    delete temp_front;
    --list_size;
  }

  template< class T >
  void List< T >::pop_back()
  {
    assert(!empty());
    ListNode< T > * temp_back = dummy->prev;
    dummy->prev = temp_back->prev;
    dummy->prev->next = dummy;
    delete temp_back;
    --list_size;
  }

  template< class T >
  void List< T >::swap(List & rhs) noexcept
  {
    std::swap(dummy, rhs.dummy);
    std::swap(list_size, rhs.list_size);
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_front();
    }
  }

  template< class T >
  void List< T >::remove(const T & value) noexcept
  {
    for (auto it = cbegin(); it != cend(); ++it)
    {
      if (*it == value)
      {
        it.node->prev->next = it.node->next;
        it.node->next->prev = it.node->prev;
        auto temp = it--;
        delete temp.node;
        --list_size;
      }
    }
  }

  template< class T >
  template< class Predicate >
  void List< T >::remove_if(Predicate pred) noexcept
  {
    for (auto it = cbegin(); it != cend(); ++it)
    {
      if (pred(*it))
      {
        it.node->prev->next = it.node->next;
        it.node->next->prev = it.node->prev;
        auto temp = it--;
        delete temp.node;
        --list_size;
      }
    }
  }

  template< class T >
  template< class... Args >
  typename List< T >::iterator List< T >::emplace(const_iterator pos, Args &&... args)
  {
    return insert(pos, T(args...));
  }

  template< class T >
  template< class... Args >
  typename List< T >::iterator List< T >::emplace_back(Args &&... args)
  {
    return insert(const_iterator(end()), T(args...));
  }

  template< class T >
  template< class... Args >
  typename List< T >::iterator List< T >::emplace_front(Args &&... args)
  {
    return insert(const_iterator(begin()), T(args...));
  }

  template< class T >
  template< class Compare >
  bool List< T >::compare_lists(const List & lhs, const List & rhs, Compare comp)
  {
    auto it = lhs.cbegin();
    auto jt = rhs.cbegin();

    for (; it != lhs.cend() && jt != rhs.cend(); ++it, ++jt)
    {
      if (comp(*it, *jt))
      {
        return true;
      }
      else if (comp(*jt, *it))
      {
        return false;
      }
    }
    return comp(lhs.size(), rhs.size());
  }

  template< class T >
  void List< T >::splice(const_iterator pos, List & rhs) noexcept
  {
    list_size += rhs.list_size;
    rhs.list_size = 0;

    pos.node->prev->next = rhs.dummy->next;
    rhs.dummy->next->prev = pos.node->prev;
    rhs.dummy->prev->next = pos.node;
    pos.node->prev = rhs.dummy->prev;

    rhs.dummy->next = rhs.dummy;
    rhs.dummy->prev = rhs.dummy;
  }

  template< class T >
  void List< T >::splice(const_iterator pos, List && rhs) noexcept
  {
    splice(pos, rhs);
  }

  template< class T >
  void List< T >::splice(const_iterator pos, List & rhs, const_iterator i) noexcept
  {
    pos.node->prev->next = i.node;
    pos.node->prev = i.node;
    i.node->prev->next = i.node->next;
    i.node->next->prev = i.node->prev;
    i.node->next = pos.node;
    i.node->prev = pos.node->prev->prev;
    list_size++;
    rhs.list_size--;
  }

  template< class T >
  void List< T >::splice(const_iterator pos, List && rhs, const_iterator i) noexcept
  {
    splice(pos, rhs, i);
  }

  template< class T >
  void List< T >::splice(const_iterator pos, List & rhs, const_iterator first, const_iterator last) noexcept
  {
    size_t dist = std::distance(first, last);
    list_size += dist;
    rhs.list_size -= dist;

    ListNode< T > * temp_first_prev = first.node->prev;

    last.node->prev->next = pos.node;
    pos.node->prev->next = first.node;
    first.node->prev->next = last.node;
    first.node->prev = pos.node->prev;
    pos.node->prev = last.node->prev;
    last.node->prev = temp_first_prev;

    temp_first_prev = nullptr;
  }

  template< class T >
  void List< T >::splice(const_iterator pos, List && rhs, const_iterator first, const_iterator last) noexcept
  {
    splice(pos, rhs, first, last);
  }

  template< class T >
  void List< T >::assign(const_iterator first, const_iterator last)
  {
    clear();
    for (auto it = first; it != last; ++it)
    {
      push_back(*it);
    }
  }

  template< class T >
  void List< T >::assign(size_t n, const T & value)
  {
    clear();
    for (size_t i = 0; i < n; ++i)
    {
      push_back(value);
    }
  }

  template< class T >
  void List< T >::assign(std::initializer_list< T > il)
  {
    clear();
    for (auto it = il.begin(); it != il.end(); ++it)
    {
      push_back(*it);
    }
  }

  template< class T >
  typename List< T >::iterator List< T >::erase(const_iterator pos) noexcept
  {
    iterator result(pos.node->next);
    pos.node->prev->next = pos.node->next;
    pos.node->next->prev = pos.node->prev;
    delete pos.node;
    --list_size;
    return result;
  }

  template< class T >
  typename List< T >::iterator List< T >::erase(const_iterator first, const_iterator last) noexcept
  {
    iterator result = nullptr;
    for (auto it = first; it != last; ++it)
    {
      result = erase(it);
    }
    return result;
  }

  template< class T >
  typename List< T >::iterator List< T >::insert(const_iterator pos, const T & value)
  {
    ListNode< T > * new_node = new ListNode< T >(value, pos.node, pos.node->prev);
    list_size++;
    pos.node->prev->next = new_node;
    pos.node->prev = new_node;
    return iterator(new_node);
  }

  template< class T >
  typename List< T >::iterator List< T >::insert(const_iterator pos, size_t n, const T & value)
  {
    iterator first(pos.node->prev);
    try
    {
      for (size_t i = 0; i < n; ++i)
      {
        insert(pos, value);
      }
    }
    catch (const std::bad_alloc & e)
    {
      erase(++first, pos);
      throw;
    }
    return ++first;
  }

  template< class T >
  template< class InputIterator >
  typename List< T >::iterator List< T >::insert(const_iterator pos, InputIterator first, InputIterator last)
  {
    iterator current_first(pos.node->prev);
    try
    {
      for (auto it = first; it != last; ++it)
      {
        insert(pos, *it);
      }
    }
    catch (const std::bad_alloc & e)
    {
      erase(++current_first, pos);
      throw;
    }
    return ++current_first;
  }

  template< class T >
  typename List< T >::iterator List< T >::insert(const_iterator pos, T && value)
  {
    ListNode< T > * new_node = new ListNode< T >(std::move(value), pos.node, pos.node->prev);
    list_size++;
    pos.node->prev->next = new_node;
    pos.node->prev = new_node;
    return iterator(new_node);
  }

  template< class T >
  typename List< T >::iterator List< T >::insert(const_iterator pos, std::initializer_list< T > il)
  {
    return insert(pos, il.begin(), il.end());
  }

  template< class T >
  void List< T >::reverse() noexcept
  {
    ListNode< T > * pos = dummy;
    do
    {
      std::swap(pos->next, pos->prev);
      pos = pos->prev;
    }
    while (pos != dummy);
  }
}

#endif
