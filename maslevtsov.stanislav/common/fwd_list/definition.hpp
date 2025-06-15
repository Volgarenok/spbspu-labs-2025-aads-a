#ifndef FWD_LIST_DEFINITION_HPP
#define FWD_LIST_DEFINITION_HPP

#include "declaration.hpp"
#include <utility>

template< class T >
maslevtsov::FwdList< T >::FwdList() noexcept:
  tail_(nullptr),
  size_(0)
{}

template< class T >
maslevtsov::FwdList< T >::FwdList(const FwdList& rhs):
  FwdList()
{
  if (rhs.empty()) {
    return;
  }
  copy(rhs.begin(), rhs.end());
}

template< class T >
maslevtsov::FwdList< T >::FwdList(FwdList&& rhs) noexcept:
  tail_(std::exchange(rhs.tail_, nullptr)),
  size_(std::exchange(rhs.size_, 0))
{}

template< class T >
maslevtsov::FwdList< T >::FwdList(std::size_t count, const T& value):
  FwdList()
{
  if (count == 0) {
    return;
  }
  for (std::size_t i = 0; i != count; ++i) {
    push_front(value);
  }
}

template< class T >
template< class InputIt >
maslevtsov::FwdList< T >::FwdList(InputIt first, InputIt last):
  FwdList()
{
  if (first == last) {
    return;
  }
  copy(first, last);
}

template< class T >
maslevtsov::FwdList< T >::FwdList(std::initializer_list< T > init):
  FwdList(init.begin(), init.end())
{}

template< class T >
maslevtsov::FwdList< T >::~FwdList()
{
  clear();
}

template< class T >
class maslevtsov::FwdList< T >::FwdList& maslevtsov::FwdList< T >::operator=(const FwdList& rhs)
{
  FwdList< T > copied_rhs(rhs);
  swap(copied_rhs);
  return *this;
}

template< class T >
class maslevtsov::FwdList< T >::FwdList& maslevtsov::FwdList< T >::operator=(FwdList&& rhs) noexcept
{
  FwdList< T > copied_rhs(std::move(rhs));
  swap(copied_rhs);
  return *this;
}

template< class T >
class maslevtsov::FwdList< T >::FwdList& maslevtsov::FwdList< T >::operator=(std::initializer_list< T > ilist)
{
  FwdList< T > copied_rhs(std::move(ilist));
  swap(copied_rhs);
  return *this;
}

template< class T >
void maslevtsov::FwdList< T >::assign(std::size_t count, const T& value)
{
  FwdList< T > assigned(count, value);
  swap(assigned);
}

template< class T >
template< class InputIt >
void maslevtsov::FwdList< T >::assign(InputIt first, InputIt last)
{
  FwdList< T > assigned(first, last);
  swap(assigned);
}

template< class T >
void maslevtsov::FwdList< T >::assign(std::initializer_list< T > ilist)
{
  FwdList< T > assigned(std::move(ilist));
  swap(assigned);
}

template< class T >
T& maslevtsov::FwdList< T >::front() noexcept
{
  assert(!empty() && "element access to empty list");
  return tail_->next_->data_;
}

template< class T >
const T& maslevtsov::FwdList< T >::front() const noexcept
{
  assert(!empty() && "element access to empty list");
  return tail_->next_->data_;
}

template< class T >
T& maslevtsov::FwdList< T >::back() noexcept
{
  assert(!empty() && "element access to empty list");
  return tail_->data_;
}

template< class T >
const T& maslevtsov::FwdList< T >::back() const noexcept
{
  assert(!empty() && "element access to empty list");
  return tail_->data_;
}

template< class T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::begin() noexcept
{
  assert(tail_ && "iterator to empty list");
  return iterator(tail_->next_);
}

template< class T >
typename maslevtsov::FwdList< T >::const_iterator maslevtsov::FwdList< T >::begin() const noexcept
{
  assert(tail_ && "iterator to empty list");
  return const_iterator(tail_->next_);
}

template< class T >
typename maslevtsov::FwdList< T >::const_iterator maslevtsov::FwdList< T >::cbegin() const noexcept
{
  assert(tail_ && "iterator to empty list");
  return const_iterator(tail_->next_);
}

template< class T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::end() noexcept
{
  assert(tail_ && "iterator to empty list");
  return iterator(tail_->next_);
}

template< class T >
typename maslevtsov::FwdList< T >::const_iterator maslevtsov::FwdList< T >::end() const noexcept
{
  assert(tail_ && "iterator to empty list");
  return const_iterator(tail_->next_);
}

template< class T >
typename maslevtsov::FwdList< T >::const_iterator maslevtsov::FwdList< T >::cend() const noexcept
{
  assert(tail_ && "iterator to empty list");
  return const_iterator(tail_->next_);
}

template< class T >
bool maslevtsov::FwdList< T >::empty() const noexcept
{
  return size_ == 0;
}

template< class T >
std::size_t maslevtsov::FwdList< T >::size() const noexcept
{
  return size_;
}

template< class T >
void maslevtsov::FwdList< T >::clear() noexcept
{
  while (!empty()) {
    pop_front();
  }
}

template< class T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::insert_after(const_iterator pos, const T& value)
{
  FwdListNode< T >* new_node = new FwdListNode< T >{value, pos.node_->next_};
  pos.node_->next_ = new_node;
  if (tail_ == pos.node_) {
    tail_ = new_node;
  }
  ++size_;
  return iterator(new_node);
}

template< class T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::insert_after(const_iterator pos, T&& value)
{
  return insert_after(pos, value);
}

template< class T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::insert_after(const_iterator pos,
  std::size_t count, const T& value)
{
  if (count == 0) {
    return iterator(pos.node_);
  }
  splice_after(pos, FwdList< T >(count, value));
  return iterator(pos.node_);
}

template< class T >
template< class InputIt >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::insert_after(const_iterator pos, InputIt first,
  InputIt last)
{
  if (first == last) {
    return iterator(pos.node_);
  }
  splice_after(pos, FwdList< T >(first, last));
  return iterator(pos.node_);
}

template< class T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::insert_after(const_iterator pos,
  std::initializer_list< T > ilist)
{
  return insert_after(pos, ilist.begin(), ilist.end());
}

template< class T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::erase_after(const_iterator pos) noexcept
{
  assert(!empty() && "empty list erasing");
  if (size_ == 1) {
    delete tail_;
    --size_;
    return iterator();
  }
  if (pos.node_->next_ == tail_) {
    tail_ = pos.node_;
  }
  FwdListNode< T >* to_delete = pos.node_->next_;
  pos.node_->next_ = to_delete->next_;
  delete to_delete;
  --size_;
  return iterator(pos.node_->next_);
}

template< class T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::erase_after(const_iterator first,
  const_iterator last) noexcept
{
  assert(!empty() && "empty list erasing");
  while (first.node_->next_ != last.node_) {
    erase_after(first);
  }
  return iterator(last.node_);
}

template< class T >
void maslevtsov::FwdList< T >::push_front(const T& value)
{
  FwdListNode< T >* new_node = new FwdListNode< T >{value, nullptr};
  emplace_front(new_node);
}

template< class T >
void maslevtsov::FwdList< T >::push_front(T&& value)
{
  FwdListNode< T >* new_node = new FwdListNode< T >{std::move(value), nullptr};
  emplace_front(new_node);
}

template< class T >
void maslevtsov::FwdList< T >::push_back(const T& value)
{
  FwdListNode< T >* new_node = new FwdListNode< T >{value, nullptr};
  emplace_back(new_node);
}

template< class T >
void maslevtsov::FwdList< T >::push_back(T&& value)
{
  FwdListNode< T >* new_node = new FwdListNode< T >{std::move(value), nullptr};
  emplace_back(new_node);
}

template< class T >
void maslevtsov::FwdList< T >::pop_front() noexcept
{
  assert(!empty() && "pop_front() to empty list");
  FwdListNode< T >* to_delete = tail_->next_;
  tail_->next_ = to_delete->next_;
  delete to_delete;
  --size_;
}

template< class T >
void maslevtsov::FwdList< T >::swap(FwdList& other) noexcept
{
  std::swap(tail_, other.tail_);
  std::swap(size_, other.size_);
}

template< class T >
void maslevtsov::FwdList< T >::splice_after(const_iterator pos, FwdList& other)
{
  other.push_front(T());
  splice_after(pos, other, other.cbegin(), other.cend());
  other.clear();
}

template< class T >
void maslevtsov::FwdList< T >::splice_after(const_iterator pos, FwdList&& other) noexcept
{
  splice_after(pos, other);
}

template< class T >
void maslevtsov::FwdList< T >::splice_after(const_iterator pos, FwdList& other, const_iterator it) noexcept
{
  splice_after(pos, other, it, ++(++const_iterator(it.node_)));
}

template< class T >
void maslevtsov::FwdList< T >::splice_after(const_iterator pos, FwdList& other, const_iterator first,
  const_iterator last) noexcept
{
  if (last == const_iterator(first.node_->next_)) {
    return;
  }
  if (other.cbegin() == first && other.cend() == last) {
    size_ += other.size_ - 1;
    other.size_ = 1;
  } else {
    std::size_t size_increase = 0;
    for (auto it = first; it != last; ++it, ++size_increase)
    {}
    size_ += size_increase - 1;
    other.size_ -= size_increase - 1;
  }

  FwdListNode< T >* pos_next = pos.node_->next_;
  if (pos.node_ == tail_) {
    tail_ = last.node_;
  }
  if (last.node_ == other.tail_->next_) {
    other.tail_ = first.node_;
  }
  pos.node_->next_ = first.node_->next_;
  while (pos.node_->next_ != last.node_) {
    ++pos;
  }
  pos.node_->next_ = pos_next;
  first.node_->next_ = last.node_;
}

template< class T >
void maslevtsov::FwdList< T >::remove(const T& value) noexcept
{
  assert(!empty() && "removing from empty list");
  remove_if([&](const T& list_value)
  {
    return list_value == value;
  });
}

template< class T >
template< class UnaryPredicate >
void maslevtsov::FwdList< T >::remove_if(UnaryPredicate condition)
{
  assert(!empty() && "removing from empty list");
  auto it = ++cbegin();
  while (it != cend()) {
    if (condition(it.node_->next_->data_)) {
      erase_after(it);
    } else {
      ++it;
    }
  }
  if (condition(*++it)) {
    erase_after(const_iterator(tail_->next_));
  }
}

template< class T >
template< class InputIt >
void maslevtsov::FwdList< T >::copy(InputIt first, InputIt last)
{
  push_back(*first);
  ++first;
  for (; first != last; ++first) {
    push_back(*first);
  }
}

template< class T >
void maslevtsov::FwdList< T >::emplace_front(FwdListNode< T >* node) noexcept
{
  if (empty()) {
    tail_ = node;
    tail_->next_ = tail_;
  } else {
    node->next_ = tail_->next_;
    tail_->next_ = node;
  }
  ++size_;
}

template< class T >
void maslevtsov::FwdList< T >::emplace_back(FwdListNode< T >* node) noexcept
{
  if (empty()) {
    tail_ = node;
    tail_->next_ = tail_;
  } else {
    node->next_ = tail_->next_;
    tail_->next_ = node;
    tail_ = node;
  }
  ++size_;
}

#endif
