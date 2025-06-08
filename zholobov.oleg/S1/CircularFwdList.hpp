#ifndef CIRCULARFWDLIST_HPP
#define CIRCULARFWDLIST_HPP

#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

#include "CircularFwdListIterators.hpp"
#include "FwdListNode.hpp"

namespace zholobov {

  namespace details {
    template < typename T >
    using IsInputIterator = std::enable_if_t<
        std::is_base_of<
            std::input_iterator_tag,
            typename std::iterator_traits< T >::iterator_category >::value >;
  }

  template < typename T >
  class CircularFwdList {
  public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = CircularFwdListIterator< value_type >;
    using const_iterator = CircularFwdListConstIterator< value_type >;

    CircularFwdList();
    explicit CircularFwdList(size_t n);
    CircularFwdList(size_t n, const value_type& val);
    CircularFwdList(std::initializer_list< value_type > init);

    CircularFwdList(const CircularFwdList& other);
    CircularFwdList(CircularFwdList&& other) noexcept;

    template < typename InputIter, typename = details::IsInputIterator< InputIter > >
    CircularFwdList(InputIter first, InputIter last);

    ~CircularFwdList();

    CircularFwdList& operator=(const CircularFwdList& other);
    CircularFwdList& operator=(CircularFwdList&& other) noexcept;
    CircularFwdList& operator=(std::initializer_list< value_type > init);

    template < typename InputIter, typename = details::IsInputIterator< InputIter > >
    void assign(InputIter first, InputIter last);
    void assign(size_t n, const T& val);
    void assign(std::initializer_list< T > init);

    iterator before_begin() noexcept;
    const_iterator before_begin() const noexcept;
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbefore_begin() const noexcept;
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

    void push_front(const value_type& val);
    void push_front(value_type&& val);
    void push_back(const value_type& val);
    void push_back(value_type&& val);

    void pop_front();
    void pop_back();

    iterator insert_after(const_iterator position, const T& x);
    iterator insert_after(const_iterator position, T&& x);
    iterator insert_after(const_iterator position, size_t n, const T& x);
    iterator insert_after(const_iterator position, std::initializer_list< T > il);
    template < typename InputIter, typename = details::IsInputIterator< InputIter > >
    iterator insert_after(const_iterator position, InputIter first, InputIter last);

    iterator erase_after(const_iterator position);
    iterator erase_after(const_iterator position, const_iterator last);

    void remove(const value_type& val);
    template < class Predicate >
    void remove_if(Predicate pred);

    template < typename List >
    void splice_after(const_iterator pos, List&& other);
    template < typename List >
    void splice_after(const_iterator pos, List&& other, const_iterator it);
    template < typename List >
    void splice_after(const_iterator pos, List&& other, const_iterator first, const_iterator last);

    void swap(CircularFwdList& other) noexcept;
    void clear() noexcept;

  private:
    FwdListNodeBase* before_head_;
    FwdListNodeBase* head_;
    FwdListNodeBase* tail_;
    size_t size_;
  };
}

template < typename T >
zholobov::CircularFwdList< T >::CircularFwdList():
  before_head_(reinterpret_cast< FwdListNodeBase* >(std::addressof(head_))),
  head_(nullptr),
  tail_(nullptr),
  size_(0)
{}

template < typename T >
zholobov::CircularFwdList< T >::CircularFwdList(size_t n):
  CircularFwdList(n, value_type())
{}

template < typename T >
zholobov::CircularFwdList< T >::CircularFwdList(size_t n, const value_type& val):
  CircularFwdList()
{
  for (size_t i = 0; i < n; ++i) {
    push_front(val);
  }
}

template < typename T >
zholobov::CircularFwdList< T >::CircularFwdList(std::initializer_list< value_type > init):
  CircularFwdList()
{
  assign(init);
}

template < typename T >
zholobov::CircularFwdList< T >::CircularFwdList(const CircularFwdList& other):
  CircularFwdList()
{
  for (const auto& elem: other) {
    push_back(elem);
  }
}

template < typename T >
zholobov::CircularFwdList< T >::CircularFwdList(CircularFwdList&& other) noexcept:
  CircularFwdList()
{
  swap(other);
}

template < typename T >
template < typename InputIter, typename SFINAE >
zholobov::CircularFwdList< T >::CircularFwdList(InputIter first, InputIter last):
  CircularFwdList()
{
  assign(first, last);
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
zholobov::CircularFwdList< T >& zholobov::CircularFwdList< T >::operator=(std::initializer_list< value_type > init)
{
  assign(init);
  return *this;
}

template < typename T >
template < typename InputIter, typename SFINAE >
void zholobov::CircularFwdList< T >::assign(InputIter first, InputIter last)
{
  CircularFwdList< T > temp;
  for (; first != last; ++first) {
    temp.push_back(*first);
  }
  swap(temp);
}

template < typename T >
void zholobov::CircularFwdList< T >::assign(size_t n, const T& val)
{
  CircularFwdList< T > temp(n, val);
  swap(temp);
}

template < typename T >
void zholobov::CircularFwdList< T >::assign(std::initializer_list< T > init)
{
  CircularFwdList< T > temp;
  for (const auto& elem: init) {
    temp.push_back(elem);
  }
  swap(temp);
}

template < typename T >
typename zholobov::CircularFwdList< T >::iterator zholobov::CircularFwdList< T >::before_begin() noexcept
{
  return iterator(std::addressof(before_head_));
}

template < typename T >
typename zholobov::CircularFwdList< T >::const_iterator zholobov::CircularFwdList< T >::before_begin() const noexcept
{
  return cbefore_begin();
}

template < typename T >
typename zholobov::CircularFwdList< T >::iterator zholobov::CircularFwdList< T >::begin() noexcept
{
  return iterator(std::addressof(head_));
}

template < typename T >
typename zholobov::CircularFwdList< T >::const_iterator zholobov::CircularFwdList< T >::begin() const noexcept
{
  return cbegin();
}

template < typename T >
typename zholobov::CircularFwdList< T >::const_iterator zholobov::CircularFwdList< T >::cbefore_begin() const noexcept
{
  return const_iterator(std::addressof(before_head_));
}

template < typename T >
typename zholobov::CircularFwdList< T >::const_iterator zholobov::CircularFwdList< T >::cbegin() const noexcept
{
  return const_iterator(std::addressof(head_));
}

template < typename T >
typename zholobov::CircularFwdList< T >::iterator zholobov::CircularFwdList< T >::end() noexcept
{
  return (size_ == 0) ? iterator(std::addressof(head_)) : iterator(std::addressof(tail_->next));
}

template < typename T >
typename zholobov::CircularFwdList< T >::const_iterator zholobov::CircularFwdList< T >::end() const noexcept
{
  return cend();
}

template < typename T >
typename zholobov::CircularFwdList< T >::const_iterator zholobov::CircularFwdList< T >::cend() const noexcept
{
  return (size_ == 0) ? const_iterator(std::addressof(head_)) : const_iterator(std::addressof(tail_->next));
}

template < typename T >
typename zholobov::CircularFwdList< T >::reference zholobov::CircularFwdList< T >::front()
{
  return static_cast< FwdListNode< T >* >(head_)->value;
}

template < typename T >
typename zholobov::CircularFwdList< T >::const_reference zholobov::CircularFwdList< T >::front() const
{
  return static_cast< FwdListNode< T >* >(head_)->value;
}

template < typename T >
typename zholobov::CircularFwdList< T >::reference zholobov::CircularFwdList< T >::back()
{
  return static_cast< FwdListNode< T >* >(tail_)->value;
}

template < typename T >
typename zholobov::CircularFwdList< T >::const_reference zholobov::CircularFwdList< T >::back() const
{
  return static_cast< FwdListNode< T >* >(tail_)->value;
}

template < typename T >
bool zholobov::CircularFwdList< T >::empty() const noexcept
{
  return size_ == 0;
}

template < typename T >
void zholobov::CircularFwdList< T >::push_front(const value_type& val)
{
  push_front(value_type(val));
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
  push_back(value_type(val));
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
  FwdListNodeBase* temp = head_;
  if (size_ == 0) {
    return;
  } else if (size_ == 1) {
    head_ = nullptr;
    tail_ = nullptr;
  } else {
    head_ = head_->next;
    tail_->next = head_;
  }
  delete temp;
  --size_;
}

template < typename T >
typename zholobov::CircularFwdList< T >::iterator
zholobov::CircularFwdList< T >::insert_after(const_iterator position, const T& x)
{
  CircularFwdList< T > list(1, x);
  splice_after(position, list);
  return ++iterator(position);
}

template < typename T >
typename zholobov::CircularFwdList< T >::iterator
zholobov::CircularFwdList< T >::insert_after(const_iterator position, T&& x)
{
  CircularFwdList< T > list;
  list.push_back(std::move(x));
  splice_after(position, list);
  return ++iterator(position);
}

template < typename T >
typename zholobov::CircularFwdList< T >::iterator
zholobov::CircularFwdList< T >::insert_after(const_iterator position, size_t n, const T& x)
{
  CircularFwdList< T > list(n, x);
  splice_after(position, list);
  auto result = iterator(position);
  std::advance(result, n);
  return result;
}

template < typename T >
typename zholobov::CircularFwdList< T >::iterator
zholobov::CircularFwdList< T >::insert_after(const_iterator position, std::initializer_list< T > il)
{
  CircularFwdList< T > list(il);
  splice_after(position, list);
  auto result = iterator(position);
  std::advance(result, il.size());
  return result;
}

template < typename T >
template < class InputIter, typename SFINAE >
typename zholobov::CircularFwdList< T >::iterator
zholobov::CircularFwdList< T >::insert_after(const_iterator position, InputIter first, InputIter last)
{
  CircularFwdList< T > list(first, last);
  splice_after(position, list);
  auto result = iterator(position);
  std::advance(result, std::distance(first, last));
  return result;
}

template < typename T >
typename zholobov::CircularFwdList< T >::iterator
zholobov::CircularFwdList< T >::erase_after(const_iterator position)
{
  auto last = position;
  if (++last == cend()) {
    return end();
  }
  ++last;
  return erase_after(position, last);
}

template < typename T >
typename zholobov::CircularFwdList< T >::iterator
zholobov::CircularFwdList< T >::erase_after(const_iterator position, const_iterator last)
{
  CircularFwdList< T > tmp;
  tmp.splice_after(tmp.cbefore_begin(), *this, position, last);
  return ++iterator(position);
}

template < typename T >
void zholobov::CircularFwdList< T >::remove(const value_type& val)
{
  remove_if([&val](const T& value) {
    return value == val;
  });
}

template < typename T >
template < class Predicate >
void zholobov::CircularFwdList< T >::remove_if(Predicate pred)
{
  while (head_ != nullptr && (pred(static_cast< FwdListNode< T >* >(head_)->value))) {
    pop_front();
  }
  if (head_ != nullptr) {
    FwdListNodeBase* p = head_;
    while (p->next != head_) {
      if (pred(static_cast< FwdListNode< T >* >(p->next)->value)) {
        FwdListNodeBase* temp = p->next;
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
template < typename List >
void zholobov::CircularFwdList< T >::splice_after(const_iterator pos, List&& other)
{
  static_assert(std::is_same< std::decay_t< List >, CircularFwdList< T > >::value, "Invalid list type");
  if (!other.empty()) {
    FwdListNodeBase* p = before_head_;
    const_iterator it = cbefore_begin();
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
      tail_->next = head_;
      other.head_ = nullptr;
      other.tail_ = nullptr;
      size_ += other.size_;
      other.size_ = 0;
    }
  }
}

template < typename T >
template < typename List >
void zholobov::CircularFwdList< T >::splice_after(const_iterator pos, List&& other, const_iterator it)
{
  static_assert(std::is_same< std::decay_t< List >, CircularFwdList< T > >::value, "Invalid list type");
  FwdListNodeBase* p = before_head_;
  const_iterator this_it = cbefore_begin();
  while ((this_it != cend()) && (this_it != pos)) {
    ++this_it;
    p = p->next;
  }
  if (this_it != cend()) {
    FwdListNodeBase** other_p = std::addressof(other.head_);
    const_iterator other_it = other.cbegin();
    while ((other_it != other.cend()) && (other_it != it)) {
      ++other_it;
      other_p = std::addressof((*other_p)->next);
    }
    FwdListNodeBase* tmp = *other_p;
    *other_p = tmp->next;
    tmp->next = p->next;
    p->next = tmp;
    --other.size_;
    ++size_;
  }
}

template < typename T >
template < typename List >
void zholobov::CircularFwdList< T >::splice_after(const_iterator pos, List&& other, const_iterator first, const_iterator last)
{
  static_assert(std::is_same< std::decay_t< List >, CircularFwdList< T > >::value, "Invalid list type");
  FwdListNodeBase** pos_p = pos.node_;
  FwdListNodeBase** first_p = first.node_;
  FwdListNodeBase** last_p = last.node_;

  size_t count = std::distance(first, last) - 1;
  if (count > 0) {
    if (last == other.cend()) {
      other.tail_ = *first_p;
    }
    if (pos == cbefore_begin()) {
      head_ = (*first_p)->next;
    }
    if (++pos == cend()) {
      tail_ = reinterpret_cast< FwdListNodeBase* >(last_p);
    }

    FwdListNodeBase* keep = (*pos_p)->next;
    (*pos_p)->next = (*first_p)->next;
    (*first_p)->next = *last_p;
    *last_p = keep;
    size_ += count;
    other.size_ -= count;
    tail_->next = head_;
    if (other.size_ == 0) {
      other.head_ = nullptr;
      other.tail_ = nullptr;
    } else {
      other.tail_->next = other.head_;
    }
  }
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
    FwdListNodeBase* curr = head_;
    do {
      FwdListNodeBase* temp = curr;
      curr = curr->next;
      delete static_cast< FwdListNode< T >* >(temp);
    } while (curr != head_);
  }
  head_ = nullptr;
  tail_ = nullptr;
  size_ = 0;
}

#endif
