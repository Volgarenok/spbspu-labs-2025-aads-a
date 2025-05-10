#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <stdexcept>
#include "const_iterators.hpp"
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
    List(List &&) noexcept;
    List< T > & operator=(const List< T > &);
    List< T > & operator=(List< T > &&) noexcept;
    ConstIterator< T > begin() const noexcept;
    ConstIterator< T > end() const noexcept;
    T & front();
    const T & front() const;
    T & back();
    const T & back() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void push_front(const T &);
    void push_back(const T &);
    void pop_front();
    void pop_back();
    void clear() noexcept;
    void swap(List &) noexcept;
  private:
    Node< T > * fake_;
    size_t size_;
  };

  template < typename T >
  List< T >::List():
    fake_(new Node< T >()),
    size_(0)
  {
    fake_->next = fake_;
  }

  template < typename T >
  List< T >::~List()
  {
    clear();
    delete fake_;
  }

  template < typename T >
  List< T >::List(const List< T > & other):
    List()
  {
    for (ConstIterator< T > it = other.begin(); it != other.end(); ++it)
    {
      push_back(*it);
    }
  }

  template < typename T >
  List< T >::List(List< T > && other) noexcept:
    fake_(other.fake_),
    size_(other.size_)
  {
    other.fake_ = new Node< T >();
    other.fake_->next = other.fake_;
    other.size_ = 0;
  }

  template < typename T >
  List< T > & List< T >::operator=(const List< T > & other)
  {
    if (this != &other)
    {
      List< T > temp(other);
      swap(temp);
    }
    return *this;
  }

  template < typename T >
  List< T > & List< T >::operator=(List< T > && other) noexcept
  {
    if (this != &other)
    {
      clear();
      delete fake_;
      fake_ = other.fake_;
      size_ = other.size_;
      other.fake_ = new Node< T >();
      other.fake_->next = other.fake_;
      other.size_ = 0;
    }
    return *this;
  }

  template < typename T >
  ConstIterator< T > List< T >::begin() const noexcept
  {
    return ConstIterator< T >(fake_->next);
  }

  template < typename T >
  ConstIterator< T > List< T >::end() const noexcept
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
  void List< T >::push_back(const T & value)
  {
    Node< T > * newNode = new Node< T >(value);
    Node< T > * current = fake_;
    while (current->next != fake_)
    {
      current = current->next;
    }
    current->next = newNode;
    newNode->next = fake_;
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
  void List< T >::pop_back()
  {
    assert(!empty());
    Node< T > * current = fake_;
    while (current->next->next != fake_)
    {
      current = current->next;
    }
    Node< T > * toDelete = current->next;
    current->next = fake_;
    delete toDelete;
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
}
#endif
