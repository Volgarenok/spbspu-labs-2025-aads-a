#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <algorithm>
#include <initializer_list>
#include "node.hpp"
#include "iterator.hpp"
#include "cIterator.hpp"

namespace abramov
{
  template< class T >
  struct ConstIterator;

  template< class T >
  struct Iterator;

  template< class T >
  struct List
  {
    List();
    List(std::initializer_list< T > il);
    List(Iterator< T > begin, Iterator< T > end);
    ~List();
    List< T > &operator=(std::initializer_list< T > il);
    T &front();
    const T &front() const;
    T &back();
    const T &back() const;
    void pushBack(const T &data);
    void pushFront(const T &data);
    void popBack();
    void popFront();
    void swap(List &rhs) noexcept;
    void clear();
    bool empty() const;
    size_t size() const;
    Iterator< T > begin() const;
    Iterator< T > end() const;
    ConstIterator< T > cbegin() const;
    ConstIterator< T > cend() const;
    void fill(Iterator< T > begin, Iterator< T > end, const T &val);
    Iterator< T > find(Iterator< T > begin, Iterator< T > end, const T &val);
    template< class C >
    Iterator< T > find_if(Iterator< T > begin, Iterator< T > end, C condition);
    Iterator< T > remove(Iterator< T > begin, Iterator< T > end, const T &val);
    template< class C >
    Iterator< T > remove_if(Iterator< T > begin, Iterator< T > end, C condition);
    void splice(ConstIterator< T > pos, List< T > &other);
    void splice(ConstIterator< T > pos, List< T > &&other);
  private:
    Node< T > *head_;
    Node< T > *tail_;
    size_t size_;
  };

  template< class T >
  List< T >::List():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< class T >
  List< T >::List(std::initializer_list < T > il):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    for (auto iter = il.begin(); iter != il.end(); ++iter)
    {
      pushBack(*iter);
    }
  }

  template< class T >
  List< T >::List(Iterator< T > begin, Iterator< T > end):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    for (auto it = begin; it != end; ++it)
    {
      pushBack(*it);
    }
  }

  template< class T >
  List< T >::~List()
  {
    clear();
  }

  template< class T >
  List< T > &List< T >::operator=(std::initializer_list< T > il)
  {
    List< T > copy{ il };
    swap(copy);
    return *this;
  }

  template< class T >
  T &List< T >::front()
  {
    return const_cast< T& >(const_cast< const List< T >* >(this)->front());
  }

  template< class T >
  const T &List< T >::front() const
  {
    return head_->data_;
  }

  template< class T >
  T &List< T >::back()
  {
    return const_cast< T& >(const_cast< const List< T >* >(this)->back());
  }

  template< class T >
  const T &List< T >::back() const
  {
    return tail_->data_;
  }

  template< class T >
  void List< T >::pushBack(const T &data)
  {
    Node< T > *node = new Node< T >{ data };
    if (empty())
    {
      head_ = node;
      tail_ = node;
    }
    else
    {
      tail_->next_ = node;
      node->prev_ = tail_;
      tail_ = node;
    }
    ++size_;
  }

  template< class T >
  void List< T >::pushFront(const T &data)
  {
    Node< T > *node = new Node< T >{ data };
    if (empty())
    {
      head_ = node;
      tail_ = node;
    }
    else
    {
      head_->prev_ = node;
      node->next_ = head_;
      head_ = node;
    }
    ++size_;
  }

  template< class T >
  void List< T >::popBack()
  {
    if (!empty())
    {
      --size_;
      if (head_ == tail_)
      {
        delete tail_;
        head_ = nullptr;
        tail_ = nullptr;
      }
      else
      {
        Node< T > *node = tail_->prev_;
        delete tail_;
        tail_ = node;
        tail_->next_ = nullptr;
      }
    }
  }

  template< class T >
  void List< T >::popFront()
  {
    if (!empty())
    {
      --size_;
      if (head_ == tail_)
      {
        delete head_;
        head_ = nullptr;
        tail_ = nullptr;
      }
      else
      {
        Node< T > *node = head_->next_;
        delete head_;
        head_ = node;
        head_->prev_ = nullptr;
      }
    }
  }

  template< class T >
  void List< T >::swap(List &rhs) noexcept
  {
    std::swap(head_, rhs.head_);
    std::swap(tail_, rhs.tail_);
    std::swap(size_, rhs.size_);
  }

  template< class T >
  void List< T >::clear()
  {
    while (head_)
    {
      Node< T > *temp = head_;
      head_ = head_->next_;
      delete temp;
    }
    size_ = 0;
  }

  template< class T >
  bool List< T >::empty() const
  {
    return size_ == 0;
  }

  template< class T >
  size_t List< T >::size() const
  {
    return size_;
  }

  template< class T >
  Iterator< T > List< T >::begin() const
  {
    return Iterator< T >(head_);
  }

  template< class T >
  Iterator< T > List< T >::end() const
  {
    return Iterator< T >(nullptr);
  }

  template< class T >
  ConstIterator< T > List< T >::cbegin() const
  {
    return ConstIterator< T >(head_);
  }

  template< class T >
  ConstIterator< T > List< T >::cend() const
  {
    return ConstIterator< T >(nullptr);
  }

  template< class T >
  void List< T >::fill(Iterator< T > begin, Iterator< T > end, const T &val)
  {
    while (begin != end)
    {
      *begin = val;
      ++begin;
    }
  }

  template< class T >
  Iterator< T > List< T >::find(Iterator< T > begin, Iterator< T > end, const T &val)
  {
    for (auto iter = begin; iter != end; ++iter)
    {
      if (*iter == val)
      {
        return iter;
      }
    }
    return end;
  }

  template< class T >
  template< class C >
  Iterator< T > List< T >::find_if(Iterator< T > begin, Iterator< T > end, C condition)
  {
    for (auto iter = begin; iter != end; ++iter)
    {
      if (condition(*iter))
      {
        return iter;
      }
    }
    return end;
  }

  template< class T >
  Iterator< T > List< T >::remove(Iterator< T > begin, Iterator< T > end, const T &val)
  {
    auto iter = find(begin, end, val);
    if (iter != end)
    {
      for (auto it = iter; ++it != end;)
      {
        if (*it != val)
        {
          *iter++ = std::move(*it);
        }
      }
    }
    return iter;
  }

  template< class T >
  template< class C >
  Iterator< T > List< T >::remove_if(Iterator< T > begin, Iterator< T > end, C condition)
  {
    auto iter = find_if(begin, end, condition);
    if (iter != end)
    {
      for (auto it = iter; ++it != end;)
      {
        if (!condition(*it))
        {
          *iter++ = std::move(*it);
        }
      }
    }
    return iter;
  }

  template< class T >
  void List< T >::splice(ConstIterator< T > pos, List< T > &other)
  {
    if (pos.node_ == begin().node_)
    {
      other.tail_->next_ = head_;
      head_->prev_ = other.tail_;
      head_ = other.head_;
    }
    pos.node_->prev_->next_ = other.head_;
    other.head_->prev_ = pos.node_->prev_;
    pos.node_->prev_ = other.tail_;
    other.tail_->next_ = pos.node_;
    other.head_ = nullptr;
    other.tail_ = nullptr;
    size_ += other.size_;
    other.size_ = 0;
  }

  template< class T >
  void List< T >::splice(ConstIterator< T > pos, List< T > &&other)
  {
    splice(pos, other);
  }
}
#endif
