#ifndef LIST_HPP
#define LIST_HPP

#include "node.hpp"
#include "iterator.hpp"
#include "cIterator.hpp"
#include <initializer_list>
#include <cassert>

namespace aleksandrov
{
  template< typename T >
  class Iterator;
  template< typename T >
  class cIterator;

  template< typename T>
  class List
  {
  public:
    List();
    List(const List< T >&);
    List(List< T >&&) noexcept;
    List(size_t, const T&);
    template< class InputIt >
    List(InputIt, InputIt);
    List(std::initializer_list< T >);
    ~List();

    List& operator=(const List&);
    List& operator=(List&&) noexcept;
    List& operator=(std::initializer_list< T >);

    Iterator< T > begin() const noexcept;
    cIterator< T > cbegin() const noexcept;
    Iterator< T > end() const noexcept;
    cIterator< T > cend() const noexcept;
    T& front() noexcept;
    T& back() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;

    void pushFront(const T&);
    void pushFront(T&&);
    void pushBack(const T&);
    void pushBack(T&&);
    void popFront() noexcept;
    void popBack() noexcept;
    void swap(List< T >&) noexcept;
    void clear() noexcept;

    void remove(const T&);
    template< typename UnaryPredicate >
    void removeIf(UnaryPredicate);
    void splice(cIterator< T >, List< T >&);
    void assign(size_t, const T&);
    template< class InputIt >
    void assign(InputIt, InputIt);
    void assign(std::initializer_list< T >);

    Iterator< T > insertAfter(cIterator< T >, const T&);
    Iterator< T > insertAfter(cIterator< T >, T&&);
    Iterator< T > eraseAfter(Iterator< T >);
    Iterator< T > eraseAfter(Iterator< T >, Iterator< T >);

    bool operator==(const List< T >&) const;
    bool operator!=(const List< T >&) const;
  private:
    Node< T >* fake_;
    Node< T >* tail_;
  };

  template< typename T >
  List< T >::List():
    fake_(new Node< T >()),
    tail_(nullptr)
  {}

  template< typename T >
  List< T >::List(const List< T >& rhs):
    List()
  {
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
    {
      pushBack(*it);
    }
  }

  template< typename T >
  List< T >::List(List< T >&& rhs) noexcept:
    fake_(rhs.fake_),
    tail_(rhs.tail_)
  {
    rhs.fake_ = new Node< T >;
    rhs.tail_ = nullptr;
  }

  template< typename T >
  List< T >::List(size_t count, const T& value):
    List()
  {
    for (size_t i = 0; i < count; ++i)
    {
      pushBack(value);
    }
  }

  template< typename T >
  template< class InputIt >
  List< T >::List(InputIt first, InputIt last):
    List()
  {
    assign(first, last);
  }

  template< typename T >
  List< T >::List(std::initializer_list< T > ilist):
    List(ilist.begin(), ilist.end())
  {}

  template< typename T >
  List< T >::~List()
  {
    clear();
    delete fake_;
  }

  template< typename T >
  List< T >& List< T >::operator=(const List< T >& rhs)
  {
    List< T >* newList = new List< T >;
    clear();
    delete fake_;
    fake_ = newList->fake_;
    tail_ = newList->tail_;
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
    {
      newList->pushBack(*it);
    }
    return *newList;
  }

  template< typename T >
  List< T >& List< T >::operator=(List< T >&& rhs) noexcept
  {
    clear();
    fake_ = rhs.fake_;
    tail_ = rhs.tail_;
    rhs.fake_ = new Node< T >;
    rhs.tail_ = nullptr;
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(std::initializer_list< T > ilist)
  {
    List< T >* newList = new List< T >(ilist);
    clear();
    delete fake_;
    fake_ = newList->fake_;
    tail_ = newList->tail_;
    return *newList;
  }

  template< typename T >
  Iterator< T > List< T >::begin() const noexcept
  {
    return Iterator< T >(fake_->next_);
  }

  template< typename T >
  cIterator< T > List< T >::cbegin() const noexcept
  {
    return cIterator< T >(fake_->next_);
  }

  template< typename T >
  Iterator< T > List< T >::end() const noexcept
  {
    return empty() ? Iterator< T >(tail_) : Iterator< T >(tail_->next_);
  }

  template< typename T >
  cIterator< T > List< T >::cend() const noexcept
  {
    return empty() ? cIterator< T >(tail_) : cIterator< T >(tail_->next_);
  }

  template< typename T >
  T& List< T >::front() noexcept
  {
    assert(begin() != nullptr);
    return *begin();
  }

  template< typename T >
  T& List< T >::back() noexcept
  {
    assert(begin() != nullptr);
    return tail_->data_;
  }

  template< typename T >
  bool List< T >::empty() const noexcept
  {
    return size() == 0;
  }

  template< typename T >
  size_t List< T >::size() const noexcept
  {
    auto tempPtr = fake_->next_;
    if (!tempPtr)
    {
      return 0;
    }
    size_t size = 0;
    while (tempPtr != fake_)
    {
      ++size;
      tempPtr = tempPtr->next_;
    }
    return size;
  }

  template< typename T >
  void List< T >::pushFront(const T& value)
  {
    Node< T >* newNode = new Node< T >(value);
    bool wasEmpty = empty();
    newNode->next_ = fake_->next_;
    fake_->next_ = newNode;

    if (wasEmpty)
    {
      tail_ = newNode;
      newNode->next_ = fake_;
    }
  }

  template< typename T >
  void List< T >::pushFront(T&& value)
  {
    Node< T >* newNode = new Node< T >(std::move(value));
    bool wasEmpty = empty();
    newNode->next_ = fake_->next_;
    fake_->next_ = newNode;

    if (wasEmpty)
    {
      tail_ = newNode;
      newNode->next_ = fake_;
    }
  }

  template< typename T >
  void List< T >::pushBack(const T& value)
  {
    Node< T >* newNode = new Node< T >(value);
    newNode->next_ = fake_;

    if (!empty())
    {
      tail_->next_ = newNode;
    }
    else
    {
      fake_->next_ = newNode;
    }
    tail_ = newNode;
  }

  template< typename T >
  void List< T >::pushBack(T&& value)
  {
    Node< T >* newNode = new Node< T >(std::move(value));
    newNode->next_ = fake_;

    if (!empty())
    {
      tail_->next_ = newNode;
    }
    else
    {
      fake_->next_ = newNode;
    }
    tail_ = newNode;
  }

  template< typename T >
  void List< T >::popFront() noexcept
  {
    assert(!empty());
    auto tempPtr = fake_->next_;
    fake_->next_ = tempPtr->next_;
    if (tempPtr == tail_)
    {
      tail_ = nullptr;
    }
    delete tempPtr;
  }

  template< typename T >
  void List< T >::popBack() noexcept
  {
    assert(!empty());
    auto beforeTail = fake_->next_;
    while (beforeTail->next_ != tail_)
    {
      beforeTail = beforeTail->next_;
    }
    if (size() == 1)
    {
      fake_->next_ = nullptr;
    }
    else
    {
      beforeTail->next_ = fake_;
    }
    delete tail_;
  }

  template< typename T >
  void List< T >::swap(List< T >& other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(tail_, other.tail_);
  }

  template< typename T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      popFront();
    }
  }

  template< typename T >
  void List< T >::remove(const T& value)
  {
    const auto pred = [&value](const T& rhs)
    {
      return value == rhs;
    };
    removeIf(pred);
  }

  template< typename T >
  template< typename UnaryPredicate >
  void List< T >::removeIf(UnaryPredicate p)
  {
    assert(!empty());
    for (Iterator< T > beforeIt(fake_), it = begin(); it != fake_;)
    {
      if (p(*it))
      {
        it = eraseAfter(beforeIt);
      }
      else
      {
        beforeIt = it++;
      }
    }
  }

  template< typename T >
  void List< T >::splice(cIterator< T > pos, List< T >& other)
  {
    Node< T >* node = pos.node_;
    assert(node != nullptr);
    if (!other.empty())
    {
      Node< T >* beforePos = begin().node_;
      while (beforePos->next_ != node)
      {
        beforePos = beforePos->next_;
      }
      beforePos->next_ = other.fake_->next_;
      other.fake_->next_ = nullptr;
      Node< T >* temp = other.tail_;
      temp->next_ = node;
      other.tail_ = nullptr;
    }
  }

  template< typename T >
  void List< T >::assign(size_t count, const T& value)
  {
    clear();
    for (size_t i = 0; i < count; ++i)
    {
      pushBack(value);
    }
  }

  template< typename T >
  template< class InputIt >
  void List< T >::assign(InputIt first, InputIt last)
  {
    clear();
    for (auto it = first; it != last; ++it)
    {
      pushBack(*it);
    }
  }

  template< typename T >
  void List< T >::assign(std::initializer_list< T > ilist)
  {
    clear();
    for (auto it = ilist.begin(); it != ilist.end(); ++it)
    {
      pushBack(*it);
    }
  }

  template< typename T >
  Iterator< T > List< T >::insertAfter(cIterator< T > pos, const T& value)
  {
    Node< T >* newNode = new Node< T >;
    newNode->data_ = value;
    Node< T >* posPtr = pos.node_;
    newNode->next_ = posPtr->next_;
    posPtr->next_ = newNode;
    return Iterator< T >(posPtr);
  }

  template< typename T >
  Iterator< T > List< T >::insertAfter(cIterator< T > pos, T&& value)
  {
    Node< T >* newNode = new Node< T >;
    newNode->data_ = std::move(value);
    Node< T >* posPtr = pos.node_;
    newNode->next_ = posPtr->next_;
    posPtr->next_ = newNode;
    return Iterator< T >(posPtr);
  }

  template< typename T >
  Iterator< T > List< T >::eraseAfter(Iterator< T > pos)
  {
    assert(!empty());
    assert(pos.node_ != nullptr);
    assert(pos.node_ != tail_);
    Node< T >* posPtr = pos.node_;
    Node< T >* toErase = posPtr->next_;
    posPtr->next_ = toErase->next_;
    if (toErase == tail_)
    {
      tail_ = posPtr;
    }
    delete toErase;
    return Iterator< T >(posPtr->next_);
  }

  template< typename T >
  Iterator< T > List< T >::eraseAfter(Iterator< T > first, Iterator< T > last)
  {
    assert(first != last);
    auto it = first;
    for (; it != last; ++it)
    {
      eraseAfter(it);
    }
    return Iterator< T >(it.node_);
  }

  template< typename T >
  bool List< T >::operator==(const List< T >& rhs) const
  {
    if (size() != rhs.size())
    {
      return false;
    }
    auto it = begin();
    auto rhsIt = rhs.begin();
    while (it != end())
    {
      if (*it != *rhsIt)
      {
        return false;
      }
      ++it;
      ++rhsIt;
    }
    return true;
  }

  template< typename T >
  bool List< T >::operator!=(const List< T >& rhs) const
  {
    return !(*this == rhs);
  }
}

#endif

