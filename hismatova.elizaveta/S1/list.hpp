#ifndef LIST_HPP
#define LIST_HPP
#include <stdexcept>
#include "node.hpp"
#include "iterator.hpp"

namespace hismatova
{
  template < typename T >
  struct List
  {
    ~List();
    List();
    List(const List< T >& num);
    Iterator< T > begin() const;
    Iterator< T > end() const;
    T& front();
    T& back();
    bool empty() const noexcept { return size_ == 0; }
    size_t size() const noexcept { return size_; }
    void push_front(const T& data);
    void pop_front();
    void swap(List< T >& num);
    void clear();
    void reverse();
  private:
    size_t size_;
    Node < T >* fakeNode;
  };
  template< typename T >
  List<T>::~List()
  {
    clear();
    delete[] reinterpret_cast< char* >(fakeNode);
  }
  template< typename T >
  T& List< T >::front()
  {
    if (empty()) throw std::out_of_range("list is empty");
    return fakeNode->next->data;
  }
  template< typename T>
  T& List< T >::back()
  {
    if (empty()) throw std::out_of_range("list is empty");
    Node< T >* current = fakeNode;
    while (current->next != fakeNode)
    {
      current = current->next;
    }
    return current->data;
  }
  template< typename T >
  Iterator< T > List< T >::begin() const
  {
    return Iterator< T >(fakeNode->next);
  }
  template< typename T >
  Iterator< T > List< T >::end() const
  {
    return Iterator< T >(fakeNode);
  }
  template< typename T >
  void List< T >::push_front(const T& data)
  {
    Node< T >* newNode = new Node< T >(data);
    newNode->next = fakeNode->next;
    fakeNode->next = newNode;
    size_++;
  }
  template< typename T >
  List< T >::List(const List< T >& num):
    List()
  {
    auto temp = num.begin();
    for (size_t i = 0; i < num.size_; i++)
    {
      push_front(*temp);
      temp++;
    }
    reverse();
  }
  template< typename T >
  void List< T >::clear()
  {
    while (!empty())
    {
      pop_front();
    }
  }
  template< class T >
  void List< T >::reverse()
  {
    if (fakeNode->next == fakeNode)
    {
      return;
    }
    Node< T >* current = fakeNode->next;
    Node< T >* prev = fakeNode;
    while (current != fakeNode)
    {
      Node< T >* next = current->next;
      current->next = prev;
      prev = current;
      current = next;
    }
    fakeNode->next->next = prev;
    fakeNode->next = prev;
  }
  template< typename T >
  List< T >::List():
    fakeNode(reinterpret_cast< Node< T >* >(new char[sizeof(Node< T >)])),
    size_(0)
  {
    fakeNode->next = fakeNode;
  }
  template< typename T >
  void List< T >::swap(List< T >& num)
  {
    std::swap(fakeNode, num.fakeNode);
    std::swap(size_, num.size_);
  }
  template < typename T >
  void List< T >::pop_front()
  {
    if (empty()) throw std::out_of_range("list is empty");
    Node< T >* toDelete = fakeNode->next;
    fakeNode->next = toDelete->next;
    delete toDelete;
    size_--;
  }
}

#endif
