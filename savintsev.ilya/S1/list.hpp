#ifndef LIST_HPP
#define LIST_HPP
#include <iterator>
#include <cassert>

namespace savintsev
{
  template< typename T >
  struct ListNode
  {
    T * data;
    ListNode< T > * next;
    ListNode< T > * prev;
    ListNode(const T & value, ListNode * nextNode = nullptr, ListNode * prevNode = nullptr):
      data(new T(value)),
      next(nextNode),
      prev(prevNode)
    {}
    ListNode():
      data(nullptr),
      next(this),
      prev(this)
    {}
    ~ListNode()
    {
      delete data;
    }
  };

  template< typename T >
  class List;

  template< typename T >
  class Iterator:
    public std::iterator< std::bidirectional_iterator_tag, T >
  {
    template< typename U > friend class List;
  public:
    Iterator(ListNode< T > * rhs):
      node(rhs)
    {}
    T & operator*()
    {
      return *(node->data);
    }
    T * operator->()
    {
      return std::addressof(*(node->data));
    }
    Iterator & operator++()
    {
      node = node->next;
      return *this;
    }
    Iterator operator++(int)
    {
      Iterator result(*this);
      ++(*this);
      return result;
    }
    Iterator & operator--()
    {
      node = node->prev;
      return *this;
    }
    Iterator operator--(int)
    {
      Iterator result(*this);
      --(*this);
      return result;
    }
    bool operator!=(const Iterator & rhs) const
    {
      return node != rhs.node;
    }
    bool operator==(const Iterator & rhs) const
    {
      return node == rhs.node;
    }
  private:
    ListNode< T > * node;
  };

  template< typename T >
  class List
  {
  public:
    using iterator = Iterator< T >;

    List();
    ~List();
    List(const List & rhs);
    List(List && rhs);

    iterator begin() const;
    iterator end() const;

    T & front() const;
    T & back() const;

    bool empty() const;
    size_t size() const;

    void push_front(const T & value);
    void push_back(const T & value);
    void pop_front();
    void pop_back();
    void swap(List & rhs);
    void clear();

    void remove(const T & value);
    template< class predicate >
    void remove_if(predicate pred);
    void splice(iterator pos, List & rhs);
    void splice(iterator pos, List & rhs, iterator i);
    void splice(iterator pos, List & rhs, iterator first, iterator last);
    void assign(iterator first, iterator last);
    void assign(size_t n, const T & value);
  private:
    ListNode< T > * dummy;
    size_t list_size;
  };
}

template< typename T >
savintsev::List< T >::List():
  dummy(new ListNode< T >()),
  list_size(0)
{}

template< typename T >
savintsev::List< T >::~List()
{
  clear();
  delete dummy;
}

template< typename T >
savintsev::List< T >::List(const List & rhs):
  dummy(new ListNode< T >()),
  list_size(0)
{
  for (auto it = rhs.begin(); it != rhs.end(); ++it)
  {
    push_back(*it);
  }
}

template< typename T >
savintsev::List< T >::List(List && rhs):
  dummy(rhs.dummy),
  list_size(rhs.list_size)
{
  rhs.dummy = nullptr;
  rhs.list_size = 0;
}

template< typename T >
typename savintsev::List< T >::iterator savintsev::List< T >::begin() const
{
  return iterator(dummy->next);
}

template< typename T >
typename savintsev::List< T >::iterator savintsev::List< T >::end() const
{
  return iterator(dummy);
}

template< typename T >
T & savintsev::List< T >::front() const
{
  return *(dummy->next->data);
}

template< typename T >
T & savintsev::List< T >::back() const
{
  return *(dummy->prev->data);
}

template< typename T >
bool savintsev::List< T >::empty() const
{
  return list_size == 0;
}

template< typename T >
size_t savintsev::List< T >::size() const
{
  return list_size;
}

template< typename T >
void savintsev::List< T >::push_front(const T & value)
{
  ListNode< T > * new_node = new ListNode< T >(value, dummy->next, dummy);
  dummy->next->prev = new_node;
  dummy->next = new_node;
  ++list_size;
}

template< typename T >
void savintsev::List< T >::push_back(const T & value)
{
  ListNode< T > * new_node = new ListNode< T >(value, dummy, dummy->prev);
  dummy->prev->next = new_node;
  dummy->prev = new_node;
  ++list_size;
}

template< typename T >
void savintsev::List< T >::pop_front()
{
  assert(!empty());
  ListNode< T > * temp_front = dummy->next;
  dummy->next = temp_front->next;
  dummy->next->prev = dummy;
  delete temp_front;
  --list_size;
}

template< typename T >
void savintsev::List< T >::pop_back()
{
  assert(!empty());
  ListNode< T > * temp_back = dummy->prev;
  dummy->prev = temp_back->prev;
  dummy->prev->next = dummy;
  delete temp_back;
  --list_size;
}

template< typename T >
void savintsev::List< T >::swap(List & rhs)
{
  ListNode< T > * temp_next = dummy->next;
  ListNode< T > * temp_prev = dummy->prev;
  dummy->next = rhs.dummy->next;
  dummy->prev = rhs.dummy->prev;
  rhs.dummy->next = temp_next;
  rhs.dummy->prev = temp_prev;
  std::swap(list_size, rhs.list_size);
}

template< typename T >
void savintsev::List< T >::clear()
{
  while (!empty())
  {
    pop_front();
  }
}

template< typename T >
void savintsev::List< T >::remove(const T & value)
{
  for (auto it = begin(); it != end(); ++it)
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

template< typename T >
template< class predicate >
void savintsev::List< T >::remove_if(predicate pred)
{
  for (auto it = begin(); it != end(); ++it)
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

template< typename T >
void savintsev::List< T >::splice(iterator pos, List & rhs)
{
  list_size += rhs.list_size;
  pos.node->prev->next = rhs.dummy->next;
  rhs.dummy->next->prev = pos.node->prev;
  rhs.dummy->prev->next = pos.node;
  pos.node->prev = rhs.dummy->prev;
  ListNode< T > * temp = new ListNode< T >();
  delete rhs.dummy;
  rhs.dummy = temp;
  temp = nullptr;
  rhs.list_size = 0;
}

template< typename T >
void savintsev::List< T >::splice(iterator pos, List & rhs, iterator i)
{
  pos.node->prev->next = i.node;
  pos.node->prev = i.node;
  list_size++;
  i.node->prev->next = i.node->next;
  i.node->next->prev = i.node->prev;
  i.node->next = pos.node;
  i.node->prev = pos.node->prev->prev;
  rhs.list_size--;
}

template< typename T >
void savintsev::List< T >::splice(iterator pos, List & rhs, iterator first, iterator last)
{
  list_size += std::distance(first, last);
  rhs.list_size -= std::distance(first, last);
  last.node->prev->next = pos.node;
  pos.node->prev->next = first.node;
  first.node->prev->next = last.node;
  ListNode< T > * temp_first_prev = first.node->prev;
  first.node->prev = pos.node->prev;
  pos.node->prev = last.node->prev;
  last.node->prev = temp_first_prev;
}

template< typename T >
void savintsev::List< T >::assign(iterator first, iterator last)
{
  clear();
  for (auto it = first; it != last; ++it)
  {
    push_back(*it);
  }
}

template< typename T >
void savintsev::List< T >::assign(size_t n, const T & value)
{
  clear();
  for (size_t i = 0; i < n; ++i)
  {
    push_back(value);
  }
}

#endif
