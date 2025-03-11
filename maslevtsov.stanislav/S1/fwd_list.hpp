#ifndef FWD_LIST_HPP
#define FWD_LIST_HPP

#include <cstddef>
#include <utility>
#include <initializer_list>
#include "fwd_list_node.hpp"
#include "fwd_iterator.hpp"

namespace maslevtsov {
  template< typename T >
  class FwdList
  {
  public:
    using iterator = FwdIterator< T, false >;
    using const_iterator = FwdIterator< T, true >;

    FwdList() noexcept;
    FwdList(const FwdList& rhs);
    FwdList(FwdList&& rhs) noexcept;
    FwdList(T value, std::size_t count);
    template< class InputIt >
    FwdList(InputIt first, InputIt last);
    FwdList(std::initializer_list< T > init);
    ~FwdList();

    FwdList& operator=(const FwdList& rhs) noexcept;
    FwdList& operator=(FwdList&& rhs) noexcept;
    FwdList& operator=(std::initializer_list< T > ilist);

    void assign(std::size_t count, const T& value);
    template< class InputIt >
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list< T > ilist);

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void clear() noexcept;
    iterator insert_after(const_iterator pos, const T& value);
    iterator insert_after(const_iterator pos, T&& value);
    iterator insert_after(const_iterator pos, std::size_t count, const T& value);
    template< class InputIt >
    iterator insert_after(const_iterator pos, InputIt first, InputIt last);
    iterator insert_after(const_iterator pos, std::initializer_list< T > ilist);
    iterator erase_after(const_iterator pos);
    iterator erase_after(const_iterator first, const_iterator last);
    void push_front(T value);
    void push_back(T value);
    void pop_front() noexcept;
    void swap(FwdList& other) noexcept;

    void splice_after(const_iterator pos, FwdList& other);
    void splice_after(const_iterator pos, FwdList&& other);
    void splice_after(const_iterator pos, FwdList& other, const_iterator it);
    void splice_after(const_iterator pos, FwdList& other, const_iterator first, const_iterator last);
    void remove(const T& value);
    template< class UnaryPredicate >
    void remove_if(UnaryPredicate condition);

  private:
    FwdListNode< T >* tail_;
    std::size_t size_;
  };
}

template< typename T >
maslevtsov::FwdList< T >::FwdList() noexcept:
  tail_(nullptr),
  size_(0)
{}

template< typename T >
maslevtsov::FwdList< T >::FwdList(const FwdList& rhs):
  FwdList()
{
  if (!rhs.empty()) {
    push_back(*(rhs.cbegin()));
    for (auto i = ++rhs.cbegin(); i != rhs.cend(); ++i) {
      push_back(*i);
    }
  }
}

template< typename T >
maslevtsov::FwdList< T >::FwdList(FwdList&& rhs) noexcept:
  tail_(std::exchange(rhs.tail_, nullptr)),
  size_(std::exchange(rhs.size_, 0))
{}

template< typename T >
maslevtsov::FwdList< T >::~FwdList()
{
  clear();
}

template< typename T >
typename maslevtsov::FwdList< T >::FwdList& maslevtsov::FwdList< T >::operator=(const FwdList& rhs) noexcept
{
  FwdList< T > copied_rhs(rhs);
  swap(copied_rhs);
  return *this;
}

template< typename T >
typename maslevtsov::FwdList< T >::FwdList& maslevtsov::FwdList< T >::operator=(FwdList&& rhs) noexcept
{
  FwdList< T > copied_rhs(rhs);
  swap(copied_rhs);
  return *this;
}

template< typename T >
T& maslevtsov::FwdList< T >::front()
{
  return tail_->next_->data_;
}

template< typename T >
const T& maslevtsov::FwdList< T >::front() const
{
  return tail_->next_->data_;
}

template< typename T >
T& maslevtsov::FwdList< T >::back()
{
  return tail_->data_;
}

template< typename T >
const T& maslevtsov::FwdList< T >::back() const
{
  return tail_->data_;
}

template< typename T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::begin() noexcept
{
  return iterator(tail_->next_);
}

template< typename T >
typename maslevtsov::FwdList< T >::const_iterator maslevtsov::FwdList< T >::begin() const noexcept
{
  return const_iterator(tail_->next_);
}

template< typename T >
typename maslevtsov::FwdList< T >::const_iterator maslevtsov::FwdList< T >::cbegin() const noexcept
{
  return const_iterator(tail_->next_);
}

template< typename T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::end() noexcept
{
  return iterator(tail_->next_);
}

template< typename T >
typename maslevtsov::FwdList< T >::const_iterator maslevtsov::FwdList< T >::end() const noexcept
{
  return const_iterator(tail_->next_);
}

template< typename T >
typename maslevtsov::FwdList< T >::const_iterator maslevtsov::FwdList< T >::cend() const noexcept
{
  return const_iterator(tail_->next_);
}

template< typename T >
bool maslevtsov::FwdList< T >::empty() const noexcept
{
  return size_ == 0;
}

template< typename T >
std::size_t maslevtsov::FwdList< T >::size() const noexcept
{
  return size_;
}

template< typename T >
void maslevtsov::FwdList< T >::clear() noexcept
{
  while (!empty()) {
    pop_front();
  }
}

template< typename T >
void maslevtsov::FwdList< T >::push_front(T value)
{
  FwdListNode< T >* new_node = new FwdListNode< T >{value, nullptr};
  if (empty()) {
    tail_ = new_node;
    tail_->next_ = tail_;
  } else {
    new_node->next_ = tail_->next_;
    tail_->next_ = new_node;
  }
  ++size_;
}

template< typename T >
void maslevtsov::FwdList< T >::push_back(T value)
{
  FwdListNode< T >* new_node = new FwdListNode< T >{value, nullptr};
  if (empty()) {
    tail_ = new_node;
    tail_->next_ = tail_;
  } else {
    new_node->next_ = tail_->next_;
    tail_->next_ = new_node;
    tail_ = new_node;
  }
  ++size_;
}

template< typename T >
void maslevtsov::FwdList< T >::pop_front() noexcept
{
  if (empty()) {
    return;
  }
  FwdListNode< T >* to_delete = tail_->next_;
  tail_->next_ = to_delete->next_;
  delete to_delete;
  --size_;
}

template< typename T >
void maslevtsov::FwdList< T >::swap(FwdList& other) noexcept
{
  std::swap(tail_, other.tail_);
  std::swap(size_, other.size_);
}

#endif
