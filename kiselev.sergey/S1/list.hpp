#ifndef LIST_HPP
#define LIST_HPP
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include "iterator.hpp"
#include "node.hpp"

namespace kiselev
{
  template< typename T >
  class List
  {
  public:
    using Iterator = detail::ListIterator< T, false >;
    using ConstIterator = detail::ListIterator< T, true >;

    List();
    List(const List< T >&);
    List(List< T >&&) noexcept;
    List(size_t, const T&);
    template< typename InputIterator >
    List(InputIterator first, InputIterator last);
    List(std::initializer_list< T >);
    ~List();

    List< T >& operator=(const List< T >&);
    List< T >& operator=(List< T >&&) noexcept;
    List< T >& operator=(std::initializer_list< T >);

    bool operator==(const List< T >&) const;
    bool operator!=(const List< T >&) const;
    bool operator<(const List< T >&) const;
    bool operator>(const List< T >&) const;
    bool operator<=(const List< T >&) const;
    bool operator>=(const List< T >&) const;

    Iterator begin() noexcept;
    ConstIterator cbegin() const noexcept;
    Iterator end() noexcept;
    ConstIterator cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    T& front() noexcept;
    const T& front() const noexcept;
    T& back() noexcept;
    const T& back() const noexcept;

    void pushBack(const T&);
    void pushBack(T&&);
    void pushFront(const T&);
    void pushFront(T&&);

    void popBack() noexcept;
    void popFront() noexcept;

    void clear() noexcept;
    void swap(List< T >&) noexcept;

    void assign(size_t, const T&);
    template< typename InputIterator >
    void assign(InputIterator first, InputIterator last);
    void assign(std::initializer_list< T >);

    Iterator erase(ConstIterator) noexcept;
    Iterator erase(ConstIterator first, ConstIterator last) noexcept;

    Iterator insert(ConstIterator, const T&);
    Iterator insert(ConstIterator, T&&);
    Iterator insert(ConstIterator, size_t, const T&);
    template< typename InputIterator >
    Iterator insert(ConstIterator pos, InputIterator first, InputIterator last);
    Iterator insert(ConstIterator, std::initializer_list< T >);

    void remove(const T&) noexcept;
    template< typename Predicate >
    void remove_if(Predicate) noexcept;

    void splice(ConstIterator, List< T >&) noexcept;
    void splice(ConstIterator, List< T >&&) noexcept;
    void splice(ConstIterator position, List< T >&, ConstIterator i) noexcept;
    void splice(ConstIterator position, List< T >&&, ConstIterator i) noexcept;
    void splice(ConstIterator pos, List< T >&, ConstIterator first, ConstIterator last) noexcept;
    void splice(ConstIterator pos, List< T >&&, ConstIterator first, ConstIterator last) noexcept;
    void reverse() noexcept;

  private:

    Node< T >* head_;
    Node< T >* end_;
    size_t size_;
  };

  template< typename T >
  List< T >::List():
    head_(nullptr),
    end_(nullptr),
    size_(0)
  {
  }

  template< typename T>
  List< T >::List(const List< T >& list):
    List()
  {
    for (ConstIterator it = list.cbegin(); it != list.cend(); ++it)
    {
      pushBack(*it);
    }
  }

  template< typename T >
  List< T >::List(List< T >&& list) noexcept:
    head_(std::exchange(list.head_, nullptr)),
    end_(std::exchange(list.end_, nullptr)),
    size_(std::exchange(list.size_, 0))
  {
  }

  template< typename T >
  List< T >::List(size_t count, const T& data):
    List()
  {
    for (size_t i = 0; i < count; ++i)
    {
      pushBack(data);
    }
  }

  template< typename T >
  template< typename InputIterator >
  List< T >::List(InputIterator first, InputIterator last):
    List()
  {
    for (; first != last; ++first)
    {
      pushFront(*first);
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
    delete end_;
  }

  template< typename T >
  List< T >& List< T >::operator=(const List< T >& list)
  {
    List< T > cpy(list);
    swap(cpy);
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(List< T >&& list) noexcept
  {
    List< T > temp(std::move(list));
    swap(temp);
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(std::initializer_list< T > il)
  {
    List< T > temp(il);
    swap(temp);
    return *this;
  }

  template< typename T >
  bool List< T >::operator==(const List< T >& list) const
  {
    if (size_ == list.size_)
    {
      ConstIterator thisIt = cbegin();
      ConstIterator otherIt = list.cbegin();
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
    ConstIterator thisIt = cbegin();
    ConstIterator otherIt = list.cbegin();
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
  typename List< T >::Iterator List< T >::begin() noexcept
  {
    return Iterator(head_);
  }

  template< typename T >
  typename List< T >::ConstIterator List< T >::cbegin() const noexcept
  {
    return ConstIterator(head_);
  }

  template< typename T >
  typename List< T >::ConstIterator List< T >::cend() const noexcept
  {
    return ConstIterator(end_);
  }

  template< typename T >
  typename List< T >::Iterator List< T >::end() noexcept
  {
    return Iterator(end_);
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
    return end_->prev->data;
  }

  template< typename T >
  const T& List< T >::back() const noexcept
  {
    return end_->prev->data;
  }

  template< typename T >
  void List< T >::pushBack(const T& data)
  {
    insert(cend(), data);
  }

  template< typename T >
  void List< T >::pushBack(T&& data)
  {
    insert(cend(), std::move(data));
  }

  template< typename T >
  void List< T >::pushFront(const T& data)
  {
    insert(cbegin(), data);
  }

  template< typename T >
  void List< T >::pushFront(T&& data)
  {
    insert(cbegin(), std::move(data));
  }

  template< typename T >
  void List< T >::popBack() noexcept
  {
    assert(!empty());
    erase(ConstIterator(end_->prev));
  }

  template< typename T >
  void List< T >::popFront() noexcept
  {
    assert(!empty());
    erase(cbegin());
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
  void List< T >::swap(List<T>& list) noexcept
  {
    std::swap(head_, list.head_);
    std::swap(end_, list.end_);
    std::swap(size_, list.size_);
  }

  template< typename T >
  void List< T >::assign(size_t count, const T& data)
  {
    List< T > temp(count, data);
    swap(temp);
  }

  template< typename T >
  template< typename InputIterator >
  void List< T >::assign(InputIterator first, InputIterator last)
  {
    List< T > temp(first, last);
    swap(temp);
  }

  template< typename T >
  void List< T >::assign(std::initializer_list< T > il)
  {
    List< T > temp(il);
    swap(temp);
  }

  template< typename T >
  typename List< T >::Iterator List< T >::erase(ConstIterator position) noexcept
  {
    Iterator it(position.node_->next);
    if (position == cbegin())
    {
      head_ = head_->next;
    }
    else if (position.node_ == end_->prev)
    {
      end_->prev = position.node_->prev;
    }
    position.node_->prev->next = position.node_->next;
    position.node_->next->prev = position.node_->prev;
    delete position.node_;
    size_--;
    return it;
  }

  template< typename T >
  typename List< T >::Iterator List< T >::erase(ConstIterator first, ConstIterator last) noexcept
  {
    for (; first != last;)
    {
      first = erase(first);
    }
    return Iterator(last.node_);
  }

  template< typename T >
  void List< T >::remove(const T& data) noexcept
  {
    if (size_ == 1)
    {
      if (head_->data == data)
      {
        popFront();
      }
    }
    else
    {
      for (ConstIterator it = cbegin(); it != cend();)
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
  }

  template< typename T >
  template< typename Predicate >
  void List< T >::remove_if(Predicate pred) noexcept
  {
    for (ConstIterator it = cbegin(); it != cend();)
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
  void List< T >::splice(ConstIterator position, List< T >& list) noexcept
  {
    splice(position, list, list.cbegin(), list.cend());
  }

  template< typename T >
  void List< T >::splice(ConstIterator position, List< T >&& list) noexcept
  {
    splice(position, list);
  }

  template< typename T >
  void List< T >::splice(ConstIterator position, List< T >& list, ConstIterator i) noexcept
  {
    splice(position, list, i, std::next(i));
  }

  template< typename T >
  void List< T >::splice(ConstIterator position, List< T >&& list, ConstIterator i) noexcept
  {
    splice(position, list, i);
  }

  template< typename T >
  void List< T >::splice(ConstIterator pos, List< T >& list, ConstIterator first, ConstIterator last) noexcept
  {
    Node< T >* firstNode = first.node_;
    Node< T >* lastNode = last.node_->prev;
    size_t distance = std::distance(first, last);
    list.size_ -= distance;
    size_ += distance;
    firstNode->prev->next = lastNode->next;
    lastNode->next->prev = firstNode->prev;
    if (firstNode == list.head_)
    {
      list.head_ = last.node_;
      list.head_->prev = end_;
      list.end_->next = list.head_;
    }
    if (lastNode == list.end_->prev)
    {
      list.end_->prev = first.node_->prev;
      first.node_->prev->next = list.end_;
    }
    if (pos == cbegin())
    {
      lastNode->next = head_;
      head_->prev = lastNode;
      head_ = firstNode;
      head_->prev = end_;
      end_->next = head_;
    }
    else
    {
      pos.node_->prev->next = firstNode;
      firstNode->prev = pos.node_->prev;
      pos.node_->prev = lastNode;
      lastNode->next = pos.node_;
    }
  }

  template< typename T >
  void List< T >::splice(ConstIterator pos, List< T >&& list, ConstIterator first, ConstIterator last) noexcept
  {
    splice(pos, list, first, last);
  }

  template< typename T >
  typename List< T >::Iterator List< T >::insert(ConstIterator position, const T& data)
  {
    Node< T >* node = new Node< T >{ data, nullptr, nullptr };
    if (empty())
    {
      try
      {
        end_ = new Node< T >{ T(), nullptr, nullptr };
      }
      catch (const std::bad_alloc&)
      {
        delete node;
        throw;
      }
      node->next = end_;
      node->prev = end_;
      end_->prev = node;
      end_->next = node;
      head_ = node;
    }
    else if (position == cbegin())
    {
      node->next = head_;
      node->prev = end_;
      head_->prev = node;
      end_->next = node;
      head_ = node;
    }
    else
    {
      node->next = position.node_;
      node->prev = position.node_->prev;
      position.node_->prev->next = node;
      position.node_->prev = node;
    }
    ++size_;
    return Iterator(node);
  }

  template< typename T >
  typename List< T >::Iterator List< T >::insert(ConstIterator position, T&& data)
  {
    return insert(position, data);
  }

  template< typename T >
  typename List< T >::Iterator List< T >::insert(ConstIterator position, size_t n, const T& data)
  {
    if (n == 0)
    {
      return Iterator(position.node_);
    }
    Iterator res = insert(position, data);
    if (n != 1)
    {
      List< T > temp(--n, data);
      splice(position, temp);
    }
    return res;
  }

  template< typename T >
  template< typename InputIterator >
  typename List< T >::Iterator List< T >::insert(ConstIterator pos, InputIterator first, InputIterator last)
  {
    if (first == last)
    {
      return Iterator(pos.node_);
    }
    Iterator res = insert(pos, *first);
    List< T > temp(++first, last);
    splice(pos, temp);
    return res;

  }

  template< typename T >
  typename List< T >::Iterator List< T >::insert(ConstIterator position, std::initializer_list< T > il)
  {
    return insert(position, il.begin(), il.end());
  }

  template< typename T >
  void List< T >::reverse() noexcept
  {
    if (empty() || size_ == 1)
    {
      return;
    }
    Node< T >* node = head_;
    head_ = end_->prev;
    end_->prev = node;
    ConstIterator it(head_);
    for (; it != cend(); ++it)
    {
      node = it.node_->next;
      it.node_->next = it.node_->prev;
      it.node_->prev = node;
    }
  }
}
#endif
