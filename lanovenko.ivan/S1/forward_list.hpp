#ifndef FORWARD_LIST_HPP
#define FORWARD_LIST_HPP

#include <cassert>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include "forward_list_node.hpp"
#include "forward_list_iterator.hpp"
#include "forward_list_const_iterator.hpp"

namespace lanovenko
{
  template< typename T > class ForwardList
  {
  public:
    using const_iterator = ForwardListConstIterator< T >;
    using iterator = ForwardListIterator< T >;

    ~ForwardList() noexcept;
    ForwardList() noexcept;
    ForwardList(const ForwardList& rhs);
    ForwardList(ForwardList&& rhs) noexcept;
    ForwardList< T >& operator=(const ForwardList& rhs);
    ForwardList< T >& operator=(ForwardList&& rhs) noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    T& front();
    T& back();
    const T& front() const;
    const T& back() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void push_front(const T& value);
    void push_front(T&& value);
    void pop_front() noexcept;
    void swap(ForwardList& rhs) noexcept;
    void clear() noexcept;
    void push_back(const T& value);
    void push_back(T&& value);
  private:
    ForwardListNode< T >* head_;
    ForwardListNode< T >* tail_;
    size_t size_;
    void push_front_value(ForwardListNode< T >* node) noexcept;
    void push_back_value(ForwardListNode< T >* node) noexcept;
  };

  template< typename T >
  ForwardList< T >::~ForwardList() noexcept
  {
    clear();
  }

  template< typename T >
  ForwardList< T >::ForwardList() noexcept:
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< typename T >
  ForwardList< T >::ForwardList(const ForwardList& rhs):
    ForwardList()
  {
    for (const_iterator cit = rhs.begin(); cit != rhs.end(); cit++)
    {
      push_back(*cit);
    }
  }

  template< typename T >
  ForwardList< T >::ForwardList(ForwardList&& rhs) noexcept:
    head_(rhs.head_),
    tail_(rhs.tail_),
    size_(rhs.size_)
  {
    rhs.head_ = rhs.tail_ = nullptr;
    rhs.size_ = 0;
  }

  template< typename T >
  ForwardList< T >& ForwardList< T >::operator=(const ForwardList& rhs)
  {
    if (this != std::addressof(rhs))
    {
      ForwardList< T > temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  ForwardList< T >& ForwardList< T >::operator=(ForwardList&& rhs) noexcept
  {
    if (this != std::addressof(rhs))
    {
      ForwardList< T > temp(std::move(rhs));
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  typename ForwardList< T >::iterator ForwardList< T >::begin() noexcept
  {
    return iterator(head_);
  }

  template< typename T >
  typename ForwardList< T >::iterator ForwardList< T >::end() noexcept
  {
    return size_ == 0 ? iterator() : iterator(tail_->next_, false);
  }

  template< typename T >
  typename ForwardList< T >::const_iterator ForwardList< T >::begin() const noexcept
  {
    return const_iterator(head_);
  }

  template< typename T >
  typename ForwardList< T >::const_iterator ForwardList< T >::end() const noexcept
  {
    return size_ == 0 ? const_iterator() : const_iterator(tail_->next_, false);
  }

  template< typename T >
  typename ForwardList< T >::const_iterator ForwardList< T >::cbegin() const noexcept
  {
    return const_iterator(head_);
  }

  template< typename T >
  typename ForwardList< T >::const_iterator ForwardList< T >::cend() const noexcept
  {
    return size_ == 0 ? const_iterator() : const_iterator(tail_->next_, false);
  }

  template< typename T >
  T& ForwardList< T >::front()
  {
    if (empty())
    {
      throw std::logic_error("Empty List");
    }
    return head_->data_;
  }

  template< typename T >
  T& ForwardList< T >::back()
  {
    if (empty())
    {
      throw std::logic_error("Empty List");
    }
    return tail_->data_;
  }

  template< typename T >
  const T& ForwardList< T >::front() const
  {
    if (empty())
    {
      throw std::logic_error("Empty List");
    }
    return head_->data_;
  }

  template< typename T >
  const T& ForwardList< T >::back() const
  {
    if (empty())
    {
      throw std::logic_error("Empty List");
    }
    return tail_->data_;
  }

  template< typename T >
  bool ForwardList< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t ForwardList< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  void ForwardList< T >::push_front_value(ForwardListNode< T >* newNode) noexcept
  {
    newNode->next_ = head_;
    head_ = newNode;
    if (empty())
    {
      tail_ = newNode;
    }
    tail_->next_ = head_;
    size_++;
  }

  template< typename T >
  void ForwardList< T >::push_front(const T& value)
  {
    push_front_value(new ForwardListNode< T >(value));
  }

  template< typename T >
  void ForwardList< T >::push_front(T&& value)
  {
    push_front_value(new ForwardListNode< T >(std::move(value)));
  }

  template< typename T >
  void ForwardList< T >::pop_front() noexcept
  {
    if (!empty())
    {
      ForwardListNode< T >* toDelete = head_;
      head_ = head_->next_;
      tail_->next_ = head_;
      delete toDelete;
      size_--;
    }
    if (empty())
    {
      head_ = tail_ = nullptr;
    }
  }

  template< typename T >
  void ForwardList< T >::push_back_value(ForwardListNode< T >* newNode) noexcept
  {
    if (!empty())
    {
      tail_->next_ = newNode;
      tail_ = newNode;
      tail_->next_ = head_;
    }
    if (empty())
    {
      head_ = newNode;
      tail_ = newNode;
      tail_->next_ = newNode;
    }
    size_++;
  }

  template< typename T >
  void ForwardList< T >::push_back(const T& value)
  {
    push_back_value(new ForwardListNode< T >(value));
  }

  template< typename T >
  void ForwardList< T >::push_back(T&& value)
  {
    push_back_value(new ForwardListNode< T >(std::move(value)));
  }

  template< typename T >
  void ForwardList< T >::swap(ForwardList& rhs) noexcept
  {
    std::swap(head_, rhs.head_);
    std::swap(tail_, rhs.tail_);
    std::swap(size_, rhs.size_);
  }

  template< typename T >
  void ForwardList< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_front();
    }
  }
}

#endif
