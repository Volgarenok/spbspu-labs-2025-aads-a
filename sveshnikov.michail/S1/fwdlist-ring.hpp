#ifndef FWDLIST_RING_HPP
#define FWDLIST_RING_HPP
#include <cassert>
#include <initializer_list>
#include "fwd-iterator.hpp"
#include "fwd-citerator.hpp"

namespace sveshnikov
{
  template< typename T >
  class FwdList
  {
  public:
    template< typename InputIterator >
    using not_int = std::enable_if_t<
        !std::is_arithmetic< typename std::remove_cv< InputIterator >::type >::value >;

    explicit FwdList();
    FwdList(const FwdList< T > &fwdlst);
    FwdList(FwdList< T > &&fwdlst);
    explicit FwdList(size_t n, const T &val);
    template< typename InputIterator, typename = not_int< InputIterator > >
    FwdList(InputIterator first, InputIterator last);
    FwdList(std::initializer_list< T > il);
    ~FwdList();
    FwdList< T > &operator=(const FwdList< T > &fwdlst);
    FwdList< T > &operator=(FwdList< T > &&fwdlst) noexcept;
    FwdList< T > &operator=(std::initializer_list< T > il);

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
    template< class Predicate >
    void remove_if(Predicate pred) noexcept;

    void splice(ConstIterator< T > pos, FwdList< T > &fwdlst);
    void splice(ConstIterator< T > pos, FwdList< T > &&fwdlst);
    void splice(ConstIterator< T > pos, FwdList< T > &fwdlst, ConstIterator< T > i) noexcept;
    void splice(ConstIterator< T > pos, FwdList< T > &&fwdlst, ConstIterator< T > i) noexcept;
    void splice(ConstIterator< T > pos, FwdList< T > &fwdlst, ConstIterator< T > first,
        ConstIterator< T > last) noexcept;
    void splice(ConstIterator< T > pos, FwdList< T > &&fwdlst, ConstIterator< T > first,
        ConstIterator< T > last) noexcept;

    void assign(size_t n, const T &val);
    template< typename InputIterator, typename = not_int< InputIterator > >
    void assign(InputIterator first, InputIterator last);
    void assign(std::initializer_list< T > il);

    Iterator< T > insert(ConstIterator< T > pos, const T &val);
    Iterator< T > insert(ConstIterator< T > pos, T &&val);
    Iterator< T > insert(ConstIterator< T > pos, size_t n, const T &val);
    template< class InputIterator, typename = not_int< InputIterator > >
    Iterator< T > insert(ConstIterator< T > pos, InputIterator first, InputIterator last);
    Iterator< T > insert(ConstIterator< T > pos, std::initializer_list< T > il);

    Iterator< T > erase(ConstIterator< T > pos);
    Iterator< T > erase(ConstIterator< T > pos, ConstIterator< T > last);

  private:
    node_t< T > *head_;
    node_t< T > *tail_;
    size_t size_;
    template< typename InputIterator >
    void copy(InputIterator first, InputIterator last);
    void push_impl(node_t< T > *node);
    void reset();
    Iterator< T > insert_impl(ConstIterator< T > pos, FwdList< T > &&fwdlst);
  };

  template< typename T >
  FwdList< T >::FwdList():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< typename T >
  template< typename InputIterator >
  void sveshnikov::FwdList< T >::copy(InputIterator first, InputIterator last)
  {
    while (first != last)
    {
      push_back(*first);
      first++;
    }
  }

  template< typename T >
  FwdList< T >::FwdList(const FwdList< T > &fwdlst):
    FwdList()
  {
    if (!fwdlst.empty())
    {
      push_back(fwdlst.front());
      copy(++fwdlst.cbegin(), fwdlst.cend());
    }
  }

  template< typename T >
  FwdList< T >::FwdList(FwdList< T > &&fwdlst):
    head_(fwdlst.head_),
    tail_(fwdlst.tail_),
    size_(fwdlst.size_)
  {
    fwdlst.head_ = nullptr;
    fwdlst.tail_ = nullptr;
    fwdlst.size_ = 0;
  }

  template< typename T >
  FwdList< T >::FwdList(size_t n, const T &val):
    FwdList()
  {
    for (size_t size = 0; size < n; size++)
    {
      push_back(val);
    }
  }

  template< typename T >
  template< typename InputIterator, typename >
  FwdList< T >::FwdList(InputIterator first, InputIterator last):
    FwdList()
  {
    copy(first, last);
  }

  template< typename T >
  FwdList< T >::FwdList(std::initializer_list< T > il):
    FwdList(il.begin(), il.end())
  {}

  template< typename T >
  FwdList< T >::~FwdList()
  {
    clear();
  }

  template< typename T >
  FwdList< T > &FwdList< T >::operator=(const FwdList< T > &fwdlst)
  {
    FwdList new_list(fwdlst);
    swap(new_list);
    return *this;
  }

  template< typename T >
  FwdList< T > &FwdList< T >::operator=(FwdList< T > &&fwdlst) noexcept
  {
    clear();
    swap(fwdlst);
    return *this;
  }

  template< typename T >
  FwdList< T > &FwdList< T >::operator=(std::initializer_list< T > il)
  {
    FwdList new_list(il);
    swap(new_list);
    return *this;
  }

  template< typename T >
  Iterator< T > FwdList< T >::begin() noexcept
  {
    return Iterator< T >(head_);
  }

  template< typename T >
  ConstIterator< T > FwdList< T >::begin() const noexcept
  {
    return cbegin();
  }

  template< typename T >
  ConstIterator< T > FwdList< T >::cbegin() const noexcept
  {
    return ConstIterator< T >(head_);
  }

  template< typename T >
  Iterator< T > FwdList< T >::end() noexcept
  {
    return begin();
  }

  template< typename T >
  ConstIterator< T > FwdList< T >::end() const noexcept
  {
    return cbegin();
  }

  template< typename T >
  ConstIterator< T > FwdList< T >::cend() const noexcept
  {
    return cbegin();
  }

  template< typename T >
  Iterator< T > FwdList< T >::before_begin() noexcept
  {
    return Iterator< T >(tail_);
  }

  template< typename T >
  ConstIterator< T > FwdList< T >::before_begin() const noexcept
  {
    return cbefore_begin();
  }

  template< typename T >
  ConstIterator< T > FwdList< T >::cbefore_begin() const noexcept
  {
    return ConstIterator< T >(tail_);
  }

  template< typename T >
  T &FwdList< T >::front() noexcept
  {
    return const_cast< T & >(static_cast< const FwdList< T > & >(*this).front());
  }

  template< typename T >
  const T &FwdList< T >::front() const noexcept
  {
    assert(head_ != nullptr);
    return head_->data_;
  }

  template< typename T >
  T &FwdList< T >::back() noexcept
  {
    return const_cast< T & >(static_cast< const FwdList< T > & >(*this).back());
  }

  template< typename T >
  const T &FwdList< T >::back() const noexcept
  {
    assert(tail_ != nullptr);
    return tail_->data_;
  }

  template< typename T >
  bool FwdList< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t FwdList< T >::getSize() const noexcept
  {
    return size_;
  }

  template< typename T >
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

  template< typename T >
  void FwdList< T >::push_front(const T &val)
  {
    node_t< T > *node = new node_t< T >{val, head_};
    push_impl(node);
    head_ = node;
  }

  template< typename T >
  void FwdList< T >::push_front(T &&val)
  {
    node_t< T > *node = new node_t< T >{std::move(val), head_};
    push_impl(node);
    head_ = node;
  }

  template< typename T >
  void FwdList< T >::push_back(const T &val)
  {
    node_t< T > *node = new node_t< T >{val, head_};
    push_impl(node);
    tail_ = node;
  }

  template< typename T >
  void FwdList< T >::push_back(T &&val)
  {
    node_t< T > *node = new node_t< T >{std::move(val), head_};
    push_impl(node);
    tail_ = node;
  }

  template< typename T >
  void FwdList< T >::reset()
  {
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
  }

  template< typename T >
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

  template< typename T >
  void FwdList< T >::pop_back() noexcept
  {
    assert(!empty());
    node_t< T > *current = tail_;
    auto it = cbegin();
    while (it != cbefore_begin())
    {
      it++;
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

  template< typename T >
  void FwdList< T >::swap(FwdList< T > &fwdlst) noexcept
  {
    std::swap(head_, fwdlst.head_);
    std::swap(tail_, fwdlst.tail_);
    std::swap(size_, fwdlst.size_);
  }

  template< typename T >
  void FwdList< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_front();
    }
  }

  template< typename T >
  void FwdList< T >::remove(const T &val) noexcept
  {
    remove_if([val](const T &other)
    {
      return val == other;
    });
  }

  template< typename T >
  template< class Predicate >
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

  template< typename T >
  void FwdList< T >::splice(ConstIterator< T > pos, FwdList< T > &fwdlst)
  {
    splice(pos, std::move(fwdlst));
  }

  template< typename T >
  void FwdList< T >::splice(ConstIterator< T > pos, FwdList< T > &&fwdlst)
  {
    fwdlst.push_front(T());
    splice(pos, std::move(fwdlst), fwdlst.cbegin(), fwdlst.cbefore_begin());
    fwdlst.pop_front();
  }

  template< typename T >
  void FwdList< T >::splice(ConstIterator< T > pos, FwdList< T > &fwdlst,
      ConstIterator< T > i) noexcept
  {
    splice(pos, std::move(fwdlst), i);
  }

  template< typename T >
  void FwdList< T >::splice(ConstIterator< T > pos, FwdList< T > &&fwdlst,
      ConstIterator< T > i) noexcept
  {
    splice(pos, std::move(fwdlst), i, ++ConstIterator< T >(i.node_));
  }

  template< typename T >
  void FwdList< T >::splice(ConstIterator< T > pos, FwdList< T > &fwdlst, ConstIterator< T > first,
      ConstIterator< T > last) noexcept
  {
    splice(pos, std::move(fwdlst), first, last);
  }

  template< typename T >
  void FwdList< T >::splice(ConstIterator< T > pos, FwdList< T > &&fwdlst, ConstIterator< T > first,
      ConstIterator< T > last) noexcept
  {
    if (fwdlst.empty() || first == last)
    {
      return;
    }

    node_t< T > *curr_first = first.node_;
    node_t< T > *aft_first = curr_first->next_;
    node_t< T > *curr_last = last.node_;
    node_t< T > *aft_last = curr_last->next_;
    size_t size_section = 0;
    auto aft_first_it = ConstIterator< T >(aft_first);
    auto aft_last_it = ConstIterator< T >(aft_last);

    for (ConstIterator< T > it = aft_first_it; it != aft_last_it; it++)
    {
      size_section++;
      if (it == fwdlst.cbegin())
      {
        fwdlst.head_ = aft_last;
      }
      else if (it == fwdlst.cbefore_begin())
      {
        fwdlst.tail_ = curr_first;
      }
    }
    curr_first->next_ = aft_last;
    fwdlst.size_ -= size_section;

    if (empty())
    {
      curr_last->next_ = aft_first;
      head_ = aft_first;
    }
    else
    {
      node_t< T > *curr = pos.node_;
      curr_last->next_ = curr->next_;
      curr->next_ = aft_first;
    }
    if (empty() || getSize() == 1 || pos == cbefore_begin())
    {
      tail_ = curr_last;
    }
    size_ += size_section;
  }

  template< typename T >
  void FwdList< T >::assign(size_t n, const T &val)
  {
    FwdList< T > new_list(n, val);
    swap(new_list);
  }

  template< typename T >
  template< typename InputIterator, typename >
  void FwdList< T >::assign(InputIterator first, InputIterator last)
  {
    FwdList< T > new_list(first, last);
    swap(new_list);
  }

  template< typename T >
  void FwdList< T >::assign(std::initializer_list< T > il)
  {
    *this = il;
  }

  template< typename T >
  Iterator< T > FwdList< T >::insert_impl(ConstIterator< T > pos, FwdList< T > &&fwdlst)
  {
    if (fwdlst.empty())
    {
      return Iterator< T >(pos.node_);
    }
    auto last = fwdlst.before_begin();
    splice(pos, std::move(fwdlst));
    return last;
  }

  template< typename T >
  Iterator< T > FwdList< T >::insert(ConstIterator< T > pos, const T &val)
  {
    try
    {
      FwdList temp = {val};
      return insert_impl(pos, std::move(temp));
    }
    catch (const std::bad_alloc &e)
    {
      return Iterator< T >(pos.node_);
    }
  }

  template< typename T >
  Iterator< T > FwdList< T >::insert(ConstIterator< T > pos, T &&val)
  {
    try
    {
      FwdList temp = {std::move(val)};
      return insert_impl(pos, std::move(temp));
    }
    catch (const std::bad_alloc &e)
    {
      return Iterator< T >(pos.node_);
    }
  }

  template< typename T >
  Iterator< T > FwdList< T >::insert(ConstIterator< T > pos, size_t n, const T &val)
  {
    try
    {
      FwdList temp(n, val);
      return insert_impl(pos, std::move(temp));
    }
    catch (const std::bad_alloc &e)
    {
      return Iterator< T >(pos.node_);
    }
  }

  template< typename T >
  template< class InputIterator, typename >
  Iterator< T > FwdList< T >::insert(ConstIterator< T > pos, InputIterator first,
      InputIterator last)
  {
    try
    {
      FwdList temp(first, last);
      return insert_impl(pos, std::move(temp));
    }
    catch (const std::bad_alloc &e)
    {
      return Iterator< T >(pos.node_);
    }
  }

  template< typename T >
  Iterator< T > FwdList< T >::insert(ConstIterator< T > pos, std::initializer_list< T > il)
  {
    return insert(pos, il.begin(), il.end());
  }

  template< typename T >
  Iterator< T > FwdList< T >::erase(ConstIterator< T > pos)
  {
    node_t< T > *current = pos.node_;
    node_t< T > *aft_curr = current->next_;
    current->next_ = aft_curr->next_;
    if (head_ == aft_curr)
    {
      head_ = aft_curr->next_;
    }
    if (tail_ == aft_curr)
    {
      tail_ = current;
    }
    delete aft_curr;
    size_--;
    if (empty())
    {
      reset();
      return begin();
    }
    return Iterator< T >(current->next_);
  }

  template< typename T >
  Iterator< T > FwdList< T >::erase(ConstIterator< T > pos, ConstIterator< T > last)
  {
    auto it_last = begin();
    while (pos.node_->next_ != last.node_)
    {
      it_last = erase(pos);
    }
    return it_last;
  }
}

#endif
