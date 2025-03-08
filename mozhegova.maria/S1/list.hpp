#ifndef LIST_HPP
#define LIST_HPP

#include <initializer_list>
#include "cstddef"
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

    Iterator< T > begin() const;
    Iterator< T > end() const;
    ConstIterator< T > cbegin() const;
    ConstIterator< T > cend() const;

    T & front();
    const T & front() const;
    T & back();
    const T & back() const;

    bool empty() const;
    size_t size() const;

    void push_front(const T &);
    void push_back(const T &);
    void pop_front();
    void pop_back();

    void clear();
    void swap(List< T > &);

    void remove(const T &);
    template< typename condition >
    void removeIf(condition);

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

    Iterator< T > erase(ConstIterator< T > pos);
    Iterator< T > erase(ConstIterator< T > first, ConstIterator< T > last);

    void assign(size_t, const T &);
    void assign(ConstIterator< T > first, ConstIterator< T > last);
    void assign(std::initializer_list< T >);
  private:
    Node< T > * fake_;
    Node< T > * tail_;
    size_t size_;
  };

  template< typename T >
  List< T >::List():
    fake_(new Node< T >()),
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
  Iterator< T > List< T >::begin() const
  {
    return Iterator< T >(fake_->next_);
  }

  template< typename T >
  Iterator< T > List< T >::end() const
  {
    return Iterator< T >(empty() ? fake_->next_ : tail_->next_);
  }

  template< typename T >
  ConstIterator< T > List< T >::cbegin() const
  {
    return ConstIterator< T >(fake_->next_);
  }

  template< typename T >
  ConstIterator< T > List< T >::cend() const
  {
    return ConstIterator< T >(empty() ? fake_->next_ : tail_->next_);
  }

  template< typename T >
  T & List< T >::front()
  {
    return fake_->next_->data_;
  }

  template< typename T >
  const T & List< T >::front() const
  {
    return fake_->next_->data_;
  }

  template< typename T >
  T & List< T >::back()
  {
    return tail_->data_;
  }

  template< typename T >
  const T & List< T >::back() const
  {
    return tail_->data_;
  }

  template< typename T >
  bool List< T >::empty() const
  {
    return size_ == 0;
  }

  template< typename T >
  size_t List< T >::size() const
  {
    return size_;
  }

  template< typename T >
  void List< T >::push_front(const T & data)
  {
    Node< T > * newNode = new Node< T > (data);
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
    Node< T > * newNode = new Node< T > (data);
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
  void List< T >::pop_front()
  {
    erase(cbegin());
  }

  template< typename T >
  void List< T >::pop_back()
  {
    erase(ConstIterator< T >(tail_));
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
  void List< T >::swap(List< T > & otherList)
  {
    std::swap(fake_->next_, otherList.fake_->next_);
    std::swap(tail_, otherList.tail_);
    std::swap(size_, otherList.size_);
  }

  template< typename T >
  void List< T >::remove(const T & val)
  {
    for (auto it = cbegin(); it != cend(); ++it)
    {
      if (*it == val)
      {
        erase(it);
      }
    }
  }

  template< typename T >
  template< typename condition >
  void List< T >::removeIf(condition c)
  {
    for (auto it = cbegin(); it != cend(); ++it)
    {
      if (c(*it))
      {
        erase(it);
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
    for (auto it = first; it != last; ++it)
    {
      x.erase(it);
    }
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T > && x, ConstIterator< T > first, ConstIterator< T > last)
  {
    splice(pos, x, first, last);
  }

  template< typename T >
  Iterator< T > List< T >::erase(ConstIterator< T > pos)
  {
    assert(!empty());
    Node< T > * tempNode = pos.getNode();
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
  Iterator< T > List< T >::erase(ConstIterator< T > first, ConstIterator< T > last)
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
    Node< T > * tempNode = pos.getNode();
    Node< T > * newNode = new Node< T >(val);
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
      return Iterator< T >(pos.getNode());
    }
    Iterator< T > temp = insert(pos, val);
    for (size_t i = 1; i < n; i++)
    {
      insert(pos, val);
    }
    return temp;
  }

  template< typename T >
  template< typename Iter >
  Iterator< T > List< T >::insert(ConstIterator< T > pos, Iter first, Iter last)
  {
    if (first == last)
    {
      return Iterator< T >(pos.getNode());
    }
    Iterator< T > temp = insert(pos, *first);
    for (auto it = ++first; it != last; ++it)
    {
      insert(pos, *it);
    }
    return temp;
  }

  template< typename T >
  Iterator< T > List< T >::insert(ConstIterator< T > pos, T && val)
  {
    return insert(pos, val);
  }

  template< typename T >
  Iterator< T > List< T >::insert(ConstIterator< T > pos, std::initializer_list< T > il)
  {
    return insert(pos, il.begin(), il.end());
  }

  template< typename T >
  void List< T >::assign(size_t n, const T & val)
  {
    clear();
    for (size_t i = 0; i < n; i++)
    {
      push_back(val);
    }
  }

  template< typename T >
  void List< T >::assign(ConstIterator< T > first, ConstIterator< T > last)
  {
    clear();
    for (auto it = first; it != last; ++it)
    {
      push_back(*it);
    }
  }

  template< typename T >
  void List< T >::assign(std::initializer_list< T > il)
  {
    clear();
    for (auto it = il.begin(); it != il.end(); ++it)
    {
      push_back(*it);
    }
  }
}

#endif
