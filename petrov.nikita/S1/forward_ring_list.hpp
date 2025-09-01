#ifndef FORWARD_RING_LIST_HPP
#define FORWARD_RING_LIST_HPP

#include <memory>
#include <cstddef>
#include <cassert>
#include <iterator>
#include <functional>
#include <initializer_list>


namespace petrov
{
  template< typename T >
  struct ListNode
  {
    T data;
    ListNode< T > * next;
    template< class... Args >
    ListNode(ListNode< T > * next, Args && ... args);
  };

  template< typename T >
  struct ForwardRingList;

  template< typename T >
  struct ConstForwardListIterator: std::iterator< std::forward_iterator_tag, T >
  {
    friend struct ForwardRingList< T >;
  public:
    using node_t = ListNode< T >;
    using this_t = ConstForwardListIterator< T >;
    ConstForwardListIterator();
    ConstForwardListIterator(const this_t & rhs) = default;
    ~ConstForwardListIterator() = default;
    this_t & operator=(const this_t & rhs) = default;
    this_t & operator++();
    this_t operator++(int);
    const T & operator*() const;
    const T * operator->() const;
    bool operator==(const this_t & rhs) const;
    bool operator!=(const this_t & rhs) const;
  private:
    ListNode< T > * node_;
    explicit ConstForwardListIterator(node_t * node);
  };

  template< typename T >
  struct ForwardListIterator: std::iterator< std::forward_iterator_tag, T >
  {
    friend struct ForwardRingList< T >;
  public:
    using node_t = ListNode< T >;
    using this_t = ForwardListIterator< T >;
    ForwardListIterator();
    ForwardListIterator(const this_t & rhs) = default;
    ~ForwardListIterator() = default;
    this_t & operator=(const this_t & rhs) = default;
    this_t & operator++();
    this_t operator++(int);
    T & operator*();
    T * operator->();
    bool operator==(const this_t & rhs) const;
    bool operator!=(const this_t & rhs) const;
  private:
    ListNode< T > * node_;
    explicit ForwardListIterator(node_t * node);
  };

  template< typename T >
  struct ForwardRingList
  {
    using this_t = ForwardRingList< T >;
    using node_t = ListNode< T >;
    using const_it_t = ConstForwardListIterator< T >;
    using it_t = ForwardListIterator< T >;

    ForwardRingList();
    ForwardRingList(const this_t & rhs);
    ForwardRingList(this_t && rhs);
    ForwardRingList(size_t n, const T & val);
    ForwardRingList(std::initializer_list< T > il);
    template < class InputIterator, class = typename std::iterator_traits< InputIterator >::value_type >
    ForwardRingList(InputIterator first, InputIterator last);
    ~ForwardRingList();

    this_t & operator=(const this_t & rhs);
    this_t & operator=(this_t && rhs);
    this_t & operator=(std::initializer_list< T > il);
    bool operator==(const this_t & rhs) const;
    bool operator!=(const this_t & rhs) const;
    bool operator<(const this_t & rhs) const;
    bool operator>(const this_t & rhs) const;
    bool operator<=(const this_t & rhs) const;
    bool operator>=(const this_t & rhs) const;

    const_it_t cbegin() const;
    const_it_t cend() const;
    it_t begin();
    it_t end();

    T & front();
    const T & front() const;
    T & back();
    const T & back() const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void assign(size_t n, const T & val);
    void assign(std::initializer_list< T > il);
    template< class InputIterator, class = typename std::iterator_traits< InputIterator >::value_type >
    void assign(InputIterator first, InputIterator last);

    template< typename T1 >
    void push_front(T1 && val);
    template< class... Args >
    it_t emplace_after(const_it_t pos, Args && ... args);
    template< class... Args >
    void emplace_front(Args && ... args);
    template< typename T1 >
    it_t insert(const_it_t pos, T1 && val);
    it_t insert(const_it_t pos, size_t n, const T & val);
    template< class InputIterator >
    it_t insert(const_it_t pos, InputIterator first, InputIterator last);
    it_t insert(const_it_t pos, std::initializer_list< T > il);
    template< class ThisType >
    void splice(const_it_t pos, ThisType && rhs);
    template< class ThisType >
    void splice(const_it_t pos, ThisType && rhs, const_it_t i);
    template< class ThisType >
    void splice(const_it_t pos, ThisType && rhs, const_it_t first, const_it_t last);
    template< typename ThisType >
    void merge(ThisType && rhs);
    template< typename ThisType, typename Compare >
    void merge(ThisType && rhs, Compare comp);

    void pop_front();
    it_t erase(const_it_t pos);
    it_t erase(const_it_t pos, const_it_t last);
    void remove(const T & val);
    template< typename Cond >
    void remove_if(Cond cond);
    void clear() noexcept;

    void reverse();
    void sort();
    template< typename Compare >
    void sort(Compare comp);

    void unique();
    template< typename BinPred >
    void unique(BinPred binary_pred);

    void swap(this_t & rhs) noexcept;

  private:
    node_t * head_;
    node_t * tail_;
    size_t size_;
    template< typename T1 >
    void push_back(T1 && val);
    template< class... Args >
    void pushBackImpl(Args && ... args);
    template< typename Compare >
    void sortImpl(node_t * low, node_t * high, Compare comp);
  };

  template< typename T >
  template< class... Args >
  ListNode< T >::ListNode(ListNode< T > * next, Args && ... args):
    data{T(std::forward< Args >(args)...)},
    next(next)
  {}

  template< typename T >
  ConstForwardListIterator< T >::ConstForwardListIterator():
    node_(nullptr)
  {}

  template< typename T >
  typename ConstForwardListIterator< T >::this_t & ConstForwardListIterator< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }

  template< typename T >
  typename ConstForwardListIterator< T >::this_t ConstForwardListIterator< T >::operator++(int)
  {
    assert(node_ != nullptr);
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  const T & ConstForwardListIterator< T >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< typename T >
  const T * ConstForwardListIterator< T >::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename T >
  bool ConstForwardListIterator< T >::operator==(const this_t & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool ConstForwardListIterator< T >::operator!=(const this_t & rhs) const
  {
    return !(rhs == *this);
  }

  template< typename T >
  ConstForwardListIterator< T >::ConstForwardListIterator(node_t * node):
    node_(node)
  {}

  template< typename T >
  ForwardListIterator< T >::ForwardListIterator():
    node_(nullptr)
  {}

  template< typename T >
  typename ForwardListIterator< T >::this_t & ForwardListIterator< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }

  template< typename T >
  typename ForwardListIterator< T >::this_t ForwardListIterator< T >::operator++(int)
  {
    assert(node_ != nullptr);
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  T & ForwardListIterator< T >::operator*()
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template< typename T >
  T * ForwardListIterator< T >::operator->()
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data);
  }

  template< typename T >
  bool ForwardListIterator< T >::operator==(const this_t & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool ForwardListIterator< T >::operator!=(const this_t & rhs) const
  {
    return !(rhs == *this);
  }

  template< typename T >
  ForwardListIterator< T >::ForwardListIterator(node_t * node):
    node_(node)
  {}

  template< typename T >
  ForwardRingList< T >::ForwardRingList():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< typename T >
  ForwardRingList< T >::ForwardRingList(const this_t & rhs):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    if (rhs.empty())
    {
      return;
    }
    auto it = rhs.cbegin();
    try
    {
      do
      {
        push_back(*it);
      }
      while (it++ != rhs.cend());
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template< typename T >
  ForwardRingList< T >::ForwardRingList(this_t && rhs):
    head_(rhs.head_),
    tail_(rhs.tail_),
    size_(rhs.size_)
  {
    rhs.head_ = nullptr;
    rhs.tail_ = nullptr;
  }

  template< typename T >
  ForwardRingList< T >::ForwardRingList(size_t n, const T & val):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    try
    {
      for (size_t i = 0; i < n; i++)
      {
        push_front(val);
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template< typename T >
  ForwardRingList< T >::ForwardRingList(std::initializer_list< T > il):
    ForwardRingList(il.begin(), il.end())
  {}

  template< typename T >
  template < class InputIterator, class >
  ForwardRingList< T >::ForwardRingList(InputIterator first, InputIterator last):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    try
    {
      for (auto it = first; it != last; ++it)
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
  ForwardRingList< T >::~ForwardRingList()
  {
    clear();
  }

  template< typename T >
  typename ForwardRingList< T >::this_t & ForwardRingList< T >::operator=(const this_t & rhs)
  {
    this_t cpy(rhs);
    swap(cpy);
    return *this;
  }

  template< typename T >
  typename ForwardRingList< T >::this_t & ForwardRingList< T >::operator=(this_t && rhs)
  {
    clear();
    head_ = rhs.head_;
    tail_ = rhs.tail_;
    size_ = rhs.size_;
    rhs.head_ = nullptr;
    rhs.tail_ = nullptr;
    rhs.size_ = 0;
    return *this;
  }

  template< typename T >
  typename ForwardRingList< T >::this_t & ForwardRingList< T >::operator=(std::initializer_list< T > il)
  {
    this_t cpy(il);
    swap(cpy);
    return *this;
  }

  template< typename T >
  bool ForwardRingList< T >::operator==(const this_t & rhs) const
  {
    if (empty() && rhs.empty())
    {
      return true;
    }
    else
    {
      if (size() == rhs.size() && size() != 0)
      {
        auto it = cbegin();
        auto rhs_it = rhs.cbegin();
        do
        {
          if (*it != *rhs_it)
          {
            return false;
          }
          ++rhs_it;
        }
        while (it++ != cend());
        return true;
      }
      else
      {
        return false;
      }
    }
  }

  template< typename T >
  bool ForwardRingList< T >::operator!=(const this_t & rhs) const
  {
    return !(*this == rhs);
  }

  template< typename T >
  bool ForwardRingList< T >::operator<(const this_t & rhs) const
  {
    if ((empty() && rhs.empty()) || rhs.empty())
    {
      return false;
    }
    else if (empty())
    {
      return true;
    }
    auto lhs_first_it = cbegin();
    auto lhs_last_it = cend();
    auto rhs_first_it = rhs.cbegin();
    auto rhs_last_it = rhs.cend();
    do
    {
      if (*rhs_first_it < *lhs_first_it)
      {
        return false;
      }
      else if (*lhs_first_it < *rhs_first_it)
      {
        return true;
      }
    }
    while (lhs_first_it++ != lhs_last_it && rhs_first_it++ != rhs_last_it);
    return (rhs_first_it != rhs_last_it && rhs_first_it != rhs.cbegin());
  }

  template< typename T >
  bool ForwardRingList< T >::operator>(const this_t & rhs) const
  {
    return rhs.operator<(*this);
  }

  template< typename T >
  bool ForwardRingList< T >::operator<=(const this_t & rhs) const
  {
    return !(operator>(rhs));
  }

  template< typename T >
  bool ForwardRingList< T >::operator>=(const this_t & rhs) const
  {
    return !(operator<(rhs));
  }

  template< typename T >
  typename ForwardRingList< T >::const_it_t ForwardRingList< T >::cbegin() const
  {
    return const_it_t(head_);
  }

  template< typename T >
  typename ForwardRingList< T >::const_it_t ForwardRingList< T >::cend() const
  {
    return const_it_t(tail_);
  }

  template< typename T >
  typename ForwardRingList< T >::it_t ForwardRingList< T >::begin()
  {
    return it_t(head_);
  }

  template< typename T >
  typename ForwardRingList< T >::it_t ForwardRingList< T >::end()
  {
    return it_t(tail_);
  }

  template< typename T >
  T & ForwardRingList< T >::front()
  {
    return const_cast< T >(static_cast< const ForwardRingList< T > * >(this)->front());
  }

  template< typename T >
  const T & ForwardRingList< T >::front() const
  {
    return head_->data;
  }

  template< typename T >
  T & ForwardRingList< T >::back()
  {
    return const_cast< T >(static_cast< const ForwardRingList< T > * >(this)->back());
  }

  template< typename T >
  const T & ForwardRingList< T >::back() const
  {
    return tail_->data;
  }

  template< typename T >
  bool ForwardRingList< T >::empty() const noexcept
  {
    return !size_;
  }

  template< typename T >
  size_t ForwardRingList< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  void ForwardRingList< T >::assign(size_t n, const T & val)
  {
    this_t cpy(n, val);
    swap(cpy);
  }

  template< typename T >
  void ForwardRingList< T >::assign(std::initializer_list< T > il)
  {
    this_t cpy(il);
    swap(cpy);
  }

  template< typename T >
  template< class InputIterator, class >
  void ForwardRingList< T >::assign(InputIterator first, InputIterator last)
  {
    this_t cpy(first, last);
    swap(cpy);
  }

  template< typename T >
  template< typename T1 >
  void ForwardRingList< T >::push_front(T1 && val)
  {
    emplace_front(std::forward< T1 >(val));
  }

  template< typename T >
  template< class... Args >
  typename ForwardRingList< T >::it_t ForwardRingList< T >::emplace_after(const_it_t pos, Args && ... args)
  {
    if (pos.node_ != tail_)
    {
      auto temp = pos.node_->next;
      pos.node_->next = new node_t{ temp, std::forward< Args >(args)... };
    }
    else
    {
      tail_->next = new node_t{ head_, std::forward< Args >(args)... };
      tail_ = tail_->next;
    }
    size_++;
    return it_t(pos.node_->next);
  }

  template< typename T >
  template< class... Args >
  void ForwardRingList< T >::emplace_front(Args && ... args)
  {
    if (empty())
    {
      head_ = new node_t{ nullptr, std::forward< Args >(args)... };
      tail_ = head_;
      tail_->next = head_;
    }
    else
    {
      tail_->next = new node_t{ head_, std::forward< Args >(args)... };
      head_ = tail_->next;
    }
    size_++;
  }

  template< typename T >
  template< typename T1 >
  typename ForwardRingList< T >::it_t ForwardRingList< T >::insert(const_it_t pos, T1 && val)
  {
    return emplace_after(pos, std::forward< T >(val));
  }

  template< typename T >
  typename ForwardRingList< T >::it_t ForwardRingList< T >::insert(const_it_t pos, size_t n, const T & val)
  {
    if (!n)
    {
      return it_t(pos.node_);
    }
    this_t to_splice(n, val);
    it_t ret_it(to_splice.tail_);
    splice(pos, to_splice);
    return ret_it;
  }

  template< typename T >
  template< class InputIterator >
  typename ForwardRingList< T >::it_t ForwardRingList< T >::insert(const_it_t pos, InputIterator first, InputIterator last)
  {
    if (first == last)
    {
      return it_t(pos.node_);
    }
    this_t to_splice(first, last);
    it_t ret_it(to_splice.tail_);
    splice(pos, to_splice);
    return ret_it;
  }

  template< typename T >
  typename ForwardRingList< T >::it_t ForwardRingList< T >::insert(const_it_t pos, std::initializer_list< T > il)
  {
    return insert(pos, il.begin(), il.end());
  }

  template< typename T >
  template< class ThisType >
  void ForwardRingList< T >::splice(const_it_t pos, ThisType && rhs)
  {
    if (!rhs.empty())
    {
      if (pos.node_ != tail_)
      {
        auto temp = pos.node_->next;
        pos.node_->next = rhs.head_;
        rhs.tail_->next = temp;
      }
      else
      {
        pos.node_->next = rhs.head_;
        rhs.tail_->next = head_;
        tail_ = rhs.tail_;
      }
      size_ += rhs.size_;
      rhs.head_ = nullptr;
      rhs.tail_ = nullptr;
      rhs.size_ = 0;
    }
  }

  template< typename T >
  template< class ThisType >
  void ForwardRingList< T >::splice(const_it_t pos, ThisType && rhs, const_it_t i)
  {
    auto added = i.node_->next;
    if (added == rhs.head_)
    {
      rhs.tail_->next = rhs.head_->next;
      rhs.head_ = rhs.tail_->next;
    }
    else if (added == rhs.tail_)
    {
      rhs.tail_ = i.node_;
      rhs.tail_->next = rhs.head_;
    }
    else
    {
      i.node_->next = added->next;
    }
    rhs.size_--;
    if (rhs.empty())
    {
      rhs.head_ = nullptr;
      rhs.tail_ = nullptr;
    }
    if (pos.node_ != tail_)
    {
      auto temp = pos.node_->next;
      pos.node_->next = added;
      added->next = temp;
    }
    else
    {
      pos.node_->next = added;
      added->next = head_;
      tail_ = added;
    }
    size_++;
  }

  template< typename T >
  template< class ThisType >
  void ForwardRingList< T >::splice(const_it_t pos, ThisType && rhs, const_it_t first, const_it_t last)
  {
    if (first.node_->next == last.node_)
    {
      return;
    }
    size_t nodes_removed = 0;
    bool is_head_removed = false;
    bool is_tail_removed = false;
    auto added_head = first.node_->next;
    if (added_head == rhs.head_)
    {
      is_head_removed = true;
    }
    else if (added_head == rhs.tail_)
    {
      is_tail_removed = true;
    }
    nodes_removed++;
    auto added_tail = added_head;
    while (added_tail->next != last.node_)
    {
      added_tail = added_tail->next;
      if (added_tail == rhs.head_)
      {
        is_head_removed = true;
      }
      else if (added_tail == rhs.tail_)
      {
        is_tail_removed = true;
      }
      nodes_removed++;
    }
    first.node_->next = last.node_;
    if (is_head_removed)
    {
      rhs.head_ = last.node_;
    }
    if (is_tail_removed)
    {
      rhs.tail_ = first.node_;
    }
    rhs.size_ -= nodes_removed;
    if (pos.node_ != tail_)
    {
      auto temp = pos.node_->next;
      pos.node_->next = added_head;
      added_tail->next = temp;
    }
    else
    {
      pos.node_->next = added_head;
      added_tail->next = head_;
      tail_ = added_tail;
    }
    size_ += nodes_removed;
  }

  template< typename T >
  template< typename ThisType >
  void ForwardRingList< T >::merge(ThisType && rhs)
  {
    merge(std::forward< ThisType >(rhs), std::less_equal< T >{});
  }

  template< typename T >
  template< typename ThisType, typename Compare >
  void ForwardRingList< T >::merge(ThisType && rhs, Compare comp)
  {
    if (rhs.empty())
    {
      return;
    }
    auto rhs_it = rhs.begin();
    if (comp(*rhs_it, *begin()))
    {
      auto temp = head_;
      head_ = (rhs_it++).node_;
      head_->next = temp;
      tail_->next = head_;
      rhs.size_--;
      size_++;
    }
    auto prev_it = begin();
    auto lhs_it = ++begin();
    while (!rhs.empty())
    {
      if (comp(*rhs_it, *lhs_it))
      {
        prev_it.node_->next = (rhs_it++).node_;
        prev_it.node_->next->next = lhs_it.node_;
        ++prev_it;
        rhs.size_--;
        size_++;
        continue;
      }
      else if (lhs_it.node_ == tail_)
      {
        tail_->next = (rhs_it++).node_;
        tail_ = tail_->next;
        tail_->next = head_;
        rhs.size_--;
        size_++;
      }
      ++prev_it;
      ++lhs_it;
    }
    rhs.head_ = nullptr;
    rhs.tail_ = nullptr;
  }

  template< typename T >
  void ForwardRingList< T >::pop_front()
  {
    if (empty())
    {
      return;
    }
    auto todelete = head_;
    head_ = head_->next;
    delete todelete;
    size_--;
    if (!size_)
    {
      head_ = nullptr;
      tail_ = nullptr;
    }
    else
    {
      tail_->next = head_;
    }
  }

  template< typename T >
  typename ForwardRingList< T >::it_t ForwardRingList< T >::erase(const_it_t pos)
  {
    if (pos.node_ == pos.node_->next)
    {
      return it_t(pos.node_);
    }
    else if (pos.node_->next == tail_)
    {
      auto todelete = pos.node_->next;
      pos.node_->next = head_;
      tail_ = pos.node_;
      delete todelete;
    }
    else if (pos.node_->next == head_)
    {
      auto todelete = pos.node_->next;
      pos.node_->next = head_->next;
      head_ = pos.node_->next;
      delete todelete;
    }
    else
    {
      auto todelete = pos.node_->next;
      pos.node_->next = todelete->next;
      delete todelete;
    }
    size_--;
    return it_t(pos.node_->next);
  }

  template< typename T >
  typename ForwardRingList< T >::it_t ForwardRingList< T >::erase(const_it_t pos, const_it_t last)
  {
    if (pos.node_ == last.node_ || pos.node_->next == last.node_)
    {
      return it_t(pos.node_);
    }
    const_it_t it(pos.node_->next);
    bool is_head_removed = false;
    bool is_tail_removed = false;
    while (it != last)
    {
      auto todelete = (it++).node_;
      if (todelete == head_)
      {
        is_head_removed = true;
      }
      else if (todelete == tail_)
      {
        is_tail_removed = true;
      }
      delete todelete;
      size_--;
    }
    pos.node_->next = last.node_;
    if (is_head_removed)
    {
      head_ = last.node_;
    }
    if (is_tail_removed)
    {
      tail_ = pos.node_;
    }
    return it_t(last.node_);
  }

  template< typename T >
  void ForwardRingList< T >::remove(const T & val)
  {
    using namespace std::placeholders;
    remove_if(std::bind(std::equal_to< T >{}, _1, val));
  }

  template< typename T >
  template< typename Cond >
  void ForwardRingList< T >::remove_if(Cond cond)
  {
    if (empty())
    {
      return;
    }
    auto prev = head_;
    auto curr = head_->next;
    while (curr != tail_)
    {
      if (cond(curr->data))
      {
        auto todelete = curr;
        curr = todelete->next;
        prev->next = curr;
        delete todelete;
        size_--;
      }
      else
      {
        curr = curr->next;
        prev = prev->next;
      }
    }
    if (cond(tail_->data))
    {
      auto todelete = tail_;
      tail_ = prev;
      tail_->next = head_;
      delete todelete;
      size_--;
    }
    if (size_ && cond(head_->data))
    {
      auto todelete = head_;
      head_ = todelete->next;
      tail_->next = head_;
      delete todelete;
      size_--;
    }
    if (!size_)
    {
      head_ = nullptr;
      tail_ = nullptr;
    }
  }

  template< typename T >
  void ForwardRingList< T >::clear() noexcept
  {
    if (empty())
    {
      return;
    }
    while (head_ != tail_)
    {
      auto todelete = head_;
      head_ = head_->next;
      delete todelete;
    }
    delete head_;
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
  }

  template< typename T >
  void ForwardRingList< T >::reverse()
  {
    if (empty())
    {
      return;
    }
    auto prev = tail_;
    auto curr = head_;
    auto next = head_->next;
    while (curr != tail_)
    {
      curr->next = prev;
      prev = curr;
      curr = next;
      next = next->next;
    }
    curr->next = prev;
    auto temp = head_;
    head_ = tail_;
    tail_ = temp;
  }

  template< typename T >
  void ForwardRingList< T >::sort()
  {
    sortImpl(head_, tail_, std::less_equal< T >{});
  }

  template< typename T >
  template< typename Compare >
  void ForwardRingList< T >::sort(Compare comp)
  {
    sortImpl(head_, tail_, comp);
  }

  template< typename T >
  void ForwardRingList< T >::unique()
  {
    unique(std::equal_to< T >{});
  }

  template< typename T >
  template< typename BinPred >
  void ForwardRingList< T >::unique(BinPred binary_pred)
  {
    if (empty() || size() == 1)
    {
      return;
    }
    auto prev = head_;
    auto subhead = head_->next;
    while (subhead != tail_)
    {
      if (binary_pred(subhead->data, prev->data))
      {
        auto todelete = subhead;
        prev->next = todelete->next;
        delete todelete;
        size_--;
        subhead = prev->next;
      }
      else
      {
        prev = prev->next;
        subhead = subhead->next;
      }
    }
    if (binary_pred(tail_->data, prev->data))
    {
      auto todelete = tail_;
      tail_ = prev;
      tail_->next = head_;
      delete todelete;
      size_--;
    }
  }

  template< typename T >
  void ForwardRingList< T >::swap(this_t & rhs) noexcept
  {
    std::swap(head_, rhs.head_);
    std::swap(tail_, rhs.tail_);
    std::swap(size_, rhs.size_);
  }

  template< typename T >
  template< typename T1 >
  void ForwardRingList< T >::push_back(T1 && val)
  {
    pushBackImpl(std::forward< T1 >(val));
  }

  template< typename T >
  template< class... Args >
  void ForwardRingList< T >::pushBackImpl(Args && ... args)
  {
    if (empty())
    {
      head_ = new node_t{ nullptr, std::forward< Args >(args)... };
      tail_ = head_;
      tail_->next = head_;
    }
    else
    {
      tail_->next = new node_t{ head_, std::forward< Args >(args)... };
      tail_ = tail_->next;
    }
    size_++;
  }

  template< typename T >
  template< typename Compare >
  void ForwardRingList< T >::sortImpl(node_t * low, node_t * high, Compare comp)
  {
    if (low != high && !(high != tail_ && high->next == low))
    {
      node_t fake(low, T());
      node_t * i = &fake;
      T pivot = high->data;
      for (auto j = low; j != high; j = j->next)
      {
        if (comp(j->data, pivot))
        {
          std::swap(i->next->data, j->data);
          i = i->next;
        }
      }
      std::swap(i->next->data, high->data);
      if (i->next == high)
      {
        sortImpl(low, i, comp);
        sortImpl(i->next, high, comp);
      }
      else
      {
        sortImpl(low, i, comp);
        sortImpl(i->next->next, high, comp);
      }
    }
  }
}

#endif

