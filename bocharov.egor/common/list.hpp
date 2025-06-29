#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>
#include <initializer_list>
#include <cassert>
#include "listNode.hpp"
#include "listIterator.hpp"

namespace bocharov
{
  template< typename T >
  class List
  {
  public:
    using iterator = Iterator< T, false >;
    using const_iterator = Iterator< T, true >;

    List() noexcept;
    List(const List& rhs);
    List(List&& rhs) noexcept;
    List(std::size_t count, const T& value);
    template< class InputIt >
    List(InputIt first, InputIt last);
    List(std::initializer_list< T > init);
    ~List();

    List& operator=(const List& rhs);
    List& operator=(List&& rhs) noexcept;
    List& operator=(std::initializer_list< T > ilist);

    void assign(std::size_t count, const T& value);
    template< class InputIt >
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list< T > ilist);

    T& front() noexcept;
    const T& front() const noexcept;
    T& back() noexcept;
    const T& back() const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void clear() noexcept;
    iterator insert_after(const_iterator pos, const T& value);
    iterator insert_after(const_iterator pos, T&& value);
    iterator insert_after(const_iterator pos, std::size_t count, const T& value);
    template< class InputIt >
    iterator insert_after(const_iterator pos, InputIt first, InputIt last);
    iterator insert_after(const_iterator pos, std::initializer_list< T > ilist);
    iterator erase_after(const_iterator pos) noexcept;
    iterator erase_after(const_iterator first, const_iterator last) noexcept;

    void push_front(const T& value);
    void push_front(T&& value);
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_front() noexcept;
    void pop_back() noexcept;
    void swap(List& other) noexcept;

    void splice_after(const_iterator pos, List& other);
    void splice_after(const_iterator pos, List&& other) noexcept;
    void splice_after(const_iterator pos, List& other, const_iterator it) noexcept;
    void splice_after(const_iterator pos, List& other, const_iterator first, const_iterator last) noexcept;

    void remove(const T& value) noexcept;
    template< class UnaryPredicate >
    void remove_if(UnaryPredicate condition);

  private:
    Node< T >* head_;
    Node< T >* tail_;
    std::size_t size_;

    void insertFirst(Node< T >* node) noexcept;
    void insertAfter(Node< T >* after, Node< T >* node) noexcept;
  };

  template< typename T >
  List< T >::List() noexcept:
    head_(new Node< T >(T())),
    tail_(head_),
    size_(0)
  {
    head_->next_ = head_;
  }

  template< typename T >
  List< T >::List(const List& rhs):
    List()
  {
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
    {
      push_back(*it);
    }
  }

  template< typename T >
  List< T >::List(List&& rhs) noexcept:
    head_(rhs.head_),
    tail_(rhs.tail_),
    size_(rhs.size_)
  {
    rhs.head_ = new Node< T >(T());
    rhs.head_->next_ = rhs.head_;
    rhs.tail_ = rhs.head_;
    rhs.size_ = 0;
  }

  template< typename T >
  List< T >::List(std::size_t count, const T& value):
    List()
  {
    for (std::size_t i = 0; i < count; ++i)
    {
        push_back(value);
    }
  }

  template< typename T >
  template< class InputIt >
  List< T >::List(InputIt first, InputIt last):
    List()
  {
    for (; first != last; ++first)
    {
      push_back(*first);
    }
  }

  template< typename T >
  List< T >::List(std::initializer_list< T > init):
    List(init.begin(), init.end())
  {}

  template< typename T >
  List< T >::~List()
  {
    clear();
    delete head_;
  }

  template< typename T >
  List<T>& List< T >::operator=(const List& rhs)
  {
    List< T > copied_rhs(rhs);
    swap(copied_rhs);
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(List&& rhs) noexcept
  {
    List< T > copied_rhs(std::move(rhs));
    swap(copied_rhs);
    return *this;
  }
 
  template< typename T >
  List< T >& List< T >::operator=(std::initializer_list< T > ilist)
  {
    List temp(ilist);
    swap(temp);
    return *this;
  }

  template< typename T >
  void List< T >::assign(std::size_t count, const T& value)
  {
    List temp(count, value);
    swap(temp);
  }

  template< typename T >
  template< class InputIt >
  void List< T >::assign(InputIt first, InputIt last)
  {
    List temp(first, last);
    swap(temp);
  }

  template< typename T >
  void List< T >::assign(std::initializer_list< T > ilist)
  {
    assign(ilist.begin(), ilist.end());
  }

  template< typename T >
  T& List< T >::front() noexcept
  {
    assert(!empty() && "element access to empty list");
    return head_->next_->data_;
  }

  template< typename T >
  const T& List< T >::front() const noexcept
  {
    assert(!empty() && "element access to empty list");
    return head_->next_->data_;
  }

  template< typename T >
  T& List< T >::back() noexcept
  {
    assert(!empty() && "element access to empty list");
    return tail_->data_;
  }

  template< typename T >
  const T& List< T >::back() const noexcept
  {
    assert(!empty() && "element access to empty list");
    return tail_->data_;
  }

  template< typename T >
  typename List< T >::iterator List< T >::begin() noexcept
  {
    assert(tail_ && "iterator to empty list");
    return iterator(head_->next_);
  }

  template< typename T >
  typename List< T >::const_iterator List< T >::begin() const noexcept
  {
    assert(tail_ && "iterator to empty list");
    return const_iterator(head_->next_);
  }

  template< typename T >
  typename List< T >::const_iterator List< T >::cbegin() const noexcept
  {
    assert(tail_ && "iterator to empty list");
    return const_iterator(head_->next_);
  }

  template< typename T >
  typename List< T >::iterator List< T >::end() noexcept
  {
    assert(tail_ && "iterator to empty list");
    return iterator(head_);
  }

  template< typename T >
  typename List< T >::const_iterator List< T >::end() const noexcept
  {
    assert(tail_ && "iterator to empty list");
    return const_iterator(head_);
  }

  template< typename T >
  typename List< T >::const_iterator List< T >::cend() const noexcept
  {
    assert(tail_ && "iterator to empty list");
    return const_iterator(head_);
  }

  template< typename T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  std::size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_front();
    }
  }

  template< typename T >
  void List< T >::insertFirst(Node< T >* node) noexcept
  {
    node->next_ = head_->next_;
    head_->next_ = node;
    if (tail_ == head_)
    {
      tail_ = node;
    }
    ++size_;
  }

  template< typename T >
  void List< T >::insertAfter(Node< T >* after, Node< T >* node) noexcept
  {
    node->next_ = after->next_;
    after->next_ = node;
    if (after == tail_)
    {
      tail_ = node;
    }
    ++size_;
  }

  template< typename T >
  void List< T >::push_back(const T& value)
  {
    Node< T >* node = new Node< T >(value);
    insertAfter(tail_, node);
  }

  template< typename T >
  void List< T >::push_back(T&& value)
  {
    Node< T >* node = new Node< T >(std::move(value));
    insertAfter(tail_, node);
  }

  template< typename T >
  void List< T >::push_front(const T& value)
  {
    Node< T >* node = new Node< T >(value);
    insertFirst(node);
  }

  template< typename T >
  void List< T >::push_front(T&& value)
  {
    Node< T >* node = new Node< T >(std::move(value));
    insertFirst(node);
  }

  template< typename T >
  void List< T >::pop_back() noexcept
  {
    assert(!empty() && "pop_back() to empty list");
    if (empty()) return;
    if (head_->next_ == tail_)
    {
      pop_front();
      return;
    }
    Node< T >* prev = head_;
    while (prev->next_ != tail_)
    {
      prev = prev->next_;
    }
    Node< T >* toDelete = tail_;
    prev->next_ = head_;
    tail_ = prev;
    delete toDelete;
    --size_;
  }

  template< typename T >
  void List< T >::pop_front() noexcept
  {
    assert(!empty() && "pop_front() to empty list");
    if (empty()) return;
    Node< T >* toDelete = head_->next_;
    head_->next_ = toDelete->next_;
    if (toDelete == tail_)
    {
      tail_ = head_;
    }
    delete toDelete;
    --size_;
  }

  template< typename T >
  void List< T >::swap(List& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

  template< typename T >
  typename List< T >::iterator List< T >::insert_after(const_iterator pos, const T& value)
  {
    Node< T >* node = new Node< T >(value);
    insertAfter(pos.node_, node);
    return iterator(node);
  }

  template< typename T >
  typename List< T >::iterator List< T >::insert_after(const_iterator pos, T&& value)
  {
    Node< T >* node = new Node< T >(std::move(value));
    insertAfter(pos.node_, node);
    return iterator(node);
  }

  template< typename T >
  typename List< T >::iterator List< T >::insert_after(const_iterator pos, std::size_t count, const T& value)
  {
    if (count == 0)
    {
      return iterator(pos.node_->next_);
    }
    iterator first = insert_after(pos, value);
    iterator current = first;
    for (std::size_t i = 1; i < count; ++i)
    {
      current = insert_after(current, value);
    }
    return current;
  }

  template< typename T >
  template< class InputIt >
  typename List< T >::iterator List< T >::insert_after(const_iterator pos, InputIt first, InputIt last)
  {
    if (first == last)
    {
      return iterator(pos.node_->next_);
    }
    iterator current = insert_after(pos, *first);
    ++first;
    iterator res = current;
    for (; first != last; ++first)
    {
      res = insert_after(current, *first);
      current = res;
    }
    return res;
  }

  template< typename T >
  typename List< T >::iterator List< T >::insert_after(const_iterator pos, std::initializer_list< T > ilist)
  {
    return insert_after(pos, ilist.begin(), ilist.end());
  }

  template< typename T >
  typename List< T >::iterator List< T >::erase_after(const_iterator pos) noexcept
  {
    assert(!empty() && "empty list erasing");
    if (pos.node_->next_ == head_)
    {
      return end();
    }
    Node< T >* toDelete = pos.node_->next_;
    pos.node_->next_ = toDelete->next_;
    if (toDelete == tail_)
    {
      tail_ = pos.node_;
    }
    delete toDelete;
    --size_;
    return iterator(pos.node_->next_);
  }

  template< typename T >
  typename List< T >::iterator List< T >::erase_after(const_iterator first, const_iterator last) noexcept
  {
    assert(!empty() && "empty list erasing");
    if (first.node_->next_ == last.node_)
    {
      return iterator(last.node_);
    }
    Node< T >* current = first.node_->next_;
    Node< T >* stop = last.node_;
    while (current != stop)
    {
      Node< T >* next = current->next_;
      if (current == tail_)
      {
        tail_ = first.node_;
      }
      delete current;
      --size_;
      current = next;
    }
    first.node_->next_ = stop;
    return iterator(stop);
  }

  template< typename T >
  void List< T >::splice_after(const_iterator pos, List& other)
  {
    if (other.empty())
    {
      return;
    }
    Node< T >* otherFirst = other.head_->next_;
    Node< T >* otherLast = other.tail_;
    std::size_t otherSize = other.size_;
    other.head_->next_ = other.head_;
    other.tail_ = other.head_;
    other.size_ = 0;

    otherLast->next_ = pos.node_->next_;
    pos.node_->next_ = otherFirst;
    if (pos.node_ == tail_)
    {
      tail_ = otherLast;
    }
    size_ += otherSize;
  }

  template< typename T >
  void List< T >::splice_after(const_iterator pos, List&& other) noexcept
  {
    splice_after(pos, other);
  }

  template< typename T >
  void List< T >::splice_after(const_iterator pos, List& other, const_iterator it) noexcept
  {
    if (it.node_->next_ == other.head_)
    {
      return;
    }
    Node< T >* node = it.node_->next_;
    it.node_->next_ = node->next_;
    if (node == other.tail_)
    {
      other.tail_ = it.node_;
    }
    --other.size_;

    insertAfter(pos.node_, node);
  }

  template< typename T >
  void List< T >::splice_after(const_iterator pos, List& other, const_iterator first, const_iterator last) noexcept
  {
    if (first.node_->next_ == last.node_)
    {
      return;
    }
    Node< T >* firstToTransfer = first.node_->next_;
    Node< T >* lastToTransfer = last.node_;
    std::size_t count = 0;
    for (auto it = first; it != last; ++it)
    {
      ++count;
    }

    first.node_->next_ = last.node_;
    if (last.node_ == other.head_)
    {
      other.tail_ = first.node_;
    }
    other.size_ -= count;

    lastToTransfer = firstToTransfer;
    while (lastToTransfer->next_ != last.node_)
    {
      lastToTransfer = lastToTransfer->next_;
    }
    lastToTransfer->next_ = pos.node_->next_;
    pos.node_->next_ = firstToTransfer;
    if (pos.node_ == tail_)
    {
      tail_ = lastToTransfer;
    }
    size_ += count;
  }

  template< typename T >
  void List< T >::remove(const T& value) noexcept
  {
    assert(!empty() && "removing from empty list");
    if (empty())
    {
        return;
    }
    Node< T >* prev = head_;
    while (prev->next_ != head_)
    {
      if (prev->next_->data_ == value)
      {
        Node< T >* toDelete = prev->next_;
        prev->next_ = toDelete->next_;
        if (toDelete == tail_)
        {
          tail_ = prev;
        }
        delete toDelete;
        --size_;
      }
      else
      {
        prev = prev->next_;
      }
    }
  }

  template< typename T >
  template< class UnaryPredicate >
  void List< T >::remove_if(UnaryPredicate condition)
  {
    assert(!empty() && "removing from empty list");
    if (empty())
    {
      return;
    }
    Node< T >* prev = head_;
    while (prev->next_ != head_)
    {
      if (condition(prev->next_->data_))
      {
        Node< T >* toDelete = prev->next_;
        prev->next_ = toDelete->next_;
        if (toDelete == tail_)
        {
          tail_ = prev;
        }
        delete toDelete;
        --size_;
      }
      else
      {
        prev = prev->next_;
      }
    }
  }
}
#endif
