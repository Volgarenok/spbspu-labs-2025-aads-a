#ifndef LIST_HPP
#define LIST_HPP
#include <cassert>
#include <initializer_list>
#include <algorithm>
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
    List(List && rhs) noexcept;
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

    List & operator=(const List & x);
    List & operator=(List && x);
    List & operator=(std::initializer_list< T > il);

    template< class U >
    void push_front(U && value);
    template< class U >
    void push_back(U && value);
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

    iterator insert(const_iterator pos, size_t n, const T & value);
    template< class InputIterator >
    iterator insert(const_iterator pos, InputIterator first, InputIterator last);
    template< class U >
    iterator insert(const_iterator pos, U && value);
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

    template< class U >
    friend bool operator==(const List< U > & lhs, const List< U > & rhs) noexcept;
    template< class U >
    friend bool operator!=(const List< U > & lhs, const List< U > & rhs) noexcept;
    template< class U >
    friend bool operator<(const List< U > & lhs, const List< U > & rhs) noexcept;
    template< class U >
    friend bool operator<=(const List< U > & lhs, const List< U > & rhs) noexcept;
    template< class U >
    friend bool operator>(const List< U > & lhs, const List< U > & rhs) noexcept;
    template< class U >
    friend bool operator>=(const List< U > & lhs, const List< U > & rhs) noexcept;

  private:
    ListNode< T > * dummy;
    size_t list_size;

    template< class Compare >
    static bool compare_lists(const List & lhs, const List & rhs, Compare comp);
    void destroy() noexcept;
    template< class Compare >
    void merge_sort(Compare comp);
  };

  template< class T >
  bool operator==(const List< T > & lhs, const List< T > & rhs) noexcept
  {
    return List< T >::compare_lists(lhs, rhs, std::equal_to< T >());
  }

  template< class T >
  bool operator!=(const List< T > & lhs, const List< T > & rhs) noexcept
  {
    return (!(lhs == rhs));
  }

  template< class T >
  bool operator<(const List< T > & lhs, const List< T > & rhs) noexcept
  {
    return List< T >::compare_lists(lhs, rhs, std::less< T >());
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
  {
    dummy->next = dummy;
    dummy->prev = dummy;
  }

  template< class T >
  List< T >::~List()
  {
    destroy();
  }

  template< class T >
  List< T >::List(const List & rhs):
    List(rhs.begin(), rhs.end())
  {}

  template< class T >
  List< T >::List(List && rhs) noexcept:
    dummy(nullptr),
    list_size(0)
  {
    swap(rhs);
  }

  template< class T >
  List< T >::List(size_t n, const T & value):
    dummy(new ListNode< T >()),
    list_size(0)
  {
    dummy->next = dummy;
    dummy->prev = dummy;
    try
    {
      for (size_t i = 0; i < n; ++i)
      {
        push_back(value);
      }
    }
    catch (const std::bad_alloc & e)
    {
      destroy();
      throw;
    }
  }

  template< class T >
  template< class InputIterator >
  List< T >::List(InputIterator first, InputIterator last):
    dummy(new ListNode< T >()),
    list_size(0)
  {
    dummy->next = dummy;
    dummy->prev = dummy;
    try
    {
      for (auto it = first; it != last; ++it)
      {
        push_back(*it);
      }
    }
    catch (const std::bad_alloc & e)
    {
      destroy();
      throw;
    }
  }

  template< class T >
  List< T >::List(std::initializer_list< T > il):
    List(il.begin(), il.end())
  {}

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
  List< T > & List< T >::operator=(const List< T > & x)
  {
    assign(x.begin(), x.end());
    return *this;
  }

  template< class T >
  List< T > & List< T >::operator=(List< T > && x)
  {
    List temp(std::move(x));
    swap(temp);
    return *this;
  }

  template< class T >
  List< T > & List< T >::operator=(std::initializer_list< T > il)
  {
    assign(il);
    return *this;
  }

  template< class T >
  template< class U >
  void List< T >::push_front(U && value)
  {
    ListNode< T > * new_node = new ListNode< T >{std::forward< U >(value), dummy->next, dummy};
    dummy->next->prev = new_node;
    dummy->next = new_node;
    ++list_size;
  }

  template< class T >
  template< class U >
  void List< T >::push_back(U && value)
  {
    ListNode< T > * new_node = new ListNode< T >{std::forward< U >(value), dummy, dummy->prev};
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
    remove_if([& value](const T & rhs)
    {
      return rhs == value;
    });
  }

  template< class T >
  template< class Predicate >
  void List< T >::remove_if(Predicate pred) noexcept
  {
    for (auto it = cbegin(); it != cend();)
    {
      if (pred(*it))
      {
        it = erase(it);
      }
      else
      {
        ++it;
      }
    }
  }

  template< class T >
  template< class... Args >
  typename List< T >::iterator List< T >::emplace(const_iterator pos, Args &&... args)
  {
    ListNode< T > * node = new ListNode< T >{T(std::forward< Args >(args)...), pos.node, pos.node->prev};
    node->next = pos.node;
    node->prev = pos.node->prev;

    pos.node->prev->next = node;
    pos.node->prev = node;

    ++list_size;
    return iterator(node);
  }

  template< class T >
  template< class... Args >
  typename List< T >::iterator List< T >::emplace_back(Args &&... args)
  {
    return emplace(cend(), std::forward< Args >(args)...);
  }

  template< class T >
  template< class... Args >
  typename List< T >::iterator List< T >::emplace_front(Args &&... args)
  {
    return emplace(cbegin(), std::forward< Args >(args)...);
  }

  template< class T >
  template< class Compare >
  void List< T >::merge_sort(Compare comp)
  {
    if (list_size < 2)
    {
      return;
    }

    size_t mid = list_size / 2;
    auto it = begin();
    std::advance(it, mid);

    List< T > right;
    right.splice(right.begin(), *this, it, end());

    merge_sort(comp);
    right.merge_sort(comp);

    merge(right, comp);
  }

  template< class T >
  void List< T >::sort()
  {
    sort(std::less< T >());
  }

  template< class T >
  template< class Compare >
  void List< T >::sort(Compare comp)
  {
    List< T > temp(*this);
    temp.merge_sort(comp);
    *this = std::move(temp);
  }

  template< class T >
  void List< T >::merge(List< T > & x)
  {
    merge(x, std::less< T >());
  }

  template< class T >
  void List< T >::merge(List< T > && x)
  {
    merge(x, std::less< T >());
  }

  template< class T >
  template< class Compare >
  void savintsev::List< T >::merge(List< T > & x, Compare comp)
  {
    auto it1 = begin();
    auto it2 = x.begin();

    while (it1 != end() && it2 != x.end())
    {
      if (comp(*it2, *it1))
      {
        auto next = std::next(it2);
        splice(it1, x, it2);
        it2 = next;
      }
      else
      {
        ++it1;
      }
    }

    if (it2 != x.end())
    {
      splice(end(), x, it2, x.end());
    }
  }

  template< class T >
  template< class Compare >
  void List< T >::merge(List< T > && x, Compare comp)
  {
    merge(x, comp);
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
      if (comp(*jt, *it))
      {
        return false;
      }
    }
    return comp(lhs.size(), rhs.size());
  }

  template< class T >
  template< class BinaryPredicate >
  void List< T >::unique(BinaryPredicate binary_pred)
  {
    for (auto it = begin(); it != end();)
    {
      if (binary_pred(*it, *std::next(it)))
      {
        it = erase(it);
      }
      else
      {
        ++it;
      }
    }
  }

  template< class T >
  void List< T >::unique()
  {
    unique(std::equal_to<>());
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
    ListNode< T >* node = i.node;

    node->prev->next = node->next;
    node->next->prev = node->prev;
    --rhs.list_size;

    node->prev = pos.node->prev;
    node->next = pos.node;
    pos.node->prev->next = node;
    pos.node->prev = node;
    ++list_size;
  }

  template< class T >
  void List< T >::splice(const_iterator pos, List && rhs, const_iterator i) noexcept
  {
    splice(pos, rhs, i);
  }

  template< class T >
  void List< T >::splice(const_iterator pos, List & rhs, const_iterator first, const_iterator last) noexcept
  {
    size_t count = std::distance(first, last);
    list_size += count;
    rhs.list_size -= count;

    ListNode< T > * first_node = first.node;
    ListNode< T > * last_node = last.node;

    first_node->prev->next = last_node;
    last_node->prev->next = first_node;
    ListNode< T > * last_prev = last_node->prev;
    last_node->prev = first_node->prev;

    first_node->prev = pos.node->prev;
    pos.node->prev->next = first_node;
    last_prev->next = pos.node;
    pos.node->prev = last_prev;
  }

  template< class T >
  void List< T >::splice(const_iterator pos, List && rhs, const_iterator first, const_iterator last) noexcept
  {
    splice(pos, rhs, first, last);
  }

  template< class T >
  void List< T >::assign(const_iterator first, const_iterator last)
  {
    List temp(first, last);
    swap(temp);
  }

  template< class T >
  void List< T >::assign(size_t n, const T & value)
  {
    List temp(n, value);
    swap(temp);
  }

  template< class T >
  void List< T >::assign(std::initializer_list< T > il)
  {
    List temp(il);
    swap(temp);
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
  template< class U >
  typename List< T >::iterator List< T >::insert(const_iterator pos, U && value)
  {
    ListNode< T > * new_node = new ListNode< T >{std::forward< U >(value), pos.node, pos.node->prev};
    pos.node->prev->next = new_node;
    pos.node->prev = new_node;
    ++list_size;
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

  template< class T >
  void List< T >::destroy() noexcept
  {
    clear();
    delete dummy;
    dummy = nullptr;
  }
}

#endif
