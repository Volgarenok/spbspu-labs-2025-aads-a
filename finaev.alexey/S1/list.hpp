#ifndef LIST_HPP
#define LIST_HPP
#include <stdexcept>
#include "node.hpp"
#include "iterator.hpp"
#include "constIterator.hpp"

namespace finaev
{
  template< class T >
  struct List
  {
  public:
    List();
    List(const List< T >&);
    List< T >& operator=(const List< T >&);
    List(List&&) noexcept;
    List< T >& operator=(List< T >&&) noexcept;
    void swap(List& other) noexcept;
    bool isEmpty() const noexcept;
    void pushBack(const T& data);
    void pushFront(const T& data);
    listIterator< T > begin() noexcept;
    listIterator< T > end() noexcept;
    constListIterator< T > constBegin() const noexcept;
    constListIterator< T > constEnd() const noexcept;
    const T& front() const;
    const T& back() const;
    size_t size() const noexcept;
    void popBack();
    void popFront();
    void clear() noexcept;
    ~List();
  private:
    Node< T >* head_;
    Node< T >* tail_;
    size_t size_;
  };

  template< class T >
  List< T >::List():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< class T >
  List< T >::List(const List< T >& other):
    List()
  {
    for (constListIterator< T > i = other.constBegin(); i != other.constEnd(); ++i)
    {
      pushBack(*i);
    }
  }

  template< class T >
  void List< T >::swap(List& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

  template< class T >
  List< T >& List< T >::operator=(const List& other)
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    List< T > temp(other);
    swap(temp);
    return *this;
  }

  template< class T >
  List< T >::List(List&& other) noexcept
  {
    head_ = other.head_;
    tail_ = other.tail_;
    size_ = other.size_;
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  template< class T >
  List< T >& List< T >::operator=(List< T >&& other) noexcept
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    swap(other);
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
    return *this;
  }

  template< class T >
  void List< T >::pushBack(const T& data)
  {
    Node< T >* elem = new Node< T >(data);
    if (isEmpty())
    {
      head_ = elem;
      tail_ = elem;
    }
    else
    {
      tail_->next = elem;
      elem->prev = tail_;
      tail_ = elem;
    }
    ++size_;
  }

  template< class T >
  void List< T >::pushFront(const T& data)
  {
    Node< T >* elem = new Node< T >(data);
    if (isEmpty())
    {
      head_ = elem;
      tail_ = elem;
    }
    else
    {
      head_->prev = elem;
      elem->next = head_;
      head_ = elem;
    }
    ++size_;
  }

  template< class T >
  listIterator< T > List< T >::begin() noexcept
  {
    return listIterator< T >(head_);
  }

  template< class T >
  listIterator< T > List< T >::end() noexcept
  {
    if (!tail_)
    {
      return listIterator< T >();
    }
    return listIterator< T >(tail_->next);
  }

  template< class T >
  constListIterator< T > List< T >::constBegin() const noexcept
  {
    return constListIterator< T >(head_);
  }

  template< class T >
  constListIterator< T > List< T >::constEnd() const noexcept
  {
    if (!tail_)
    {
      return constListIterator< T >();
    }
    return constListIterator< T >(tail_->next);
  }

  template< class T >
  const T& List< T >::front() const
  {
    return head_->data;
  }

  template< class T >
  const T& List< T >::back() const
  {
    return tail_->data;
  }

  template< class T >
  bool List< T >::isEmpty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  void List< T >::popBack()
  {
    Node< T >* toDelete = tail_;
    if (tail_)
    {
      tail_ = tail_->prev;
    }
    tail_->next = nullptr;
    delete toDelete;
    --size_;
  }

  template< class T >
  void List< T >::popFront()
  {
    Node< T >* toDelete = head_;
    if (head_)
    {
      head_ = head_->next;
    }
    if (head_)
    {
      head_->prev = nullptr;
    }
    else
    {
      tail_ = nullptr;
    }
    delete toDelete;
    --size_;
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    while (head_)
    {
      Node< T >* temp = head_;
      head_ = head_->next;
      delete temp;
    }
    tail_ = nullptr;
    size_ = 0;
  }

  template< class T >
  List< T >::~List()
  {
    clear();
  }
}

#endif
