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
  class List
  {
  public:
    //using iterator = Iterator;
    class Iterator:
      public std::iterator< std::bidirectional_iterator_tag, T >
    {
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
      bool operator!=(const Iterator & rhs) const
      {
        return node != rhs.node;
      }
    private:
      ListNode< T > * node;
    };

    List();
    ~List();

    Iterator begin();
    Iterator end();

    T & front();
    T & back();

    bool empty() const;
    size_t size() const;

    void push_front(const T & value);
    void push_back(const T & value);
    void pop_front();
    //void pop_back();
    //void swap(List & rhs);
    void clear();
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

template <typename T>
savintsev::List<T>::~List()
{
  clear();
  delete dummy;
}

template< typename T >
typename savintsev::List< T >::Iterator savintsev::List< T >::begin()
{
  return Iterator(dummy->next);
}

template< typename T >
typename savintsev::List< T >::Iterator savintsev::List< T >::end()
{
  return Iterator(dummy);
}

template< typename T >
T & savintsev::List< T >::front()
{
  return *(dummy->next->data);
}

template< typename T >
T & savintsev::List< T >::back()
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
  ListNode< T > * new_node = new ListNode< T >(value, dummy, dummy->next);
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
void savintsev::List< T >::clear()
{
  while (!empty())
  {
    pop_front();
  }
}

#endif
