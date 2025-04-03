#ifndef FORWARD_RING_LIST_2_HPP
#define FORWARD_RING_LIST_2_HPP

#include <cstddef>
#include <memory>
#include <cassert>

namespace petrov
{
  template< typename T >
  struct ListNode
  {
    T data_;
    ListNode< T > * next_;
    ListNode() = default;
  };

  template< typename T >
  struct ConstForwardListIterator
  {
    const ListNode< T > * node_;
    using node_t = ListNode< T >;
    using this_t = ConstForwardListIterator< T >;
    ConstForwardListIterator(): node_(nullptr) {}
    ConstForwardListIterator(const this_t & rhs) = default;
    ConstForwardListIterator(const node_t * node): node_(node) {}
    ~ConstForwardListIterator() = default;
    this_t & operator=(const this_t & rhs) = default;
    this_t & operator++();
    this_t operator++(int);
    const T & operator*() const;
    const T * operator->() const;
    bool operator==(const this_t & rhs) const;
    bool operator!=(const this_t & rhs) const;
  };

  template< typename T >
  struct ForwardListIterator
  {
    ListNode< T > * node_;
    using node_t = ListNode< T >;
    using this_t = ForwardListIterator< T >;
    ForwardListIterator(): node_(nullptr) {}
    ForwardListIterator(const this_t & rhs) = default;
    ForwardListIterator(node_t * node): node_(node) {}
    ~ForwardListIterator() = default;
    this_t & operator=(const this_t & rhs) = default;
    this_t & operator++();
    this_t operator++(int);
    T & operator*();
    T * operator->();
    bool operator==(const this_t & rhs) const;
    bool operator!=(const this_t & rhs) const;
  };

  template< typename T >
  struct ForwardRingList
  {
  public:
    using this_t = ForwardRingList< T >;
    using node_t = ListNode< T >;
    using const_it_t = ConstForwardListIterator< T >;
    using it_t = ForwardListIterator< T >;
    ForwardRingList():
      head_(nullptr),
      tail_(nullptr)
    {}
    ForwardRingList(const this_t & rhs);
    ForwardRingList(this_t && rhs);
    ~ForwardRingList();
    this_t & operator=(const this_t & rhs);
    this_t & operator=(this_t && rhs);
    bool operator==(const this_t & rhs) const;
    const_it_t cbegin() const;
    const_it_t cend() const;
    it_t begin();
    it_t end();
    T front() const;
    T back() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void push_front(const T & val);
    void push_front (T && val);
    void pop_front();
    void remove(const T & val);
    void clear();
    void reverse();
    void swap(this_t & rhs) noexcept;
  private:
    node_t * head_;
    node_t * tail_;
  };

  template< typename T >
  typename ConstForwardListIterator< T >::this_t & ConstForwardListIterator< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
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
    return node_->data_;
  }

  template< typename T >
  const T * ConstForwardListIterator< T >::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
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
  typename ForwardListIterator< T >::this_t & ForwardListIterator< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
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
    return node_->data_;
  }

  template< typename T >
  T * ForwardListIterator< T >::operator->()
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
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
  ForwardRingList< T >::ForwardRingList(const this_t & rhs):
    head_(nullptr),
    tail_(nullptr)
  {
    if (rhs.empty())
    {
      return;
    }
    auto it = rhs.cbegin();
    head_ = new node_t;
    head_->data_ = *it;
    auto subhead = head_;
    while (it++ != rhs.cend())
    {
      subhead->next_ = new node_t;
      subhead->next_->data_ = *it;
      subhead = subhead->next_;
    }
    subhead->next_ = head_;
    tail_ = subhead;
  }

  template< typename T >
  ForwardRingList< T >::ForwardRingList(this_t && rhs):
    head_(nullptr),
    tail_(nullptr)
  {
    if (rhs.empty())
    {
      return;
    }
    auto it = rhs.cbegin();
    head_ = new node_t;
    head_->data_ = *it;
    auto subhead = head_;
    while (it++ != rhs.cend())
    {
      subhead->next_ = new node_t;
      subhead->next_->data_ = *it;
      subhead = subhead->next_;
    }
    subhead->next_ = head_;
    tail_ = subhead;
  }

  template< typename T >
  ForwardRingList< T >::~ForwardRingList()
  {
    this->clear();
  }

  template< typename T >
  typename ForwardRingList< T >::this_t & ForwardRingList< T >::operator=(const this_t & rhs)
  {
    this_t cpy(rhs);
    this->swap(cpy);
    return *this;
  }

  template< typename T >
  typename ForwardRingList< T >::this_t & ForwardRingList< T >::operator=(this_t && rhs)
  {
    this->swap(rhs);
    return *this;
  }

  template< typename T >
  bool ForwardRingList< T >::operator==(const this_t & rhs) const
  {
    if (this->empty() && rhs.empty())
    {
      return true;
    }
    else
    {
      if (this->size() == rhs.size() && this->size() != 0)
      {
        auto it = this->cbegin();
        auto rhs_it = rhs.cbegin();
        do
        {
          if (*it != *rhs_it)
          {
            return false;
          }
          ++rhs_it;
        }
        while (it++ != this->cend());
        return true;
      }
      else if (this->size() == rhs.size() && this->size() == 0)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
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
  T ForwardRingList< T >::front() const
  {
    return head_->data_;
  }

  template< typename T >
  T ForwardRingList< T >::back() const
  {
    return tail_->data_;
  }

  template< typename T >
  bool ForwardRingList< T >::empty() const noexcept
  {
    return !this->head_;
  }

  template< typename T >
  size_t ForwardRingList< T >::size() const noexcept
  {
    if (this->empty())
    {
      return 0;
    }
    size_t count = 0;
    auto it = this->cbegin();
    do
    {
      ++count;
    }
    while (it++ != this->cend());
    return count;
  }

  template< typename T >
  void ForwardRingList< T >::push_front(const T & val)
  {
    if (this->empty())
    {
      head_ = new node_t;
      head_->data_ = val;
      tail_ = head_;
      head_->next_ = head_;
      tail_->next_ = head_;
    }
    else
    {
      auto temp = tail_->next_;
      tail_->next_ = new node_t;
      tail_->next_->data_ = val;
      head_ = tail_->next_;
      head_->next_ = temp;
    }
  }

  template< typename T >
  void ForwardRingList< T >::push_front(T && val)
  {
    if (this->empty())
    {
      head_ = new node_t;
      head_->data_ = std::move(val);
      tail_ = head_;
      head_->next_ = head_;
      tail_->next_ = head_;
    }
    else
    {
      auto temp = tail_->next_;
      tail_->next_ = new node_t;
      tail_->next_->data_ = std::move(val);
      head_ = tail_->next_;
      head_->next_ = temp;
    }
  }

  template< typename T >
  void ForwardRingList< T >::pop_front()
  {
    if (this->empty())
    {
      return;
    }
    size_t size = this->size();
    auto todelete = head_;
    head_ = head_->next_;
    delete todelete;
    size--;
    if (!size)
    {
      head_ = nullptr;
      tail_ = nullptr;
    }
    else
    {
      tail_->next_ = head_;
    }
  }

  template< typename T >
  void ForwardRingList< T >::remove(const T & val)
  {
    if (this->empty())
    {
      return;
    }
    if (this->size() == 1)
    {
      if (head_->data_ == val)
      {
        this->pop_front();
      }
      return;
    }
    size_t size = this->size();
    size_t count = 0;
    auto it = this->cbegin();
    do
    {
      if (*it == val)
      {
        count++;
      }
    }
    while (it++ != this->cend());
    if (count == size)
    {
      this->clear();
      return;
    }
    auto subhead = head_;
    auto prev_subhead = tail_;
    do
    {
      if (subhead->data_ == val)
      {
        auto todelete = subhead;
        subhead = todelete->next_;
        prev_subhead->next_ = subhead;
        if (todelete == head_)
        {
          head_ = subhead;
        }
        else if (todelete == tail_)
        {
          tail_ = prev_subhead;
        }
        delete todelete;
        todelete = nullptr;
      }
      else
      {
        prev_subhead = subhead;
        subhead = subhead->next_;
      }
    }
    while (subhead != head_);
  }

  template< typename T >
  void ForwardRingList< T >::clear()
  {
    if (this->empty())
    {
      return;
    }
    auto it = this->begin();
    while (it != this->end())
    {
      auto temp = it.node_->next_;
      delete it.node_;
      it.node_ = temp;
    }
    delete it.node_;
    head_ = nullptr;
    tail_ = nullptr;
  }

  template< typename T >
  void ForwardRingList< T >::reverse()
  {
    if (this->empty())
    {
      return;
    }
    auto prev = this->end();
    auto next = ++this->begin();
    auto it = this->begin();
    while (it != this->end())
    {
      it.node_->next_ = prev.node_;
      prev = it;
      it = next++;
    }
    it.node_->next_ = prev.node_;
    prev = it;
    auto temp = this->begin().node_;
    head_ = this->end().node_;
    tail_ = temp;
  }

  template< typename T >
  void ForwardRingList< T >::swap(this_t & rhs) noexcept
  {
    std::swap(head_, rhs.head_);
    std::swap(tail_, rhs.tail_);
  }
}

#endif
