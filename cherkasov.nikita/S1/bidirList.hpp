#ifndef BIDIR_LIST_HPP
#define BIDIR_LIST_HPP
#include "node.hpp"
#include <cstddef>

namespace cherkasov
{
  template<typename T>
  class BidirList
  {
    private:
      Node<T> * head;
      Node<T> * tail;
      size_t size;
    public:
      class Iterator
      {
        private:
          Node<T>* current;
        public:
          Iterator(Node<T> * node):
          current(node)
          {}
          T & operator*();
          Iterator & operator++();
          Iterator & operator--();
          bool operator!=(const Iterator & other) const;
      };
    BidirList();
    ~BidirList();
    Iterator begin();
    Iterator end();
    T & front();
    T & back();
    bool empty() const;
    size_t size() const;
    void push_back(const T & data);
    void pop_back();
    void clear();
  };
  template<typename T>
  BidirList<T>::BidirList():
  head(nullptr),
  tail(nullptr),
  size(0)
  {}

  template<typename T>
  BidirList<T>::~BidirList()
  {
    clear();
  }
  template<typename T>
  T & BidirList<T>::Iterator::operator*()
  {
    return current->data;
  }
  template<typename T>
  typename BidirList<T>::Iterator & BidirList<T>::Iterator::operator++()
  {
    current = current->next;
    return *this;
  }
  template<typename T>
  typename BidirList<T>::Iterator & BidirList<T>::Iterator::operator--()
  {
    current = current->prev;
    return *this;
  }
  template<typename T>
  bool BidirList<T>::Iterator::operator!=(const Iterator & other) const
  {
    return current != other.current;
  }
  template<typename T>
  void BidirList<T>::push_back(const T& val)
  {
    Node<T>* newNode = new Node<T>(val);
    if (!tail)
    {
      head = tail = newNode;
    }
    else
    {
      tail->next = newNode;
      newNode->prev = tail;
      tail = newNode;
    }
    ++size;
  }
  template<typename T>
  void BidirList<T>::pop_back()
  {
    if (!tail)
    {
      return;
    }
    Node<T> * temp = tail;
    tail = tail->prev;
    if (tail)
    {
      tail->next = nullptr;
    }
    else
    {
      head = nullptr;
    }
    delete temp;
    --size;
}

  template<typename T>
  void BidirList<T>::clear()
  {
    while (head)
    {
      Node<T>* temp = head;
      head = head->next;
      delete temp;
    }
    tail = nullptr;
    size = 0;
}
  template<typename T>
  typename BidirList<T>::Iterator BidirList<T>::begin()
  {
    return Iterator(head);
  }
  template<typename T>
  typename BidirList<T>::Iterator BidirList<T>::end()
  {
    return Iterator(nullptr);
  }
  template<typename T>
  T & BidirList<T>::front()
  {
    return head->data;
  }
  template<typename T>
  T & BidirList<T>::back()
  {
    return tail->data;
  }
  template<typename T>
  bool BidirList<T>::empty() const
  {
    return size == 0;
  }
  template<typename T>
  size_t BidirList<T>::size() const
  {
    return size;
  }
}
#endif
