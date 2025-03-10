#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <algorithm>
#include "iterator.hpp"
#include "citerator.hpp"
#include "nodelist.hpp"

namespace karnauhova
{
  template< typename T >
  class List
  {
  public:
    using Node = NodeList< T >;
    using Iterator = ListIterator< T >;
    using CIterator = ConstListIterator< T >;
    List();
    List(const List< T >& other);
    ~List();
    bool empty() const noexcept;
    size_t size() const noexcept;

    Iterator begin() const;
    Iterator end() const;
    CIterator cbegin() const;
    CIterator cend() const;

    T& front();
    T& back();
    void clear();
    void swap(List< T >& other);
    void push_front(const T& data);
    void pop_front();
    void reverse();
  private:
    Node* fake_;
    size_t size_;
  };

  template< class T >
  void List< T >::reverse()
  {
    if (fake_->next == fake_)
    {
        return;
    }
    NodeList< T >* next = fake_->next;
    NodeList< T >* last = fake_;
    while (next->next != fake_)
    {
        NodeList< T >* remember = next->next;
        next->next = last;
        last = next;
        next = remember;
    }
    next->next = last;
    fake_->next = next;
  }

  template< typename T >
  List< T >::List():
    fake_(reinterpret_cast< NodeList< T >* >(new char[sizeof(NodeList< T >)])),
    size_(0)
  {
    fake_->next = fake_;
  }

  template< typename T >
  void List< T >::swap(List< T >& other)
  {
    std::swap(fake_, other.fake_);
    std::swap(size_, other.size_);
  }

  template< typename T >
  void List< T >::pop_front()
  {
    fake_->next = fake_->next->next;
    if (fake_->next == fake_)
    {
      return;
    }
    delete fake_->next;
    size_--;
  }

  template< typename T >
  void List< T >::push_front(const T& data)
  {
    Node* new_element = new Node{data, fake_->next};
    fake_->next = new_element;
    size_++;
  }

  template< typename T >
  List< T >::List(const List< T >& other):
    List()
  {
    auto it = other.begin();
    for (size_t i = other.size(); i > 0; i--)
    {
      push_front(*it);
      it++;
    }
    reverse();
  }

  template< typename T >
  void List< T >::clear()
  {
    NodeList< T >* last = fake_->next;
    while (size_)
    {
      NodeList< T >* now = last->next;
      delete last;
      last = now;
      size_--;
    }
  }

  template< typename T >
  List< T >::~List()
  {
    clear();
    delete[] reinterpret_cast< char* >(fake_);
  }

  template< typename T >
  T& List< T >::front()
  {
    return fake_->next->data;
  }

  template< typename T >
  T& List< T >::back()
  {
    NodeList< T >* now = fake_;
    while (now->next != fake_)
    {
      now = now->next;
    }
    return now->data;
  }

  template< typename T >
  ListIterator< T > List< T >::begin() const
  {
    return ListIterator< T >(fake_->next);
  }

  template< typename T >
  ConstListIterator< T > List< T >::cbegin() const
  {
    return CIterator(fake_->next);
  }

  template< typename T >
  ConstListIterator< T > List< T >::cend() const
  {
    return CIterator(fake_);
  }

  template< typename T >
  ListIterator< T > List< T >::end() const
  {
    return ListIterator< T >(fake_);
  }

  template< typename T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }
}

#endif
