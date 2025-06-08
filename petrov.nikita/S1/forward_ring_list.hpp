#ifndef FORWARD_RING_LIST_HPP
#define FORWARD_RING_LIST_HPP

#include <iterator>
#include <cstddef>
#include <memory>
#include <cassert>

namespace petrov
{
  template< typename T >
  struct ListNode
  {
    T data;
    ListNode< T > * next;
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
    ConstForwardListIterator():
      node_(nullptr)
    {}
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
    const ListNode< T > * node_;
    explicit ConstForwardListIterator(const node_t * node):
      node_(node)
    {}
  };

  template< typename T >
  struct ForwardListIterator: std::iterator< std::forward_iterator_tag, T >
  {
    friend struct ForwardRingList< T >;
  public:
    using node_t = ListNode< T >;
    using this_t = ForwardListIterator< T >;
    ForwardListIterator():
      node_(nullptr)
    {}
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
    explicit ForwardListIterator(node_t * node):
      node_(node)
    {}
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
      tail_(nullptr),
      size_(0)
    {}
    ForwardRingList(const this_t & rhs);
    ForwardRingList(this_t && rhs);
    ~ForwardRingList();
    this_t & operator=(const this_t & rhs);
    this_t & operator=(this_t && rhs);
    bool operator==(const this_t & rhs) const;
    bool operator!=(const this_t & rhs) const;
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
    template< typename U >
    void push_front (U && val);
    void pop_front();
    void remove(const T & val);
    void clear();
    void reverse();
    void swap(this_t & rhs) noexcept;
  private:
    node_t * head_;
    node_t * tail_;
    size_t size_;
  };

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
  ForwardRingList< T >::ForwardRingList(const this_t & rhs):
    head_(nullptr),
    tail_(nullptr),
    size_(rhs.size_)
  {
    if (rhs.empty())
    {
      return;
    }
    auto it = rhs.cbegin();
    head_ = new node_t{ *it, nullptr };
    head_->data = *it;
    auto subhead = head_;
    try
    {
      while (it++ != rhs.cend())
      {
        subhead->next = new node_t{ *it, nullptr };
        subhead = subhead->next;
      }
    }
    catch (...)
    {
      while (head_->next)
      {
        auto todelete = head_;
        head_ = todelete->next;
        delete todelete;
      }
      delete head_;
      throw;
    }
    subhead->next = head_;
    tail_ = subhead;
  }

  template< typename T >
  ForwardRingList< T >::ForwardRingList(this_t && rhs):
    head_(nullptr),
    tail_(nullptr),
    size_(rhs.size_)
  {
    swap(rhs);
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
      else if (size() == rhs.size() && size() == 0)
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
  bool ForwardRingList< T >::operator!=(const this_t & rhs) const
  {
    return !(*this == rhs);
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
    return const_cast< T >(static_cast< const T >(this)->front());
  }

  template< typename T >
  const T & ForwardRingList< T >::front() const
  {
    return head_->data;
  }

  template< typename T >
  T & ForwardRingList< T >::back()
  {
    return const_cast< T >(static_cast< const T >(this)->back());
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
  template< typename U >
  void ForwardRingList< T >::push_front(U && val)
  {
    if (empty())
    {
      head_ = new node_t{ val, head_ };
      tail_ = head_;
      tail_->next = head_;
      size_++;
    }
    else
    {
      auto temp = tail_->next;
      tail_->next = new node_t{ val, nullptr };
      head_ = tail_->next;
      head_->next = temp;
      size_++;
    }
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
  void ForwardRingList< T >::remove(const T & val)
  {
    if (empty())
    {
      return;
    }
    auto subhead = head_;
    auto prev_subhead = tail_;
    while (subhead != tail_ && size() != 1)
    {
      if (subhead->data == val)
      {
        auto todelete = subhead;
        subhead = todelete->next;
        prev_subhead->next = subhead;
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
        size_--;
      }
      else
      {
        prev_subhead = subhead;
        subhead = subhead->next;
      }
    }
    if (size() == 1)
    {
      if (head_->data == val)
      {
        pop_front();
      }
    }
    else
    {
      if (subhead->data == val)
      {
        auto todelete = subhead;
        subhead = todelete->next;
        prev_subhead->next = subhead;
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
        size_--;
      }
      else
      {
        prev_subhead = subhead;
        subhead = subhead->next;
      }
    }
    return;
  }

  template< typename T >
  void ForwardRingList< T >::clear()
  {
    if (empty())
    {
      return;
    }
    auto it = begin();
    while (it != end())
    {
      auto temp = it.node_->next;
      delete it.node_;
      it.node_ = temp;
    }
    delete it.node_;
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
    auto prev = end();
    auto next = ++begin();
    auto it = begin();
    while (it != end())
    {
      it.node_->next = prev.node_;
      prev = it;
      it = next++;
    }
    it.node_->next = prev.node_;
    prev = it;
    auto temp = begin().node_;
    head_ = end().node_;
    tail_ = temp;
  }

  template< typename T >
  void ForwardRingList< T >::swap(this_t & rhs) noexcept
  {
    std::swap(head_, rhs.head_);
    std::swap(tail_, rhs.tail_);
    std::swap(size_, rhs.size_);
  }
}

#endif
