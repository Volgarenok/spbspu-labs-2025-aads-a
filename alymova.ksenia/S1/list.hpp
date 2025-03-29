#ifndef LIST_HPP
#define LIST_HPP
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <algorithm>
#include "listNode.hpp"

namespace alymova
{
  template< typename T >
  struct Iterator;
  template< typename T >
  struct ConstIterator;

  template< typename T >
  class List
  {
  public:
    List();
    List(const List< T >& other);
    List(List< T >&& other) noexcept;
    List(size_t n, const T& value = T());
    template< typename InputIterator>
    List(InputIterator first, InputIterator last);
    List(std::initializer_list< T > il);
    ~List() noexcept;

    List< T >& operator=(const List< T >& other);
    List< T >& operator=(List< T >&& other) noexcept;
    List< T >& operator=(std::initializer_list< T > il);

    Iterator< T > begin() noexcept;
    ConstIterator< T > begin() const noexcept;
    ConstIterator< T > cbegin() const noexcept;
    Iterator< T > end() noexcept;
    ConstIterator< T > end() const noexcept;
    ConstIterator< T > cend() const noexcept;

    T& front() noexcept;
    T& back() noexcept;
    const T& front() const noexcept;
    const T& back() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void assign(size_t n, const T& value);
    template< typename InputIterator >
    void assign(InputIterator first, InputIterator last);
    void assign(std::initializer_list< T > il);
    void push_front(const T& value);
    void push_front(T&& value);
    void pop_front() noexcept;
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back() noexcept;
    Iterator< T > insert(Iterator< T > position, const T& value);
    Iterator< T > insert(Iterator< T > position, T&& value);
    Iterator< T > insert(Iterator< T > position, size_t n, const T& value);
    template< typename InputIterator >
    Iterator< T > insert(Iterator< T > position, InputIterator first, InputIterator last);
    Iterator< T > insert(Iterator< T > position, std::initializer_list< T > il);
    Iterator< T > erase(Iterator< T > position);
    Iterator< T > erase(Iterator< T > first, Iterator< T > last);
    template< typename... Args >
    Iterator< T > emplace_front(Args&&... args);
    template< typename... Args >
    Iterator< T > emplace_back(Args&&... args);
    template< typename... Args >
    Iterator< T > emplace(Iterator< T > position, Args&&... args);
    void swap(List< T >& other) noexcept;
    void clear() noexcept;

    void splice(Iterator< T > position, List< T >& other);
    void splice(Iterator< T > position, List< T >& other, Iterator< T > i);
    void splice(Iterator< T > position, List< T >& other, Iterator< T > first, Iterator< T > last);
    void remove(const T& value) noexcept;
    template< typename Predicate >
    void remove_if(Predicate pred);
    void unique();
    template< typename Comparator >
    void unique(Comparator cmp);
    void sort();
    template< typename Comparator >
    void sort(Comparator cmp);
    void merge(List< T >& other);
    template< typename Comparator >
    void merge(List< T >& other, Comparator cmp);
    void reverse() noexcept;
  private:
    detail::ListNode< T >* fake_;
    detail::ListNode< T >* head_;

    void push_single(detail::ListNode< T >* node);
  };

  template< typename T >
  bool operator==(const List< T >& lhs, const List< T >& rhs) noexcept;

  template< typename T >
  bool operator!=(const List< T >& lhs, const List< T >& rhs) noexcept;

  template< typename T >
  bool operator<(const List< T >& lhs, const List< T >& rhs) noexcept;

  template< typename T >
  bool operator<=(const List< T >& lhs, const List< T >& rhs) noexcept;

  template< typename T >
  bool operator>(const List< T >& lhs, const List< T >& rhs) noexcept;

  template< typename T >
  bool operator>=(const List< T >& lhs, const List< T >& rhs) noexcept;

  template< typename T >
  bool operator==(const List< T >& lhs, const List< T >& rhs) noexcept
  {
    if (lhs.size() != rhs.size())
    {
      return false;
    }
    for (auto it_lhs = lhs.cbegin(), it_rhs = rhs.cbegin(); it_lhs != lhs.cend(); ++it_lhs, ++it_rhs)
    {
      if ((*it_lhs) != (*it_rhs))
      {
        return false;
      }
    }
    return true;
  }

  template< typename T >
  bool operator!=(const List< T >& lhs, const List< T >& rhs) noexcept
  {
    return (!(lhs == rhs));
  }

  template< typename T >
  bool operator<(const List< T >& lhs, const List< T >& rhs) noexcept
  {
    auto it_lhs = lhs.cbegin(), it_rhs = rhs.cbegin();
    for (; it_lhs != lhs.cend() && it_rhs != rhs.cend(); ++it_lhs, ++it_rhs)
    {
      if ((*it_lhs) != (*it_rhs))
      {
        return (*it_lhs) < (*it_rhs);
      }
    }
    return (it_lhs == lhs.cend() && it_rhs != rhs.cend());
  }

  template< typename T >
  bool operator<=(const List< T >& lhs, const List< T >& rhs) noexcept
  {
    return (!(rhs < lhs));
  }

  template< typename T >
  bool operator>(const List< T >& lhs, const List< T >& rhs) noexcept
  {
    return (rhs < lhs);
  }

  template< typename T >
  bool operator>=(const List< T >& lhs, const List< T >& rhs) noexcept
  {
    return (!(lhs < rhs));
  }

  template< typename T >
  List< T >::List():
    fake_(new detail::ListNode< T >{T(), nullptr, nullptr}),
    head_(fake_)
  {}

  template< typename T >
  List< T >::List(const List< T >& other):
    List(other.begin(), other.end())
  {}

  template< typename T >
  List< T >::List(List< T >&& other) noexcept:
    fake_(std::exchange(other.fake_, nullptr)),
    head_(std::exchange(other.head_, nullptr))
  {}

  template< typename T >
  List< T >::List(size_t n, const T& value):
    List()
  {
    for (size_t i = 0; i < n; i++)
    {
      push_back(value);
    }
  }

  template< typename T >
  template< typename InputIterator >
  List< T >::List(InputIterator first, InputIterator last):
    List()
  {
    for(; first != last; ++first)
    {
      push_back(*first);
    }
  }

  template< typename T >
  List< T >::List(std::initializer_list< T > il):
    List(il.begin(), il.end())
  {}

  template< typename T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    assert(this != std::addressof(other) && "Assigning a list to itself");
    List< T > copy(other);
    swap(copy);
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(List< T >&& other) noexcept
  {
    assert(this != std::addressof(other) && "Assigning a list to itself");
    clear();
    fake_ = std::exchange(other.fake_, nullptr);
    head_ = std::exchange(other.head_, nullptr);
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(std::initializer_list< T > il)
  {
    List< T > copy(il);
    swap(copy);
    return *this;
  }

  template< typename T >
  List< T >::~List() noexcept
  {
    clear();
    delete fake_;
  }

  template< typename T >
  Iterator< T > List< T >::begin() noexcept
  {
    return Iterator< T >(head_);
  }

  template< typename T >
  ConstIterator< T > List< T >::begin() const noexcept
  {
    return ConstIterator< T >(head_);
  }

  template< typename T >
  ConstIterator< T > List< T >::cbegin() const noexcept
  {
    return begin();
  }

  template< typename T >
  Iterator< T > List< T >::end() noexcept
  {
    return Iterator< T >(fake_);
  }

  template< typename T >
  ConstIterator< T > List< T >::end() const noexcept
  {
    return ConstIterator< T >(fake_);
  }

  template< typename T >
  ConstIterator< T > List< T >::cend() const noexcept
  {
    return end();
  }

  template< typename T >
  T& List< T >::front() noexcept
  {
    assert(!empty());
    return head_->data;
  }

  template< typename T >
  T& List< T >::back() noexcept
  {
    assert(!empty());
    return fake_->prev->data;
  }

  template< typename T >
  const T& List< T >::front() const noexcept
  {
    assert(!empty());
    return head_->data;
  }

  template< typename T >
  const T& List< T >::back() const noexcept
  {
    assert(!empty());
    return fake_->prev->data;
  }

  template< typename T >
  bool List< T >::empty() const noexcept
  {
    return cbegin() == cend();
  }

  template< typename T >
  size_t List< T >::size() const noexcept
  {
    size_t size = 0;
    for (auto it = begin(); it != end(); ++it)
    {
      ++size;
    }
    return size;
  }

  template< typename T >
  void List< T >::assign(size_t n, const T& value)
  {
    clear();
    for (size_t i = 0; i < n; ++i)
    {
      push_back(value);
    }
  }

  template< typename T >
  template< typename InputIterator >
  void List< T >::assign(InputIterator first, InputIterator last)
  {
    clear();
    for (; first != last; ++first)
    {
      push_back(*first);
    }
  }

  template< typename T >
  void List< T >::assign(std::initializer_list< T > il)
  {
    assign(il.begin(), il.end());
  }

  template< typename T >
  void List< T >::push_front(const T& value)
  {
    emplace_front(value);
  }

  template< typename T >
  void List< T >::push_front(T&& value)
  {
    emplace_front(std::forward< T >(value));
  }

  template< typename T >
  void List< T >::pop_front() noexcept
  {
    assert(!empty());
    detail::ListNode< T >* subhead = head_->next;
    delete head_;
    head_ = subhead;
    head_->prev = nullptr;
  }

  template< typename T >
  void List< T >::push_back(const T& value)
  {
    emplace_back(value);
  }

  template< typename T >
  void List< T >::push_back(T&& value)
  {
    emplace_back(std::forward< T >(value));
  }

  template< typename T >
  void List< T >::pop_back() noexcept
  {
    assert(!empty());
    detail::ListNode< T >* subhead = fake_->prev;
    if (size() == 1)
    {
      head_ = fake_;
      fake_->prev = nullptr;
      delete subhead;
    }
    else
    {
      subhead->prev->next = fake_;
      fake_->prev = subhead->prev;
      delete subhead;
    }
  }

  template< typename T >
  Iterator< T > List< T >::insert(Iterator< T > position, const T& value)
  {
    return emplace(position, value);
  }

  template< typename T >
  Iterator< T > List< T >::insert(Iterator< T > position, T&& value)
  {
    return emplace(position, std::forward< T >(value));
  }

  template< typename T >
  Iterator< T > List< T >::insert(Iterator< T > position, size_t n, const T& value)
  {
    Iterator< T > return_it = insert(position, value);
    for (size_t i = 1; i < n; i++)
    {
      Iterator< T > tmp_it = insert(position, value);
      tmp_it = return_it;
    }
    return return_it;
  }

  template< typename T >
  template< typename InputIterator >
  Iterator< T > List< T >::insert(Iterator< T > position, InputIterator first, InputIterator last)
  {
    Iterator< T > return_it = insert(position, *first);
    ++first;
    for (; first != last; ++first)
    {
      Iterator< T > tmp_it = insert(position, *first);
      tmp_it = return_it;
    }
    return return_it;
  }

  template< typename T >
  Iterator< T > List< T >::insert(Iterator< T > position, std::initializer_list< T > il)
  {
    return insert(position, il.begin(), il.end());
  }

  template< typename T >
  Iterator< T > List< T >::erase(Iterator< T > position)
  {
    auto return_it = Iterator< T >(position.node_->next);
    if (position == begin())
    {
      pop_front();
    }
    else if (position == --end())
    {
      pop_back();
    }
    else
    {
      detail::ListNode< T >* node = position.node_;
      node->prev->next = node->next;
      node->next->prev = node->prev;
      delete node;
    }
    return return_it;
  }

  template< typename T >
  Iterator< T > List< T >::erase(Iterator< T > first, Iterator< T > last)
  {
    while (first != last)
    {
      auto next = Iterator< T >(first.node_->next);
      Iterator< T > tmp_it = erase(first);
      tmp_it = last;
      first = next;
    }
    return last;
  }

  template< typename T >
  template< typename... Args >
  Iterator< T > List< T >::emplace_front(Args&&... args)
  {
    auto node = new detail::ListNode< T >{T{std::forward< Args >(args)...}, nullptr, nullptr};
    if (empty())
    {
      push_single(node);
    }
    else
    {
      detail::ListNode< T >* subhead = head_;
      head_ = node;
      head_->next = subhead;
      subhead->prev = head_;
    }
    return begin();
  }

  template< typename T >
  template< typename... Args >
  Iterator< T > List< T >::emplace_back(Args&&... args)
  {
    auto node = new detail::ListNode< T >{T{std::forward< Args >(args)...}, nullptr, nullptr};
    if (empty())
    {
      push_single(node);
    }
    else
    {
      detail::ListNode< T >* subhead = fake_->prev;
      subhead->next = node;
      node->prev = subhead;
      node->next = fake_;
      fake_->prev = node;
    }
    return (--end());
  }

  template< typename T >
  template< typename... Args >
  Iterator< T > List< T >::emplace(Iterator< T > position, Args&&... args)
  {
    if (position == begin())
    {
      emplace_front(std::forward< Args >(args)...);
    }
    else if (position == end())
    {
      emplace_back(std::forward< Args >(args)...);
    }
    else
    {
      auto node_new = new detail::ListNode< T >{T{std::forward< Args >(args)...}, nullptr, nullptr};
      detail::ListNode< T >* node_now = position.node_;
      node_now->prev->next = node_new;
      node_new->prev = node_now->prev;
      node_now->prev = node_new;
      node_new->next = node_now;
    }
    return --position;
  }

  template< typename T >
  void List< T >::swap(List< T >& other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(head_, other.head_);
  }

  template< typename T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_front();
    }
  }

  template< typename T >
  void List< T >::splice(Iterator< T > position, List< T >& other)
  {
    assert(*this != other && "Splicing a list to itself");
    splice(position, other, other.begin(), other.end());
  }

  template< typename T >
  void List< T >::splice(Iterator< T > position, List< T >& other, Iterator< T > other_it)
  {
    assert(position.node_ != nullptr && "Iterator is not valid");
    assert(other_it.node_ != nullptr && "Iterator is not valid");
    assert(other_it != other.end() && "Iterator is not valid");

    detail::ListNode< T >* node_now = position.node_;
    detail::ListNode< T >* other_node_now = other_it.node_;
    if (position == begin())
    {
      head_ = other_node_now;
    }
    else
    {
      node_now->prev->next = other_node_now;
    }
    if (other_it == other.begin())
    {
      other.head_ = other_node_now->next;
    }
    else
    {
      other_node_now->prev->next = other_node_now->next;
    }
    other_node_now->next->prev = other_node_now->prev;
    other_node_now->prev = node_now->prev;
    other_node_now->next = node_now;
    node_now->prev = other_node_now;
  }

  template< typename T >
  void List< T >::splice(Iterator< T > position, List< T >& other, Iterator< T > first, Iterator< T > last)
  {
    auto other_it = first;
    while (other_it != last)
    {
      auto other_it_next = Iterator< T >(other_it.node_->next);
      splice(position, other, other_it);
      other_it = other_it_next;
    }
  }

  template< typename T >
  void List < T >::remove(const T& value) noexcept
  {
    struct EqualNode
    {
      const T& value;
      EqualNode(const T& new_value):
        value(new_value)
      {}
      bool operator()(const T& data)
      {
        return value == data;
      }
    };
    remove_if(EqualNode{value});
  }

  template< typename T >
  template< typename Predicate >
  void List< T >::remove_if(Predicate pred)
  {
    assert(!empty());
    auto it = begin();
    while (it != end())
    {
      auto it_next = Iterator< T >(it.node_->next);
      if (pred(*it))
      {
        erase(it);
      }
      it = it_next;
    }
  }

  template< typename T >
  void List< T >::unique()
  {
    unique(std::equal_to< T >());
  }

  template< typename T >
  template< typename Comparator >
  void List< T >::unique(Comparator cmp)
  {
    detail::ListNode< T >* subhead = head_;
    auto it = begin();
    ++it;
    while (it != end() && it.node_ != nullptr)
    {
      auto it_next = Iterator< T >(it.node_->next);
      if (cmp(*it, subhead->data))
      {
        erase(it);
      }
      else
      {
        subhead = subhead->next;
      }
      it = it_next;
    }
  }

  template< typename T >
  void List< T >::sort()
  {
    sort(std::less< T >());
  }

  template< typename T >
  template< typename Comparator >
  void List< T >::sort(Comparator cmp)
  {
    for (auto it1 = begin(); it1 != end(); ++it1)
    {
      auto min_it = it1;
      auto it2 = Iterator< T >(it1.node_->next);
      for (; it2 != end(); ++it2)
      {
        if (cmp(*it2, *min_it))
        {
          min_it = it2;
        }
      }
      std::swap(*it1, *min_it);
    }
  }

  template< typename T >
  void List< T >::merge(List< T >& other)
  {
    merge(other, std::less< T >());
  }

  template< typename T >
  template< typename Comparator >
  void List< T >::merge(List< T >& other, Comparator cmp)
  {
    if (this == std::addressof(other))
    {
      return;
    }
    if (!(std::is_sorted(begin(), end(), cmp) && std::is_sorted(other.begin(), other.end(), cmp)))
    {
      throw std::logic_error("Lists are not sorted");
    }
    auto it_other = other.begin();
    auto it = begin();
    while (!other.empty())
    {
      auto it_other_next = Iterator< T >(it_other.node_->next);
      while (!cmp(*it_other, *it) && it != end())
      {
        ++it;
      }
      splice(it, other, it_other);
      it_other = it_other_next;
    }
  }

  template< typename T >
  void List< T >::reverse() noexcept
  {
    if (size() < 2)
    {
      return;
    }
    auto it = ++begin();
    std::swap(head_->next, head_->prev);
    head_->next = fake_;
    while (it != --end())
    {
      auto it_next = Iterator< T >(it.node_->next);
      detail::ListNode< T >* node = it.node_;
      std::swap(node->next, node->prev);
      it = it_next;
    }
    detail::ListNode< T >* tail = it.node_;
    std::swap(tail->next, tail->prev);
    tail->prev = nullptr;
    fake_->prev = head_;
    head_ = tail;
  }

  template< typename T >
  void List< T >::push_single(detail::ListNode< T >* node)
  {
    head_ = node;
    head_->next = fake_;
    fake_->prev = head_;
  }
}
#endif
