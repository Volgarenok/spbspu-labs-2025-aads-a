#ifndef FWDLIST_RING_HPP
#define FWDLIST_RING_HPP
#include <cassert>
#include "fwd-iterator.hpp"
#include "fwd-citerator.hpp"

namespace
{
  template < typename T >
  struct Comparator
  {
  public:
    Comparator(const T &val):
      val_(val)
    {}

    bool operator()(const T &other) const
    {
      return val_ == other;
    }

  private:
    T val_;
  };
}

namespace sveshnikov
{
  template < typename T >
  class FwdList
  {
  public:
    explicit FwdList();
    explicit FwdList(size_t n, const T &val);
    FwdList(const FwdList< T > &fwdlst);
    FwdList(FwdList< T > &&fwdlst);
    ~FwdList();

    Iterator< T > begin() noexcept;
    ConstIterator< T > begin() const noexcept;
    ConstIterator< T > cbegin() const noexcept;
    Iterator< T > end() noexcept;
    ConstIterator< T > end() const noexcept;
    ConstIterator< T > cend() const noexcept;
    Iterator< T > before_begin() noexcept;
    ConstIterator< T > before_begin() const noexcept;
    ConstIterator< T > cbefore_begin() const noexcept;
    T &front() noexcept;
    const T &front() const noexcept;
    T &back() noexcept;
    const T &back() const noexcept;
    bool empty() const noexcept;
    size_t getSize() const noexcept;
    void push_front(const T &val);
    void push_front(T &&val);
    void push_back(const T &val);
    void push_back(T &&val);
    void pop_front() noexcept;
    void pop_back() noexcept;
    void swap(FwdList< T > &fwdlst) noexcept;
    void clear() noexcept;

    void remove(const T &val) noexcept;
    template <class Predicate>
    void remove_if(Predicate pred) noexcept;
    void splice(ConstIterator< T > pos, FwdList< T > &fwdlst) noexcept;
    void splice(ConstIterator< T > pos, FwdList< T > &&fwdlst) noexcept;
    void splice(ConstIterator< T > pos, FwdList< T > &fwdlst, ConstIterator< T > i) noexcept;
    void splice(ConstIterator< T > pos, FwdList< T > &&fwdlst, ConstIterator< T > i) noexcept;
    void splice(ConstIterator< T > pos, FwdList< T > &fwdlst,
        ConstIterator< T > first, ConstIterator< T > last) noexcept;
    void splice(ConstIterator< T > pos, FwdList< T > &&fwdlst,
        ConstIterator< T > first, ConstIterator< T > last) noexcept;
    void assign(size_t n, const T &val);

  private:
    node_t< T > *head_;
    node_t< T > *tail_;
    size_t size_;
    void push_impl(node_t< T > *node);
    void reset();
    node_t< T > *shiftPointer(ConstIterator< T > pos, node_t< T > *start_node);
  };

  template < typename T >
  FwdList< T >::FwdList():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template < typename T >
  FwdList< T >::FwdList(size_t n, const T &val):
    FwdList()
  {
    try
    {
      assign(n, val);
    }
    catch (std::bad_alloc &e)
    {
      clear();
      throw;
    }
  }

  template < typename T >
  FwdList< T >::FwdList(const FwdList< T > &fwdlst):
    FwdList()
  {
    if (!fwdlst.empty())
    {
      push_back(fwdlst.front());
      for (ConstIterator< T > it = ++fwdlst.cbegin(); it != fwdlst.cend(); it++)
      {
        try
        {
          push_back(*it);
        }
        catch (std::bad_alloc &e)
        {
          clear();
          throw;
        }
      }
    }
  }

  template < typename T >
  FwdList< T >::FwdList(FwdList< T > &&fwdlst):
    head_(fwdlst.head_),
    tail_(fwdlst.tail_),
    size_(fwdlst.size_)
  {
    fwdlst.head_ = nullptr;
    fwdlst.tail_ = nullptr;
    fwdlst.size_ = 0;
  }

  template < typename T >
  FwdList< T >::~FwdList()
  {
    clear();
  }

  template < typename T >
  Iterator< T > FwdList< T >::begin() noexcept
  {
    return Iterator< T >(head_);
  }

  template < typename T >
  ConstIterator< T > FwdList< T >::begin() const noexcept
  {
    return cbegin();
  }

  template < typename T >
  ConstIterator< T > FwdList< T >::cbegin() const noexcept
  {
    return ConstIterator< T >(head_);
  }

  template < typename T >
  Iterator< T > FwdList< T >::end() noexcept
  {
    return begin();
  }

  template < typename T >
  ConstIterator< T > FwdList< T >::end() const noexcept
  {
    return cbegin();
  }

  template < typename T >
  ConstIterator< T > FwdList< T >::cend() const noexcept
  {
    return cbegin();
  }

  template < typename T >
  Iterator< T > FwdList< T >::before_begin() noexcept
  {
    return Iterator< T >(tail_);
  }

  template < typename T >
  ConstIterator< T > FwdList< T >::before_begin() const noexcept
  {
    return cbefore_begin();
  }

  template < typename T >
  ConstIterator< T > FwdList< T >::cbefore_begin() const noexcept
  {
    return ConstIterator< T >(tail_);
  }

  template < typename T >
  T &FwdList< T >::front() noexcept
  {
    return const_cast< T & >(static_cast< const FwdList< T > & >(*this).front());
  }

  template < typename T >
  const T &FwdList< T >::front() const noexcept
  {
    assert(head_ != nullptr);
    return head_->data_;
  }

  template < typename T >
  T &FwdList< T >::back() noexcept
  {
    return const_cast< T & >(static_cast< const FwdList< T > & >(*this).back());
  }

  template < typename T >
  const T &FwdList< T >::back() const noexcept
  {
    assert(tail_ != nullptr);
    return tail_->data_;
  }

  template < typename T >
  bool FwdList< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < typename T >
  size_t FwdList< T >::getSize() const noexcept
  {
    return size_;
  }

  template < typename T >
  void FwdList< T >::push_impl(node_t< T > *node)
  {
    if (empty())
    {
      node->next_ = node;
      tail_ = node;
      head_ = node;
    }
    else
    {
      tail_->next_ = node;
    }
    size_++;
  }

  template < typename T >
  void FwdList< T >::push_front(const T &val)
  {
    node_t< T > *node = new node_t< T >{val, head_};
    push_impl(node);
    head_ = node;
  }

  template < typename T >
  void FwdList< T >::push_front(T &&val)
  {
    node_t< T > *node = new node_t< T >{std::move(val), head_};
    push_impl(node);
    head_ = node;
  }

  template < typename T >
  void FwdList< T >::push_back(const T &val)
  {
    node_t< T > *node = new node_t< T >{val, head_};
    push_impl(node);
    tail_ = node;
  }

  template < typename T >
  void FwdList< T >::push_back(T &&val)
  {
    node_t< T > *node = new node_t< T >{std::move(val), head_};
    push_impl(node);
    tail_ = node;
  }

  template < typename T >
  void FwdList< T >::reset()
  {
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
  }

  template < typename T >
  void FwdList< T >::pop_front() noexcept
  {
    assert(!empty());
    node_t< T > *node = head_->next_;
    delete head_;
    size_--;
    if (empty())
    {
      reset();
    }
    else
    {
      head_ = node;
      tail_->next_ = head_;
    }
  }

  template < typename T >
  void FwdList< T >::pop_back() noexcept
  {
    assert(!empty());
    node_t< T > *current = tail_;
    for (ConstIterator< T > it = cbegin(); it != cbefore_begin(); it++)
    {
      current = current->next_;
    }
    delete tail_;
    size_--;
    if (empty())
    {
      reset();
    }
    else
    {
      tail_ = current;
      tail_->next_ = head_;
    }
  }

  template < typename T >
  void FwdList< T >::swap(FwdList< T > &fwdlst) noexcept
  {
    std::swap(head_, fwdlst.head_);
    std::swap(tail_, fwdlst.tail_);
    std::swap(size_, fwdlst.size_);
  }

  template < typename T >
  void FwdList< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_front();
    }
  }

  template < typename T >
  void FwdList< T >::remove(const T &val) noexcept
  {
    remove_if(Comparator< T >(val));
  }

  template < typename T >
  template <class Predicate>
  void FwdList< T >::remove_if(Predicate pred) noexcept
  {
    if (empty())
    {
      return;
    }
    node_t< T > *prev = tail_;
    node_t< T > *current = head_;
    size_t count = size_;
    while (count > 0)
    {
      node_t< T > *next = current->next_;
      if (pred(current->data_))
      {
        if (current == head_)
        {
          head_ = next;
        }
        if (current == tail_)
        {
          tail_ = prev;
        }
        prev->next_ = next;
        delete current;
        size_--;
        if (size_ == 0)
        {
          reset();
        }
      }
      else
      {
        prev = current;
      }
      current = next;
      count--;
    }
  }

  template < typename T >
  node_t< T > *FwdList< T >::shiftPointer(ConstIterator< T > pos, node_t< T > *start_node)
  {
    ConstIterator< T > it = cbegin();
    while (it != pos)
    {
      it++;
      start_node = start_node->next_;
    }
    return start_node;
  }

  template < typename T >
  void FwdList< T >::splice(ConstIterator< T > pos, FwdList< T > &fwdlst) noexcept
  {
    splice(pos, std::move(fwdlst));
  }

  template < typename T >
  void FwdList< T >::splice(ConstIterator< T > pos, FwdList< T > &&fwdlst) noexcept
  {
    if (fwdlst.empty())
    {
      return;
    }
    if (empty())
    {
      swap(fwdlst);
      return;
    }
    node_t< T > *current = shiftPointer(pos, head_);
    fwdlst.tail_->next_ = current->next_;
    current->next_ = fwdlst.head_;
    if (pos == cbefore_begin())
    {
      tail_ = fwdlst.tail_;
    }
    size_ += fwdlst.size_;
    fwdlst.reset();
  }

  template < typename T >
  void FwdList< T >::splice(ConstIterator< T > pos, FwdList< T > &fwdlst, ConstIterator< T > i) noexcept
  {
    splice(pos, std::move(fwdlst), i);
  }

  template < typename T >
  void FwdList< T >::splice(ConstIterator< T > pos, FwdList< T > &&fwdlst, ConstIterator< T > i) noexcept
  {
    if (fwdlst.empty())
    {
      return;
    }
    node_t< T > *prev_i = fwdlst.shiftPointer(i, fwdlst.tail_);
    node_t< T > *node_i = prev_i->next_;
    if (fwdlst.size_ == 1)
    {
      fwdlst.reset();
    }
    else
    {
      prev_i->next_ = node_i->next_;
      fwdlst.size_--;
      if (node_i == fwdlst.head_)
      {
        fwdlst.head_ = node_i->next_;
      }
      else if (node_i == fwdlst.tail_)
      {
        fwdlst.tail_ = prev_i;
      }
    }

    if (empty())
    {
      head_ = node_i;
      tail_ = node_i;
      tail_->next_ = head_;
    }
    else
    {
      node_t< T > *current = shiftPointer(pos, head_);
      node_i->next_ = current->next_;
      current->next_ = node_i;
      if (pos == cbefore_begin())
      {
        tail_ = node_i;
      }
    }
    size_++;
  }

  template < typename T >
  void FwdList< T >::splice(ConstIterator< T > pos, FwdList< T > &fwdlst,
      ConstIterator< T > first, ConstIterator< T > last) noexcept
  {
    splice(pos, std::move(fwdlst), first, last);
  }

  template < typename T >
  void FwdList< T >::splice(ConstIterator< T > pos, FwdList< T > &&fwdlst,
      ConstIterator< T > first, ConstIterator< T > last) noexcept
  {
    ConstIterator< T > aft_first_it = first;
    aft_first_it++;
    if (fwdlst.empty() || first == last || last == aft_first_it)
    {
      return;
    }
    node_t< T > *curr_first = fwdlst.shiftPointer(first, fwdlst.head_);
    node_t< T > *aft_first = curr_first->next_;
    node_t< T > *prev_last = fwdlst.shiftPointer(last, fwdlst.tail_);
    node_t< T > *curr_last = prev_last->next_;
    size_t size_section = 0;
    for (ConstIterator< T > it = aft_first_it; it != last; it++)
    {
      size_section++;
      if (it == fwdlst.cbegin())
      {
        fwdlst.head_ = curr_last;
      }
      else if (it == fwdlst.cbefore_begin())
      {
        fwdlst.tail_ = curr_first;
      }
    }
    curr_first->next_ = curr_last;
    fwdlst.size_ -= size_section;

    if (empty())
    {
      prev_last->next_ = aft_first;
      head_ = aft_first;
    }
    else
    {
      node_t< T > *curr = shiftPointer(pos, head_);
      prev_last->next_ = curr->next_;
      curr->next_ = aft_first;
    }
    if (empty() || getSize() == 1 || pos == cbefore_begin())
    {
      tail_ = prev_last;
    }
    size_ += size_section;
  }

  template < typename T >
  void FwdList< T >::assign(size_t n, const T &val)
  {
    clear();
    for (size_t size = 0; size < n; size++)
    {
      push_back(val);
    }
  }
}

#endif
