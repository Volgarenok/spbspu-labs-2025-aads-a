#ifndef LIST_HPP
#define LIST_HPP

#include <cstdlib>
#include <cassert>
#include <initializer_list>
#include "node.hpp"
#include "iterator.hpp"
#include "const-iterator.hpp"
#include "list-utils.hpp"

namespace aleksandrov
{
  template< typename T >
  class Iterator;
  template< typename T >
  class ConstIterator;

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
    const T& front() const noexcept;
    T& back() noexcept;
    const T& back() const noexcept;

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
    void removeIf(UnaryPredicate) noexcept;

    void spliceAfter(ConstIterator< T >, List&) noexcept;
    void spliceAfter(ConstIterator< T >, List&&) noexcept;
    void spliceAfter(ConstIterator< T >, List&, ConstIterator< T >) noexcept;
    void spliceAfter(ConstIterator< T >, List&&, ConstIterator< T >) noexcept;
    void spliceAfter(ConstIterator< T >, List&, ConstIterator< T >, ConstIterator< T >) noexcept;
    void spliceAfter(ConstIterator< T >, List&&, ConstIterator< T >, ConstIterator< T >) noexcept;

    void assign(size_t, const T&);
    template< class InputIt >
    void assign(InputIt, InputIt);
    void assign(std::initializer_list< T >);

    Iterator< T > insertAfter(ConstIterator< T >, const T&);
    Iterator< T > insertAfter(ConstIterator< T >, T&&);
    Iterator< T > insertAfter(ConstIterator< T >, size_t, const T&);
    template< class InputIt >
    Iterator< T > insertAfter(ConstIterator< T >, InputIt, InputIt);
    Iterator< T > insertAfter(ConstIterator< T >, std::initializer_list< T >);

    Iterator< T > eraseAfter(ConstIterator< T >);
    Iterator< T > eraseAfter(ConstIterator< T >, ConstIterator< T >);

    void reverse() noexcept;
    template< class... Args >

    void emplaceBack(Args&&...);
    template< class... Args >
    void emplaceFront(Args&&...);
    template< class... Args >
    Iterator< T > emplaceAfter(ConstIterator< T >, Args&&...);

    void unique();
    template< class BinaryPredicate >
    void unique(BinaryPredicate);

    void merge(List&);
    void merge(List&&);
    template< class Compare >
    void merge(List&, Compare);
    template< class Compare >
    void merge(List&&, Compare);

    void sort();
    template< class Compare >
    void sort(Compare);

    bool operator==(const List&) const noexcept;
    bool operator!=(const List&) const noexcept;
    bool operator<(const List&) const noexcept;
    bool operator<=(const List&) const noexcept;
    bool operator>(const List&) const noexcept;
    bool operator>=(const List&) const noexcept;
  private:
    using Node = detail::Node< T >;
    Node* fake_;
    Node* tail_;
    size_t size_;

    template< typename U >
    void spliceAfterImpl(ConstIterator< T >, U&&) noexcept;
    template< typename U >
    void spliceAfterImpl(ConstIterator< T >, U&&, ConstIterator< T >) noexcept;
    template< typename U >
    void spliceAfterImpl(ConstIterator< T >, U&&, ConstIterator< T >, ConstIterator< T >) noexcept;

    template< typename U, class Compare >
    void mergeImpl(U&&, Compare);
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
      emplaceBack(*it);
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
      emplaceBack(value);
    }
  }

  template< typename T >
  template< class InputIt >
  List< T >::List(InputIt first, InputIt last):
    List()
  {
    for (auto it = first; it != last; ++it)
    {
      emplaceBack(*it);
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
  const T& List< T >::front() const noexcept
  {
    assert(!empty());
    return *cbegin();
  }

  template< typename T >
  T& List< T >::back() noexcept
  {
    assert(!empty());
    return tail_->data;
  }

  template< typename T >
  const T& List< T >::back() const noexcept
  {
    assert(!empty());
    return tail_->data;
  }

  template< typename T >
  bool List< T >::empty() const noexcept
  {
    return !size_;
  }

  template< typename T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  void List< T >::pushFront(const T& value)
  {
    emplaceFront(value);
  }

  template< typename T >
  void List< T >::pushFront(T&& value)
  {
    emplaceFront(std::move(value));
  }

  template< typename T >
  void List< T >::pushBack(const T& value)
  {
    emplaceBack(value);
  }

  template< typename T >
  void List< T >::pushBack(T&& value)
  {
    emplaceBack(std::move(value));
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
    if (empty())
    {
      return;
    }
    auto beforeTail = fake_->next;
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
    class Equalizer
    {
    public:
      Equalizer(const T& value):
        value_(value)
      {}
      bool operator()(const T& rhs)
      {
        return rhs == value_;
      }
    private:
      const T& value_;
    };
    removeIf(Equalizer(value));
  }

  template< typename T >
  template< typename UnaryPredicate >
  void List< T >::removeIf(UnaryPredicate p) noexcept
  {
    for (ConstIterator< T > beforeIt(fake_), it = cbegin(); it.node_ != fake_;)
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
  template< typename U >
  void List< T >::spliceAfterImpl(ConstIterator< T > pos, U&& other) noexcept
  {
    if (other.empty())
    {
      return;
    }
    other.tail_->next = pos.node_->next;
    pos.node_->next = other.fake_->next;
    if (pos.node_ == tail_)
    {
      tail_ = other.tail_;
    }
    size_ += other.size_;
    other.size_ = 0;
    other.fake_->next = nullptr;
    other.tail_ = nullptr;
  }

  template< typename T >
  void List< T >::spliceAfter(ConstIterator< T > pos, List& other) noexcept
  {
    spliceAfterImpl(pos, other);
  }

  template< typename T >
  void List< T >::spliceAfter(ConstIterator< T > pos, List&& other) noexcept
  {
    spliceAfterImpl(pos, std::move(other));
  }

  template< typename T >
  template< typename U >
  void List< T >::spliceAfterImpl(ConstIterator< T > pos, U&& other, ConstIterator< T > it) noexcept
  {
    assert(it.node_);
    assert(pos.node_);
    if (it.node_ == other.tail_ || !it.node_->next)
    {
      return;
    }
    Node* toMove = it.node_->next;
    it.node_->next = toMove->next;
    if (toMove == other.tail_)
    {
      other.tail_ = it.node_;
    }
    --other.size_;
    toMove->next = pos.node_->next;
    pos.node_->next = toMove;
    if (pos.node_ == tail_)
    {
      tail_ = toMove;
    }
    ++size_;
  }

  template< typename T >
  void List< T >::spliceAfter(ConstIterator< T > pos, List& other, ConstIterator< T > it) noexcept
  {
    spliceAfterImpl(pos, other, it);
  }

  template< typename T >
  void List< T >::spliceAfter(ConstIterator< T > pos, List< T >&& other, ConstIterator< T > it) noexcept
  {
    spliceAfterImpl(pos, std::move(other), it);
  }

  template< typename T >
  template< typename U >
  void List< T >::spliceAfterImpl(ConstIterator< T > pos, U&& other, ConstIterator< T > first, ConstIterator< T > last) noexcept
  {
   assert(first.node_ != other.tail_);
    if ((first == last && first != other.cend()) || first.node_->next == last.node_ || other.empty())
    {
      return;
    }
    if (first.node_->next->next == last.node_)
    {
      spliceAfter(pos, other, first);
      return;
    }
    size_t pieceSize = 0;
    Node* beforeLast = first.node_;
    while (beforeLast->next != last.node_)
    {
      beforeLast = beforeLast->next;
      ++pieceSize;
    }

    if (!pos.node_->next)
    {
      beforeLast->next = pos.node_;
    }
    else
    {
      beforeLast->next = pos.node_->next;
    }
    pos.node_->next = first.node_->next;
    if (first == last && first == other.cend())
    {
      other.fake_->next = nullptr;
      other.tail_ = nullptr;
    }
    else if (first == other.cend())
    {
      other.fake_->next = last.node_;
    }
    else if (last == other.cend())
    {
      first.node_->next = last.node_;
      other.tail_ = first.node_;
    }
    else
    {
      first.node_->next = last.node_;
    }
    if (pos.node_ == tail_)
    {
      tail_ = beforeLast;
    }
    size_ += pieceSize;
    other.size_ -= pieceSize;
  }

  template< typename T >
  void List< T >::spliceAfter(ConstIterator< T > pos, List& other, ConstIterator< T > first, ConstIterator< T > last) noexcept
  {
    spliceAfterImpl(pos, other, first, last);
  }

  template< typename T >
  void List< T >::spliceAfter(ConstIterator< T > pos, List< T >&& other, ConstIterator< T > first, ConstIterator< T > last) noexcept
  {
    spliceAfterImpl(pos, std::move(other), first, last);
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
    assert(first != last);
    List< T > newList(first, last);
    swap(newList);
  }

  template< typename T >
  Iterator< T > List< T >::insertAfter(ConstIterator< T > pos, const T& value)
  {
    return emplaceAfter(pos, value);
  }

  template< typename T >
  Iterator< T > List< T >::insertAfter(ConstIterator< T > pos, T&& value)
  {
    return emplaceAfter(pos, std::move(value));
  }

  template< typename T >
  Iterator< T > List< T >::insertAfter(ConstIterator< T > pos, size_t count, const T& value)
  {
    assert(pos.node_ != nullptr);
    if (!count)
    {
      return Iterator< T >(pos.node_);
    }
    auto lastInserted = emplaceAfter(pos, value);
    for (size_t i = 1; i < count; ++i)
    {
      lastInserted = emplaceAfter(pos, value);
    }
    return lastInserted;
  }

  template< typename T >
  template< class InputIt >
  Iterator< T > List< T >::insertAfter(ConstIterator< T > pos, InputIt first, InputIt last)
  {
    assert(pos.node_ != nullptr);
    if (first == last)
    {
      return Iterator< T >(pos.node_);
    }
    auto lastInserted = emplaceAfter(pos, *first);
    for (auto it = ++first; it != last; ++it)
    {
      lastInserted = emplaceAfter(pos, *it);
    }
    return lastInserted;
  }

  template< typename T >
  Iterator< T > List< T >::insertAfter(ConstIterator< T > pos, std::initializer_list< T > ilist)
  {
    assert(pos.node_ != nullptr);
    if (!ilist.size())
    {
      return Iterator< T >(pos.node_);
    }
    return insertAfter(pos, ilist.begin(), ilist.end());
  }

  template< typename T >
  Iterator< T > List< T >::eraseAfter(ConstIterator< T > pos)
  {
    assert(pos.node_ != nullptr);
    if (pos.node_ == tail_)
    {
      return end();
    }
    Node* toDelete = pos.node_->next;
    pos.node_->next = toDelete->next;
    if (toDelete == tail_)
    {
      tail_ = pos.node_;
    }
    delete toDelete;
    --size_;
    return Iterator< T >(pos.node_->next);
  }

  template< typename T >
  Iterator< T > List< T >::eraseAfter(ConstIterator< T > first, ConstIterator< T > last)
  {
    assert(first != last);
    auto it = first;
    for (; it != last; ++it)
    {
      eraseAfter(it);
    }
    return Iterator< T >(last.node_);
  }

  template< typename T >
  bool List< T >::operator==(const List< T >& rhs) const noexcept
  {
    if (size_ != rhs.size_)
    {
      return false;
    }
    auto it = cbegin();
    auto rhsIt = rhs.cbegin();
    while (it != cend())
    {
      if (*(it++) != *(rhsIt++))
      {
        return false;
      }
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
    return !(rhs < *this);
  }

  template< typename T >
  bool List< T >::operator>(const List< T >& rhs) const noexcept
  {
    return rhs < *this;
  }

  template< typename T >
  bool List< T >::operator>=(const List< T >& rhs) const noexcept
  {
    return !(*this < rhs);
  }

  template< typename T >
  void List< T >::reverse() noexcept
  {
    if (size_ <= 1)
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

  template< typename T >
  template< class... Args >
  void List< T >::emplaceBack(Args&&... args)
  {
    Node* newNode = new Node(std::forward< Args >(args)...);
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
  template< class... Args >
  void List< T >::emplaceFront(Args&&... args)
  {
    Node* newNode = new Node(std::forward< Args >(args)...);
    const bool wasEmpty = empty();
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
  template< class... Args >
  Iterator< T > List< T >::emplaceAfter(ConstIterator< T > pos, Args&&... args)
  {
    assert(pos.node_ != nullptr);
    Node* newNode = new Node(std::forward< Args >(args)...);
    newNode->next = pos.node_->next;
    pos.node_->next = newNode;
    ++size_;
    return Iterator< T >(newNode);
  }

  template< typename T >
  void List< T >::unique()
  {
    class Equalizer
    {
    public:
      bool operator()(const T& a, const T& b) const
      {
        return a == b;
      }
    };
    unique(Equalizer());
  }

  template< typename T >
  template< class BinaryPredicate >
  void List< T >::unique(BinaryPredicate p)
  {
    if (size_ <= 1)
    {
      return;
    }
    Node* pos = fake_->next;
    while (pos != fake_ && pos->next != fake_)
    {
      if (p(pos->data, pos->next->data))
      {
        Node* toDelete = pos->next;
        pos->next = toDelete->next;
        delete toDelete;
        --size_;
      }
      else
      {
        pos = pos->next;
      }
    }
    tail_ = (pos == fake_) ? nullptr : pos;
  }

  template< typename T >
  void List< T >::merge(List& other)
  {
    merge(other, std::less< T >());
  }

  template< typename T >
  void List< T >::merge(List&& other)
  {
    merge(std::move(other), std::less< T >());
  }

  template< typename T >
  template< typename U, class Compare >
  void List< T >::mergeImpl(U&& other, Compare comp)
  {
    if (other.empty())
    {
      return;
    }
    if (empty())
    {
      swap(other);
      return;
    }

    auto it = ConstIterator< T >(fake_);
    do
    {
      if (comp(other.front(), it.node_->next->data))
      {
        spliceAfter(it, other, other.cend());
      }
      else
      {
        ++it;
      }
    }
    while (it.node_ != tail_ && !other.empty());
    if (!other.empty())
    {
      spliceAfter(it, other);
    }
  }

  template< typename T >
  template< class Compare >
  void List< T >::merge(List& other, Compare comp)
  {
    mergeImpl(other, comp);
  }

  template< typename T >
  template< class Compare >
  void List< T >::merge(List&& other, Compare comp)
  {
    mergeImpl(std::move(other), comp);
  }

  template< typename T >
  void List< T >::sort()
  {
    sort(std::less< T >());
  }

  template< typename T >
  template< class Compare >
  void List< T >::sort(Compare comp)
  {
    if (size_ <= 1)
    {
      return;
    }
    for (auto pos1 = begin(); pos1 != end(); ++pos1)
    {
      auto minNode = pos1;
      auto pos2 = Iterator< T >(pos1.node_->next);
      for (; pos2 != end(); ++pos2)
      {
        if (comp(*pos2, *minNode))
        {
          minNode = pos2;
        }
      }
      std::swap(*pos1, *minNode);
    }
  }
}

#endif

