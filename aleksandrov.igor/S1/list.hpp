#ifndef LIST_HPP
#define LIST_HPP

#include "node.hpp"
#include "iterator.hpp"
#include "const-iterator.hpp"
#include "list-utils.hpp"
#include <initializer_list>
#include <cassert>
#include <cstdlib>

namespace aleksandrov
{
  template< typename T >
  class Iterator;
  template< typename T >
  class ConstIterator;
  template< typename T >
  class Equalizer;

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
    ConstIterator< T > cbegin() const noexcept;
    Iterator< T > end() const noexcept;
    ConstIterator< T > cend() const noexcept;
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

    void remove(const T&) noexcept;
    template< typename UnaryPredicate >
    void removeIf(UnaryPredicate);
    void splice(ConstIterator< T >, List< T >&);
    void splice(ConstIterator< T >, List< T >&&);
    void splice(ConstIterator< T >, List< T >&, ConstIterator< T >);
    void splice(ConstIterator< T >, List< T >&&, ConstIterator< T >);
    void splice(ConstIterator< T >, List< T >&, ConstIterator< T >, ConstIterator< T >);
    void splice(ConstIterator< T >, List< T >&&, ConstIterator< T >, ConstIterator< T >);
    void assign(size_t, const T&);
    template< class InputIt >
    void assign(InputIt, InputIt);
    void assign(std::initializer_list< T >);

    Iterator< T > insertAfter(ConstIterator< T >, const T&);
    Iterator< T > insertAfter(ConstIterator< T >, T&&);
    Iterator< T > eraseAfter(ConstIterator< T >);
    Iterator< T > eraseAfter(ConstIterator< T >, ConstIterator< T >);

    void reverse() noexcept;

    bool operator==(const List< T >&) const noexcept;
    bool operator!=(const List< T >&) const noexcept;
    bool operator<(const List< T >&) const noexcept;
    bool operator<=(const List< T >&) const noexcept;
    bool operator>(const List< T >&) const noexcept;
    bool operator>=(const List< T >&) const noexcept;
  private:
    using Node = detail::Node< T >;
    Node* fake_;
    Node* tail_;
    size_t size_;
  };

  template< typename T >
  List< T >::List():
    fake_(static_cast< Node* >(malloc(sizeof(Node)))),
    tail_(nullptr),
    size_(0)
  {
    fake_->next = nullptr;
  }

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
    tail_(rhs.tail_),
    size_(rhs.size_)
  {
    rhs.fake_ = nullptr;
    rhs.tail_ = nullptr;
    rhs.size_ = 0;
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
    for (auto it = first; it != last; ++it)
    {
      pushBack(*it);
    }
  }

  template< typename T >
  List< T >::List(std::initializer_list< T > ilist):
    List(ilist.begin(), ilist.end())
  {}

  template< typename T >
  List< T >::~List()
  {
    clear();
    free(fake_);
  }

  template< typename T >
  List< T >& List< T >::operator=(const List< T >& rhs)
  {
    List< T > newList(rhs);
    swap(newList);
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(List< T >&& rhs) noexcept
  {
    List< T > newList(std::move(rhs));
    swap(newList);
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(std::initializer_list< T > ilist)
  {
    List< T > newList(ilist);
    swap(newList);
    return *this;
  }

  template< typename T >
  Iterator< T > List< T >::begin() const noexcept
  {
    assert(fake_);
    return Iterator< T >(fake_->next);
  }

  template< typename T >
  ConstIterator< T > List< T >::cbegin() const noexcept
  {
    assert(fake_);
    return ConstIterator< T >(fake_->next);
  }

  template< typename T >
  Iterator< T > List< T >::end() const noexcept
  {
    return empty() ? Iterator< T >(nullptr) : Iterator< T >(tail_->next);
  }

  template< typename T >
  ConstIterator< T > List< T >::cend() const noexcept
  {
    return empty() ? ConstIterator< T >(nullptr) : ConstIterator< T >(tail_->next);
  }

  template< typename T >
  T& List< T >::front() noexcept
  {
    assert(!empty());
    return *begin();
  }

  template< typename T >
  T& List< T >::back() noexcept
  {
    assert(!empty());
    return tail_->data;
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
  void List< T >::pushFront(const T& value)
  {
    Node* newNode = new Node(value);
    bool wasEmpty = empty();
    newNode->next = fake_->next;
    fake_->next = newNode;

    if (wasEmpty)
    {
      tail_ = newNode;
      newNode->next = fake_;
    }
    ++size_;
  }

  template< typename T >
  void List< T >::pushFront(T&& value)
  {
    Node* newNode = new Node(std::move(value));
    bool wasEmpty = empty();
    newNode->next = fake_->next;
    fake_->next = newNode;

    if (wasEmpty)
    {
      tail_ = newNode;
      newNode->next = fake_;
    }
    ++size_;
  }

  template< typename T >
  void List< T >::pushBack(const T& value)
  {
    Node* newNode = new Node(value);
    newNode->next = fake_;

    if (!empty())
    {
      tail_->next = newNode;
    }
    else
    {
      fake_->next = newNode;
    }
    tail_ = newNode;
    ++size_;
  }

  template< typename T >
  void List< T >::pushBack(T&& value)
  {
    Node* newNode = new Node(std::move(value));
    newNode->next = fake_;

    if (!empty())
    {
      tail_->next = newNode;
    }
    else
    {
      fake_->next = newNode;
    }
    tail_ = newNode;
    ++size_;
  }

  template< typename T >
  void List< T >::popFront() noexcept
  {
    auto tempPtr = fake_->next;
    fake_->next = tempPtr->next;
    if (tempPtr == tail_)
    {
      tail_ = nullptr;
    }
    delete tempPtr;
    --size_;
  }

  template< typename T >
  void List< T >::popBack() noexcept
  {
    auto beforeTail = fake_->next;
    if (!beforeTail)
    {
      return;
    }
    while (beforeTail->next != tail_)
    {
      beforeTail = beforeTail->next;
    }
    if (size_ == 1)
    {
      fake_->next = nullptr;
    }
    else
    {
      beforeTail->next = fake_;
    }
    delete tail_;
    --size_;
  }

  template< typename T >
  void List< T >::swap(List< T >& other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
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
  void List< T >::remove(const T& value) noexcept
  {
    removeIf(Equalizer< T >(value));
  }

  template< typename T >
  template< typename UnaryPredicate >
  void List< T >::removeIf(UnaryPredicate p)
  {
    for (ConstIterator< T > beforeIt(fake_), it = cbegin(); it != fake_;)
    {
      if (p(*it))
      {
        it = ConstIterator< T >(eraseAfter(beforeIt).node_);
      }
      else
      {
        beforeIt = it++;
      }
    }
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T >& other)
  {
    splice(pos, other, other.cbegin(), other.cend());
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T >&& other)
  {
    splice(pos, other);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T >& other, ConstIterator< T > it)
  {
    splice(pos, other, it, it->next);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T >&& other, ConstIterator< T > it)
  {
    splice(pos, other, it);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T >& other, ConstIterator< T > first, ConstIterator< T > last)
  {
    assert(last.node_ != first.node_);
    if (other.empty())
    {
      return;
    }
    size_t pieceSize = 1;
    Node* beforePos = fake_;
    while (beforePos->next != pos.node_)
    {
      beforePos = beforePos->next;
    }
    Node* beforeFirst = other.fake_;
    while (beforeFirst->next != first.node_)
    {
      beforeFirst = beforeFirst->next;
    }
    Node* beforeLast = first.node_;
    while (beforeLast->next != last.node_)
    {
      beforeLast = beforeLast->next;
      ++pieceSize;
    }

    beforePos->next = first.node_;
    if (beforeFirst == fake_ && beforeLast == tail_)
    {
      other.fake_->next = nullptr;
      other.tail_ = nullptr;
    }
    else if (beforeFirst == fake_)
    {
      beforeFirst->next = last.node_;
      beforeLast->next = pos.node_;
    }
    else if (beforeLast == tail_)
    {
      beforeFirst->next = other.fake_;
      beforeLast->next = pos.node_;
      other.tail_ = beforeFirst;
    }
    else
    {
      beforeFirst->next = last.node_;
      beforeLast->next = pos.node_;
    }
    size_ += pieceSize;
    other.size_ -= pieceSize;
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T >&& other, ConstIterator< T > first, ConstIterator< T > last)
  {
    splice(pos, other, first, last);
  }

  template< typename T >
  void List< T >::assign(size_t count, const T& value)
  {
    List< T > newList(count, value);
    swap(newList);
  }

  template< typename T >
  void List< T >::assign(std::initializer_list< T > ilist)
  {
    List< T > newList(ilist);
    swap(newList);
  }

  template< typename T >
  template< class InputIt >
  void List< T >::assign(InputIt first, InputIt last)
  {
    List< T > newList(first, last);
    swap(newList);
  }

  template< typename T >
  Iterator< T > List< T >::insertAfter(ConstIterator< T > pos, const T& value)
  {
    assert(pos != nullptr);
    Node* newNode = new Node(value);
    newNode->next = pos.node_->next;
    pos.node_->next = newNode;
    ++size_;
    return Iterator< T >(newNode);
  }

  template< typename T >
  Iterator< T > List< T >::insertAfter(ConstIterator< T > pos, T&& value)
  {
    assert(pos != nullptr);
    Node* newNode = new Node(std::move(value));
    newNode->next = pos.node_->next;
    pos.node_->next = newNode;
    ++size_;
    return Iterator< T >(newNode);
  }

  template< typename T >
  Iterator< T > List< T >::eraseAfter(ConstIterator< T > pos)
  {
    assert(pos != nullptr);
    assert(pos != tail_);
    Node* posPtr = pos.node_;
    Node* toErase = posPtr->next;
    posPtr->next = toErase->next;
    if (toErase == tail_)
    {
      tail_ = posPtr;
    }
    delete toErase;
    --size_;
    return Iterator< T >(posPtr->next);
  }

  template< typename T >
  Iterator< T > List< T >::eraseAfter(ConstIterator< T > first, ConstIterator< T > last)
  {
    assert(first != last);
    ConstIterator< T > it = first;
    for (; it != last; ++it)
    {
      eraseAfter(it);
    }
    return Iterator< T >(it.node_);
  }

  template< typename T >
  bool List< T >::operator==(const List< T >& rhs) const noexcept
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
  bool List< T >::operator!=(const List< T >& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< typename T >
  bool List< T >::operator<(const List< T >& rhs) const noexcept
  {
    return lexicographicalCompare(cbegin(), cend(), rhs.cbegin(), rhs.cend());
  }

  template< typename T >
  bool List< T >::operator<=(const List< T >& rhs) const noexcept
  {
    return !(*this > rhs);
  }

  template< typename T >
  bool List< T >::operator>(const List< T >& rhs) const noexcept
  {
    return !(*this < rhs || *this == rhs);
  }

  template< typename T >
  bool List< T >::operator>=(const List< T >& rhs) const noexcept
  {
    return !(*this < rhs);
  }

  template< typename T >
  void List< T >::reverse() noexcept
  {
    if (size() <= 1)
    {
      return;
    }
    Node* beforePos = fake_;
    Node* pos = fake_->next;
    Node* afterPos = pos->next;
    fake_->next = tail_;
    tail_ = pos;
    while (pos != fake_)
    {
      pos->next = beforePos;
      beforePos = pos;
      pos = afterPos;
      afterPos = afterPos->next;
    }
  }
}

#endif

