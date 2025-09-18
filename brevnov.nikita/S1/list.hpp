#ifndef LIST_HPP
#define LIST_HPP
#include <initializer_list>
#include "iterator.hpp"
namespace brevnov
{
  template< typename T >
  class List
  {
  public:
    using Iter = Iterator< T, false >;
    using ConstIter = Iterator< T, true >;

    List();
    List(size_t n, const T& val);
    List(const List< T > & copy);
    List(List< T > && del);
    template< typename InputIter >
    List(InputIter begin, InputIter end);
    List(std::initializer_list< T > il);
    ~List();

    List< T > & operator=(const List< T > & copy);
    List< T > & operator=(List< T > && copy);
    List< T > & operator=(std::initializer_list< T > il);

    bool operator==(const List< T >&) const;
    bool operator!=(const List< T >&) const;
    bool operator<(const List< T >&) const;
    bool operator>(const List< T >&) const;
    bool operator<=(const List< T >&) const;
    bool operator>=(const List< T >&) const;

    Iter begin() noexcept;
    ConstIter cbegin() const noexcept;
    Iter end() noexcept;
    ConstIter cend() const noexcept;

    T& front() noexcept;
    const T& front() const noexcept;
    T& back() noexcept;
    const T& back() const noexcept;

    void push_back(const T&);
    void push_back(T&&);
    void push_front(const T&);
    void push_front(T&&);

    void pop_front() noexcept;
    void pop_back() noexcept;

    void clear();
    void swap(List< T > & hl);

    void assign(size_t, const T&);
    template< typename InputIter >
    void assign(InputIter first, InputIter last);
    void assign(std::initializer_list< T >);

    Iter erase(ConstIter) noexcept;
    Iter erase(ConstIter first, ConstIter last) noexcept;

    void remove(const T&) noexcept;
    template< typename Predicate >
    void remove_if(Predicate) noexcept;

    void splice(ConstIter, List< T >&) noexcept;
    void splice(ConstIter, List< T >&&) noexcept;
    void splice(ConstIter pos, List< T >&, ConstIter i) noexcept;
    void splice(ConstIter pos, List< T >&&, ConstIter i) noexcept;
    void splice(ConstIter pos, List< T >&, ConstIter first, ConstIter last) noexcept;
    void splice(ConstIter pos, List< T >&&, ConstIter first, ConstIter last) noexcept;

    Iter insert(ConstIter, const T&);
    Iter insert(ConstIter, T&&);
    Iter insert(ConstIter, size_t, const T&);
    template< typename InputIter >
    Iter insert(ConstIter pos, InputIter first, InputIter last);
    Iter insert(ConstIter, std::initializer_list< T >);

    void reverse() noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;
  private:
    Node< T > * head_;
    Node< T > * tail_;
    size_t size_;
  };

  template< typename T >
  List< T >::List():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< typename T >
  List< T >::List(size_t n, const T& val):
    List()
  {
    try
    {
      for (size_t i = 0; i < n; i++)
      {
        push_back(val);
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template< typename T >
  List< T >::List(const List< T >& copy):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    try
    {
      for (ConstIter it = copy.cbegin(); it != copy.cend(); ++it)
      {
        push_back(*it);
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template< typename T >
  List< T >::List(List< T > && del):
    head_(del.head_),
    tail_(del.tail_),
    size_(del.size_)
  {
    del.head_ = nullptr;
    del.tail_ = nullptr;
    del.size_ = 0;
  }

  template< typename T >
  template< typename InputIter >
  List< T >::List(InputIter begin, InputIter end):
    List()
  {
    for (; begin != end; ++begin)
    {
      push_back(*begin);
    }
  }

  template< typename T >
  List< T >::List(std::initializer_list< T > il):
    List(il.begin(), il.end())
  {}

  template< typename T >
  List< T >::~List()
  {
    clear();
  }

  template< typename T >
  void List< T >::clear()
  {
    while (!empty())
    {
      pop_front();
    }
  }

  template< typename T >
  List< T > & List< T >::operator=(const List< T > & copy)
  {
    if (this != &copy)
    {
      List< T > cpy(copy);
      swap(cpy);
    }
    return *this;
  }

  template< typename T >
  void List< T >::swap(List< T > & hl)
  {
    std::swap(head_, hl.head_);
    std::swap(tail_, hl.tail_);
    std::swap(size_, hl.size_);
  }

  template< typename T >
  List< T > & List< T >::operator=(List< T > && copy)
  {
    List< T > cpy(std::move(copy));
    swap(cpy);
    return *this;
  }

  template< typename T >
  List< T > & List< T >::operator=(std::initializer_list< T > il)
  {
    List< T > cpy(il);
    swap(cpy);
    return *this;
  }

  template< typename T >
  typename List< T >::Iter List< T >::begin() noexcept
  {
    return Iter(head_);
  }

  template< typename T >
  typename List< T >::ConstIter List< T >::cbegin() const noexcept
  {
    return ConstIter(head_);
  }

  template< typename T >
  typename List< T >::Iter List< T >::end() noexcept
  {
    return Iter(nullptr);
  }

  template< typename T >
  typename List< T >::ConstIter List< T >::cend() const noexcept
  {
    return ConstIter(nullptr);
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
  T& List< T >::front() noexcept
  {
    return head_->data;
  }

  template< typename T >
  const T& List< T >::front() const noexcept
  {
    return head_->data;
  }

  template< typename T >
  T& List< T >::back() noexcept
  {
    return tail_->data;
  }

  template< typename T >
  const T& List< T >::back() const noexcept
  {
    return tail_->data;
  }

  template< class T >
  void List< T >::push_back(const T &data)
  {
    insert(cend(), data);
  }

  template< class T >
  void List< T >::push_back(T &&data)
  {
    insert(cend(), std::move(data));
  }

  template< class T >
  void List< T >::push_front(const T &data)
  {
    insert(cbegin(), data);
  }

  template< class T >
  void List< T >::push_front(T &&data)
  {
    insert(cbegin(), std::move(data));
  }

  template< class T >
  void List< T >::pop_back() noexcept
  {
    assert(!empty());
    erase(ConstIter(tail_));
  }

  template< class T >
  void List< T >::pop_front() noexcept
  {
    assert(!empty());
    erase(cbegin());
  }

  template< typename T >
  bool List< T >::operator==(const List< T >& list) const
  {
    if (size_ == list.size_)
    {
      ConstIter thisIt = cbegin();
      ConstIter otherIt = list.cbegin();
      for (; thisIt != cend(); ++thisIt, ++otherIt)
      {
        if (*thisIt != *otherIt)
        {
          return false;
        }
      }
      return true;
    }
    return false;
  }

  template< typename T >
  bool List< T >::operator!=(const List<T>& list) const
  {
    return !(*this == list);
  }

  template< typename T >
  bool List< T >::operator<(const List< T >& list) const
  {
    size_t min = std::min(size_, list.size_);
    ConstIter thisIt = cbegin();
    ConstIter otherIt = list.cbegin();
    for (size_t i = 0; i < min; ++i, ++thisIt, ++otherIt)
    {
      if (*thisIt != *otherIt)
      {
        return *thisIt < * otherIt;
      }
    }
    return false;
  }

  template< typename T >
  bool List< T >::operator>(const List< T >& list) const
  {
    return list < *this;
  }

  template< typename T >
  bool List< T >::operator<=(const List< T >& list) const
  {
    return !(*this > list);
  }

  template< typename T >
  bool List< T >::operator>=(const List<T>& list) const
  {
    return !(*this < list);
  }

  template< typename T >
  void List< T >::assign(size_t count, const T& data)
  {
    List< T > list(count, data);
    swap(list);
  }

  template< typename T >
  template< typename InputIter >
  void List< T >::assign(InputIter first, InputIter last)
  {
    List< T > list(first, last);
    swap(list);
  }

  template< typename T >
  void List< T >::assign(std::initializer_list< T > il)
  {
    List< T > list(il);
    swap(list);
  }

  template< typename T >
  typename List< T >::Iter List< T >::erase(ConstIter pos) noexcept
  {
    if (pos.node_ == nullptr || empty())
    {
      return end();
    }
    Node< T >* toDelete = pos.node_;
    Node< T >* nextNode = toDelete->next;
    if (toDelete->prev)
    {
      toDelete->prev->next = nextNode;
    }
    else
    {
      head_ = nextNode;
    }
    if (nextNode)
    {
      nextNode->prev = toDelete->prev;
    }
    else
    {
      tail_ = toDelete->prev;
    }
    delete toDelete;
    size_--;
    return Iter(nextNode);
  }

  template< typename T >
  typename List< T >::Iter List< T >::erase(ConstIter first, ConstIter last) noexcept
  {
    for (; first != last;)
    {
      first = erase(first);
    }
    return Iter(last.node_);
  }

  template< typename T >
  void List< T >::remove(const T& data) noexcept
  {
    for (ConstIter it = cbegin(); it != cend();)
    {
      if (it.node_->data == data)
      {
        it = erase(it);
      }
      else
      {
        it++;
      }
    }
  }

  template< typename T >
  template< typename Predicate >
  void List< T >::remove_if(Predicate pred) noexcept
  {
    for (ConstIter it = cbegin(); it != cend();)
    {
      if (pred(*it))
      {
        it = erase(it);
      }
      else
      {
        ++it;
      }
    }
  }

  template< typename T >
  void List< T >::splice(ConstIter pos, List< T >& list) noexcept
  {
    if (!list.empty())
    {
      splice(pos, list, list.cbegin(), list.cend());
    }
  }

  template< typename T >
  void List< T >::splice(ConstIter pos, List< T >&& list) noexcept
  {
    splice(pos, list);
  }

  template< typename T >
  void List< T >::splice(ConstIter pos, List< T >& list, ConstIter i) noexcept
  {
    if (i != list.cend())
    {
      splice(pos, list, i, std::next(i));
    }
  }

  template< typename T >
  void List< T >::splice(ConstIter pos, List< T >&& list, ConstIter i) noexcept
  {
    splice(pos, list, i);
  }

  template< typename T >
  void List< T >::splice(ConstIter pos, List< T >& list, ConstIter first, ConstIter last) noexcept
  {
    if (first == last || std::addressof(list) == this || list.empty())
    {
      return;
    }
    size_t dist = 0;
    Node<T>* firstNode = first.node_;
    Node<T>* lastNode = firstNode;
    for (ConstIter it = first; it != last; ++it, ++dist)
    {
      lastNode = it.node_;
    }
    if (firstNode->prev)
    {
      firstNode->prev->next = lastNode->next;
    }
    else
    {
      list.head_ = lastNode->next;
    }
    if (lastNode->next)
    {
      lastNode->next->prev = firstNode->prev;
    }
    else
    {
      list.tail_ = firstNode->prev;
    }
    if (pos == cbegin())
    {
      firstNode->prev = nullptr;
      lastNode->next = head_;
      if (head_)
      {
        head_->prev = lastNode;
      }
      head_ = firstNode;
    }
    else if (pos == cend())
    {
      lastNode->next = nullptr;
      firstNode->prev = tail_;
      if (tail_)
      {
        tail_->next = firstNode;
      }
      tail_ = lastNode;
    }
    else
    {
      firstNode->prev = pos.node_->prev;
      lastNode->next = pos.node_;
      pos.node_->prev->next = firstNode;
      pos.node_->prev = lastNode;
    }
    list.size_ -= dist;
    size_ += dist;
  }

  template< typename T >
  void List< T >::splice(ConstIter pos, List< T >&& list, ConstIter first, ConstIter last) noexcept
  {
    splice(pos, list, first, last);
  }

  template< typename T >
  typename List< T >::Iter List< T >::insert(ConstIter pos, const T& data)
  {
    Node< T >* node = new Node< T >{ data, nullptr, nullptr };
    if (empty())
    {
      head_ = node;
      tail_ = head_;
    }
    else if (pos == cbegin())
    {
      node->next = head_;
      head_->prev = node;
      head_ = node;
    }
    else if (pos == cend())
    {
      node->prev = tail_;
      tail_->next = node;
      tail_ = node;
    }
    else
    {
      node->next = pos.node_;
      node->prev = pos.node_->prev;
      pos.node_->prev->next = node;
      pos.node_->prev = node;
    }
    ++size_;
    return Iter(node);
  }

  template< typename T >
  typename List< T >::Iter List< T >::insert(ConstIter pos, T&& data)
  {
    return insert(pos, data);
  }

  template< typename T >
  typename List< T >::Iter List< T >::insert(ConstIter pos, size_t n, const T& data)
  {
    if (n == 0)
    {
      return Iter(pos.node_);
    }
    Iter result = insert(pos, data);
    if (n != 1)
    {
      List< T > temp(--n, data);
      splice(pos, temp);
    }
    return result;
  }

  template< typename T >
  template< typename InputIter >
  typename List< T >::Iter List< T >::insert(ConstIter pos, InputIter first, InputIter last)
  {
    if (first == last)
    {
      return Iter(pos.node_);
    }
    Iter result = insert(pos, *first);
    List< T > temp(++first, last);
    splice(pos, temp);
    return result;
  }

  template< typename T >
  typename List< T >::Iter List< T >::insert(ConstIter pos, std::initializer_list< T > il)
  {
    return insert(pos, il.begin(), il.end());
  }

  template< typename T >
  void List< T >::reverse() noexcept
  {
    if (empty() || size_ == 1)
    {
      return;
    }
    Node< T >* node = head_;
    head_ = tail_;
    tail_ = node;
    ConstIter it(head_);
    for (; it != cend(); ++it)
    {
      node = it.node_->next;
      it.node_->next = it.node_->prev;
      it.node_->prev = node;
    }
  }
}
#endif
