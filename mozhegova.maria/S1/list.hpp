#ifndef LIST_HPP
#define LIST_HPP

#include <initializer_list>
#include <functional>
#include "iterator.hpp"
#include "constIterator.hpp"

namespace mozhegova
{
  template< typename T >
  class List
  {
  public:
    List();
    List(const List< T > &);
    List(List< T > &&);
    List(size_t, const T &);
    List(std::initializer_list< T >);
    List(ConstIterator< T > first, ConstIterator< T > last);
    ~List();

    List< T > & operator=(const List< T > &);
    List< T > & operator=(std::initializer_list< T >);

    Iterator< T > begin() const noexcept;
    Iterator< T > end() const noexcept;
    ConstIterator< T > cbegin() const noexcept;
    ConstIterator< T > cend() const noexcept;

    T & front() noexcept;
    const T & front() const noexcept;
    T & back() noexcept;
    const T & back() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void push_front(const T &);
    void push_back(const T &);
    void pop_front() noexcept;
    void pop_back() noexcept;

    void clear() noexcept;
    void swap(List< T > &) noexcept;

    void remove(const T &) noexcept;
    template< typename condition >
    void removeIf(condition) noexcept;

    void splice(ConstIterator< T > pos, List< T > & x);
    void splice(ConstIterator< T > pos, List< T > && x);
    void splice(ConstIterator< T > pos, List< T > & x, ConstIterator< T > i);
    void splice(ConstIterator< T > pos, List< T > && x, ConstIterator< T > i);
    void splice(ConstIterator< T > pos, List< T > & x, ConstIterator< T > first, ConstIterator< T > last);
    void splice(ConstIterator< T > pos, List< T > && x, ConstIterator< T > first, ConstIterator< T > last);

    Iterator< T > insert(ConstIterator< T > pos, const T & val);
    Iterator< T > insert(ConstIterator< T > pos, size_t n, const T & val);
    template< typename Iter >
    Iterator< T > insert(ConstIterator< T > pos, Iter first, Iter last);
    Iterator< T > insert(ConstIterator< T > pos, T && val);
    Iterator< T > insert(ConstIterator< T > pos, std::initializer_list< T >);

    Iterator< T > erase(ConstIterator< T > pos) noexcept;
    Iterator< T > erase(ConstIterator< T > first, ConstIterator< T > last) noexcept;

    void assign(size_t, const T &);
    void assign(ConstIterator< T > first, ConstIterator< T > last);
    void assign(std::initializer_list< T >);
  private:
    detail::Node< T > * fake_;
    detail::Node< T > * tail_;
    size_t size_;
  };

  template< typename T >
  List< T >::List():
    fake_(new detail::Node< T >()),
    tail_(nullptr),
    size_(0)
  {}

  template< typename T >
  List< T >::List(const List< T > & rhs):
    List()
  {
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
    {
      push_back(*it);
    }
  }

  template< typename T >
  List< T >::List(List< T > && rhs):
    fake_(rhs.fake_),
    tail_(rhs.tail_),
    size_(rhs.size_)
  {
    rhs.fake_ = nullptr;
    rhs.tail_ = nullptr;
    rhs.size_ = 0;
  }

  template< typename T >
  List< T >::List(size_t n, const T & val):
    List()
  {
    assign(n, val);
  }

  template< typename T >
  List< T >::List(std::initializer_list< T > il):
    List()
  {
    assign(il);
  }

  template< typename T >
  List< T >::List(ConstIterator< T > first, ConstIterator< T > last):
    List()
  {
    assign(first, last);
  }

  template< typename T >
  List< T >::~List()
  {
    clear();
    delete fake_;
  }

  template< typename T >
  List< T > & List< T >::operator=(const List< T > & other)
  {
    assign(other.cbegin(), other.cend());
    return *this;
  }

  template< typename T >
  List< T > & List< T >::operator=(std::initializer_list< T > il)
  {
    assign(il);
    return *this;
  }

  template< typename T >
  Iterator< T > List< T >::begin() const noexcept
  {
    return Iterator< T >(fake_->next_);
  }

  template< typename T >
  Iterator< T > List< T >::end() const noexcept
  {
    return Iterator< T >(empty() ? fake_->next_ : tail_->next_);
  }

  template< typename T >
  ConstIterator< T > List< T >::cbegin() const noexcept
  {
    return ConstIterator< T >(fake_->next_);
  }

  template< typename T >
  ConstIterator< T > List< T >::cend() const noexcept
  {
    return ConstIterator< T >(empty() ? fake_->next_ : tail_->next_);
  }

  template< typename T >
  T & List< T >::front() noexcept
  {
    assert(fake_->next_ != nullptr);
    return fake_->next_->data_;
  }

  template< typename T >
  const T & List< T >::front() const noexcept
  {
    assert(fake_->next_ != nullptr);
    return fake_->next_->data_;
  }

  template< typename T >
  T & List< T >::back() noexcept
  {
    assert(tail_ != nullptr);
    return tail_->data_;
  }

  template< typename T >
  const T & List< T >::back() const noexcept
  {
    assert(tail_ != nullptr);
    return tail_->data_;
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
  void List< T >::push_front(const T & data)
  {
    detail::Node< T > * newNode = new detail::Node< T > (data);
    newNode->next_ = fake_->next_;
    if (!empty())
    {
      fake_->next_->prev_ = newNode;
    }
    else
    {
      tail_ = newNode;
    }
    fake_->next_ = newNode;
    newNode->prev_ = fake_;
    size_++;
  }

  template< typename T >
  void List< T >::push_back(const T & data)
  {
    detail::Node< T > * newNode = new detail::Node< T > (data);
    if (!empty())
    {
      newNode->prev_ = tail_;
      tail_->next_ = newNode;
    }
    else
    {
      newNode->prev_ = fake_;
      fake_->next_ = newNode;
    }
    tail_ = newNode;
    size_++;
  }

  template< typename T >
  void List< T >::pop_front() noexcept
  {
    erase(cbegin());
  }

  template< typename T >
  void List< T >::pop_back() noexcept
  {
    erase(ConstIterator< T >(tail_));
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
  void List< T >::swap(List< T > & otherList) noexcept
  {
    std::swap(fake_, otherList.fake_);
    std::swap(tail_, otherList.tail_);
    std::swap(size_, otherList.size_);
  }

  template< typename T >
  bool isSame(const T & a, const T & b)
  {
    return a == b;
  }

  template< typename T >
  void List< T >::remove(const T & val) noexcept
  {
    removeIf(std::bind(isSame< T >, std::placeholders::_1, val));
  }

  template< typename T >
  template< typename condition >
  void List< T >::removeIf(condition c) noexcept
  {
    Iterator< T > it = begin();
    ConstIterator< T > cit = cbegin();
    while (it != end() && cit != cend())
    {
      if (c(*cit))
      {
        it = erase(cit);
        ++cit;
      }
      else
      {
        ++it;
        ++cit;
      }
    }
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T > & x)
  {
    insert(pos, x.cbegin(), x.cend());
    x.clear();
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T > && x)
  {
    splice(pos, x);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T > & x, ConstIterator< T > i)
  {
    insert(pos, *i);
    x.erase(i);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T > && x, ConstIterator< T > i)
  {
    splice(pos, i);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T > & x, ConstIterator< T > first, ConstIterator< T > last)
  {
    insert(pos, first, last);
    x.erase(first, last);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T > && x, ConstIterator< T > first, ConstIterator< T > last)
  {
    splice(pos, x, first, last);
  }

  template< typename T >
  Iterator< T > List< T >::erase(ConstIterator< T > pos) noexcept
  {
    assert(!empty());
    detail::Node< T > * tempNode = pos.node_;
    Iterator< T > it(tempNode->next_);
    if (pos == cbegin())
    {
      fake_->next_ = tempNode->next_;
    }
    else
    {
      tempNode->prev_->next_ = tempNode->next_;
    }
    if (tempNode == tail_)
    {
      tail_ = tempNode->prev_;
    }
    else
    {
      tempNode->next_->prev_ = tempNode->prev_;
    }
    delete tempNode;
    size_--;
    return it;
  }

  template< typename T >
  Iterator< T > List< T >::erase(ConstIterator< T > first, ConstIterator< T > last) noexcept
  {
    Iterator< T > temp;
    for (auto it = first; it != last; ++it)
    {
      temp = erase(it);
    }
    return temp;
  }

  template< typename T >
  Iterator< T > List< T >::insert(ConstIterator< T > pos, const T & val)
  {
    if (empty())
    {
      push_back(val);
      return begin();
    }
    detail::Node< T > * tempNode = pos.node_;
    detail::Node< T > * newNode = new detail::Node< T >(val);
    tempNode->prev_->next_ = newNode;
    newNode->prev_ = tempNode->prev_;
    newNode->next_ = tempNode;
    tempNode->prev_ = newNode;
    size_++;
    return Iterator< T >(newNode);
  }

  template< typename T >
  Iterator< T > List< T >::insert(ConstIterator< T > pos, size_t n, const T & val)
  {
    if (n == 0)
    {
      return Iterator< T >(pos.node_);
    }
    List< T > tempList(n, val);
    detail::Node< T > * head = tempList.fake_->next_;
    delete head->prev_;
    detail::Node< T > * tempNode = pos.node_;
    tempNode->prev_->next_ = head;
    head->prev_ = tempNode->prev_;
    tempList.tail_->next_ = tempNode;
    tempNode->prev_ = tempList.tail_;
    size_ += n;
    tempList.fake_ = nullptr;
    tempList.tail_ = nullptr;
    tempList.size_ = 0;
    return Iterator< T >(head);
  }

  template< typename T >
  template< typename Iter >
  Iterator< T > List< T >::insert(ConstIterator< T > pos, Iter first, Iter last)
  {
    if (first == last)
    {
      return Iterator< T >(pos.node_);
    }
    List< T > tempList(first, last);
    detail::Node< T > * head = tempList.fake_->next_;
    delete head->prev_;
    detail::Node< T > * tempNode = pos.node_;
    tempNode->prev_->next_ = head;
    head->prev_ = tempNode->prev_;
    tempList.tail_->next_ = tempNode;
    tempNode->prev_ = tempList.tail_;
    size_ += tempList.size_;
    tempList.fake_ = nullptr;
    tempList.tail_ = nullptr;
    tempList.size_ = 0;
    return Iterator< T >(head);
  }

  template< typename T >
  Iterator< T > List< T >::insert(ConstIterator< T > pos, T && val)
  {
    return insert(pos, val);
  }

  template< typename T >
  Iterator< T > List< T >::insert(ConstIterator< T > pos, std::initializer_list< T > il)
  {
    if (il.size() == 0)
    {
      return Iterator< T >(pos.node_);
    }
    List< T > tempList(il);
    detail::Node< T > * head = tempList.fake_->next_;
    delete head->prev_;
    detail::Node< T > * tempNode = pos.node_;
    tempNode->prev_->next_ = head;
    head->prev_ = tempNode->prev_;
    tempList.tail_->next_ = tempNode;
    tempNode->prev_ = tempList.tail_;
    size_ += tempList.size_;
    tempList.fake_ = nullptr;
    tempList.tail_ = nullptr;
    tempList.size_ = 0;
    return Iterator< T >(head);
  }

  template< typename T >
  void List< T >::assign(size_t n, const T & val)
  {
    List< T > temp;
    for (size_t i = 0; i < n; i++)
    {
      try
      {
        temp.push_back(val);
      }
      catch(const std::bad_alloc &)
      {
        temp.clear();
        throw;
      }
    }
    swap(temp);
  }

  template< typename T >
  void List< T >::assign(ConstIterator< T > first, ConstIterator< T > last)
  {
    List< T > temp;
    for (auto it = first; it != last; ++it)
    {
      try
      {
        temp.push_back(*it);
      }
      catch(const std::bad_alloc &)
      {
        temp.clear();
        throw;
      }
    }
    swap(temp);
  }

  template< typename T >
  void List< T >::assign(std::initializer_list< T > il)
  {
    List< T > temp;
    for (auto it = il.begin(); it != il.end(); ++it)
    {
      try
      {
        temp.push_back(*it);
      }
      catch(const std::bad_alloc &)
      {
        temp.clear();
        throw;
      }
    }
    swap(temp);
  }
}

#endif
