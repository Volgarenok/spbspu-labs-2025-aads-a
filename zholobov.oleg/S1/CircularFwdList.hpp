#ifndef CIRCULARFWDLIST_HPP
#define CIRCULARFWDLIST_HPP

#include <cstddef>
#include <utility>

#include "CircularFwdListIterators.hpp"
#include "FwdListNode.hpp"

namespace zholobov {

  template < typename T >
  class CircularFwdList {
  public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const reference;
    using iterator = CircularFwdListIterator< T >;
    using const_iterator = CircularFwdListConstIterator< T >;

    CircularFwdList();
    explicit CircularFwdList(size_t n);
    CircularFwdList(size_t n, const value_type& val);
    CircularFwdList(std::initializer_list< value_type > init);

    CircularFwdList(const CircularFwdList& other);
    CircularFwdList(CircularFwdList&& other) noexcept;

    ~CircularFwdList();

    CircularFwdList& operator=(const CircularFwdList& other);
    CircularFwdList& operator=(CircularFwdList&& other) noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    bool empty() const noexcept;
    size_t size() const { return size_; }

    void assign(size_t n, const value_type& val);
    void push_front(const value_type& val);
    void push_front(value_type&& val);
    void push_back(const value_type& val);
    void push_back(value_type&& val);

    void pop_front();
    void pop_back();
    void remove(const value_type& val);
    template < class Predicate >
    void remove_if(Predicate pred);

    void splice_after(const_iterator pos, CircularFwdList< T >& other);
    void splice_after(const_iterator pos, CircularFwdList< T >&& other);
    void splice_after(const_iterator pos, CircularFwdList< T >& other, const_iterator it);
    void splice_after(const_iterator pos, CircularFwdList< T >&& other, const_iterator it);
    void splice_after(const_iterator pos, CircularFwdList< T >& other, const_iterator first, const_iterator last);
    void splice_after(const_iterator pos, CircularFwdList< T >&& other, const_iterator first, const_iterator last);

    void swap(CircularFwdList& other) noexcept;
    void clear() noexcept;

  private:
    FwdListNode< value_type >* head_;
    FwdListNode< value_type >* tail_;
    size_t size_;

    friend class CircularFwdListIterator< T >;
    friend class CircularFwdListConstIterator< T >;
  };
}

template < typename T >
zholobov::CircularFwdList< T >::CircularFwdList():
  head_(nullptr), tail_(nullptr), size_(0)
{}

template < typename T >
zholobov::CircularFwdList< T >::CircularFwdList(size_t n):
  CircularFwdList(n, value_type())
{}

template < typename T >
zholobov::CircularFwdList< T >::CircularFwdList(size_t n, const value_type& val):
  head_(nullptr), tail_(nullptr), size_(0)
{
  for (size_t i = 0; i < n; ++i) {
    push_front(val);
  }
}

template < typename T >
zholobov::CircularFwdList< T >::CircularFwdList(std::initializer_list< value_type > init):
  head_(nullptr), tail_(nullptr), size_(0)
{
  for (auto it = init.begin(); it != init.end(); ++it) {
    push_back(*it);
  }
}

template < typename T >
zholobov::CircularFwdList< T >::CircularFwdList(const CircularFwdList& other):
  head_(nullptr), tail_(nullptr), size_(0)
{
  FwdListNode< value_type >* other_p = other.head_;
  if (other_p != nullptr) {
    try {
      head_ = new FwdListNode< value_type >(other_p->value, head_);
      ++size_;
      other_p = other_p->next;
      FwdListNode< value_type >* prev = head_;
      while (other_p != other.head_) {
        auto curr = new FwdListNode< value_type >(other_p->value, head_);
        ++size_;
        prev->next = curr;
        other_p = other_p->next;
        prev = prev->next;
      };
      tail_ = prev;
    } catch (const std::bad_alloc&) {
      clear();
      throw;
    }
  }
}

template < typename T >
zholobov::CircularFwdList< T >::CircularFwdList(CircularFwdList&& other) noexcept:
  head_(nullptr), tail_(nullptr), size_(0)
{
  swap(other);
}

template < typename T >
zholobov::CircularFwdList< T >::~CircularFwdList()
{
  clear();
}

template < typename T >
zholobov::CircularFwdList< T >& zholobov::CircularFwdList< T >::operator=(const CircularFwdList& other)
{
  if (this != std::addressof(other)) {
    CircularFwdList< T > temp(other);
    swap(temp);
  }
  return *this;
}

template < typename T >
zholobov::CircularFwdList< T >& zholobov::CircularFwdList< T >::operator=(CircularFwdList&& other) noexcept
{
  if (this != std::addressof(other)) {
    clear();
    swap(other);
  }
  return *this;
}

template < typename T >
typename zholobov::CircularFwdList< T >::iterator zholobov::CircularFwdList< T >::begin() noexcept
{
  return (size_ == 0) ? iterator(nullptr) : iterator(std::addressof(head_));
}

template < typename T >
typename zholobov::CircularFwdList< T >::const_iterator zholobov::CircularFwdList< T >::begin() const noexcept
{
  return cbegin();
}

template < typename T >
typename zholobov::CircularFwdList< T >::const_iterator zholobov::CircularFwdList< T >::cbegin() const noexcept
{
  return (size_ == 0) ? const_iterator(nullptr) : const_iterator(std::addressof(head_));
}

template < typename T >
typename zholobov::CircularFwdList< T >::iterator zholobov::CircularFwdList< T >::end() noexcept
{
  return (size_ == 0) ? iterator(nullptr) : iterator(std::addressof(tail_->next));
}

template < typename T >
typename zholobov::CircularFwdList< T >::const_iterator zholobov::CircularFwdList< T >::end() const noexcept
{
  return cend();
}

template < typename T >
typename zholobov::CircularFwdList< T >::const_iterator zholobov::CircularFwdList< T >::cend() const noexcept
{
  return (size_ == 0) ? const_iterator(nullptr) : const_iterator(std::addressof(tail_->next));
}

template < typename T >
typename zholobov::CircularFwdList< T >::reference zholobov::CircularFwdList< T >::front()
{
  return head_->value;
}

template < typename T >
typename zholobov::CircularFwdList< T >::const_reference zholobov::CircularFwdList< T >::front() const
{
  return head_->value;
}

template < typename T >
typename zholobov::CircularFwdList< T >::reference zholobov::CircularFwdList< T >::back()
{
  return tail_->value;
}

template < typename T >
typename zholobov::CircularFwdList< T >::const_reference zholobov::CircularFwdList< T >::back() const
{
  return tail_->value;
}

template < typename T >
bool zholobov::CircularFwdList< T >::empty() const noexcept
{
  return size_ == 0;
}

template < typename T >
void zholobov::CircularFwdList< T >::assign(size_t n, const value_type& val)
{
  CircularFwdList< T > temp(n, val);
  swap(temp);
}

template < typename T >
void zholobov::CircularFwdList< T >::push_front(const value_type& val)
{
  auto new_node = new FwdListNode< value_type >(val, head_);
  head_ = new_node;
  if (size_ == 0) {
    tail_ = head_;
  }
  tail_->next = head_;
  ++size_;
}

template < typename T >
void zholobov::CircularFwdList< T >::push_front(value_type&& val)
{
  auto new_node = new FwdListNode< value_type >(std::move(val), head_);
  head_ = new_node;
  if (size_ == 0) {
    tail_ = head_;
  }
  tail_->next = head_;
  ++size_;
}

template < typename T >
void zholobov::CircularFwdList< T >::push_back(const value_type& val)
{
  auto new_node = new FwdListNode< value_type >(val, head_);
  if (size_ == 0) {
    head_ = new_node;
    head_->next = head_;
  } else {
    tail_->next = new_node;
  }
  tail_ = new_node;
  ++size_;
}

template < typename T >
void zholobov::CircularFwdList< T >::push_back(value_type&& val)
{
  auto new_node = new FwdListNode< value_type >(std::move(val), head_);
  if (size_ == 0) {
    head_ = new_node;
    head_->next = head_;
  } else {
    tail_->next = new_node;
  }
  tail_ = new_node;
  ++size_;
}

template < typename T >
void zholobov::CircularFwdList< T >::pop_front()
{
  if (size_ == 0) {
    return;
  } else if (size_ == 1) {
    delete head_;
    head_ = nullptr;
    tail_ = nullptr;
  } else {
    FwdListNode< value_type >* temp = head_;
    head_ = head_->next;
    tail_->next = head_;
    delete temp;
  }
  --size_;
}

template < typename T >
void zholobov::CircularFwdList< T >::pop_back()
{
  if (size_ == 0) {
    return;
  } else if (size_ == 1) {
    delete tail_;
    head_ = nullptr;
    tail_ = nullptr;
  } else {
    FwdListNode< value_type >* cur = head_;
    while (cur->next != tail_) {
      ++cur;
    }
    delete tail_;
    tail_ = cur;
    tail_->next = head_;
  }
  --size_;
}

template < typename T >
void zholobov::CircularFwdList< T >::remove(const value_type& val)
{
  remove_if([&val](const T& value) { return value == val; });
}

template < typename T >
template < class Predicate >
void zholobov::CircularFwdList< T >::remove_if(Predicate pred)
{
  while (head_ != nullptr && (pred(head_->value))) {
    pop_front();
  }
  if (head_ != nullptr) {
    FwdListNode< value_type >* p = head_;
    while (p->next != head_) {
      if (pred(p->next->value)) {
        FwdListNode< value_type >* temp = p->next;
        p->next = temp->next;
        if (temp == tail_) {
          tail_ = p;
        }
        delete temp;
        --size_;
      } else {
        p = p->next;
      }
    }
  }
}

template < typename T >
void zholobov::CircularFwdList< T >::splice_after(const_iterator pos, CircularFwdList< T >& other)
{
  splice_after(pos, std::move(other));
}

template < typename T >
void zholobov::CircularFwdList< T >::splice_after(const_iterator pos, CircularFwdList< T >&& other)
{
  FwdListNode< value_type >* p = head_;
  const_iterator it = cbegin();
  while ((it != cend()) && (it != pos)) {
    ++it;
    p = p->next;
  }
  if (it != cend()) {
    other.tail_->next = p->next;
    p->next = other.head_;
    if (p == tail_) {
      tail_ = other.tail_;
    }
    other.head_ = nullptr;
    other.tail_ = nullptr;
    size_ += other.size_;
    other.size_ = 0;
  }
}

template < typename T >
void zholobov::CircularFwdList< T >::splice_after(const_iterator pos,
                                                  CircularFwdList< T >& other, const_iterator it)
{
  splice_after(pos, std::move(other), it);
}

template < typename T >
void zholobov::CircularFwdList< T >::splice_after(const_iterator pos,
                                                  CircularFwdList< T >&& other, const_iterator it)
{
  FwdListNode< value_type >* p = head_;
  const_iterator this_it = cbegin();
  while ((this_it != cend()) && (this_it != pos)) {
    ++this_it;
    p = p->next;
  }
  if (this_it != cend()) {
    FwdListNode< value_type >** other_p = std::addressof(other.head_);
    const_iterator other_it = other.cbegin();
    while ((other_it != other.cend()) && (other_it != it)) {
      ++other_it;
      other_p = std::addressof((*other_p)->next);
    }
    FwdListNode< value_type >* tmp = *other_p;
    *other_p = tmp->next;
    tmp->next = p->next;
    p->next = tmp;
    --other.size_;
    ++size_;
  }
}

template < typename T >
void zholobov::CircularFwdList< T >::splice_after(
  const_iterator pos, CircularFwdList< T >& other, const_iterator first, const_iterator last)
{
  splice_after(pos, std::move(other), first, last);
}

template < typename T >
void zholobov::CircularFwdList< T >::splice_after(
  const_iterator pos, CircularFwdList< T >&& other, const_iterator first, const_iterator last)
{
  FwdListNode< value_type >* p = head_;
  FwdListNode< value_type >* other_first = nullptr;
  FwdListNode< value_type >* other_last = nullptr;

  const_iterator it = cbegin();
  while ((it != cend()) && (it != pos)) {
    ++it;
    p = p->next;
  }
  if (it == cend()) {
    return;
  }

  FwdListNode< value_type >* other_p = other.head_;
  const_iterator other_it = other.cbegin();
  while (other_it != other.cend() && other_it != first) {
    ++other_it;
    other_p = other_p->next;
  }
  if (other_it == other.cend()) {
    return;
  }
  other_first = other_p;

  size_t count = 0;
  ++other_it;
  while (other_it != other.cend() && other_it != last) {
    ++other_it;
    other_p = other_p->next;
    ++count;
  }
  other_last = other_p;

  if (other_last == other.tail_) {
    other.tail_ = other_first;
  }
  if (++pos == cend()) {
    tail_ = other_last;
  }

  FwdListNode< value_type >* tmp = other_first->next;
  other_first->next = other_last->next;
  other_last->next = p->next;
  p->next = tmp;
  size_ += count;
  other.size_ -= count;
}

template < typename T >
void zholobov::CircularFwdList< T >::swap(CircularFwdList& other) noexcept
{
  std::swap(head_, other.head_);
  std::swap(tail_, other.tail_);
  std::swap(size_, other.size_);
}

template < typename T >
void zholobov::CircularFwdList< T >::clear() noexcept
{
  if (size_ != 0) {
    FwdListNode< value_type >* curr = head_->next;
    while (curr != head_) {
      FwdListNode< value_type >* temp = curr;
      curr = curr->next;
      delete temp;
    }
    delete head_;
  }
  head_ = nullptr;
  tail_ = nullptr;
  size_ = 0;
}

#endif
