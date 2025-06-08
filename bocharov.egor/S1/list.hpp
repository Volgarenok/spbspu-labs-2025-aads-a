#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>
#include <initializer_list>
#include "node.hpp"
#include "iterator.hpp"

namespace bocharov
{
  template< typename T >
  class List
  {
  public:
    using iterator = Iterator< T, false >;
    using const_iterator = Iterator< T, true >;

    List() noexcept;
    List(const List & rhs);
    List(List && rhs) noexcept;
    List(std::size_t count, const T & value);
    template< class InputIt >
    List(InputIt first, InputIt last);
    List(std::initializer_list< T > init);
    ~List();

    List & operator=(const List & rhs);
    List & operator=(List && rhs) noexcept;
    List & operator=(std::initializer_list< T > ilist);

    void assign(std::size_t count, const T & value);
    template< class InputIt >
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list< T > ilist);

    T & front() noexcept;
    const T & front() const noexcept;
    T & back() noexcept;
    const T & back() const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void clear() noexcept;
    iterator insert_after(const_iterator pos, const T & value);
    iterator insert_after(const_iterator pos, T && value);
    iterator insert_after(const_iterator pos, std::size_t count, const T & value);
    template< class InputIt >
    iterator insert_after(const_iterator pos, InputIt first, InputIt last);
    iterator insert_after(const_iterator pos, std::initializer_list< T > ilist);
    iterator erase_after(const_iterator pos) noexcept;
    iterator erase_after(const_iterator first, const_iterator last) noexcept;

    void push_front(const T & value);
    void push_front(T && value);
    void push_back(const T & value);
    void push_back(T && value);
    void pop_front() noexcept;
    void pop_back() noexcept;
    void swap(List & other) noexcept;

    void splice_after(const_iterator pos, List & other);
    void splice_after(const_iterator pos, List && other) noexcept;
    void splice_after(const_iterator pos, List & other, const_iterator it) noexcept;
    void splice_after(const_iterator pos, List & other, const_iterator first, const_iterator last) noexcept;

    void remove(const T & value) noexcept;
    template< class UnaryPredicate >
    void remove_if(UnaryPredicate condition);

  private:
    Node< T > * tail_;
    std::size_t size_;
    template< class InputIt >
    void copy(InputIt first, InputIt last);
    void emplace_front(Node< T > * node) noexcept;
    void emplace_back(Node< T > * node) noexcept;
  };
}

template< typename T >
bocharov::List< T >::List() noexcept:
  tail_(nullptr),
  size_(0)
{}

template< typename T >
bocharov::List< T >::List(const List & rhs):
  List()
{
  if (rhs.empty())
  {
    return;
  }
  copy(rhs.begin(), rhs.end());
}

template< typename T >
bocharov::List< T >::List(List && rhs) noexcept:
  tail_(std::exchange(rhs.tail_, nullptr)),
  size_(std::exchange(rhs.size_, 0))
{}

template< class T >
bocharov::List< T >::List(std::size_t count, const T & value):
  List()
{
  if (count == 0)
  {
    return;
  }
  for (std::size_t i = 0; i != count; ++i)
  {
    push_front(value);
  }
}

template< class T >
template< class InputIt >
bocharov::List< T >::List(InputIt first, InputIt last):
  List()
{
  if (first == last)
  {
    return;
  }
  copy(first, last);
}

template< class T >
bocharov::List< T >::List(std::initializer_list< T > init):
  List(init.begin(), init.end())
{}

template< typename T >
bocharov::List< T >::~List()
{
  clear();
}

template< typename T >
class bocharov::List< T >::List & bocharov::List< T >::operator=(const List & rhs)
{
  List< T > copied_rhs(rhs);
  swap(copied_rhs);
  return *this;
}

template< typename T >
class bocharov::List< T >::List & bocharov::List< T >::operator=(List && rhs) noexcept
{
  List< T > copied_rhs(std::move(rhs));
  swap(copied_rhs);
  return *this;
}

template< class T >
class bocharov::List< T >::List & bocharov::List< T >::operator=(std::initializer_list< T > ilist)
{
  List< T > copied_rhs(std::move(ilist));
  swap(copied_rhs);
  return *this;
}

template< class T >
void bocharov::List< T >::assign(std::size_t count, const T & value)
{
  List< T > assigned(count, value);
  swap(assigned);
}

template< class T >
template< class InputIt >
void bocharov::List< T >::assign(InputIt first, InputIt last)
{
  List< T > assigned(first, last);
  swap(assigned);
}

template< class T >
void bocharov::List< T >::assign(std::initializer_list< T > ilist)
{
  List< T > assigned(std::move(ilist));
  swap(assigned);
}

template< typename T >
T & bocharov::List< T >::front() noexcept
{
  assert(!empty() && "element access to empty list");
  return tail_->next_->data_;
}

template< typename T >
const T & bocharov::List< T >::front() const noexcept
{
  assert(!empty() && "element access to empty list");
  return tail_->next_->data_;
}

template< typename T >
T & bocharov::List< T >::back() noexcept
{
  assert(!empty() && "element access to empty list");
  return tail_->data_;
}

template< typename T >
const T & bocharov::List< T >::back() const noexcept
{
  assert(!empty() && "element access to empty list");
  return tail_->data_;
}

template< typename T >
typename bocharov::List< T >::iterator bocharov::List< T >::begin() noexcept
{
  assert(tail_ && "iterator to empty list");
  return iterator(tail_->next_);
}

template< typename T >
typename bocharov::List< T >::const_iterator bocharov::List< T >::begin() const noexcept
{
  assert(tail_ && "iterator to empty list");
  return const_iterator(tail_->next_);
}

template< typename T >
typename bocharov::List< T >::const_iterator bocharov::List< T >::cbegin() const noexcept
{
  assert(tail_ && "iterator to empty list");
  return const_iterator(tail_->next_);
}

template< typename T >
typename bocharov::List< T >::iterator bocharov::List< T >::end() noexcept
{
  assert(tail_ && "iterator to empty list");
  return iterator(tail_->next_);
}

template< typename T >
typename bocharov::List< T >::const_iterator bocharov::List< T >::end() const noexcept
{
  assert(tail_ && "iterator to empty list");
  return const_iterator(tail_->next_);
}

template< typename T >
typename bocharov::List< T >::const_iterator bocharov::List< T >::cend() const noexcept
{
  assert(tail_ && "iterator to empty list");
  return const_iterator(tail_->next_);
}

template< typename T >
bool bocharov::List< T >::empty() const noexcept
{
  return size_ == 0;
}

template< typename T >
std::size_t bocharov::List< T >::size() const noexcept
{
  return size_;
}

template< typename T >
void bocharov::List< T >::clear() noexcept
{
  while (!empty())
  {
    pop_front();
  }
}

template< class T >
typename bocharov::List< T >::iterator bocharov::List< T >::insert_after(const_iterator pos, const T & value)
{
  Node< T > * new_node = new Node< T >{value, pos.node_->next_};
  pos.node_->next_ = new_node;
  if (tail_ == pos.node_)
  {
    tail_ = new_node;
  }
  ++size_;
  return iterator(new_node);
}

template< class T >
typename bocharov::List< T >::iterator bocharov::List< T >::insert_after(const_iterator pos, T && value)
{
  return insert_after(pos, value);
}

template< class T >
typename bocharov::List< T >::iterator bocharov::List< T >::insert_after(const_iterator pos, std::size_t count, const T & value)
{
  if (count == 0)
  {
    return iterator(pos.node_);
  }
  splice_after(pos, List< T >(count, value));
  return iterator(pos.node_);
}

template< class T >
template< class InputIt >
typename bocharov::List< T >::iterator bocharov::List< T >::insert_after(const_iterator pos, InputIt first, InputIt last)
{
  if (first == last)
  {
    return iterator(pos.node_);
  }
  splice_after(pos, List< T >(first, last));
  return iterator(pos.node_);
}

template< class T >
typename bocharov::List< T >::iterator bocharov::List< T >::insert_after(const_iterator pos, std::initializer_list< T > ilist)
{
  return insert_after(pos, ilist.begin(), ilist.end());
}

template< class T >
typename bocharov::List< T >::iterator bocharov::List< T >::erase_after(const_iterator pos) noexcept
{
  assert(!empty() && "empty list erasing");
  if (size_ == 1)
  {
    delete tail_;
    --size_;
    return iterator();
  }
  if (pos.node_->next_ == tail_)
  {
    tail_ = pos.node_;
  }
  Node< T > * to_delete = pos.node_->next_;
  pos.node_->next_ = to_delete->next_;
  delete to_delete;
  --size_;
  return iterator(pos.node_->next_);
}

template< class T >
typename bocharov::List< T >::iterator bocharov::List< T >::erase_after(const_iterator first, const_iterator last) noexcept
{
  assert(!empty() && "empty list erasing");
  while (first.node_->next_ != last.node_)
  {
    erase_after(first);
  }
  return iterator(last.node_);
}

template< typename T >
void bocharov::List< T >::push_front(const T & value)
{
  Node< T > * new_node = new Node< T >{value, nullptr};
  emplace_front(new_node);
}

template< class T >
void bocharov::List< T >::push_front(T && value)
{
  Node< T > * new_node = new Node< T >{std::move(value), nullptr};
  emplace_front(new_node);
}

template< typename T >
void bocharov::List< T >::push_back(const T & value)
{
  Node< T > * new_node = new Node< T >{value, nullptr};
  emplace_back(new_node);
}

template< class T >
void bocharov::List< T >::push_back(T && value)
{
  Node< T > * new_node = new Node< T >{std::move(value), nullptr};
  emplace_back(new_node);
}

template< typename T >
void bocharov::List< T >::pop_front() noexcept
{
  assert(!empty() && "pop_front() to empty list");
  Node< T > * to_delete = tail_->next_;
  tail_->next_ = to_delete->next_;
  delete to_delete;
  --size_;
}

template< typename T >
void bocharov::List< T >::pop_back() noexcept
{
  assert(!empty() && "pop_back() to empty list");
  if (size_ == 1)
  {
    delete tail_;
    tail_ = nullptr;
    size_ = 0;
    return;
  }
  Node< T > * pred = tail_->next_;
  while (pred->next_ != tail_)
  {
    pred = pred->next_;
  }
  pred->next_ = tail_->next_;
  delete tail_;
  tail_ = pred;
  --size_;
}


template< typename T >
void bocharov::List< T >::swap(List & other) noexcept
{
  std::swap(tail_, other.tail_);
  std::swap(size_, other.size_);
}

template< class T >
void bocharov::List< T >::splice_after(const_iterator pos, List & other)
{
  other.push_front(T());
  splice_after(pos, other, other.cbegin(), other.cend());
  other.clear();
}

template< class T >
void bocharov::List< T >::splice_after(const_iterator pos, List && other) noexcept
{
  splice_after(pos, other);
}

template< class T >
void bocharov::List< T >::splice_after(const_iterator pos, List & other, const_iterator it) noexcept
{
  splice_after(pos, other, it, ++(++const_iterator(it.node_)));
}

template< class T >
void bocharov::List< T >::splice_after(const_iterator pos, List & other, const_iterator first,
  const_iterator last) noexcept
{
  if (last == const_iterator(first.node_->next_))
  {
    return;
  }
  if (other.cbegin() == first && other.cend() == last)
  {
    size_ += other.size_ - 1;
    other.size_ = 1;
  }
  else
  {
    std::size_t size_increase = 0;
    for (auto it = first; it != last; ++it, ++size_increase)
    {}
    size_ += size_increase - 1;
    other.size_ -= size_increase - 1;
  }

  Node< T > * pos_next = pos.node_->next_;
  if (pos.node_ == tail_)
  {
    tail_ = last.node_;
  }
  if (last.node_ == other.tail_->next_)
  {
    other.tail_ = first.node_;
  }
  pos.node_->next_ = first.node_->next_;
  while (pos.node_->next_ != last.node_)
  {
    ++pos;
  }
  pos.node_->next_ = pos_next;
  first.node_->next_ = last.node_;
}

template< class T >
void bocharov::List< T >::remove(const T & value) noexcept
{
  assert(!empty() && "removing from empty list");
  remove_if([&](const T & list_value)
  {
    return list_value == value;
  });
}

template< class T >
template< class UnaryPredicate >
void bocharov::List< T >::remove_if(UnaryPredicate condition)
{
  assert(!empty() && "removing from empty list");
  auto it = ++cbegin();
  while (it != cend())
  {
    if (condition(it.node_->next_->data_))
    {
      erase_after(it);
    }
    else
    {
      ++it;
    }
  }
  if (condition(*++it))
  {
    erase_after(const_iterator(tail_->next_));
  }
}

template< class T >
template< class InputIt >
void bocharov::List< T >::copy(InputIt first, InputIt last)
{
  push_back(*first);
  ++first;
  for (; first != last; ++first)
  {
    push_back(*first);
  }
}

template< class T >
void bocharov::List< T >::emplace_front(Node< T > * node) noexcept
{
  if (empty())
  {
    tail_ = node;
    tail_->next_ = tail_;
  }
  else
  {
    node->next_ = tail_->next_;
    tail_->next_ = node;
  }
  ++size_;
}

template< class T >
void bocharov::List< T >::emplace_back(Node< T > * node) noexcept
{
  if (empty())
  {
    tail_ = node;
    tail_->next_ = tail_;
  }
  else
  {
    node->next_ = tail_->next_;
    tail_->next_ = node;
    tail_ = node;
  }
  ++size_;
}

#endif
