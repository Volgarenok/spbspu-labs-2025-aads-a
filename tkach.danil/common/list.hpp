#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <initializer_list>
#include <utility>
#include "node.hpp"
#include "list_const_iterator.hpp"
#include "list_iterator.hpp"

namespace tkach
{
  template< typename T >
  class List
  {
  public:
    List();
    List(const List< T >& other);
    List(List< T >&& other) noexcept;
    List(size_t count, const T & data);
    template< class InputIt >
    List(InputIt first, InputIt last);
    List(std::initializer_list< T > in_list);
    ~List();
    List< T >& operator=(const List< T >& other);
    List< T >& operator=(List< T >&& other) noexcept;
    List< T >& operator=(std::initializer_list< T > in_list);
    ListIterator< T > begin() noexcept;
    CListIterator< T > cbegin() const noexcept;
    ListIterator< T > end() noexcept;
    CListIterator< T > cend() const noexcept;
    bool empty() const noexcept;
    T& front();
    const T& front() const;
    size_t size() const;
    void pushFront(T&& data);
    void pushFront(const T& data);
    void pushBack(T&& data);
    void pushBack(const T& data);
    void popFront();
    void spliceAfter(CListIterator< T > pos, List< T >& other);
    void spliceAfter(CListIterator< T > pos, List< T >&& other);
    void spliceAfter(CListIterator< T > pos, List< T >& other, CListIterator< T > first);
    void spliceAfter(CListIterator< T > pos, List< T >&& other, CListIterator< T > first);
    void spliceAfter(CListIterator< T > pos, List< T >& other, CListIterator< T > first, CListIterator< T > last);
    void spliceAfter(CListIterator< T > pos, List< T >&& other, CListIterator< T > first, CListIterator< T > last);
    void remove(const T& value);
    template< class UnaryPredicate >
    void removeIf(UnaryPredicate p);
    void assign(size_t count, const T& value);
    template< class InputIt >
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list< T > in_list);
    ListIterator< T > insertAfter(CListIterator < T > pos, const T& value);
    ListIterator< T > insertAfter(CListIterator < T > pos, T&& value);
    ListIterator< T > insertAfter(CListIterator < T > pos, size_t count, const T & data);
    template< class InputIt >
    ListIterator< T > insertAfter(CListIterator < T > pos, InputIt first, InputIt last);
    ListIterator< T > insertAfter(CListIterator < T > pos, std::initializer_list< T > in_list);
    ListIterator< T > eraseAfter(CListIterator < T > pos);
    ListIterator< T > eraseAfter(CListIterator < T > first, CListIterator < T > last);
    void clear();
    void swap(List< T >& other) noexcept;
  private:
    Node< T >* tail_;
    size_t size_;
    List< T > getList(const List< T >& other) const;
    List< T > getList(size_t count, const T & data) const;
    template< class... Args >
    void pushFt(Args&&... args);
    template< class... Args >
    void pushBk(Args&&... args);
    template< class... Args >
    ListIterator< T > insertAft(CListIterator< T > pos, Args&&... args);
    Node< T >* getHead() const noexcept;
  };

  template< typename T >
  Node< T >* List< T >::getHead() const noexcept
  {
    return tail_ ? tail_->next_ : nullptr;
  }

  template< typename T >
  List< T >::List():
    tail_(nullptr),
    size_(0)
  {}

  template< typename T >
  template< typename InputIt >
  List< T >::List(InputIt first, InputIt last):
    List()
  {
    if (first == last)
    {
      return;
    }
    try
    {
      while (first != last)
      {
        pushBack(*(first++));
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template< typename T >
  List< T >::List(std::initializer_list< T > in_list):
    List(in_list.begin(), in_list.end())
  {}

  template< typename T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  List< T > List< T >::getList(const List< T >& other) const
  {
    List< T > temp;
    if (other.empty())
    {
      return temp;
    }
    temp.pushBack(*(other.cbegin()));
    for (auto it = ++other.cbegin(); it != other.cend(); ++it)
    {
      temp.pushBack(*it);
    }
    return temp;
  }

  template< typename T >
  List< T > List< T >::getList(size_t count, const T & data) const
  {
    List< T > temp;
    for (size_t i = 0; i < count; ++i)
    {
      temp.pushFront(data);
    }
    return temp;
  }

  template< typename T >
  List< T >::List(const size_t count, const T & data):
    List(getList(count, data))
  {}

  template< typename T >
  ListIterator< T > List< T >::begin() noexcept
  {
    Node< T >* head = getHead();
    if (empty())
    {
      return ListIterator< T >(head, head, true);
    }
    else
    {
      return ListIterator< T >(head, head, false);
    }
  }

  template< typename T >
  ListIterator< T > List< T >::end() noexcept
  {
    Node< T >* head = getHead();
    return ListIterator< T >(head, head, true);
  }

  template< typename T >
  CListIterator< T > List< T >::cbegin() const noexcept
  {
    Node< T >* head = getHead();
    if (empty())
    {
      return CListIterator< T >(head, head, true);
    }
    else
    {
      return CListIterator< T >(head, head, false);
    }
  }

  template< typename T >
  CListIterator< T > List< T >::cend() const noexcept
  {
    Node< T >* head = getHead();
    return CListIterator< T >(head, head, true);
  }

  template< typename T >
  T& List< T >::front()
  {
    return const_cast< T& >(static_cast< const List< T >* >(this)->front());
  }

  template< typename T >
  const T& List< T >::front() const
  {
    return tail_->next_->data_;
  }

  template< typename T >
  size_t List< T >::size() const
  {
    return size_;
  }

  template< typename T >
  void List< T >::swap(List< T >& other) noexcept
  {
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

  template< typename T >
  void List< T >::pushFront(T&& data)
  {
    pushFt(std::move(data));
  }

  template< typename T >
  void List< T >::pushFront(const T& data)
  {
    pushFt(data);
  }

  template< typename T >
  void List< T >::pushBack(T&& data)
  {
    pushBk(std::move(data));
  }

  template< typename T >
  void List< T >::pushBack(const T& data)
  {
    pushBk(data);
  }

  template< typename T >
  List< T >::List(const List< T >& other):
    List(getList(other))
  {}

  template< typename T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    List< T > temp(other);
    swap(temp);
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(List< T >&& other) noexcept
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    List< T > temp(std::move(other));
    swap(temp);
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(std::initializer_list< T > in_list)
  {
    List< T > temp(std::move(in_list));
    swap(temp);
    return *this;
  }

  template< class T >
  template< class... Args >
  void List< T >::pushFt(Args&&... args)
  {
    Node< T >* new_node = new Node< T >{nullptr, std::forward< Args >(args)...};
    if (empty())
    {
      tail_ = new_node;
      tail_->next_ = tail_;
    }
    else
    {
      new_node->next_ = tail_->next_;
      tail_->next_ = new_node;
    }
    size_++;
  }

  template< class T >
  template< class... Args >
  void List< T >::pushBk(Args&&... args)
  {
    Node< T >* new_node = new Node< T >{nullptr, std::forward< Args >(args)...};
    if (empty())
    {
      tail_ = new_node;
      tail_->next_ = tail_;
    }
    else
    {
      new_node->next_ = tail_->next_;
      tail_->next_ = new_node;
      tail_ = new_node;
    }
    size_++;
  }

  template< typename T >
  void List< T >::popFront()
  {
    if (empty())
    {
      return;
    }
    eraseAfter(CListIterator< T >(tail_, getHead(), false));
  }

  template< typename T >
  void List< T >::clear()
  {
    while (tail_)
    {
      popFront();
    }
  }

  template< typename T >
  void List< T >::remove(const T& value)
  {
    removeIf(
      [&](const T& temp)
      {
        return temp == value;
      }
    );
  }

  template< typename T >
  template< class UnaryPredicate >
  void List< T >::removeIf(UnaryPredicate p)
  {
    if (empty())
    {
      return;
    }
    while (!empty() && p(tail_->next_->data_))
    {
      popFront();
      if (empty())
      {
          return;
      }
    }
    for (auto it = cbegin(); it != cend();)
    {
      if (p(*(std::next(it))))
      {
        eraseAfter(it);
      }
      else
      {
        it++;
      }
    }
  }

  template< typename T >
  List< T >::~List()
  {
    clear();
  }

  template< typename T >
  List< T >::List(List< T >&& other) noexcept:
    tail_(other.tail_),
    size_(other.size_)
  {
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  template< typename T >
  ListIterator< T > List< T >::eraseAfter(CListIterator< T > pos)
  {
    if (empty())
    {
      throw std::logic_error("No elements in list");
    }
    if (tail_->next_ == tail_)
    {
      delete tail_;
      tail_ = nullptr;
      size_--;
      return ListIterator< T >(nullptr, nullptr, true);
    }
    else
    {
      Node< T >* list_delete = pos.node_->next_;
      if (pos.node_->next_ == tail_)
      {
        tail_ = pos.node_;
      }
      pos.node_->next_ = list_delete->next_;
      delete list_delete;
      size_--;
      return ListIterator< T >(pos.node_->next_, getHead(), false);
    }
    return ListIterator< T >(nullptr, nullptr, true);
  }

  template< typename T >
  void List< T >::assign(size_t count, const T& value)
  {
    List< T > temp(count, value);
    swap(temp);
  }

  template< typename T >
  void List< T >::spliceAfter(CListIterator< T > pos, List< T >& other)
  {
    if (other.empty())
    {
      return;
    }
    Node< T >* temp = pos.node_;
    Node< T >* temp2 = temp->next_;
    if (temp == tail_)
    {
      tail_ = other.tail_;
    }
    temp->next_ = (other.tail_)->next_;
    other.tail_->next_ = temp2;
    size_ += other.size_;
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  template< typename T >
  void List< T >::spliceAfter(CListIterator< T > pos, List< T >&& other)
  {
    spliceAfter(pos, other);
  }

  template< typename T >
  void List< T >::spliceAfter(CListIterator< T > pos, List< T >& other, CListIterator< T > first)
  {
    spliceAfter(pos, other, first, std::next(first, 2));
  }

  template< typename T >
  void List< T >::spliceAfter(CListIterator< T > pos, List< T >&& other, CListIterator< T > first)
  {
    spliceAfter(pos, other, first, std::next(first, 2));
  }

  template< typename T >
  void List< T >::spliceAfter(CListIterator< T > pos, List< T >& other, CListIterator< T > first, CListIterator< T > last)
  {
    if (other.empty() || first.node_ == nullptr || last.node_ == nullptr || (std::next(first) == last))
    {
      return;
    }
    size_t splice_size = std::distance(first, last) - 1;
    Node< T >* temp = pos.node_;
    Node< T >* temp_first = first.node_;
    Node< T >* temp_last = last.node_;
    Node< T >* temp_next = temp->next_;
    if (first.node_ == (other.tail_)->next_ && last.node_ == (other.tail_)->next_)
    {
      size_ += other.size() - 1;
      other.size_ = 1;
    }
    else
    {
      size_ += splice_size;
      other.size_ -= splice_size;
    }
    if (last.node_ == (other.tail_)->next_)
    {
      other.tail_ = temp_first;
    }
    if (temp == tail_)
    {
      tail_ = temp_last;
    }
    temp->next_ = temp_first->next_;
    while (temp->next_ != temp_last)
    {
      temp = temp->next_;
    }
    temp->next_ = temp_next;
    temp_first->next_ = temp_last;
  }

  template< typename T >
  void List< T >::spliceAfter(CListIterator< T > pos, List< T >&& other, CListIterator< T > first, CListIterator< T > last)
  {
    spliceAfter(pos, other, first, last);
  }

  template< typename T >
  template< class... Args >
  ListIterator< T > List< T >::insertAft(CListIterator< T > pos, Args&&... args)
  {
    if (empty())
    {
      return ListIterator< T >(nullptr, nullptr, true);
    }
    Node< T >* temp = pos.node_;
    Node< T >* new_node = new Node< T >{temp->next_, std::forward< Args >(args)...};
    if (temp == tail_)
    {
      tail_ = new_node;
    }
    temp->next_ = new_node;
    size_++;
    return ListIterator< T >(new_node, getHead(), false);
  }

  template< typename T >
  ListIterator< T > List< T >::insertAfter(CListIterator< T > pos, T&& value)
  {
    return insertAft(pos, std::move(value));
  }

  template< typename T >
  ListIterator< T > List< T >::insertAfter(CListIterator< T > pos, const T& value)
  {
    return insertAft(pos, value);
  }

  template< typename T >
  template< class InputIt >
  ListIterator< T > List< T >::insertAfter(CListIterator < T > pos, InputIt first, InputIt last)
  {
    spliceAfter(pos, List< T >(first, last));
    size_t splice_size = std::distance(first, last);
    for (size_t i = 0; i < splice_size; ++i)
    {
      ++pos;
    }
    if (splice_size > 0)
    {
      return ListIterator< T >(pos.node_, getHead(), false);
    }
    else
    {
      return ListIterator< T >(pos.node_, getHead(), pos.is_past_the_end_);
    }
  }

  template< typename T >
  ListIterator< T > List< T >::insertAfter(CListIterator < T > pos, std::initializer_list< T > in_list)
  {
    return insertAfter(pos, in_list.begin(), in_list.end());
  }

  template< typename T >
  ListIterator< T > List< T >::insertAfter(CListIterator< T > pos, size_t count, const T & data)
  {
    spliceAfter(pos, List< T >(count, data));
    for (size_t i = 0; i < count; ++i)
    {
      ++pos;
    }
    if (count > 0)
    {
      return ListIterator< T >(pos.node_, getHead(), false);
    }
    else
    {
      return ListIterator< T >(pos.node_, getHead(), pos.is_past_the_end_);
    }
  }

  template< typename T >
  template< class InputIt >
  void List< T >::assign(InputIt first, InputIt last)
  {
    List< T > temp(first, last);
    swap(temp);
  }

  template< typename T >
  void List< T >::assign(std::initializer_list< T > in_list)
  {
    List< T > temp(in_list);
    swap(temp);
  }

  template< typename T >
  ListIterator< T > List< T >::eraseAfter(CListIterator< T > first, CListIterator< T > last)
  {
    while (std::next(first) != last)
    {
      eraseAfter(first);
    }
    return ListIterator< T >(last.node_, getHead(), last.is_past_the_end_);
  }
}

#endif
