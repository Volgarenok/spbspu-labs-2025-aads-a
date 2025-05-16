#ifndef FORWARD_LIST_HPP
#define FORWARD_LIST_HPP

#include <cassert>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include "forward_list_node.hpp"
#include "forward_list_const_iterator.hpp"
#include "forward_list_iterator.hpp"

namespace lanovenko
{
  template< typename T > class ForwardList
  {
  public:
    friend class const_iterator;
    friend class iterator;
    using const_iterator = ForwardListConstIterator< T >;
    using iterator = ForwardListIterator< T >;
    ForwardList();
    ~ForwardList();
    ForwardList(const ForwardList&);
    ForwardList(ForwardList&&) noexcept;
    ForwardList< T >& operator=(ForwardList);
    ForwardList< T >& operator=(ForwardList&&);
    iterator begin() const noexcept;
    iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    T& front() const;
    T& back() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void push_front(const T& value);
    void push_front(T&& value);
    void pop_front();
    void swap(ForwardList& fwsdlst);
    void clear() noexcept;
    void push_back(const T& value);
    void push_back(T&& value);
  private:
    ForwardListNode< T >* head_;
    ForwardListNode< T >* tail_;
    size_t size_;
    void push_front_value(ForwardListNode< T >* node);
    void push_back_value(ForwardListNode< T >* node);
  };
}

template < typename T >
lanovenko::ForwardList< T >::ForwardList():
  head_(nullptr),
  tail_(nullptr),
  size_(0)
{}

template < typename T >
lanovenko::ForwardList<T>::ForwardList(const ForwardList& rhs):
  head_(nullptr),
  tail_(nullptr),
  size_(0)
{
  ForwardList<T> temporary{};
  for (iterator it = rhs.begin(); it != rhs.end(); it++)
  {
    temporary.push_front(*it);
  }
  for (iterator it = temporary.begin(); it != temporary.end(); it++)
  {
    push_front(*it);
  }
}

template < typename T >
lanovenko::ForwardList< T >::ForwardList(ForwardList&& rhs) noexcept:
  head_(rhs.head_),
  tail_(rhs.tail_),
  size_(rhs.size_)
{
  rhs.head_ = rhs.tail_ = nullptr;
  rhs.size_ = 0;
}

template < typename T >
lanovenko::ForwardList< T >& lanovenko::ForwardList< T >::operator=(ForwardList rhs)
{
  swap(rhs);
  return *this;
}

template <typename T>
lanovenko::ForwardList< T >& lanovenko::ForwardList< T >::operator=(ForwardList&& rhs)
{
  if (this != &rhs)
  {
    clear();
    head_ = rhs.head_;
    tail_ = rhs.tail_;
    size_ = rhs.size_;
    rhs.tail_ = rhs.head_ = nullptr;
    rhs.size_ = 0;
  }
  return *this;
}

template < typename T >
lanovenko::ForwardList< T >::~ForwardList()
{
  clear();
}

template <typename T>
typename lanovenko::ForwardList< T >::iterator lanovenko::ForwardList< T >::begin() const noexcept
{
  return iterator(head_);
}

template <typename T>
typename lanovenko::ForwardList< T >::iterator lanovenko::ForwardList< T >::end() const noexcept
{
  if (size_ == 0)
  {
    return iterator(nullptr);
  }
  iterator it(tail_->next_);
  it.firstIteration_ = false;
  return it;
}

template<typename T>
typename lanovenko::ForwardList< T >::const_iterator lanovenko::ForwardList< T >::cbegin() const noexcept
{
  return const_iterator(head_);
}

template<typename T>
typename lanovenko::ForwardList< T >::const_iterator lanovenko::ForwardList< T >::cend() const noexcept
{
  if (size_ == 0)
  {
    return const_iterator(nullptr);
  }
  const_iterator it(tail_->next_);
  it.firstIteration_ = false;
  return it;
}

template < typename T >
T& lanovenko::ForwardList< T >::front() const
{
  if (empty())
  {
    throw std::logic_error("Empty List");
  }
  return head_->data_;
}

template < typename T >
T& lanovenko::ForwardList< T >::back() const
{
  if (empty())
  {
    throw std::logic_error("Empty List");
  }
  return tail_->data_;
}

template < typename T >
bool lanovenko::ForwardList< T >::empty() const noexcept
{
  return size_ == 0;
}

template < typename T >
size_t lanovenko::ForwardList< T >::size() const noexcept
{
  return size_;
}

template <typename T>
void lanovenko::ForwardList< T >::push_front_value(ForwardListNode< T >* newNode)
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

template <typename T>
void lanovenko::ForwardList< T >::push_front(const T& value)
{
  push_front_value(new ForwardListNode<T>(value));
}

template < typename T >
void lanovenko::ForwardList< T >::push_front(T&& value)
{
  push_front_value(new ForwardListNode< T >(std::move(value)));
}

template < typename T >
void lanovenko::ForwardList< T >::pop_front()
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
    head_ = nullptr;
    tail_ = nullptr;
  }
}

template< typename T >
void lanovenko::ForwardList< T >::push_back_value(ForwardListNode< T >* newNode)
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
void lanovenko::ForwardList< T >::push_back(const T& value)
{
  push_back_value(new ForwardListNode<T>(value));
}

template< typename T >
void lanovenko::ForwardList< T >::push_back(T&& value)
{
  push_back_value(new ForwardListNode< T >(std::move(value)));
}

template < typename T >
void lanovenko::ForwardList< T >::swap(ForwardList& fwdlst)
{
  std::swap(head_, fwdlst.head_);
  std::swap(tail_, fwdlst.tail_);
  std::swap(size_, fwdlst.size_);
}

template < typename T >
void lanovenko::ForwardList< T >::clear() noexcept
{
  while (!empty())
  {
    pop_front();
  }
}

#endif
