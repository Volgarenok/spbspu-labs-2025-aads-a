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
      Node<T> * head_;
      Node<T> * tail_;
      size_t size_;
    public:
      class Iterator
      {
        private:
          Node<T> * current;
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
  head_(nullptr),
  tail_(nullptr),
  size_(0)
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
    if (!tail_)
    {
      head_ = tail_ = newNode;
    }
    else
    {
      tail_->next = newNode;
      newNode->prev = tail_;
      tail_ = newNode;
    }
    ++size_;
  }
  template<typename T>
  void BidirList<T>::pop_back()
  {
    if (!tail_)
    {
      return;
    }
    Node<T> * temp = tail_;
    tail_ = tail_->prev;
    if (tail_)
    {
      tail_->next = nullptr;
    }
    else
    {
      head_ = nullptr;
    }
    delete temp;
    --size_;
}
  template<typename T>
  void BidirList<T>::clear()
  {
    while (head_)
    {
      Node<T>* temp = head_;
      head_ = head_->next;
      delete temp;
    }
    tail_ = nullptr;
    size_ = 0;
}
  template<typename T>
  typename BidirList<T>::Iterator BidirList<T>::begin()
  {
    return Iterator(head_);
  }
  template<typename T>
  typename BidirList<T>::Iterator BidirList<T>::end()
  {
    return Iterator(nullptr);
  }
  template<typename T>
  T & BidirList<T>::front()
  {
    return head_->data;
  }
  template<typename T>
  T & BidirList<T>::back()
  {
    return tail_->data;
  }
  template<typename T>
  bool BidirList<T>::empty() const
  {
    return size_ == 0;
  }
  template<typename T>
  size_t BidirList<T>::size() const
  {
    return size;
  }
}
#endif
