#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <stdexcept>
#include <initializer_list>
#include "const_iterators.hpp"
#include "iterators.hpp"
#include "node.hpp"

namespace smirnov
{
  template< typename T >
  class List
  {
  public:
    List();
    ~List();
    List(const List< T > &);
    List(List &&);
    List< T > & operator=(const List< T > &);
    List< T > & operator=(List< T > &&);
    ConstIterator< T > cbegin() const noexcept;
    ConstIterator< T > cend() const noexcept;
    Iterator< T > begin() noexcept;
    Iterator< T > end() noexcept;
    T & front();
    const T & front() const;
    T & back();
    const T & back() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void push_front(const T &);
    void pop_front();
    void clear() noexcept;
    void swap(List &) noexcept;
  private:
    Node< T > * fake_;
    size_t size_;
  };

  template < typename T >
  List< T >::List():
    fake_(static_cast< Node< T > * >(operator new(sizeof(Node< T >)))),
    size_(0)
  {
    fake_->next = fake_;
  }

  template < typename T >
  List< T >::~List()
  {
    clear();
    operator delete(fake_);
  }

  template < typename T >
  List< T >::List(const List< T > & other):
    fake_(static_cast< Node< T > * >(operator new(sizeof(Node< T >)))),
    size_(0)
  {
    fake_->next = fake_;
    Node< T >* current = other.fake_->next;
    Node< T >* tail = fake_;
    while (current != other.fake_)
    {
      Node< T >* newNode = new Node< T >(current->data);
      tail->next = newNode;
      tail = newNode;
      current = current->next;
      size_++;
    }
    tail->next = fake_;
  }

  template < typename T >
  List< T >::List(List< T > && other):
    fake_(other.fake_),
    size_(other.size_)
  {
    other.fake_ = static_cast< Node< T > * >(operator new(sizeof(Node< T >)));
    other.fake_->next = other.fake_;
    other.size_ = 0;
  }

  template < typename T >
  List< T > & List< T >::operator=(const List< T > & other)
  {
    if (this != std::addressof(other))
    {
      List< T > temp(other);
      swap(temp);
    }
    return *this;
  }

  template < typename T >
  List< T > & List< T >::operator=(List< T > && other)
  {
    if (this != std::addressof(other))
    {
      clear();
      delete fake_;
      fake_ = other.fake_;
      size_ = other.size_;
      other.fake_ = static_cast< Node< T > * >(operator new(sizeof(Node< T >)));
      other.fake_->next = other.fake_;
      other.size_ = 0;
    }
    return *this;
  }

  template < typename T >
  ConstIterator< T > List< T >::cbegin() const noexcept
  {
    return ConstIterator< T >(fake_->next);
  }

  template < typename T >
  ConstIterator< T > List< T >::cend() const noexcept
  {
    return ConstIterator< T >(fake_);
  }

  template < typename T >
  T & List< T >::front()
  {
    assert(!empty());
    return fake_->next->data;
  }

  template < typename T >
  const T & List< T >::front() const
  {
    assert(!empty());
    return fake_->next->data;
  }

  template < typename T >
  T & List< T >::back()
  {
    assert(!empty());
    Node< T > * current = fake_->next;
    while (current->next != fake_)
    {
      current = current->next;
    }
    return current->data;
  }

  template < typename T >
  const T & List< T >::back() const
  {
    assert(!empty());
    Node< T > * current = fake_->next;
    while (current->next != fake_)
    {
      current = current->next;
    }
    return current->data;
  }

  template < typename T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < typename T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template < typename T >
  void List< T >::push_front(const T & value)
  {
    Node< T > * newNode = new Node< T >(value);
    newNode->next = fake_->next;
    fake_->next = newNode;
    ++size_;
  }

  template < typename T >
  void List< T >::pop_front()
  {
    assert(!empty());
    Node< T > * temp = fake_->next;
    fake_->next = temp->next;
    delete temp;
    --size_;
  }

  template < typename T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_front();
    }
  }

  template < typename T >
  void List< T >::swap(List & other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(size_, other.size_);
  }

  template < typename T >
  Iterator< T > List< T >::begin() noexcept
  {
    return Iterator< T >(fake_->next);
  }

  template < typename T >
  Iterator< T > List< T >::end() noexcept
  {
    return Iterator< T >(fake_);
  }
}
#endif
