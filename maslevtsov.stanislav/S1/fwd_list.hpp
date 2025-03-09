#ifndef FWD_LIST_HPP
#define FWD_LIST_HPP

#include <cstddef>
#include <utility>
#include "fwd_list_node.hpp"
#include "fwd_iterator.hpp"

namespace maslevtsov {
  template< typename T >
  class FwdList
  {
  public:
    FwdList();
    FwdList(const FwdList& rhs);
    FwdList(FwdList&& rhs) noexcept;
    ~FwdList();
    FwdList& operator=(const FwdList& rhs) noexcept;
    FwdList& operator=(FwdList&& rhs) noexcept;

    FwdIterator< T > begin() const noexcept;
    FwdIterator< T > end() const noexcept;

    T& front() const noexcept;
    T& back() const noexcept;

    std::size_t size() const noexcept;
    bool empty() const noexcept;

    void push_front(T value);
    void push_back(T value);
    void pop_front() noexcept;
    void swap(FwdList& other) noexcept;
    void clear() noexcept;

  private:
    FwdListNode< T >* tail_;
    std::size_t size_;
  };
}

template< typename T >
maslevtsov::FwdList< T >::FwdList():
  tail_(nullptr),
  size_(0)
{}

template< typename T >
maslevtsov::FwdList< T >::FwdList(const FwdList& rhs):
  FwdList()
{
  for (auto i = rhs.begin(); i != rhs.end(); ++i) {
    push_back(*i);
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
  FwdList< T > copied_rhs(std::move(rhs));
  swap(copied_rhs);
  return *this;
}

template< typename T >
maslevtsov::FwdIterator< T > maslevtsov::FwdList< T >::begin() const noexcept
{
  return FwdIterator< T >(tail_->next_);
}

template< typename T >
maslevtsov::FwdIterator< T > maslevtsov::FwdList< T >::end() const noexcept
{
  return FwdIterator< T >(tail_->next_);
}

template< typename T >
T& maslevtsov::FwdList< T >::front() const noexcept
{
  assert(size_);
  return tail_->next_->data_;
}

template< typename T >
T& maslevtsov::FwdList< T >::back() const noexcept
{
  assert(size_);
  return tail_->data_;
}

template< typename T >
std::size_t maslevtsov::FwdList< T >::size() const noexcept
{
  return size_;
}

template< typename T >
bool maslevtsov::FwdList< T >::empty() const noexcept
{
  return size_ == 0;
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

template< typename T >
void maslevtsov::FwdList< T >::clear() noexcept
{
  while (!empty()) {
    pop_front();
  }
}

#endif
