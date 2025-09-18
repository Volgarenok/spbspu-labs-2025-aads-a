#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <iterator>
#include <functional>
#include <stdexcept>
#include "list-node.hpp"
#include "list-iterators.hpp"
#include "list-constiterators.hpp"

namespace krylov
{
  template< typename T >
  class Iterator;

  template< typename T >
  class ConstIterator;

  template< typename T >
  class List
  {
  public:
    friend class Iterator< T >;
    friend class ConstIterator< T >;
    List();
    List(const List< T >& other);
    List(List< T >&& other) noexcept;
    List(size_t n, const T& value);
    ~List();
    List< T >& operator=(const List< T >& other);
    List< T >& operator=(List< T >&& other) noexcept;
    Iterator< T > begin() noexcept;
    Iterator< T > end() noexcept;
    ConstIterator< T > cbegin() const noexcept;
    ConstIterator< T > cend() const noexcept;
    void push_back(const T& value);
    void push_front(const T& value);
    void pop_back() noexcept;
    void pop_front() noexcept;
    T& front() noexcept;
    T& back() noexcept;
    const T& front() const noexcept;
    const T& back() const noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear() noexcept;
    void swap(List< T >& other) noexcept;
    void assign(size_t n, const T& value) noexcept;
    void remove(const T& value) noexcept;
    template < typename Predicate >
    void remove_if(Predicate pred) noexcept;
    void splice(ConstIterator< T > position, List< T >& other) noexcept;
    void splice(ConstIterator< T > position, List< T >&& other) noexcept;
    void splice(ConstIterator< T > position, List< T >& other, ConstIterator< T > it) noexcept;
    void splice(ConstIterator< T > position, List< T >&& other, ConstIterator< T > it) noexcept;
    void splice(ConstIterator< T > position, List< T >& other, ConstIterator< T > first, ConstIterator< T > last) noexcept;
    void splice(ConstIterator< T > position, List< T >&& other, ConstIterator< T > first, ConstIterator< T > last) noexcept;
  private:
    Node< T >* head_;
    Node< T >* tail_;
    size_t size_;
  };

  template< typename T >
  List< T >::List():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< typename T >
  List< T >::List(const List< T >& other):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    Node< T >* current = other.head_;
    try
    {
      while (current)
      {
        push_back(current->data_);
        current = current->next_;
      }
    }
    catch(const std::exception& e)
    {
      clear();
      throw;
    }
  }

  template< typename T >
  List< T >::List(List< T >&& other) noexcept:
    head_(other.head_),
    tail_(other.tail_),
    size_(other.size_)
  {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  template< typename T >
  List< T >::List(size_t n, const T& value):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    try
    {
      for (size_t i = 0; i < n; ++i)
      {
        push_back(value);
      }
    }
    catch (const std::bad_alloc& e)
    {
      clear();
      throw;
    }
  }

  template< typename T >
  List< T >::~List()
  {
    clear();
  }

  template< typename T >
  void List< T >::assign(size_t n, const T& value) noexcept
  {
    List< T > temp(n, value);
    if (temp.head_ != nullptr)
    {
      swap(temp);
    }
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > position, List< T >& other) noexcept
  {
    if (this == std::addressof(other) || other.empty())
    {
      return;
    }
    splice(position, other, other.cbegin(), other.cend());
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > position, List< T >&& other) noexcept
  {
    splice(position, other);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > position, List< T >& other, ConstIterator< T > it) noexcept
  {
    if (this == std::addressof(other) || it.current_ == nullptr)
    {
      return;
    }
    ConstIterator< T > next = it;
    ++next;
    splice(position, other, it, next);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > position, List< T >&& other, ConstIterator< T > it) noexcept
  {
    splice(position, other, it);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > position, List< T >& other, ConstIterator< T > first, ConstIterator< T > last) noexcept
  {
    if (this == std::addressof(other) || first == last)
    {
      return;
    }
    Node< T >* firstNode = first.current_;
    Node< T >* lastNode = last.current_ ? last.current_->prev_ : other.tail_;
    size_t count = 0;
    for (auto it = first; it != last; ++it)
    {
      ++count;
    }
    if (firstNode->prev_)
    {
      firstNode->prev_->next_ = last.current_;
    }
    else
    {
      other.head_ = last.current_;
    }
    if (last.current_)
    {
      last.current_->prev_ = firstNode->prev_;
    }
    else
    {
      other.tail_ = firstNode->prev_;
    }
    firstNode->prev_ = position.current_->prev_;
    lastNode->next_ = position.current_;
    if (position.current_->prev_)
    {
      position.current_->prev_->next_ = firstNode;
    }
    else
    {
      head_ = firstNode;
    }
    position.current_->prev_ = lastNode;
    size_ += count;
    other.size_ -= count;
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > position, List< T >&& other, ConstIterator< T > first, ConstIterator< T > last) noexcept
  {
    splice(position, other, first, last);
  }

  template< typename T >
  void List< T >::remove(const T& value) noexcept
  {
    remove_if(std::bind(std::equal_to< T >(), std::placeholders::_1, value));
  }

  template< typename T >
  template< typename Predicate >
  void List< T >::remove_if(Predicate pred) noexcept
  {
    for (auto it = begin(); it != end(); )
    {
      if (pred(*it))
      {
        Node< T >* nodeToDelete = it.current_;
        if (nodeToDelete->prev_)
        {
          nodeToDelete->prev_->next_ = nodeToDelete->next_;
        }
        else
        {
          head_ = nodeToDelete->next_;
        }
        if (nodeToDelete->next_)
        {
          nodeToDelete->next_->prev_ = nodeToDelete->prev_;
        }
        else
        {
          tail_ = nodeToDelete->prev_;
        }
        it = Iterator< T >(nodeToDelete->next_);
        delete nodeToDelete;
        --size_;
      }
      else
      {
        ++it;
      }
    }
  }

  template< typename T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    List< T > temp(other);
    swap(temp);
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(List< T >&& other) noexcept
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    clear();
    swap(other);
    return *this;
  }

  template< typename T >
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

  template< typename T >
  void List< T >::push_front(const T& value)
  {
    Node< T >* newNode = new Node< T >(value);
    if (!head_)
    {
      head_ = newNode;
      tail_ = newNode;
    }
    else
    {
      head_->prev_ = newNode;
      newNode->next_ = head_;
      head_ = newNode;
    }
    ++size_;
  }

  template< typename T >
  void List< T >::pop_back() noexcept
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
    --size_;
  }

  template< typename T >
  void List< T >::pop_front() noexcept
  {
    if (!head_)
    {
      return;
    }
    Node< T >* temp = head_;
    head_ = head_->next_;
    if (head_)
    {
      head_->prev_ = nullptr;
    }
    else
    {
      tail_ = nullptr;
    }
    delete temp;
    --size_;
  }

  template< typename T >
  T& List< T >::front() noexcept
  {
    return head_->data_;
  }

  template< typename T >
  T& List< T >::back() noexcept
  {
    return tail_->data_;
  }

  template< typename T >
  const T& List< T >::front() const noexcept
  {
    return head_->data_;
  }

  template< typename T >
  const T& List< T >::back() const noexcept
  {
    return tail_->data_;
  }

  template< typename T >
  Iterator< T > List< T >::begin() noexcept
  {
    return Iterator< T >(head_, this);
  }

  template< typename T >
  Iterator< T > List< T >::end() noexcept
  {
    return Iterator< T >(nullptr, this);
  }

  template< typename T >
  ConstIterator< T > List< T >::cbegin() const noexcept
  {
    return ConstIterator< T >(head_, this);
  }

  template< typename T >
  ConstIterator< T > List< T >::cend() const noexcept
  {
    return ConstIterator< T >(nullptr, this);
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

  template< typename T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_back();
    }
  }

  template< typename T >
  void List< T >::swap(List< T >& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }
}

#endif
