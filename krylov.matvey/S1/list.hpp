#ifndef LIST_HPP
#define LIST_HPP
#include <iostream>
#include <cstddef>
#include <iterator>
#include <stdexcept>

namespace krylov
{
  template< class T >
  class List;

  template < typename T >
  class Node
  {
  public:
    T data_;
    Node< T >* prev_;
    Node< T >* next_;
    Node(const T& value);
  };

  template < typename T >
  class Iterator
  {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    Iterator();
    Iterator(Node< T >* node);
    Iterator(Node< T >* node, const List< T >* list);
    T& operator*();
    T* operator->();
    Iterator& operator++();
    Iterator operator++(int);
    Iterator& operator--();
    Iterator operator--(int);
    bool operator!=(const Iterator& other) const;
    bool operator==(const Iterator& other) const;
  private:
    Node< T >* current_;
    const List< T >* list_;
    friend class List< T >;
  };

  template < typename T >
  class List
  {
  public:
    friend class Iterator< T >;
    List();
    ~List();
    List(const List< T >& other);
    Iterator< T > begin() const;
    Iterator< T > end() const;
    void push_back(const T& value);
    void pop_back();
    T& front();
    T& back();
    bool empty() const;
    size_t size() const;
    void clear();
    void swap(List< T >& other);

  private:
    Node< T >* head_;
    Node< T >* tail_;
    size_t size_;
  };

  template < typename T >
  Node< T >::Node(const T& value):
    data_(value),
    prev_(nullptr),
    next_(nullptr)
  {}

  template < typename T >
  List< T >::List():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template < typename T >
  List< T >::~List()
  {
    clear();
  }

  template < typename T >
  List< T >::List(const List< T >& other):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    Node< T >* current = other.head_;
    while (current)
    {
      push_back(current->data_);
      current = current->next_;
    }
  }

  template < typename T >
  Iterator< T >::Iterator(Node< T >* node):
    current_(node),
    list_(nullptr)
  {}

  template < typename T >
  Iterator< T >::Iterator(Node< T >* node, const List< T >* list):
    current_(node),
    list_(list)
  {}

  template < typename T >
  T& Iterator< T >::operator*()
  {
    return current_->data_;
  }

  template < typename T >
  T* Iterator< T >::operator->()
  {
    return &(current_->data_);
  }

  template < typename T >
  Iterator< T >& Iterator< T >::operator++()
  {
    if (current_ && current_->next_)
    {
      current_ = current_->next_;
    }
    else
    {
      current_ = nullptr;
    }
    return *this;
  }

  template < typename T >
  Iterator< T > Iterator< T >::operator++(int)
  {
    Iterator< T > temp = *this;
    ++(*this);
    return temp;
  }

  template < typename T >
  Iterator< T >& Iterator< T >::operator--()
  {
    if (!current_)
    {
      current_ = list_->tail_;
    }
    else
    {
      current_ = current_->prev_;
    }
    return *this;
  }

  template < typename T >
  Iterator< T > Iterator< T >::operator--(int)
  {
    Iterator< T > temp = *this;
    --(*this);
    return temp;
  }

  template < typename T >
  bool Iterator< T >::operator!=(const Iterator& other) const
  {
    return current_ != other.current_;
  }

  template < typename T >
  bool Iterator < T >::operator==(const Iterator& other) const
  {
    return current_ == other.current_;
  }

  template < typename T >
  Iterator< T > List< T >::begin() const
  {
    return Iterator< T >(head_);
  }

  template < typename T >
  Iterator< T > List< T >::end() const
  {
    return Iterator< T >(nullptr, this);
  }

  template < typename T >
  void List< T >::push_back(const T& value)
  {
    Node< T >* newNode = new Node< T >(value);
    if (!tail_)
    {
      head_ = newNode;
      tail_ = newNode;
    }
    else
    {
      tail_->next_ = newNode;
      newNode->prev_ = tail_;
      tail_ = newNode;
    }
    ++size_;
  }

  template < typename T >
  void List< T >::pop_back()
  {
    if (!tail_)
    {
      return;
    }
    Node< T >* temp = tail_;
    tail_ = tail_->prev_;
    if (tail_)
    {
      tail_->next_ = nullptr;
    }
    else
    {
      head_ = nullptr;
    }
    delete temp;
    temp = nullptr;
    --size_;
  }

  template < typename T >
  T& List< T >::front()
  {
    if (!head_)
    {
      throw std::out_of_range("List is empty");
    }
    return head_->data_;
  }

  template < typename T >
  T& List< T >::back()
  {
    if (!tail_)
    {
      throw std::out_of_range("List is empty");
    }
    return tail_->data_;
  }

  template < typename T >
  bool List< T >::empty() const
  {
    return size_ == 0;
  }

  template < typename T >
  size_t List< T >::size() const
  {
    return size_;
  }

  template < typename T >
  void List< T >::clear()
  {
    while (!empty())
    {
      pop_back();
    }
  }

  template < typename T >
  void List< T >::swap(List< T >& other)
  {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }
}

#endif
