#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <algorithm>
#include <initializer_list>
#include "node.hpp"
#include "iterator.hpp"
#include "cIterator.hpp"

namespace abramov
{
  template< class T >
  struct ConstIterator;

  template< class T >
  struct Iterator;

  template< class T >
  struct List
  {
    List();
    List(size_t n, T val);
    List(std::initializer_list< T > il);
    List(Iterator< T > begin, Iterator< T > end);
    ~List();
    List< T > &operator=(std::initializer_list< T > il);
    T &front() noexcept;
    const T &front() const noexcept;
    T &back() noexcept;
    const T &back() const noexcept;
    void pushBack(const T &data);
    void pushFront(const T &data);
    void popBack() noexcept;
    void popFront() noexcept;
    void swap(List &rhs) noexcept;
    void clear();
    bool empty() const noexcept;
    size_t size() const noexcept;
    Iterator< T > begin() const;
    Iterator< T > end() const;
    ConstIterator< T > cbegin() const;
    ConstIterator< T > cend() const;
    Iterator< T > find(Iterator< T > begin, Iterator< T > end, const T &val) const noexcept;
    template< class C >
    Iterator< T > find_if(Iterator< T > begin, Iterator< T > end, C condition) const noexcept;
    Iterator< T > remove(Iterator< T > begin, Iterator< T > end, const T &val);
    template< class C >
    Iterator< T > remove_if(Iterator< T > begin, Iterator< T > end, C condition);
    void splice(ConstIterator< T > pos, List< T > &other) noexcept;
    void splice(ConstIterator< T > pos, List< T > &&other) noexcept;
    void splice(ConstIterator< T > pos, List< T > &other, ConstIterator< T > it) noexcept;
    void splice(ConstIterator< T > pos, List< T > &&other, ConstIterator< T > it) noexcept;
    void splice(ConstIterator< T > pos, List< T > &other, ConstIterator< T > first, ConstIterator< T > last) noexcept;
    void splice(ConstIterator< T > pos, List< T > &&other, ConstIterator< T > first, ConstIterator< T > last) noexcept;
    void assign(size_t count, const T &val);
    void insert(ConstIterator< T > pos, const T &val);
    void insert(ConstIterator< T > pos, T &&val);
    void insert(ConstIterator< T > pos, size_t count, const T &val);
    Iterator< T > erase(Iterator< T > pos) noexcept;
    Iterator< T > erase(Iterator< T > first, Iterator< T > last) noexcept;
    void assign(Iterator< T > first, Iterator< T > last);
    void assign(std::initializer_list< T > il);
  private:
    Node< T > *head_;
    Node< T > *tail_;
    size_t size_;
    friend struct Iterator< T >;
    friend struct ConstIterator< T >;
  };

  template< class T >
  List< T >::List():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< class T >
  List< T >::List(size_t n, T val):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    try
    {
      for (size_t i = 0; i < n; ++i)
      {
        pushBack(val);
      }
    }
    catch (const std::bad_alloc &)
    {
      clear();
    }
  }

  template< class T >
  List< T >::List(std::initializer_list < T > il):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    for (auto iter = il.begin(); iter != il.end(); ++iter)
    {
      pushBack(*iter);
    }
  }

  template< class T >
  List< T >::List(Iterator< T > begin, Iterator< T > end):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    for (auto it = begin; it != end; ++it)
    {
      pushBack(*it);
    }
  }

  template< class T >
  List< T >::~List()
  {
    clear();
  }

  template< class T >
  List< T > &List< T >::operator=(std::initializer_list< T > il)
  {
    List< T > copy{ il };
    swap(copy);
    return *this;
  }

  template< class T >
  T &List< T >::front() noexcept
  {
    return const_cast< T& >(const_cast< const List< T >* >(this)->front());
  }

  template< class T >
  const T &List< T >::front() const noexcept
  {
    return head_->data_;
  }

  template< class T >
  T &List< T >::back() noexcept
  {
    return const_cast< T& >(const_cast< const List< T >* >(this)->back());
  }

  template< class T >
  const T &List< T >::back() const noexcept
  {
    return tail_->data_;
  }

  template< class T >
  void List< T >::pushBack(const T &data)
  {
    Node< T > *node = new Node< T >{ data };
    if (empty())
    {
      head_ = node;
      tail_ = node;
    }
    else
    {
      tail_->next_ = node;
      node->prev_ = tail_;
      tail_ = node;
    }
    ++size_;
  }

  template< class T >
  void List< T >::pushFront(const T &data)
  {
    Node< T > *node = new Node< T >{ data };
    if (empty())
    {
      head_ = node;
      tail_ = node;
    }
    else
    {
      head_->prev_ = node;
      node->next_ = head_;
      head_ = node;
    }
    ++size_;
  }

  template< class T >
  void List< T >::popBack() noexcept
  {
    if (!empty())
    {
      --size_;
      if (head_ == tail_)
      {
        delete tail_;
        head_ = nullptr;
        tail_ = nullptr;
      }
      else
      {
        Node< T > *node = tail_->prev_;
        delete tail_;
        tail_ = node;
        tail_->next_ = nullptr;
      }
    }
  }

  template< class T >
  void List< T >::popFront() noexcept
  {
    if (!empty())
    {
      --size_;
      if (head_ == tail_)
      {
        delete head_;
        head_ = nullptr;
        tail_ = nullptr;
      }
      else
      {
        Node< T > *node = head_->next_;
        delete head_;
        head_ = node;
        head_->prev_ = nullptr;
      }
    }
  }

  template< class T >
  void List< T >::swap(List &rhs) noexcept
  {
    std::swap(head_, rhs.head_);
    std::swap(tail_, rhs.tail_);
    std::swap(size_, rhs.size_);
  }

  template< class T >
  void List< T >::clear()
  {
    while (head_)
    {
      Node< T > *temp = head_;
      head_ = head_->next_;
      delete temp;
    }
    size_ = 0;
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  Iterator< T > List< T >::begin() const
  {
    return Iterator< T >(head_, this);
  }

  template< class T >
  Iterator< T > List< T >::end() const
  {
    return Iterator< T >(nullptr, this);
  }

  template< class T >
  ConstIterator< T > List< T >::cbegin() const
  {
    return ConstIterator< T >(head_, this);
  }

  template< class T >
  ConstIterator< T > List< T >::cend() const
  {
    return ConstIterator< T >(nullptr, this);
  }

  template< class T >
  Iterator< T > List< T >::find(Iterator< T > begin, Iterator< T > end, const T &val) const noexcept
  {
    for (auto iter = begin; iter != end; ++iter)
    {
      if (*iter == val)
      {
        return iter;
      }
    }
    return end;
  }

  template< class T >
  template< class C >
  Iterator< T > List< T >::find_if(Iterator< T > begin, Iterator< T > end, C condition) const noexcept
  {
    for (auto iter = begin; iter != end; ++iter)
    {
      if (condition(*iter))
      {
        return iter;
      }
    }
    return end;
  }

  template< class T >
  Iterator< T > List< T >::remove(Iterator< T > begin, Iterator< T > end, const T &val)
  {
    auto iter = find(begin, end, val);
    if (iter != end)
    {
      for (auto it = iter; ++it != end;)
      {
        if (*it != val)
        {
          *iter++ = std::move(*it);
        }
      }
    }
    return iter;
  }

  template< class T >
  template< class C >
  Iterator< T > List< T >::remove_if(Iterator< T > begin, Iterator< T > end, C condition)
  {
    auto iter = find_if(begin, end, condition);
    if (iter != end)
    {
      for (auto it = iter; ++it != end;)
      {
        if (!condition(*it))
        {
          *iter++ = std::move(*it);
        }
      }
    }
    return iter;
  }

  template< class T >
  void List< T >::splice(ConstIterator< T > pos, List< T > &other) noexcept
  {
    if (pos.node_ == head_)
    {
      other.tail_->next_ = head_;
      head_->prev_ = other.tail_;
      head_ = other.head_;
    }
    if ((--pos).node_ == tail_)
    {
      other.head_->prev_ = tail_;
      tail_->next_ = other.head_;
      tail_ = other.tail_;
    }
    pos.node_->prev_->next_ = other.head_;
    other.head_->prev_ = pos.node_->prev_;
    pos.node_->prev_ = other.tail_;
    other.tail_->next_ = pos.node_;
    other.head_ = nullptr;
    other.tail_ = nullptr;
    size_ += other.size_;
    other.size_ = 0;
  }

  template< class T >
  void List< T >::splice(ConstIterator< T > pos, List< T > &&other) noexcept
  {
    splice(pos, other);
  }

  template< class T >
  void List< T >::splice(ConstIterator< T > pos, List< T > &other, ConstIterator< T > it) noexcept
  {
    auto next = it->node_->next_;
    auto prev = it->node_->prev_;
    if (pos.node_ == head_)
    {
      it->node_->next_ = head_;
      head_->prev_ = it->node_;
      head_ = it->node_;
    }
    if ((--pos).node_ == tail_)
    {
      it->node_->prev_ = tail_;
      tail_->next_ = it->node_;
      tail_ = it->node_;
    }
    if (it.node_ == other.head_)
    {
      next->prev_ = nullptr;
      other.head_ = next;
    }
    if (it.node_ == other.tail_)
    {
      prev->next_ = nullptr;
      other.tail_ = prev;
    }
    next->prev_ = prev;
    prev->next_ = next;
    ++size_;
  }

  template< class T >
  void List< T >::splice(ConstIterator< T > pos, List< T > &&other, ConstIterator< T > it) noexcept
  {
    splice(pos, other, it);
  }

  template< class T >
  void List< T >::splice(ConstIterator< T > pos, List< T > &other, ConstIterator< T > first, ConstIterator< T > last) noexcept
  {
    for (auto iter = first; first != last; ++iter)
    {
      splice(pos, other, iter);
    }
  }

  template< class T >
  void List< T >::splice(ConstIterator< T > pos, List< T > &&other, ConstIterator< T > first, ConstIterator< T > last) noexcept
  {
    splice(pos, other, first, last);
  }

  template< class T >
  void List< T >::assign(size_t count, const T &val)
  {
    clear();
    try
    {
      for (size_t i = 0; i < count; ++i)
      {
        pushBack(val);
      }
    }
    catch (const std::bad_alloc &)
    {
      clear();
    }
  }

  template< class T >
  void List< T >::insert(ConstIterator< T > pos, const T &val)
  {
    if (pos.node_ == head_)
    {
      pushFront(val);
    }
    else if ((--pos).node_ == tail_)
    {
      pushBack(val);
    }
    else
    {
      ++pos;
      Node< T > *node = new Node< T >{ val };
      if (empty())
      {
        head_ = node;
        tail_ = node;
      }
      else
      {
        (--pos).node_->next_ = node;
        (++pos).node_->prev_ = node;
        node->next_ = pos.node_;
      }
      ++size_;
    }
  }

  template< class T >
  void List< T >::insert(ConstIterator< T > pos, T &&val)
  {
    insert(pos, val);
  }

  template< class T >
  void List< T >::insert(ConstIterator< T > pos, size_t count, const T &val)
  {
    for (size_t i = 0; i < count; ++i)
    {
      insert(pos, val);
    }
  }

  template< class T >
  Iterator< T > List< T >::erase(Iterator< T > pos) noexcept
  {
    Node< T > *node = nullptr;
    if (pos.node_ == head_)
    {
      head_ = head_->next;
      head_->prev = nullptr;
      node = head_;
    }
    else if (pos.node_ == tail_)
    {
      tail_ = tail_->prev;
      tail_->next = nullptr;
      node = tail_;
    }
    else
    {
      pos.node_->prev_->next_ = pos.node_->next_;
      pos.node_->next_->prev_ = pos.node_->prev_;
      node = pos.node_->next_;
    }
    delete pos.node_;
    --size_;
    return Iterator< T >{ node, this };
  }

  template< class T >
  Iterator< T > List< T >::erase(Iterator< T > first, Iterator< T > last) noexcept
  {
    for (auto it = first; first != last; ++it)
    {
      erase(it);
    }
  }

  template< class T >
  void List< T >::assign(Iterator< T > first, Iterator< T > last)
  {
    clear();
    for (auto it = first; first != last; ++it)
    {
      pushBack(it.node_->data);
    }
  }

  template< class T >
  void List< T >::assign(std::initializer_list< T > il)
  {
    List< T > copy{ il };
    swap(copy);
  }
}
#endif
