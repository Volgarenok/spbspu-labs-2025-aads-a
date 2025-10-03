#ifndef LIST_HPP
#define LIST_HPP
#include <stdexcept>
#include "iterator.hpp"
#include "constIterator.hpp"

namespace lebedev
{
  template< class T >
  struct List
  {
  public:
    List() noexcept;
    explicit List(size_t count, const T& value);
    List(const List& other);
    List(List&& other) noexcept;
    ~List();
    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;
    Iterator< T > begin() noexcept;
    constIterator< T > constBegin() const noexcept;
    Iterator< T > end() noexcept;
    constIterator< T > constEnd() const noexcept;
    const T& front() const;
    const T& back() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void push_back(const T& value);
    void push_front(const T& value);
    void pop_back();
    void pop_front();
    void clear() noexcept;
    void swap(List& other) noexcept;
  private:
    Node< T >* head_;
    Node< T >* tail_;
    size_t size_;
  };

  template< class T >
  List<T>::List() noexcept:
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< class T >
  List< T >::List(size_t count, const T& value):
    List()
  {
    for (size_t i = 0; i < count; i++)
    {
      push_back(value);
    }
  }

  template< class T >
  List< T >::List(const List& other):
    List()
  {
    for (constIterator< T > it = other.constBegin(); it != other.constEnd(); it++)
    {
      push_back(*it);
    }
  }

  template< class T >
  List< T >::List(List&& other) noexcept:
    head_(other.head_),
    tail_(other.tail_),
    size_(other.size_)
  {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  template< class T >
  List< T >::~List()
  {
    clear();
  }

  template< class T >
  List< T >& List< T >::operator=(const List& other)
  {
    if (this != std::addressof(other))
    {
      List tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template< class T >
  List< T >& List< T >::operator=(List&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      clear();
      head_ = other.head_;
      tail_ = other.tail_;
      size_ = other.size_;
      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  template< class T >
  Iterator< T > List< T >::begin() noexcept
  {
    return Iterator< T >(head_);
  }

  template< class T >
  constIterator< T > List< T >::constBegin() const noexcept
  {
    return constIterator< T >(head_);
  }

  template< class T >
  Iterator< T > List< T >::end() noexcept
  {
    if (!tail_)
    {
      return Iterator< T >();
    }
    return Iterator< T >(tail_->next);
  }

  template< class T >
  constIterator< T > List< T >::constEnd() const noexcept
  {
    if (!tail_)
    {
      return constIterator< T >();
    }
    return constIterator< T >(tail_->next);
  }

  template< class T >
  const T& List< T >::front() const
  {
    if (empty())
    {
      throw std::out_of_range("List is empty!");
    }
    return head_-> data;
  }

  template< class T >
  const T& List< T >::back() const
  {
    if (empty())
    {
      throw std::out_of_range("List is empty!");
    }
    return tail_-> data;
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  void List< T >::push_back(const T& value)
  {
    Node< T >* newNode = new Node< T >(value);
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

  template< class T >
  void List< T >::push_front(const T& value)
  {
    Node< T >* newNode = new Node< T >(value);
    if (!head_)
    {
      head_ = tail_ = newNode;
    }
    else
    {
      newNode->next = head_;
      head_->prev = newNode;
      head_ = newNode;
    }
    ++size_;
  }

  template< class T >
  void List< T >::pop_back()
  {
    if (empty())
    {
      throw std::out_of_range("List is empty!\n");
    }
    Node< T >* tmp = tail_;
    tail_ = tail_->prev;
    if (tail_)
    {
      tail_->next = nullptr;
    }
    else
    {
      head_ = nullptr;
    }
    delete tmp;
    --size_;
  }

  template< class T >
  void List< T >::pop_front()
  {
    if (empty())
    {
      throw std::out_of_range("List is empty!\n");
    }
    Node< T >* tmp = head_;
    head_ = head_->next;
    if (head_)
    {
      head_->prev = nullptr;
    }
    else
    {
      tail_ = nullptr;
    }
    delete tmp;
    --size_;
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    while (head_)
    {
      Node< T >* tmp = head_;
      head_ = head_->next;
      delete tmp;
    }
    tail_ = nullptr;
    size_ = 0;
  }

  template< class T >
  void List< T >::swap(List& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }
}
#endif
